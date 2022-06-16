// 01_raytrace_baseView.cpp : implementation of the CRaytraceView class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FrameBuffer.h"
#include "resource.h"
#include "FrameBuffer.h"

#include "RaytraceView.h"
#include "SceneObject.h"
#include "OmniLightSource.h"
#include "Intersection.h"
#include "Ray.h"
#include "CheckerShader.h"
#include "SimpleMaterial.h"
#include "Torus.h"
#include "HyperbolicParaboloid.h"
#include "Paraboloid.h"
#include "Sphere.h"
#include "SimpleDiffuseShader.h"
#include "ConicCylinder.h"
#include "Plane.h"

CRaytraceView::CRaytraceView()
	: m_pFrameBuffer(std::make_unique<CFrameBuffer>(800, 600))
{
	/*
	Задаем цвет заднего фона сцены
	*/
	m_scene.SetBackdropColor(CVector4f(1, 0, 1, 1));

	//AddSomeTorus();
	AddSomeParaboloid();
	//AddSomeHyperbolicParaboloid();
	AddSomeLight();

	/*
	Задаем параметры видового порта и матрицы проецирования в контексте визуализации
	*/
	m_context.SetViewPort(CViewPort(0, 0, 800, 600));
	CMatrix4d proj;
	proj.LoadPerspective(60, 800.0 / 600.0, 0.1, 10);
	m_context.SetProjectionMatrix(proj);

	// Задаем матрицу камеры
	CMatrix4d modelView;
	modelView.LoadLookAtRH(
		0, 5, 7,
		0, 0, 0,
		0, 1, 0);
	m_context.SetModelViewMatrix(modelView);
}

void CRaytraceView::AddSomeTorus()
{
	CSimpleMaterial white;
	white.SetDiffuseColor(CVector4f(1, 1, 1, 1));
	CSimpleMaterial yellow;
	yellow.SetDiffuseColor(CVector4f(1, 1, 0, 1));
	CSimpleMaterial red;
	red.SetDiffuseColor(CVector4f(1, 0, 0, 1));
	CSimpleMaterial orange;
	orange.SetDiffuseColor(CVector4f(1, 0.61f, 0, 1));
	CSimpleMaterial pink;
	pink.SetDiffuseColor(CVector4f(0.97f, 0.45f, 0.65f, 1.f));
	CSimpleMaterial green;
	green.SetDiffuseColor(CVector4f(0, 1, 0, 1));

	CMatrix4d first = CMatrix4d();
	first.Translate(0, -1.5, 0);
	first.Scale(1, 1, 1);
	AddTorus(CreateSimpleDiffuseShader(white), 1, 0.4, first);

	CMatrix4d second = CMatrix4d();
	second.Translate(0, -0.8, 0);
	second.Scale(0.9, 0.9, 0.9);
	AddTorus(CreateSimpleDiffuseShader(red), 1, 0.4, second);

	CMatrix4d third = CMatrix4d();
	third.Translate(0, -0.1, 0);
	third.Scale(0.8, 0.8, 0.8);
	AddTorus(CreateSimpleDiffuseShader(green), 1, 0.4, third);

	CMatrix4d fourth = CMatrix4d();
	fourth.Translate(0, 0.4, 0);
	fourth.Scale(0.7, 0.7, 0.7);
	//AddTorus(CreateSimpleDiffuseShader(pink), 1, 0.4, fourth);

	CMatrix4d fifth = CMatrix4d();
	fifth.Translate(0, 0.9, 0);
	fifth.Scale(0.6, 0.6, 0.6);
	//AddTorus(CreateSimpleDiffuseShader(yellow), 1, 0.4, fifth);

	CMatrix4d sixth = CMatrix4d();
	sixth.Translate(0, 1.3, 0);
	sixth.Scale(0.5, 0.5, 0.5);
	//AddTorus(CreateSimpleDiffuseShader(orange), 1, 0.4, sixth);

	AddSphere(CreateSimpleDiffuseShader(red), 0.5, CVector3d(0, 1.8, 0));
}

