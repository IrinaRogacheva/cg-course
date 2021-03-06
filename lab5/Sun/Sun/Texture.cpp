#include "tinyxml2.h"
#include "FilesystemUtils.h"
#include "Texture.h"
#include "Utils.h"
#include <codecvt>
#include <cstdlib>
#include <map>
#include <string>
#include <string_view>
#include <functional>

namespace xml = tinyxml2;
using namespace std::literals;

namespace
{
	GLenum ConvertEnum(TextureWrapMode mode)
	{
		// ???????? ????????? ????? ?? GL_CLAMP_TO_EDGE_EXT ?????????? GLEW.
#if defined(_WIN32) && !defined(GL_CLAMP_TO_EDGE)
#define GL_CLAMP_TO_EDGE 0x812F
#endif
		static const std::map<TextureWrapMode, GLenum> MAPPING = {
			{ TextureWrapMode::CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE },
			{ TextureWrapMode::REPEAT, GL_REPEAT },
		};
		return MAPPING.at(mode);
	}

	class CPropertyListParser
	{
	public:
		using MetaHandler = std::function<void(const std::string& textureName, glm::ivec2& size)>;
		using FrameHandler = std::function<void(const std::string&, const CFloatRect&)>;

		CPropertyListParser(const fs::path& xmlPath)
			: m_xmlPath(xmlPath.generic_string())
		{
		}

		void DoOnParsedTextureMeta(const MetaHandler& handler)
		{
			m_onParsedTextureMeta = handler;
		}

		void DoOnParsedFrame(const FrameHandler& handler)
		{
			m_onParsedFrame = handler;
		}

		void Parse()
		{
			const std::string xml = CFilesystemUtils::LoadFileAsString(m_xmlPath);
			xml::XMLDocument document;
			m_error = document.Parse(xml.c_str(), xml.length());
			CheckError();

			const auto* plist = FindChild(&document, "plist");
			const auto* dict = FindChild(plist, "dict");
			ParseMetadata(GetValueNode(dict, "metadata"));
			ParseFrames(GetValueNode(dict, "frames"));
		}

	private:
		const xml::XMLElement* FindChild(const xml::XMLNode* parent, const std::string& name)
		{
			const auto* child = parent->FirstChildElement(name.c_str());
			if (!child)
			{
				throw std::logic_error("Child element '" + name + "' not found in '"
					+ m_xmlPath + "'");
			}
			return child;
		}

		const xml::XMLElement* GetNextSibling(const xml::XMLElement* node)
		{
			const auto* sibling = node->NextSiblingElement();
			if (!sibling)
			{
				std::string name = node->Name();
				throw std::logic_error("Sibling for '" + name + "' not found in '"
					+ m_xmlPath + "'");
			}
			return sibling;
		}

		const xml::XMLElement* GetValueNode(const xml::XMLElement* dict, std::string_view key)
		{
			const auto* pKey = dict->FirstChildElement("key");
			while (pKey)
			{
				std::string_view currentKey = pKey->GetText();
				if (currentKey == key)
				{
					return GetNextSibling(pKey);
				}
				pKey = pKey->NextSiblingElement("key");
			}

			std::string reason = "Key '"s + std::string(key) + "' not found in '"s + m_xmlPath + "'"s;
			throw std::logic_error(reason);
		}

		void ParseMetadata(const xml::XMLElement* metadata)
		{
			const auto* pString = GetValueNode(metadata, "textureFileName");
			const std::string filename = pString->GetText();

			pString = GetValueNode(metadata, "size");
			const std::string sizeStr = pString->GetText();

			glm::ivec2 size;
			if (sscanf_s(sizeStr.c_str(), "{%d, %d}", &size.x, &size.y) != size.length())
			{
				throw std::logic_error("Invalid size value '" + sizeStr + "' in '"
					+ m_xmlPath + "'");
			}
			m_onParsedTextureMeta(filename, size);
		}

		void ParseFrames(const xml::XMLElement* frames)
		{
			const auto* pKey = frames->FirstChildElement("key");
			while (pKey)
			{
				const std::string spriteName = pKey->GetText();
				const auto* dict = GetNextSibling(pKey);
				CFloatRect rect = ParseFrameRect(dict);
				m_onParsedFrame(spriteName, rect);
				pKey = pKey->NextSiblingElement("key");
			}
		}

		CFloatRect ParseFrameRect(const xml::XMLElement* dict)
		{
			const auto* nodeX = GetValueNode(dict, "x");
			const auto* nodeY = GetValueNode(dict, "y");
			const auto* nodeWidth = GetValueNode(dict, "width");
			const auto* nodeHeight = GetValueNode(dict, "height");

			const auto x = std::stoul(nodeX->GetText());
			const auto y = std::stoul(nodeY->GetText());
			const auto width = std::stoul(nodeWidth->GetText());
			const auto height = std::stoul(nodeHeight->GetText());

			return CFloatRect(glm::vec2{ float(x), float(y) },
				glm::vec2{ float(x + width), float(y + height) });
		}

