#pragma once

#include "D3D Framework.h" 

using namespace D3D11Framework;

class MyRender : public Render
{
public:
	MyRender();
	bool Init(HWND nwnd);
	bool Draw();
	void Close();
private:
	ID3D11Buffer* IndexBuffer;
	ID3D11Buffer* VertBuffer;
	ID3D11Buffer* constMatrixBuffer;
	ID3D11Buffer* constLightBuffer;
	XMMATRIX camView;
	Shader* shader;

	BitmapFont* m_font;
	Text* text1;
	BitmapFont* m_font2;
	Text* text2;
};