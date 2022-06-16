#include "stdafx.h"
#include "Window.h"
#include <glm/gtc/matrix_transform.hpp>

namespace
{
    const char EARTH_TEX_PATH[] = "textures/earth.jpg";
    const char SUN_TEX_PATH[] = "textures/sun.jpg";
    const char MOON_TEX_PATH[] = "textures/moon.jpg";
    const glm::vec4 BLACK = { 0, 0, 0, 1 };
    const float MATERIAL_SHININESS = 30.f;
    const glm::vec4 FADED_WHITE_RGBA = { 0.3f, 0.3f, 0.3f, 1.f };
    const float CAMERA_INITIAL_DISTANCE = 7.f;
    const float EARTH_ROTATION_SPEED = 30.f;
    const float SUN_ROTATION_SPEED = 10.f;
    const float MOON_ROTATION_SPEED = 0.1f;
    const float EARTH_INCLINATION_ANGLE = 23.43f;
    const float EARTH_ORBIT_RADIUS = -3.f;
    const float EARTH_ROTATION_SPEED_AROUND_SUN = 0.3f;
    const float MOON_ROTATION_SPEED_AROUND_EARTH = 10.f;
    const float MOON_ORBIT_RADIUS = -1.f;
    const unsigned SPHERE_PRECISION = 40;

    void SetupOpenGLState()
    {
        // включаем механизмы трёхмерного мира.
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glFrontFace(GL_CCW);
        glCullFace(GL_BACK);

        // включаем систему освещения
        glEnable(GL_LIGHTING);

        // включаем текстурирование в старом стиле (OpenGL 1.1)
        glEnable(GL_TEXTURE_2D);
    }

    glm::vec3 CalculateTranslationAtCircle(float radius, float alpha)
    {
        float x = radius;
        float y = 0.f;
        float z = radius;
        float deltaX = z * cos(alpha) - x * sin(alpha);
        float deltaZ = x * cos(alpha) + z * sin(alpha);
        return { deltaX, y, deltaZ };
    }
}

CWindow::CWindow()
    : m_camera(CAMERA_INITIAL_DISTANCE)
    , m_light(GL_LIGHT0)
    , m_sun(SPHERE_PRECISION, SPHERE_PRECISION)
    , m_earth(SPHERE_PRECISION, SPHERE_PRECISION)
    , m_moon(SPHERE_PRECISION, SPHERE_PRECISION)
{
    SetBackgroundColor(BLACK);

    m_earth.SetRotationSpeed(EARTH_ROTATION_SPEED);
    m_earth.SetInclinationAngle(EARTH_INCLINATION_ANGLE);
    m_sun.SetRotationSpeed(SUN_ROTATION_SPEED);

    const glm::vec4 WHITE_RGBA = { 1, 1, 1, 1 };
    m_light.SetDiffuse(WHITE_RGBA);
    m_light.SetAmbient(0.1f * WHITE_RGBA);
    m_light.SetSpecular(WHITE_RGBA);
    
}

void CWindow::OnWindowInit(const glm::ivec2& size)
{
    (void)size;
    SetupOpenGLState();

    m_pSkybox = std::make_unique<CSkybox>();

    CTexture2DLoader loader;
    loader.SetWrapMode(TextureWrapMode::REPEAT);
    m_pSunTexture = loader.Load(SUN_TEX_PATH);
    m_pEarthTexture = loader.Load(EARTH_TEX_PATH);
    m_pMoonTexture = loader.Load(MOON_TEX_PATH);
}

void CWindow::OnUpdateWindow(float deltaSeconds)
{
    m_earth.Update(deltaSeconds);
    m_sun.Update(deltaSeconds);
    m_pSkybox->Update(deltaSeconds);

    m_earthRotationAngleAroundSun = fmodf(m_earthRotationAngleAroundSun + EARTH_ROTATION_SPEED_AROUND_SUN * deltaSeconds, 360.f);
    m_moonRotationAngleAroundEarth = fmodf(m_moonRotationAngleAroundEarth + MOON_ROTATION_SPEED_AROUND_EARTH * deltaSeconds, 360.f);
}

void CWindow::OnDrawWindow(const glm::ivec2& size)
{
    SetupView(size);

    m_light.Setup();
    m_pSkybox->Draw();
    m_pSunTexture->DoWhileBinded([&] {
        glDisable(GL_LIGHTING);
        m_sun.Draw();
        glEnable(GL_LIGHTING);
        });
    glm::vec3 earthTranslateVector = CalculateTranslationAtCircle(EARTH_ORBIT_RADIUS, m_earthRotationAngleAroundSun);
    m_pEarthTexture->DoWhileBinded([&] {
        glPushMatrix();
        glTranslatef(earthTranslateVector.x, earthTranslateVector.y, earthTranslateVector.z);
        glScalef(0.6f, 0.6f, 0.6f);
        m_earth.Draw();
        glPopMatrix();
        });
    m_pMoonTexture->DoWhileBinded([&] {
        glPushMatrix();
        glm::vec3 moonTranslateVector = CalculateTranslationAtCircle(MOON_ORBIT_RADIUS, m_moonRotationAngleAroundEarth);
        glTranslatef(earthTranslateVector.x, earthTranslateVector.y, earthTranslateVector.z);
        glRotatef(m_moonRotationAngleAroundEarth, 0, 1, 0);
        glTranslatef(MOON_ORBIT_RADIUS, 0, MOON_ORBIT_RADIUS);
        glScalef(0.3f, 0.3f, 0.3f);
        m_moon.SetRotationAngle(180);
        m_moon.Draw();
        glPopMatrix();
        });
}

void CWindow::SetupView(const glm::ivec2& size)
{
    glViewport(0, 0, size.x, size.y);

    // Матрица вида возвращается камерой и составляет
    // начальное значение матрицы GL_MODELVIEW.
    glLoadMatrixf(glm::value_ptr(m_camera.GetMatrix()));

    // Матрица перспективного преобразования вычисляется функцией
    // glm::perspective, принимающей угол обзора, соотношение ширины
    // и высоты окна, расстояния до ближней и дальней плоскостей отсечения.
    constexpr float fieldOfView = glm::radians(70.f);
    const float aspect = float(size.x) / float(size.y);
    const float zNear = 0.01f;
    const float zFar = 100.f;
    const glm::mat4 proj = glm::perspective(fieldOfView, aspect, zNear, zFar);
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(glm::value_ptr(proj));
    glMatrixMode(GL_MODELVIEW);
}

void CWindow::OnDragBegin(int button, const glm::vec2& pos)
{
    if (button == SDL_BUTTON_LEFT)
    {
        m_leftButtonPressed = true;
    }
}

void CWindow::OnDragMotion(const glm::vec2& pos)
{
    if (m_leftButtonPressed)
    {
        const auto windowSize = GetWindowSize();

        const auto mouseDelta = pos - m_mousePos;
        const float xAngle = mouseDelta.y * M_PI / windowSize.y;
        const float yAngle = mouseDelta.x * M_PI / windowSize.x;
        m_camera.Rotate(xAngle, yAngle);
    }
    m_mousePos = pos;
}

void CWindow::OnDragEnd(const glm::vec2& pos)
{
    if (m_leftButtonPressed)
    {
        m_leftButtonPressed = false;
    }
}

