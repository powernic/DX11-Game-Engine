#pragma once

#include "D3D Framework.h" 

using namespace D3D11Framework;

class BitmapFont;
class Text;

class MyRender : public Render
{
public:
	MyRender();
	bool Init(HWND nwnd);
	bool Draw();
	void Close();
	void TurnOnAlphaBlending();
	void TurnOffAlphaBlending();
private:
	friend BitmapFont;
	friend Text;

	BitmapFont* m_font;
	Text* text1;
	Text* text2;
	Text* text3;

	XMMATRIX m_Ortho;

	ID3D11BlendState* m_alphaEnableBlendingState;
	ID3D11BlendState* m_alphaDisableBlendingState;
};