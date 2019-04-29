#include "MyRender.h"
#include "Image.h"
 
MyRender::MyRender()
{
	m_img = nullptr;
}

bool MyRender::Init(HWND hwnd)
{
	m_Ortho = XMMatrixOrthographicLH(640.0f, 480.0f, 0.0f, 1000.0f);

	m_img = new Image();
	if (!m_img->Init(this, 640, 480, L"texture.jpg", 256, 256))
		return false;

	return true;
}

bool MyRender::Draw()
{
	TurnZBufferOff();
	m_img->Render(100, 100);
	return true;
} 

void MyRender::Close()
{
	_CLOSE(m_img);
}