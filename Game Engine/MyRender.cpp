#include "MyRender.h"
#include "BitmapFont.h"
#include "Text.h"

 
MyRender::MyRender()
{
	m_font = nullptr;
	text1 = nullptr;
	text2 = nullptr;
	text3 = nullptr;
}

bool MyRender::Init(HWND hwnd)
{
	D3D11_BLEND_DESC bSDesc;
	ZeroMemory(&bSDesc, sizeof(D3D11_BLEND_DESC));
	bSDesc.RenderTarget[0].BlendEnable = TRUE;
	bSDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	bSDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	bSDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	bSDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	bSDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	bSDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	bSDesc.RenderTarget[0].RenderTargetWriteMask = 0x0f;
	HRESULT result = m_pd3dDevice->CreateBlendState(&bSDesc, &m_alphaEnableBlendingState);
	if (FAILED(result))
		return false;

	bSDesc.RenderTarget[0].BlendEnable = FALSE;
	result = m_pd3dDevice->CreateBlendState(&bSDesc, &m_alphaDisableBlendingState);
	if (FAILED(result))
		return false;

	m_Ortho = XMMatrixOrthographicLH(800.0f, 600.0f, 0.0f, 1000.0f);

	m_font = new BitmapFont(this);
	if (!m_font->Init("font.fnt"))
		return false;

	text1 = new Text(this, m_font);
	text1->Init(L"Hello", 800, 600);

	text2 = new Text(this, m_font);
	text2->Init(L"World", 800, 600);

	text3 = new Text(this, m_font);
	text3->Init(L"Привет Мир", 800, 600);

	return true;
}

bool MyRender::Draw()
{
	TurnZBufferOff();
	TurnOnAlphaBlending();

	text1->Render(1.0, 1.0, 0.0, 100, 100);
	text2->Render(1.0, 0.0, 1.0, 290, 100);
	text3->Render(0.0, 1.0, 1.0, 100, 180);

	TurnOffAlphaBlending();
	TurnZBufferOn();
	return true;
} 

void MyRender::Close()
{
	_RELEASE(m_alphaEnableBlendingState);
	_RELEASE(m_alphaDisableBlendingState);
	_CLOSE(text1);
	_CLOSE(text2);
	_CLOSE(text3);
	_CLOSE(m_font);
}

void MyRender::TurnOnAlphaBlending()
{
	float blendFactor[4];
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;
	m_pImmediateContext->OMSetBlendState(m_alphaEnableBlendingState, blendFactor, 0xffffffff);
}

void MyRender::TurnOffAlphaBlending()
{
	float blendFactor[4];
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;
	m_pImmediateContext->OMSetBlendState(m_alphaDisableBlendingState, blendFactor, 0xffffffff);
}