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

MyRender::MyRender()
{
	m_vb_grid = nullptr;
	m_ib_grid = nullptr;
	m_texture_grid = nullptr;
	m_vb_billboard = nullptr;
	m_ib_billboard = nullptr;
	m_texture_billboard = nullptr;
	m_constMatrixBuffer = nullptr;
	m_shader = nullptr;

	m_leftcam = m_rightcam = false;
}

bool MyRender::Init()
{
	// геометрия (plane)
	Vertex vert1[] =
	{
		{ XMFLOAT3(-5.0f, 0.0f, 5.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(5.0f, 0.0f, 5.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(-5.0f, 0.0f, -5.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(-5.0f, 0.0f, -5.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(5.0f, 0.0f, 5.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(5.0f, 0.0f, -5.0f), XMFLOAT2(1.0f, 1.0f) }
	};
	unsigned long indices1[6] =
	{
		0,1,2,
		3,4,5
	};

	// создаем вершинный и индексный буферы
	m_vb_grid = Buffer::CreateVertexBuffer(m_pd3dDevice, sizeof(Vertex) * 6, false, &vert1);
	m_ib_grid = Buffer::CreateIndexBuffer(m_pd3dDevice, sizeof(unsigned long) * 6, false, &indices1);

	D3DX11CreateShaderResourceViewFromFile(m_pd3dDevice, L"grid01.png", NULL, NULL, &m_texture_grid, NULL);


	// геометрия (квадрат)
	Vertex vert2[] =
	{
		{ XMFLOAT3(-1.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(1.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(1.0f, -1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) }
	};
	unsigned long indices2[6] =
	{
		0,1,2,
		2,1,3
	};

	// создаем вершинный и индексный буферы
	m_vb_billboard = Buffer::CreateVertexBuffer(m_pd3dDevice, sizeof(Vertex) * 4, false, &vert2);
	m_ib_billboard = Buffer::CreateIndexBuffer(m_pd3dDevice, sizeof(unsigned long) * 6, false, &indices2);

	D3DX11CreateShaderResourceViewFromFile(m_pd3dDevice, L"image.png", NULL, NULL, &m_texture_billboard, NULL);

	// создаем константный буфер
	m_constMatrixBuffer = Buffer::CreateConstantBuffer(m_pd3dDevice, sizeof(WVPBuffer), false);

	// инициализируем шейдер и входной формат
	m_shader = new Shader(this);
	m_shader->AddInputElementDesc("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	m_shader->AddInputElementDesc("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	if (!m_shader->CreateShader(L"shader.vs", L"shader.ps"))
		return false;

	// настраиваем камеру
	m_cam.SetPos(0.0f, 0.1f, -8.0f);

	return true;
}
bool MyRender::Draw()
{
	m_cam.MoveLeft(m_leftcam);
	m_cam.MoveRight(m_rightcam);
	m_cam.Render();

	XMMATRIX camView = m_cam.GetViewMatrix();

	WVPBuffer cb;
	unsigned int stride = sizeof(Vertex);
	unsigned int offset = 0;

	// Рисуем сетку (plane)
	// установка позиции plane
	XMFLOAT3 modelPosition;
	modelPosition.x = 0.0f;
	modelPosition.y = -2.0f;
	modelPosition.z = 0.0f;
	XMMATRIX wldMatrix = XMMatrixTranslation(modelPosition.x, modelPosition.y, modelPosition.z);

	cb.WVP = XMMatrixTranspose(wldMatrix * camView * m_Projection);
	m_pImmediateContext->UpdateSubresource(m_constMatrixBuffer, 0, NULL, &cb, 0, 0);
	m_pImmediateContext->VSSetConstantBuffers(0, 1, &m_constMatrixBuffer);
	m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_pImmediateContext->IASetVertexBuffers(0, 1, &m_vb_grid, &stride, &offset);
	m_pImmediateContext->IASetIndexBuffer(m_ib_grid, DXGI_FORMAT_R32_UINT, 0);
	m_shader->Draw();
	m_pImmediateContext->PSSetShaderResources(0, 1, &m_texture_grid);
	m_pImmediateContext->DrawIndexed(6, 0, 0);
	// рисуем billboard

	// Установка позиции billboard
	modelPosition.x = 0.0f;
	modelPosition.y = 0.0f;
	modelPosition.z = -1.0f;

	// позиция камеры
	XMFLOAT3 camPos = m_cam.GetPos();

	// Вычисление угла на который нужно повернуть billboard чтобы он был направлен перпендикулярно направлению камеры
	float angle = atan2(modelPosition.x - camPos.x, modelPosition.z - camPos.z) * (180.0 / XM_PI);
	// перевод угла из градусов в радианы
	float rot = angle * 0.0174532925f;

	// вращаем мировую матрицу billboard
	wldMatrix = XMMatrixRotationY(rot);
	// перемещаем мировую матрицу billboard
	wldMatrix *= XMMatrixTranslation(modelPosition.x, modelPosition.y, modelPosition.z);

	cb.WVP = XMMatrixTranspose(wldMatrix * camView * m_Projection);
	m_pImmediateContext->UpdateSubresource(m_constMatrixBuffer, 0, NULL, &cb, 0, 0);
	m_pImmediateContext->VSSetConstantBuffers(0, 1, &m_constMatrixBuffer);
	m_pImmediateContext->IASetVertexBuffers(0, 1, &m_vb_billboard, &stride, &offset);
	m_pImmediateContext->IASetIndexBuffer(m_ib_billboard, DXGI_FORMAT_R32_UINT, 0);
	m_shader->Draw();
	m_pImmediateContext->PSSetShaderResources(0, 1, &m_texture_billboard);
	m_pImmediateContext->DrawIndexed(6, 0, 0);

	return true;
}

void MyRender::Close()
{
	_RELEASE(m_vb_grid);
	_RELEASE(m_ib_grid);
	_RELEASE(m_vb_billboard);
	_RELEASE(m_ib_billboard);
	_RELEASE(m_constMatrixBuffer);
	_RELEASE(m_texture_billboard);
	_RELEASE(m_texture_grid);
	_CLOSE(m_shader);
}