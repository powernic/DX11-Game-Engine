#include "MyRender.h"
#include "PlaneModel.h"

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
	m_leftcam = m_rightcam = false;
}

bool MyRender::Init()
{
	m_cam.SetPos(0.0f, 2.8f, -11.0f);

	// инициализируем модель
	m_model.Init(this);

	// настраиваем свет
	m_Light1.SetDiffuseColor(1.0f, 0.0f, 0.0f, 1.0f);
	m_Light1.SetPosition(-3.0f, 1.0f, 3.0f);

	m_Light2.SetDiffuseColor(0.0f, 1.0f, 0.0f, 1.0f);
	m_Light2.SetPosition(3.0f, 1.0f, 3.0f);

	m_Light3.SetDiffuseColor(0.0f, 0.0f, 1.0f, 1.0f);
	m_Light3.SetPosition(-3.0f, 1.0f, -3.0f);

	m_Light4.SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light4.SetPosition(3.0f, 1.0f, -3.0f);

	return true;
}
bool MyRender::Draw()
{
	m_cam.MoveLeft(m_leftcam);
	m_cam.MoveRight(m_rightcam);
	m_cam.Render();

	XMMATRIX camView = m_cam.GetViewMatrix();
	XMFLOAT4 diffuseColor[4];
	XMFLOAT4 lightPosition[4];
	diffuseColor[0] = m_Light1.GetDiffuseColor();
	diffuseColor[1] = m_Light2.GetDiffuseColor();
	diffuseColor[2] = m_Light3.GetDiffuseColor();
	diffuseColor[3] = m_Light4.GetDiffuseColor();
	lightPosition[0] = m_Light1.GetPosition();
	lightPosition[1] = m_Light2.GetPosition();
	lightPosition[2] = m_Light3.GetPosition();
	lightPosition[3] = m_Light4.GetPosition();

	m_model.Render(camView, m_Projection, diffuseColor, lightPosition);

	return true;
}

void MyRender::Close()
{
	m_model.Close();
}