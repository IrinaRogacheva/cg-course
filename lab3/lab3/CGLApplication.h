#pragma once
#include <gl/glut.h>
#include <functional>

class CGLApplication
{
public:
	void MainLoop(void);
protected:
	CGLApplication(
		const char* title,
		int width = 0,
		int height = 0,
		bool needDepth = true,
		bool needStencil = false
	);
	//virtual ~CGLApplication(void);

	virtual void OnInit();
	// ������-����������� ������� ����� ���� ����������� � ������-����������
	virtual void OnDisplay() = 0;// ������ ����� ������ ������ ���� ����������
	virtual void OnReshape(int width, int height);
	virtual void OnKeyboard(unsigned char key, int x, int y);
	virtual void OnMouse(int button, int state, int x, int y);
	virtual void OnMotion(int x, int y);
	virtual void OnIdle();

	// ���������� ����������� ����������� � ����
	void PostRedisplay();

	// ��������� ����������� �������
	//template<class T>
	void SetTimer(int milliseconds, void (*func)(int value), int value)
	{
		//(static_cast<T*>(this)->*func)(0);
		// ������������� ������
		//m_pApplication->SetTimer(milliseconds, func, value);
		glutTimerFunc(milliseconds, func, value);
	}
	//virtual void Callback(int value);
private:
	void InitEventHandlers();
	static void DisplayHandler();
	static void ReshapeHandler(int width, int height);
	static void KeyboardHandler(unsigned char key, int x, int y);
	static void MouseHandler(int button, int state, int x, int y);
	static void MotionHandler(int x, int y);
	static void IdleHandler();
	static CGLApplication* m_pApplication;
};
