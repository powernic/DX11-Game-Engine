#include "MyRender.h"
 
MyRender::MyRender()
{
	m_mesh = nullptr;
	Transparency = nullptr;
}

bool MyRender::Init(HWND hwnd)
{
	XMVECTOR Eye = XMVectorSet(0.0f, 0.0f, -2.8f, 0.0f);
	XMVECTOR At = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	m_view = XMMatrixLookAtLH(Eye, At, Up);

	m_mesh = new StaticMesh();
	if (!m_mesh->Init(this, L"mesh.ms3d"))
		return false;


	D3D11_RENDER_TARGET_BLEND_DESC rtbd;
	ZeroMemory(&rtbd, sizeof(rtbd));
	rtbd.BlendEnable = true;
	rtbd.SrcBlend = D3D11_BLEND_SRC_COLOR;
	rtbd.DestBlend = D3D11_BLEND_BLEND_FACTOR;
	rtbd.BlendOp = D3D11_BLEND_OP_ADD;
	rtbd.SrcBlendAlpha = D3D11_BLEND_ONE;
	rtbd.DestBlendAlpha = D3D11_BLEND_ZERO;
	rtbd.BlendOpAlpha = D3D11_BLEND_OP_ADD;
	rtbd.RenderTargetWriteMask = D3D10_COLOR_WRITE_ENABLE_ALL;

	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));
	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.RenderTarget[0] = rtbd;
	m_pd3dDevice->CreateBlendState(&blendDesc, &Transparency);

	return true;
}

bool MyRender::Draw()
{
	m_pImmediateContext->OMSetBlendState(0, 0, 0xffffffff);
	m_mesh->Identity();
	m_mesh->Rotate(1.35f, 0.0, 1.0, 0.0);
	m_mesh->Translate(-40, 0.0, 0.0);
	m_mesh->Scale(0.03f, 0.03f, 0.03f);
	m_mesh->Draw(m_view);


	float blendFactor[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	m_pImmediateContext->OMSetBlendState(Transparency, blendFactor, 0xffffffff);
	m_mesh->Identity();
	m_mesh->Rotate(1.35f, 0.0, 1.0, 0.0);
	m_mesh->Translate(40, 0.0, 0.0);
	m_mesh->Scale(0.03f, 0.03f, 0.03f);
	m_mesh->Draw(m_view);
	return true;
} 

void MyRender::Close()
{
	_CLOSE(m_mesh);
	_RELEASE(Transparency);
}