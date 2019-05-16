#pragma once

#include "D3D Framework.h"
#include "Light.h"

using namespace D3D11Framework;

class MyRender;

class ShadowShader
{
public:
	ShadowShader(MyRender* render);

	bool Init();
	void Close();
	void Render(int indexCount, CXMMATRIX worldMatrix, CXMMATRIX WVP, CXMMATRIX WVPlight, ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* depthMapTexture, Light& light, CXMMATRIX WVPlight2, ID3D11ShaderResourceView* depthMapTexture2, Light& light2);

private:
	MyRender* m_render;
	Shader* m_shader;
	ID3D11SamplerState* m_sampleStateWrap;
	ID3D11SamplerState* m_sampleStateClamp;
	ID3D11Buffer* m_matrixBuffer;
	ID3D11Buffer* m_lightBuffer;
	ID3D11Buffer* m_lightBuffer2;
};