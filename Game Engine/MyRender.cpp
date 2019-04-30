#include "MyRender.h" 

std::wstring intToStr(int i)
{
	wchar_t str[255];
	swprintf(str, 255, L"%d", i);
	return str;
}

MyRender::MyRender()
{
	m_mesh = nullptr;
	m_font = nullptr;
	m_text = nullptr;
	m_leftcam = m_rightcam = false;
}

bool MyRender::Init()
{
	m_mesh = new StaticMesh(this);
	if (!m_mesh->Init(L"sphere.ms3d"))
		return false;

	m_font = new BitmapFont(this);
	if (!m_font->Init("font.fnt"))
		return false;

	m_text = new Text(this, m_font);
	if (!m_text->Init(L"", false, 20))
		return false;

	if (!m_modellist.Init(300))
		return false;

	if (!m_timer.Init())
		return false;

	m_cam.SetPosition(0.0f, 0.0f, -35.0f);

	return true; 
}

bool MyRender::Draw()
{
	m_timer.Frame();
	m_cam.TurnLeft(m_leftcam);
	m_cam.TurnRight(m_rightcam);
	m_cam.Render(m_timer.GetTime());

	XMMATRIX viewMatrix = m_cam.GetViewMatrix();

	m_frustum.ConstructFrustum(1000.0f, m_Projection, viewMatrix);

	int modelCount = m_modellist.GetModelCount();
	int renderCount = 0;

	for (int i = 0; i < modelCount; i++)
	{
		float x, y, z;
		m_modellist.GetData(i, x, y, z);

		bool renderModel = m_frustum.CheckSphere(x, y, z, 1.0f);
		if (renderModel)
		{
			m_mesh->Identity();
			m_mesh->Translate(x, y, z);
			m_mesh->Draw(viewMatrix);

			renderCount++;
		}
	}

	TurnZBufferOff();
	TurnOnAlphaBlending();

	std::wstring text = L"Сфер на экране: " + intToStr(renderCount);
	m_text->SetText(text);
	m_text->Draw(1.0f, 1.0f, 1.0f, 0.0f, 20.0f);

	TurnZBufferOn();
	TurnOffAlphaBlending();

	return true;
}

void MyRender::Close()
{
	_CLOSE(m_font);
	_CLOSE(m_text);
	_CLOSE(m_mesh);
	m_modellist.Close();
}