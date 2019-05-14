#pragma once

#include "D3D Framework.h"

using namespace D3D11Framework;

class RenderWindow
{
	struct Vertex
	{
		XMFLOAT3 pos;
		XMFLOAT2 tex;
	};
public:
	RenderWindow();
	bool Init(ID3D11Device*, int, int, int, int);
	void Close();
	void Render(ID3D11DeviceContext*);

private:
	ID3D11Buffer* m_vertexBuffer, * m_indexBuffer;
	int m_vertexCount, m_indexCount;
	int m_screenWidth, m_screenHeight;
	int m_bitmapWidth, m_bitmapHeight;
	int m_previousPosX, m_previousPosY;
};