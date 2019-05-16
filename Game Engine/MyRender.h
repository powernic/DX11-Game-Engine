#pragma once

#include "D3D Framework.h"
#include "Light.h"
#include "DepthShader.h"
#include "RenderTarget.h"
#include "ShadowShader.h"
#include "SoftShadowShader.h"

using namespace D3D11Framework;

class MyRender : public Render
{
public:
	MyRender();
	bool Init();
	bool Draw();
	void Close();

	void RenderSceneToTexture(RenderTarget* RenderTexture, Light& light);
	void RenderSceneToWindow();
	bool RenderHorizontalBlurToTexture();
	bool RenderVerticalBlurToTexture();
private:
	friend DepthShader;
	friend RenderTarget;
	friend ShadowShader;
	friend SoftShadowShader;
	friend class MyInput;

	Camera m_cam;
	Light m_Light;
	Light m_Light2;

	RenderTarget* m_RenderTexture;
	RenderTarget* m_RenderTexture2;
	DepthShader* m_DepthShader;
	ShadowShader* m_ShadowShader;
	SoftShadowShader* m_SoftShadowShader;
	HorizontalBlurShader* m_HorizontalBlurShader;
	VerticalBlurShader* m_VerticalBlurShader;

	D3D11_VIEWPORT m_viewport;

	bool m_key_up;
	bool m_key_down;
	bool m_key_left;
	bool m_key_right;
	bool m_key_a;
	bool m_key_z;
	bool m_key_s;
	bool m_key_x;

	// ��������� � ��������� ������ ��� ����� � �����������
	ID3D11Buffer* m_vb_ground;
	ID3D11Buffer* m_ib_ground;
	ID3D11Buffer* m_vb_box;
	ID3D11Buffer* m_ib_box;

	// ��������
	ID3D11ShaderResourceView* m_texture_ground;
	ID3D11ShaderResourceView* m_texture_box1;
	ID3D11ShaderResourceView* m_texture_box2;

	// ������� ������� � ������� �����
	XMFLOAT3 m_posbox1;
	XMFLOAT3 m_posbox2;
};