#pragma once

#include "D3D Framework.h"
#include "MyRender.h"

using namespace D3D11Framework;

class MyInput : public InputListener
{
public:
	MyInput(MyRender* render);

	bool KeyPressed(const KeyEvent& arg);
	bool KeyReleased(const KeyEvent& arg);

private:
	MyRender* m_render;
};