#pragma once

#include "D3D Framework.h"
#include "Light.h"
#include "PlaneModel.h"

using namespace D3D11Framework;

class MyRender : public Render
{
public:
	MyRender();
	bool Init();
	bool Draw();
	void Close(); 
private:
	friend class MyInput;
	friend PlaneModel;
	PlaneModel m_model;

	Camera m_cam;
	Light m_Light1, m_Light2, m_Light3, m_Light4; 

	bool m_leftcam;
	bool m_rightcam;
};