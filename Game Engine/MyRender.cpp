#include "MyRender.h"
 
MyRender::MyRender()
{
	m_mesh = nullptr;
}

bool MyRender::Init(HWND hwnd)
{
	XMVECTOR Eye = XMVectorSet(0.0f, 0.0f, -2.8f, 0.0f);
	XMVECTOR At = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	m_View = XMMatrixLookAtLH(Eye, At, Up);

	m_mesh = new StaticMesh();
	if (!m_mesh->Init(this, L"mesh.ms3d"))
		return false;

	return true;
}

bool MyRender::Draw()
{
	static float rot = 0.0f;
	rot += .0005f;
	if (rot > 6.26f)
		rot = 0.0f;

	m_mesh->Identity();
	m_mesh->Rotate(10-rot, 0.0, 1.0, 0.0);
	m_mesh->Translate(-40, 0.0, 0.0);
	m_mesh->Scale(0.02,0.02, 0.02);
	m_mesh->Draw(m_View);

	m_mesh->Identity();
	m_mesh->Rotate(rot, 0.0, 1.0, 0.0);
	m_mesh->Translate(40, 0.0, 0.0);
	m_mesh->Scale(0.02, 0.02, 0.02);
	m_mesh->Draw(m_View);
	return true;
} 

void MyRender::Close()
{
	_CLOSE(m_mesh);
}