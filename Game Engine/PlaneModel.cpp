#include "PlaneModel.h"
#include <fstream>
#include "MyRender.h"
using namespace std;

const int NUM_LIGHTS = 4;

struct VertexType
{
	XMFLOAT3 position;
	XMFLOAT2 texture;
	XMFLOAT3 normal;
};

struct MatrixBufferType
{
	XMMATRIX world;
	XMMATRIX view;
	XMMATRIX projection;
};

struct LightColorBufferType
{
	XMFLOAT4 diffuseColor[NUM_LIGHTS];
};

struct LightPositionBufferType
{
	XMFLOAT4 lightPosition[NUM_LIGHTS];
};

PlaneModel::PlaneModel()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_model = 0;
}

bool PlaneModel::Init(MyRender* render)
{
	m_render = render;

	if (!LoadModel("plane01.txt"))
		return false;

	if (!InitializeBuffers())
		return false;

	// инициализируем шейдер и входной формат
	m_shader = new Shader(m_render);
	m_shader->AddInputElementDesc("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	m_shader->AddInputElementDesc("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	m_shader->AddInputElementDesc("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	if (!m_shader->CreateShader(L"shader.vs", L"shader.ps"))
		return false;
	if (!m_shader->AddTexture(L"tex2.jpg"))
		return false;

	// создаем константный буфер
	m_constMatrixBuffer = Buffer::CreateConstantBuffer(m_render->m_pd3dDevice, sizeof(MatrixBufferType), false);
	m_lightColorBuffer = Buffer::CreateConstantBuffer(m_render->m_pd3dDevice, sizeof(LightColorBufferType), false);
	m_lightPositionBuffer = Buffer::CreateConstantBuffer(m_render->m_pd3dDevice, sizeof(LightPositionBufferType), false);

	return true;
}

bool PlaneModel::LoadModel(char* filename)
{
	ifstream fin;
	char input;

	fin.open(filename);
	if (fin.fail())
		return false;

	fin.get(input);
	while (input != ':')
		fin.get(input);

	fin >> m_vertexCount;

	m_indexCount = m_vertexCount;

	m_model = new ModelType[m_vertexCount];
	if (!m_model)
		return false;

	fin.get(input);
	while (input != ':')
		fin.get(input);

	fin.get(input);
	fin.get(input);

	for (int i = 0; i < m_vertexCount; i++)
	{
		fin >> m_model[i].x >> m_model[i].y >> m_model[i].z;
		fin >> m_model[i].tu >> m_model[i].tv;
		fin >> m_model[i].nx >> m_model[i].ny >> m_model[i].nz;
	}

	fin.close();

	return true;
}

bool PlaneModel::InitializeBuffers()
{
	VertexType* vertices = new VertexType[m_vertexCount];
	if (!vertices)
		return false;

	unsigned long* indices = new unsigned long[m_indexCount];
	if (!indices)
		return false;

	for (int i = 0; i < m_vertexCount; i++)
	{
		vertices[i].position = XMFLOAT3(m_model[i].x, m_model[i].y, m_model[i].z);
		vertices[i].texture = XMFLOAT2(m_model[i].tu, m_model[i].tv);
		vertices[i].normal = XMFLOAT3(m_model[i].nx, m_model[i].ny, m_model[i].nz);

		indices[i] = i;
	}

	m_vertexBuffer = Buffer::CreateVertexBuffer(m_render->m_pd3dDevice, sizeof(VertexType) * m_vertexCount, false, vertices);
	m_indexBuffer = Buffer::CreateIndexBuffer(m_render->m_pd3dDevice, sizeof(unsigned long) * m_indexCount, false, indices);

	_DELETE_ARRAY(vertices);
	_DELETE_ARRAY(indices);

	return true;
}

void PlaneModel::Close()
{
	_DELETE_ARRAY(m_model);
	_RELEASE(m_vertexBuffer);
	_RELEASE(m_indexBuffer);
	_RELEASE(m_constMatrixBuffer);
	_RELEASE(m_lightColorBuffer);
	_RELEASE(m_lightPositionBuffer);
	_CLOSE(m_shader);
}

void PlaneModel::Render(CXMMATRIX view, CXMMATRIX proj, XMFLOAT4 diffuseColor[], XMFLOAT4 lightPosition[])
{
	MatrixBufferType cb;

	cb.world = XMMatrixTranspose(XMMatrixIdentity());
	cb.view = XMMatrixTranspose(view);
	cb.projection = XMMatrixTranspose(proj);
	m_render->m_pImmediateContext->UpdateSubresource(m_constMatrixBuffer, 0, NULL, &cb, 0, 0);
	m_render->m_pImmediateContext->VSSetConstantBuffers(0, 1, &m_constMatrixBuffer);

	LightPositionBufferType lpb;
	lpb.lightPosition[0] = lightPosition[0];
	lpb.lightPosition[1] = lightPosition[1];
	lpb.lightPosition[2] = lightPosition[2];
	lpb.lightPosition[3] = lightPosition[3];
	m_render->m_pImmediateContext->UpdateSubresource(m_lightPositionBuffer, 0, NULL, &lpb, 0, 0);
	m_render->m_pImmediateContext->VSSetConstantBuffers(1, 1, &m_lightPositionBuffer);

	LightColorBufferType lcb;
	lcb.diffuseColor[0] = diffuseColor[0];
	lcb.diffuseColor[1] = diffuseColor[1];
	lcb.diffuseColor[2] = diffuseColor[2];
	lcb.diffuseColor[3] = diffuseColor[3];
	m_render->m_pImmediateContext->UpdateSubresource(m_lightColorBuffer, 0, NULL, &lcb, 0, 0);
	m_render->m_pImmediateContext->PSSetConstantBuffers(0, 1, &m_lightColorBuffer);

	unsigned int stride = sizeof(VertexType);
	unsigned int offset = 0;
	m_render->m_pImmediateContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
	m_render->m_pImmediateContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	m_render->m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_shader->Draw();
	m_render->m_pImmediateContext->DrawIndexed(m_indexCount, 0, 0);
}