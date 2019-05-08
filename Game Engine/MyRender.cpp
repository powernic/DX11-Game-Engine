#include "MyRender.h" 

// описание вершины
struct Vertex
{
	XMFLOAT3 pos;
	XMFLOAT2 tex;
};

// буфер констант
struct ConstantBuffer
{
	XMMATRIX WVP;
};

MyRender::MyRender()
{
	m_vertexBuffer = nullptr;
	m_indexBuffer = nullptr;
	m_constantBuffer = nullptr;
	m_textures[0] = nullptr;
	m_textures[1] = nullptr;
	m_sampleState = nullptr;
	m_shader = nullptr;
}

bool MyRender::Init()
{
	// Загружаем две текстуры
	HRESULT result;
	result = D3DX11CreateShaderResourceViewFromFile(m_pd3dDevice, L"tex1.jpg", NULL, NULL, &m_textures[0], NULL);
	if (FAILED(result))
		return false;
	result = D3DX11CreateShaderResourceViewFromFile(m_pd3dDevice, L"tex2.jpg", NULL, NULL, &m_textures[1], NULL);
	if (FAILED(result))
		return false;

	// геометрия (квадрат)
	Vertex vert[] =
	{
		{ XMFLOAT3(-1.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(1.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(1.0f, -1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) }
	};
	unsigned long indices[6] =
	{
		0,1,2,
		2,1,3
	};

	// создаем три буфера
	m_vertexBuffer = Buffer::CreateVertexBuffer(m_pd3dDevice, sizeof(Vertex) * 4, false, &vert);
	m_indexBuffer = Buffer::CreateIndexBuffer(m_pd3dDevice, sizeof(unsigned long) * 6, false, &indices);
	m_constantBuffer = Buffer::CreateConstantBuffer(m_pd3dDevice, sizeof(ConstantBuffer), false);
	
	// инициализируем шейдер и входной формат
	m_shader = new Shader(this);
	m_shader->AddInputElementDesc("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	m_shader->AddInputElementDesc("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	if (!m_shader->CreateShader(L"multitexture.vs", L"multitexture.ps"))
		return false;

	// создаем семплер (в шейдере он не создан, потому что туда не была загружена текстура)
	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	if (FAILED(m_pd3dDevice->CreateSamplerState(&samplerDesc, &m_sampleState)))
		return false;

	// настраиваем камеру
	XMVECTOR camPosition = XMVectorSet(0.0f, 0.0f, -1.5f, 0.0f);
	XMVECTOR camTarget = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR camUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	camView = XMMatrixLookAtLH(camPosition, camTarget, camUp);

	return true;
}

bool MyRender::Draw()
{
	unsigned int stride = sizeof(Vertex);
	unsigned int offset = 0;
	m_pImmediateContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
	m_pImmediateContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	ConstantBuffer cb;
	cb.WVP = XMMatrixTranspose(camView * m_Projection);
	m_pImmediateContext->UpdateSubresource(m_constantBuffer, 0, NULL, &cb, 0, 0);
	m_pImmediateContext->VSSetConstantBuffers(0, 1, &m_constantBuffer);
	m_pImmediateContext->PSSetSamplers(0, 1, &m_sampleState);

	// передаем в шейдер две текстуры
	m_pImmediateContext->PSSetShaderResources(0, 2, m_textures);

	m_shader->Draw();
	m_pImmediateContext->DrawIndexed(6, 0, 0);

	return true;
}

void MyRender::Close()
{
	_RELEASE(m_vertexBuffer);
	_RELEASE(m_indexBuffer);
	_RELEASE(m_constantBuffer);
	_RELEASE(m_textures[0]);
	_RELEASE(m_textures[1]);
	_RELEASE(m_sampleState);
	_CLOSE(m_shader);
}