void CRaytraceView::AddSomeHyperbolicParaboloid()
{
	CSimpleMaterial green;
	green.SetDiffuseColor(CVector4f(0, 1, 0, 1));

	CMatrix4d transform;
	transform.Scale(2, 2, 2);
	transform.Rotate(240, 0, 1, 0);

	AddHyperbolicParaboloid(CreateSimpleDiffuseShader(green), transform);
}

void CRaytraceView::AddSomeParaboloid()
{
	CSimpleMaterial yellow;
	yellow.SetDiffuseColor(CVector4f(1, 1, 0, 1));
	CSimpleMaterial red;
	red.SetDiffuseColor(CVector4f(1, 0, 0, 1));
	CSimpleMaterial pink;
	pink.SetDiffuseColor(CVector4f(0.97f, 0.45f, 0.65f, 1.f));
	CSimpleMaterial green;
	green.SetDiffuseColor(CVector4f(0, 1, 0, 1));

	CMatrix4d first = CMatrix4d();
	first.Translate(0, -0.5, 0);
	first.Scale(1.5, 1.5, 1.5);
	AddParaboloid(CreateSimpleDiffuseShader(yellow), first);

	CMatrix4d second = CMatrix4d();
	second.Translate(0, 1.0, 0);
	second.Scale(1.1, 1.1, 1.1);
	AddParaboloid(CreateSimpleDiffuseShader(red), second);

	CMatrix4d third = CMatrix4d();
	third.Translate(0, 2.1, 0);
	third.Scale(0.7, 0.7, 0.7);
	AddParaboloid(CreateSimpleDiffuseShader(pink), third);

	CMatrix4d fourth = CMatrix4d();
	fourth.Translate(0, 2.8, 0);
	fourth.Scale(0.3, 0.3, 0.3);
	AddParaboloid(CreateSimpleDiffuseShader(green), fourth);
}

// Добавляем бесконечную шахматную плоскость y = 0
void CRaytraceView::AddSomePlane()
{
	// Задаем смещение текстурных координат в 1/2 размера шахматного кубика для того чтобы избежать
	// визуальных артефактов при определении цвета клетки, связанных с погрешностями вычислений
	CMatrix4d checkerShaderTransform;
	checkerShaderTransform.Scale(0.2, 0.2, 0.2);
	checkerShaderTransform.Translate(0.25, 0.25, 0.25);
	AddPlane(CreateCheckerShader(checkerShaderTransform), 0, 1, 0, 0);
}

// Добавляем несколько сфер
void CRaytraceView::AddSomeSpheres()
{
	CSimpleMaterial yellow;
	yellow.SetDiffuseColor(CVector4f(1, 1, 0, 1));
	CSimpleDiffuseShader& shader = CreateSimpleDiffuseShader(yellow);
	AddSphere(shader, 1, CVector3d(0, 1, 0));
	AddSphere(shader, 0.5, CVector3d(2, 0, 0));
}

// Создаем и добавляем в сцену точечный источник света
void CRaytraceView::AddSomeLight()
{
	COmniLightPtr pLight(new COmniLightSource(CVector3d(1, 4, 6)));
	pLight->SetDiffuseIntensity(CVector4f(1, 1, 1, 1));
	pLight->SetAttenuation(1, 0, 0.0005);
	m_scene.AddLightSource(pLight);

	COmniLightPtr pLight1(new COmniLightSource(CVector3d(8, 0, 0)));
	pLight1->SetDiffuseIntensity(CVector4f(0.8f, 0.8f, 0.8f, 1.f));
	pLight1->SetAttenuation(1, 0, 0.0005);
	m_scene.AddLightSource(pLight1);
}

// Добавляем несколько конических цилиндров
void CRaytraceView::AddSomeConicCylinders()
{
	CSimpleMaterial white;
	white.SetDiffuseColor(CVector4f(1, 1, 1, 1));

	CMatrix4d transform;
	transform.Translate(-2.5, 0, 0);
	transform.Rotate(-90, 1, 0, 0);

	AddConicCylinder(CreateSimpleDiffuseShader(white), 2, 1, 1, transform);

	CSimpleMaterial red;
	red.SetDiffuseColor(CVector4f(1, 0, 0, 1));
	CMatrix4d coneTransform;
	coneTransform.Translate(0, 0, 2);
	coneTransform.Rotate(-90, 1, 0, 0);

	AddConicCylinder(CreateSimpleDiffuseShader(red), 1, 1, 0, coneTransform);

	CMatrix4d conicFrustumTransform;
	conicFrustumTransform.Translate(4.0, 0.0, 0.0);
	conicFrustumTransform.Rotate(-90, 1, 0, 0);
	CSimpleMaterial green;
	green.SetDiffuseColor(CVector4f(0, 1, 0, 1));
	AddConicCylinder(CreateSimpleDiffuseShader(green), 1, 0.5, 0.3, conicFrustumTransform);
}

