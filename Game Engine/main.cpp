#include "D3D Framework.h"

using namespace D3D11Framework;

class MyInput : public InputListener
{
public:
	bool KeyPressed(const KeyEvent& arg)
	{
		printf("key press %c\n", arg.wc);
		return false;
	}
	bool MouseMove(const MouseEvent& arg)
	{
		printf("mouse %d - %d\n", arg.x, arg.y);
		return false;
	}
};

class MyRender : public Render
{
public:
	bool Init(HWND nwnd) { return true; }
	bool Draw() { return true; }
	void Close() {}
};

int main()
{
	Framework framework;

	MyRender* render = new MyRender();
	MyInput* input = new MyInput();

	framework.SetRender(render);
	framework.Init();
	framework.AddInputListener(input);

	framework.Run();

	framework.Close();

	return 0;
}