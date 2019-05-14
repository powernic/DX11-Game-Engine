#pragma once

#include "D3D Framework.h"   

#include "RenderTarget.h"
#include "RenderWindow.h"

using namespace D3D11Framework;

class MyRender : public Render
{
public:
	MyRender();
	bool Init();
	bool Draw();
	void Close();

	void RenderTexture();
	void RenderScene();
private:
	ID3D11Buffer* m_vertexBuffer;
	ID3D11Buffer* m_indexBuffer;
	ID3D11Buffer* m_constantBuffer;
	Shader* m_shaderScene;
	Shader* m_shaderRT;

	XMMATRIX camView; 

	RenderTarget m_rt;
	RenderWindow m_rw;
};