CRaytraceView::~CRaytraceView()
{
	// Необходимо остановить фоновую работу объекта CRenderer до разрушения
	// данных класса CRaytraceView, т.к. CRenderer использует для своей работы
	// другие объекты, в частности, буфер кадра, разрушать которые можно только
	// после остановки CRenderer
	m_renderer.Stop();
}

BOOL CRaytraceView::PreTranslateMessage(MSG* pMsg)
{
	pMsg;
	return FALSE;
}

LRESULT CRaytraceView::OnEraseBkgnd(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// Сообщаем системе, что дальнейшая очистка буфера не требуется
	return 1;
}

LRESULT CRaytraceView::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CPaintDC dc(m_hWnd);

	// Получаем размеры клиентской области окна
	CRect clientRect;
	GetClientRect(clientRect);
	int clientWidth = clientRect.Width();
	int clientHeight = clientRect.Height();

	// Кисть, используемая для очистки окна
	CBrush whiteBrush;
	whiteBrush.CreateSolidBrush(0xffffff);

	// Проверка на случай отсутствия буфера кадра
	if (m_pFrameBuffer.get())
	{
		int w = m_pFrameBuffer->GetWidth();
		int h = m_pFrameBuffer->GetHeight();
		// Рисуем буфер кадра в левом верхнем углу
		// клиентской области окна
		DrawFrameBuffer(dc, 0, 0);

		// Т.к. мы отключили очистку заднего фона экрана,
		// область справа и снизу от изображения придется
		// очистить вручную

		// Очищаем правую часть клиентской области
		if (w < clientWidth)
		{
			dc.FillRect(CRect(w, 0, clientWidth, h), whiteBrush);
		}

		// Очищаем нижную часть клиентской области
		if (h < clientHeight)
		{
			dc.FillRect(CRect(0, h, clientWidth, clientHeight), whiteBrush);
		}
	}
	else // Буфер кадра по каким-то причинам не определен
	{
		ATLASSERT(!"Something bad with the frame buffer");
	}

	return 0;
}

void CRaytraceView::DrawFrameBuffer(CDC& dc, int x, int y)
{
	int w = m_pFrameBuffer->GetWidth();
	int h = m_pFrameBuffer->GetHeight();

	// Заполняем структуру BITMAPINFO (см. MSDN), чтобы
	// перекинуть содержимое буфера кадра на экран без создания дополнительных bitmap-ов
	BITMAPINFO bmpInfo;
	memset(&bmpInfo, 0, sizeof(bmpInfo));
	BITMAPINFOHEADER& hdr = bmpInfo.bmiHeader;
	hdr.biSize = sizeof(hdr);
	hdr.biWidth = w;
	// По умолчанию BMP файл хранится "вверх ногами" (сначала нижний ряд пикселей)
	// Чтобы использовать привычный вариант хранения пикселей высота должна быть отрицательной
	hdr.biHeight = -h;
	hdr.biPlanes = 1; // Количество цветовых плоскостей в изображении
	hdr.biBitCount = sizeof(std::uint32_t) << 3; // Цвет каждого пикселя занимает 32 бита
	hdr.biCompression = BI_RGB;
	hdr.biSizeImage = w * h * static_cast<int>(sizeof(std::uint32_t));

	// Используя функцию SetDIBitsToDevice (см. MSDN) перекидываем массив пикселей
	// из памяти в контекст устройства
	dc.SetDIBitsToDevice(
		x, y, // Координаты вывода внутри контекста устройства
		w, h, // Ширина и высота изображений
		0, 0, // Координаты рисуемой области изображения
		0, h, // Номер начальной строки и количество строк
		m_pFrameBuffer->GetPixels(), // Адрес пикселей
		&bmpInfo, // Адрес информации о пикселях
		DIB_RGB_COLORS); // сигнализируем о том, что значения в таблице
	// bmpInfo.bmiColors интерпретируются как RGB значения,
	// а не индексы логической палитры
}

