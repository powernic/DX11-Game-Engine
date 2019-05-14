#include "MyRender.h"

// описание вершины
struct Vertex
{
	XMFLOAT3 pos;
	XMFLOAT2 tex;
};

// буфер констант
struct WVPBuffer
{
	XMMATRIX WVP;
};


struct TranslateBuffer
{
	float translation;
	XMFLOAT3 padding;	// не используется
};

MyRender::MyRender()
{
	m_vertexBuffer = nullptr;
	m_indexBuffer = nullptr;
	m_constMatrixBuffer = nullptr;
	m_shader = nullptr;
	m_translateBuffer = nullptr;
}

bool MyRender::Init()
{

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
	m_indexBuffer = Buffer::CreateIndexBuffer(m_pd3dDevice, sizeof(DWORD) * 36, false, indices);
	m_vertexBuffer = Buffer::CreateVertexBuffer(m_pd3dDevice, sizeof(Vertex) * 24, false, vert);
	m_constMatrixBuffer = Buffer::CreateConstantBuffer(m_pd3dDevice, sizeof(WVPBuffer), false);
	m_translateBuffer = Buffer::CreateConstantBuffer(m_pd3dDevice, sizeof(TranslateBuffer), false);

	// инициализируем шейдер и входной формат
	m_shader = new Shader(this);
	m_shader->AddInputElementDesc("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	m_shader->AddInputElementDesc("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	if (!m_shader->CreateShader(L"shader.vs", L"shader.ps"))
		return false;
	// добавляем текстуры
	if (!m_shader->AddTexture(L"image.jpg"))
		return false;


	// настраиваем камеру
	XMVECTOR camPosition = XMVectorSet(0.0f, 1.5f, -3.5f, 0.0f);
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

	WVPBuffer cb;
	cb.WVP = XMMatrixTranspose(camView*m_Projection);
	m_pImmediateContext->UpdateSubresource( m_constMatrixBuffer, 0, NULL, &cb, 0, 0 );
	m_pImmediateContext->VSSetConstantBuffers(0, 1, &m_constMatrixBuffer);

	static float textureTranslation = 0.0f;
	textureTranslation += 0.0001f;
	if(textureTranslation > 1.0f)
		textureTranslation -= 1.0f;


	TranslateBuffer tb;
	tb.translation = textureTranslation;
	m_pImmediateContext->UpdateSubresource( m_translateBuffer, 0, NULL, &tb, 0, 0 );
	m_pImmediateContext->PSSetConstantBuffers(0, 1, &m_translateBuffer);
			
	m_shader->Draw();
	m_pImmediateContext->DrawIndexed(6, 0, 0);

	return true;
} 

void MyRender::Close()
{
	_RELEASE(m_vertexBuffer);
	_RELEASE(m_indexBuffer);
	_RELEASE(m_constMatrixBuffer);
	_CLOSE(m_shader);
}