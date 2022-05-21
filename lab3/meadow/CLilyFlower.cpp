#include "CLilyFlower.h"

CLilyFlower::CLilyFlower(int xCenter, int yCenter, int width, int height)
	: m_xCenter(xCenter)
	, m_yCenter(yCenter)
    , m_width(width)
    , m_height(height)
{
}

void CLilyFlower::Draw() const
{
    glColor3ub(68, 173, 47);

    //стебель
    glLineWidth(2);

    glBegin(GL_LINES);
    glVertex2f(m_xCenter + m_width * 0.3, m_yCenter + m_height); //нижняя точка цветка
    glVertex2f(m_xCenter - m_width * 0.2, m_yCenter + m_height * 1.8); //низ стебля
    glEnd();
    glLineWidth(1);
    
    //левая травинка
    glBegin(GL_TRIANGLES);
    glVertex2f(m_xCenter - m_width * 0.2, m_yCenter + m_height * 1.8); //низ стебля
    glVertex2f(m_xCenter - m_width * 1.7, m_yCenter + m_height * 0.5); //верх травы
    glVertex2f(m_xCenter - m_width * 0.6, m_yCenter + m_height * 1.8); //низ травы
    glEnd();

    //правая травинка
    glBegin(GL_TRIANGLES);
    glVertex2f(m_xCenter - m_width * 0.2, m_yCenter + m_height * 1.8); //низ стебля
    glVertex2f(m_xCenter + m_width * 1.7, m_yCenter + m_height * 0.5); //верх травы
    glVertex2f(m_xCenter + m_width * 0.3, m_yCenter + m_height * 1.8); //низ травы
    glEnd();

    //правая травинка 2
    glBegin(GL_TRIANGLES);
    glVertex2f(m_xCenter + m_width * 0.3, m_yCenter + m_height * 1.8); //низ левого края травы
    glVertex2f(m_xCenter + m_width * 3, m_yCenter + m_height * 0.5); //верх травы
    glVertex2f(m_xCenter + m_width * 0.7, m_yCenter + m_height * 1.8); //низ травы
    glEnd();

    glColor3ub(239, 245, 237);
    //нижняя часть цветка
    glBegin(GL_TRIANGLES);
    glVertex2f(m_xCenter, m_yCenter + m_height * 0.8); //левая точка
    glVertex2f(m_xCenter + m_width * 0.3, m_yCenter + m_height); //нижняя точка
    glVertex2f(m_xCenter + m_width * 0.7, m_yCenter + m_height * 0.8); //правая точка
    glEnd();

    //верхняя часть цветка
    glBegin(GL_TRIANGLES);
    glVertex2f(m_xCenter, m_yCenter + m_height * 0.8); //левая точка
    glVertex2f(m_xCenter + m_width, m_yCenter); //верхняя точка
    glVertex2f(m_xCenter + m_width * 0.7, m_yCenter + m_height * 0.8); //правая точка
    glEnd();
}

void CLilyFlower::DrawAtTheOrigin() const
{/*
    glColor3ub(68, 173, 47);
    triangle(-.5, -1.0, -.45, -1.0, -.35, -.65);
    glBegin(GL_TRIANGLES);
    glVertex2f(a, b);
    glVertex2f(c, d);
    glVertex2f(e, f);
    glEnd();


    triangle(-.5, -1.0, -.45, -1.0, -.6, -.65);

    triangle(-.2, -1.0, -.15, -1.0, -.35, -.65);
    triangle(-.2, -1.0, -.15, -1.0, -.0, -.65);

    triangle(-.45, -1.0, -.4, -1.0, -.2, -.65);

    triangle(.15, -1.0, .2, -1.0, .35, -.65);
    triangle(.15, -1.0, .2, -1.0, .05, -.65);

    triangle(.45, -1.0, .5, -1.0, .35, -.65);

    triangle(.55, -1.0, .6, -1.0, .55, -.65);
    triangle(.6, -1.0, .65, -1.0, .8, -.65);

    triangle(.85, -1.0, .9, -1.0, 1.0, -.65);

    glLineWidth(2);

    glColor3ub(68, 173, 47);
    line(-.5, -1.0, -.43, -.65);
    line(-.3, -1.0, -.25, -.65);
    line(-.15, -1.0, -.1, -.65);
    line(.0, -1.0, .1, -.65);

    line(.25, -1.0, .2, -.6);
    line(.28, -1.0, .32, -.65);
    line(.35, -1.0, .4, -.65);
    line(.45, -1.0, .58, -.62);
    line(.53, -1.0, .5, -.65);
    line(.6, -1.0, .7, -.65);
    line(.75, -1.0, .85, -.65);
    */
    ///////////white flower

    glColor3ub(239, 245, 237);
    glTranslated(.032, .0, 0);
    glBegin(GL_POLYGON);
    glVertex2f(-.5, -.7); glVertex2f(-.48, -.73); glVertex2f(-.45, -.7);
    glVertex2f(-.43, -.55);
    glEnd();


    glTranslated(.187, .02, 0);
    glBegin(GL_POLYGON);
    glVertex2f(-.5, -.7); glVertex2f(-.48, -.73); glVertex2f(-.45, -.7);
    glVertex2f(-.43, -.55);
    glEnd();

    glTranslated(.15, .0, 0);
    glBegin(GL_POLYGON);
    glVertex2f(-.5, -.7); glVertex2f(-.48, -.73); glVertex2f(-.45, -.7);
    glVertex2f(-.45, -.55);
    glEnd();

    glTranslated(.2, .0, 0);
    glBegin(GL_POLYGON);
    glVertex2f(-.5, -.7); glVertex2f(-.486, -.73); glVertex2f(-.46, -.7);
    glVertex2f(-.44, -.55);
    glEnd();

    glTranslated(.12, .04, 0);
    glBegin(GL_POLYGON);
    glVertex2f(-.5, -.7); glVertex2f(-.48, -.73); glVertex2f(-.46, -.7);
    glVertex2f(-.52, -.55);
    glEnd();

    glTranslated(.11, .0, 0);
    glBegin(GL_POLYGON);
    glVertex2f(-.5, -.7); glVertex2f(-.48, -.73); glVertex2f(-.46, -.7);
    glVertex2f(-.46, -.55);
    glEnd();

    glTranslated(.07, -.05, 0);
    glBegin(GL_POLYGON);
    glVertex2f(-.5, -.7); glVertex2f(-.48, -.73); glVertex2f(-.46, -.7);
    glVertex2f(-.46, -.58);
    glEnd();

    glTranslated(.115, .02, 0);
    glBegin(GL_POLYGON);
    glVertex2f(-.5, -.7); glVertex2f(-.48, -.73); glVertex2f(-.46, -.7);
    glVertex2f(-.5, -.58);
    glEnd();

    glTranslated(.058, .02, 0);
    glBegin(GL_POLYGON);
    glVertex2f(-.48, -.7); glVertex2f(-.48, -.73); glVertex2f(-.44, -.7);
    glVertex2f(-.4, -.55);
    glEnd();

    glTranslated(.132, .02, 0);
    glBegin(GL_POLYGON);
    glVertex2f(-.48, -.7); glVertex2f(-.48, -.73); glVertex2f(-.44, -.7);
    glVertex2f(-.4, -.57);
    glEnd();

    glTranslated(.148, .0, 0);
    glBegin(GL_POLYGON);
    glVertex2f(-.48, -.7); glVertex2f(-.48, -.73); glVertex2f(-.44, -.7);
    glVertex2f(-.4, -.55);
    glEnd();

    glLoadIdentity();
}
