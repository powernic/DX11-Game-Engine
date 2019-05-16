#pragma once

#include "D3D Framework.h"

using namespace D3D11Framework;

class Light
{
public:
	void SetAmbientColor(float, float, float, float);
	void SetDiffuseColor(float, float, float, float);
	void SetPosition(float, float, float);
	void SetLookAt(float, float, float);

	XMFLOAT4 GetAmbientColor();
	XMFLOAT4 GetDiffuseColor();
	XMFLOAT3 GetPosition();

	void GenerateViewMatrix();
	void GenerateProjectionMatrix(float, float);

	XMMATRIX GetViewMatrix();
	XMMATRIX GetProjectionMatrix();


private:
	XMMATRIX m_viewMatrix;
	XMMATRIX m_projectionMatrix;

	XMFLOAT4 m_ambientColor;
	XMFLOAT4 m_diffuseColor;
	XMFLOAT3 m_position;
	XMFLOAT3 m_lookAt;
};