LRESULT CRaytraceView::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// Запускаем процесс визуализации
	m_renderer.Render(m_scene, m_context, *m_pFrameBuffer);

	// Инициализируем таймер для отображения построенного изображения
	SetTimer(FRAMEBUFFER_UPDATE_TIMER, 500);
	return 0;
}

bool CRaytraceView::UpdateFrameBuffer()
{
	// Инициируем перерисовку окна
	RedrawWindow();

	unsigned totalChunks = 0;
	unsigned renderedChunks = 0;

	// Возвращаем true, если изображение в буфере кадра полностью построено
	return m_renderer.GetProgress(renderedChunks, totalChunks);
}

CSceneObject& CRaytraceView::AddTorus(IShader const& shader, double radius, double smallRadius, CMatrix4d const& transform)
{
	const auto& Torus = *m_geometryObjects.emplace_back(
		std::make_unique<CTorus>(radius, smallRadius, transform));

	return AddSceneObject(Torus, shader);
}

CSceneObject& CRaytraceView::AddHyperbolicParaboloid(IShader const& shader, CMatrix4d const& transform)
{
	const auto& hyperbolic = *m_geometryObjects.emplace_back(
		std::make_unique<HyperbolicParaboloid>(transform));
	return AddSceneObject(hyperbolic, shader);
}

CSceneObject& CRaytraceView::AddParaboloid(IShader const& shader, CMatrix4d const& transform)
{
	const auto& parabaloid = *m_geometryObjects.emplace_back(
		std::make_unique<CParaboloid>(transform));

	return AddSceneObject(parabaloid, shader);
}

CSceneObject& CRaytraceView::AddSphere(IShader const& shader, double radius, CVector3d const& center, CMatrix4d const& transform)
{
	const auto& sphere = *m_geometryObjects.emplace_back(
		std::make_unique<CSphere>(radius, center, transform));

	return AddSceneObject(sphere, shader);
}

CSceneObject& CRaytraceView::AddConicCylinder(IShader const& shader, double height, double baseRadius, double capRadius, CMatrix4d const& transform)
{
	const auto& conicCylinder = *m_geometryObjects.emplace_back(
		std::make_unique<CConicCylinder>(height, baseRadius, capRadius, transform));
	return AddSceneObject(conicCylinder, shader);
}

CSceneObject& CRaytraceView::AddPlane(IShader const& shader, double a, double b, double c, double d, CMatrix4d const& transform)
{
	const auto& plane = *m_geometryObjects.emplace_back(
		std::make_unique<CPlane>(a, b, c, d, transform));
	return AddSceneObject(plane, shader);
}

CSceneObject& CRaytraceView::AddSceneObject(IGeometryObject const& object, IShader const& shader)
{
	auto obj = std::make_shared<CSceneObject>(object, shader);
	m_scene.AddObject(obj);
	return *obj;
}

CSimpleDiffuseShader& CRaytraceView::CreateSimpleDiffuseShader(CSimpleMaterial const& material)
{
	auto shader = std::make_unique<CSimpleDiffuseShader>(material);
	auto& shaderRef = *shader;
	m_shaders.emplace_back(std::move(shader));
	return shaderRef;
}

CCheckerShader& CRaytraceView::CreateCheckerShader(CMatrix4d const& textureTransform)
{
	auto shader = std::make_unique<CCheckerShader>(textureTransform);
	auto& shaderRef = *shader;
	m_shaders.emplace_back(std::move(shader));
	return shaderRef;
}

LRESULT CRaytraceView::OnTimer(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	WPARAM timerId = wParam;
	switch (timerId)
	{
	case FRAMEBUFFER_UPDATE_TIMER:
		// Если выяснилось, что изображение построено, выключаем таймер,
		// перекидывающий буфер кадра в окно
		if (UpdateFrameBuffer())
		{
			KillTimer(FRAMEBUFFER_UPDATE_TIMER);
		}
		break;
	}

	return 0;
}
