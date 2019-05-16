#pragma once

#include "D3D Framework.h"

using namespace D3D11Framework;

class MyRender;

class DepthShader
{
public:
	DepthShader(MyRender* render);

	bool Init();
	void Close();
	void Render(int index, CXMMATRIX WVP);

private:
	MyRender* m_render;

	Shader* m_shader;
	ID3D11Buffer* m_matrixBuffer;
};