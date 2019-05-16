#pragma once

#include "D3D Framework.h"
using namespace D3D11Framework;

class MyRender;

class PlaneModel
{
	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};
public:
	PlaneModel();

	bool Init(MyRender* render);
	void Close();
	void Render(CXMMATRIX view, CXMMATRIX proj, XMFLOAT4 diffuseColor[], XMFLOAT4 lightPosition[]);

private:
	MyRender* m_render;
	bool InitializeBuffers();
	bool LoadModel(char*);

	ID3D11Buffer* m_vertexBuffer, * m_indexBuffer;
	int m_vertexCount, m_indexCount;
	ModelType* m_model;

	ID3D11Buffer* m_constMatrixBuffer;
	ID3D11Buffer* m_lightColorBuffer;
	ID3D11Buffer* m_lightPositionBuffer;

	Shader* m_shader;
};