		void CheckError()
		{
			if (m_error != xml::XML_SUCCESS)
			{
				const std::string errorCode = std::to_string(unsigned(m_error));
				throw std::runtime_error("Failed to load plist file '" + m_xmlPath
					+ "': xml error #" + errorCode);
			}
		}

		MetaHandler m_onParsedTextureMeta;
		FrameHandler m_onParsedFrame;
		std::string m_xmlPath;
		xml::XMLError m_error = xml::XML_SUCCESS;
	};
} // namespace

CTexture2D::CTexture2D(const glm::ivec2& size, bool hasAlpha)
	: m_size(size)
	, m_hasAlpha(hasAlpha)
{
	glGenTextures(1, &m_textureId);
}

CTexture2D::~CTexture2D()
{
	glDeleteTextures(1, &m_textureId);
}

glm::ivec2 CTexture2D::GetSize() const
{
	return m_size;
}

bool CTexture2D::HasAlpha() const
{
	return m_hasAlpha;
}

void CTexture2D::Bind() const
{
	glBindTexture(GL_TEXTURE_2D, m_textureId);
}

void CTexture2D::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

CTexture2DUniquePtr CTexture2DLoader::Load(const fs::path& path)
{
	SDLSurfacePtr pSurface = CFilesystemUtils::LoadImageByPath(path);
	const glm::ivec2 surfaceSize = { pSurface->w, pSurface->h };
	const bool hasAlpha = SDL_ISPIXELFORMAT_ALPHA(pSurface->format->format);

	// ??? ??????????? ????? ?????????????? ? RGB ??? RGBA,
	//  ? ??????????? ?? ??????? ?????-?????? ? ???????? ???????????.
	const GLenum pixelFormat = hasAlpha ? GL_RGBA : GL_RGB;
	const uint32_t requiredFormat = hasAlpha
		? SDL_PIXELFORMAT_ABGR8888
		: SDL_PIXELFORMAT_RGB24;
	if (pSurface->format->format != requiredFormat)
	{
		pSurface.reset(SDL_ConvertSurfaceFormat(pSurface.get(), requiredFormat, 0));
	}

	// ? ??????? ????????? OpenGL ?????? ???? ? ?????? ????? ?????,
	//  ? ?? ? ??????? ?????, ??????? ?????????????? ???????????.
	CUtils::FlipSurfaceVertically(*pSurface);

	auto pTexture = std::make_unique<CTexture2D>(surfaceSize, hasAlpha);
	pTexture->DoWhileBinded([&] {
		glTexImage2D(GL_TEXTURE_2D, 0, GLint(pixelFormat), pSurface->w, pSurface->h,
			0, pixelFormat, GL_UNSIGNED_BYTE, pSurface->pixels);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GLint(ConvertEnum(m_wrapS)));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GLint(ConvertEnum(m_wrapT)));
		});

	return pTexture;
}

void CTexture2DLoader::SetWrapMode(TextureWrapMode wrap)
{
	m_wrapS = wrap;
	m_wrapT = wrap;
}

void CTexture2DLoader::SetWrapMode(TextureWrapMode wrapS, TextureWrapMode wrapT)
{
	m_wrapS = wrapS;
	m_wrapT = wrapT;
}

CTexture2DAtlas::CTexture2DAtlas(const fs::path& xmlPath, CTexture2DLoader loader)
{
	const fs::path abspath = CFilesystemUtils::GetResourceAbspath(xmlPath);
	glm::vec2 frameScale;

	CPropertyListParser parser(abspath);
	parser.DoOnParsedTextureMeta([&](const std::string& filename, const glm::ivec2& size) {
		// ?????????? ???????????? ??? ?????????????? ???? ?????????
		//    ? ?????? ??????? ? ????????? [0..1].
		frameScale = { float(1.f / size.x),
			float(1.f / size.y) };
		m_pTexture = loader.Load(abspath.parent_path() / filename);
		});
	parser.DoOnParsedFrame([&](const std::string& name, const CFloatRect& rect) {
		// ??????????? ?????????? ? ?????? ??????? ? ????????? [0..1]
		CFloatRect texRect = rect.GetScaled(frameScale);
		// ?????????????? ?? ??? Y, ????? ????????????????
		//    ? ??????????? ???????? ? ????????? ???????.
		const float flippedY = 1.f - texRect.GetBottomRight().y;
		texRect.MoveTo({ texRect.GetTopLeft().x, flippedY });

		m_frames[name] = texRect;
		});
	parser.Parse();
}

const CTexture2D& CTexture2DAtlas::GetTexture() const
{
	return *m_pTexture;
}

CFloatRect CTexture2DAtlas::GetFrameRect(const std::string& frameName) const
{
	try
	{
		return m_frames.at(frameName);
	}
	catch (...)
	{
		throw std::runtime_error("Frame not found: " + frameName);
	}
}
