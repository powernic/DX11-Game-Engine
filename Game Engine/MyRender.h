#pragma once

#include "D3D Framework.h"   

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
	ModelList m_modellist;
	Camera m_cam;
	Frustum m_frustum;
	Timer m_timer;

	StaticMesh* m_mesh;

	BitmapFont* m_font;
	Text* m_text;

	bool m_leftcam;
	bool m_rightcam;
};