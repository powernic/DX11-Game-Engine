#pragma once

#include "D3D Framework.h"

using namespace D3D11Framework;

class Light
{
public:
	void SetDiffuseColor(float, float, float, float);
	void SetPosition(float, float, float);

	XMFLOAT4 GetDiffuseColor();
	XMFLOAT4 GetPosition();

private:
	XMFLOAT4 m_diffuseColor;
	XMFLOAT4 m_position;
};