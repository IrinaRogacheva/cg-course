#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include "MainFrame.h"
#include "wx/wx.h"

class CApp : public wxApp
{
public:
	virtual bool OnInit();
private:
	MainFrame* m_frame;
};