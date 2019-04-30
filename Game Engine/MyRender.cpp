#include "MyRender.h" 

struct cbMatrixData
{
	XMMATRIX WVP;
	XMMATRIX World;
};

struct Light
{
	Light()
	{
		ZeroMemory(this, sizeof(Light));
	}
	XMFLOAT3 pos;
	float range;
	XMFLOAT3 dir;
	float cone;
	XMFLOAT3 att;
	float pad2;
	XMFLOAT4 ambient;
	XMFLOAT4 diffuse;

} light;

struct cbLightData
{
	Light light;
};

struct Vertex
{
	Vertex(float x, float y, float z,
		float u, float v,
		float nx, float ny, float nz) :
		pos(x, y, z), tex(u, v), normal(nx, ny, nz) {}

	XMFLOAT3 pos;
	XMFLOAT2 tex;
	XMFLOAT3 normal;
};


MyRender::MyRender()
{
	IndexBuffer = nullptr;
	VertBuffer = nullptr;
	constMatrixBuffer = nullptr;
	constLightBuffer = nullptr;
	shader = nullptr;
}

bool MyRender::Init()
{
	shader = new Shader(this);
	if (!shader)
		return false;


	if (!shader->LoadTexture(L"image.png"))
		return false;

	shader->AddInputElementDesc("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	shader->AddInputElementDesc("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	shader->AddInputElementDesc("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);

	if (!shader->CreateShader(L"pointlight.vs", L"pointlight.ps"))
		return false;

	Vertex v[] =
	{
		Vertex(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f,-1.0f, -1.0f, -1.0f),
		Vertex(-1.0f,  1.0f, -1.0f, 0.0f, 0.0f,-1.0f,  1.0f, -1.0f),
		Vertex(1.0f,  1.0f, -1.0f, 1.0f, 0.0f, 1.0f,  1.0f, -1.0f),
		Vertex(1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f, -1.0f),

		Vertex(-1.0f, -1.0f, 1.0f, 1.0f, 1.0f,-1.0f, -1.0f, 1.0f),
		Vertex(1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 1.0f, -1.0f, 1.0f),
		Vertex(1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f,  1.0f, 1.0f),
		Vertex(-1.0f,  1.0f, 1.0f, 1.0f, 0.0f,-1.0f,  1.0f, 1.0f),

		Vertex(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f,-1.0f, 1.0f, -1.0f),
		Vertex(-1.0f, 1.0f,  1.0f, 0.0f, 0.0f,-1.0f, 1.0f,  1.0f),
		Vertex(1.0f, 1.0f,  1.0f, 1.0f, 0.0f, 1.0f, 1.0f,  1.0f),
		Vertex(1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f),

		Vertex(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f,-1.0f, -1.0f, -1.0f),
		Vertex(1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, -1.0f, -1.0f),
		Vertex(1.0f, -1.0f,  1.0f, 0.0f, 0.0f, 1.0f, -1.0f,  1.0f),
		Vertex(-1.0f, -1.0f,  1.0f, 1.0f, 0.0f,-1.0f, -1.0f,  1.0f),

		Vertex(-1.0f, -1.0f,  1.0f, 0.0f, 1.0f,-1.0f, -1.0f,  1.0f),
		Vertex(-1.0f,  1.0f,  1.0f, 0.0f, 0.0f,-1.0f,  1.0f,  1.0f),
		Vertex(-1.0f,  1.0f, -1.0f, 1.0f, 0.0f,-1.0f,  1.0f, -1.0f),
		Vertex(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f,-1.0f, -1.0f, -1.0f),

		Vertex(1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, -1.0f, -1.0f),
		Vertex(1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 1.0f,  1.0f, -1.0f),
		Vertex(1.0f,  1.0f,  1.0f, 1.0f, 0.0f, 1.0f,  1.0f,  1.0f),
		Vertex(1.0f, -1.0f,  1.0f, 1.0f, 1.0f, 1.0f, -1.0f,  1.0f),
	};

	DWORD indices[] =
	{
		0,  1,  2,
		0,  2,  3,

		4,  5,  6,
		4,  6,  7,

		8,  9, 10,
		8, 10, 11,

		12, 13, 14,
		12, 14, 15,

		16, 17, 18,
		16, 18, 19,

		20, 21, 22,
		20, 22, 23
	};

	IndexBuffer = Buffer::CreateIndexBuffer(m_pd3dDevice, sizeof(DWORD) * 36, false, indices);
	VertBuffer = Buffer::CreateVertexBuffer(m_pd3dDevice, sizeof(Vertex) * 24, false, v);
	constMatrixBuffer = Buffer::CreateConstantBuffer(m_pd3dDevice, sizeof(cbMatrixData), false);
	constLightBuffer = Buffer::CreateConstantBuffer(m_pd3dDevice, sizeof(cbLightData), false);

	XMVECTOR camPosition = XMVectorSet(0.0f, 3.0f, -8.0f, 0.0f);
	XMVECTOR camTarget = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR camUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	camView = XMMatrixLookAtLH(camPosition, camTarget, camUp);

	light.pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	light.range = 100.0f;
	light.att = XMFLOAT3(0.0f, 0.2f, 0.0f);
	light.ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	light.diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	light.cone = 10.0f;


	// Создаем шрифт m_font из текстуры font_0.png с помощью font.fnt 
	// Шрифт создан в программе "BMFont" 
	m_font = new BitmapFont(this);
	if (!m_font->Init("font.fnt"))
		return false;

	text1 = new Text(this, m_font); // Создаем текстовую строку
	// Сначала строка, затем флаг статический ли текст(у нас false), затем макс. число символов.
	text1->Init(L"Динамический текст", false, 20); // text1 будет выведен как "Динамический текст". 

	// Создаем шрифт
	// Шрифт создан в программе "BMFont" 
	m_font2 = new BitmapFont(this);
	if (!m_font2->Init("font.fnt"))// Вы можете заменить на fontBOLD, чтобы шриыт стал жирным
		return false;

	text2 = new Text(this, m_font2); // Создаем текстовую строку
	// текст будет выведен как "Статический текст". 
	// Сначала строка, затем флаг статический ли текст(у нас true), затем макс. число символов.
	text2->Init(L"\"Статический текст\"", true, 5);

	return true;
}

bool MyRender::Draw()
{ 
	TurnOnAlphaBlending(); //Включаем прозрачность

	text1->SetText(L"Дин текст изменен");
	//Выводим текст зеленого цвета в левом углу экрана в координатах (10.0;10.0).
	text1->Draw(0.0f, 1.0f, 0.0f, 10.0f, 00.0f); // Красный - 0.0f Зеленый - 1.0 Синий - 0.0f

	//Выводим текст желого цвета в левом углу экрана в координатах (10.0;40.0).
	text2->Draw(1.0f, 1.0f, 0.0f, 10.0f, 70.0f); // Красный - 0.0f Зеленый - 1.0 Синий - 0.0f

	TurnOffAlphaBlending(); //Выключаем прозрачность

	static float rot = 0.0f;
	static DWORD dwTimeStart = 0;
	DWORD dwTimeCur = GetTickCount();
	if (dwTimeStart == 0)
		dwTimeStart = dwTimeCur;
	rot = (dwTimeCur - dwTimeStart) / 1000.0f;

	light.pos.x = 0.0f;
	light.pos.y = 0.0f - 4; // Ставим "фонарь" в центр, чуть повыше уровня "земли" из 4 кубов.
	light.pos.z = 0.0f;

	light.dir.x = 0.0f - light.pos.x;
	light.dir.y = 0.0f - light.pos.y;
	light.dir.z = 0.0f - light.pos.z;


	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	m_pImmediateContext->IASetVertexBuffers(0, 1, &VertBuffer, &stride, &offset);
	m_pImmediateContext->IASetIndexBuffer(IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	cbLightData cblgh;
	cblgh.light = light;
	m_pImmediateContext->UpdateSubresource(constLightBuffer, 0, NULL, &cblgh, 0, 0);
	m_pImmediateContext->PSSetConstantBuffers(0, 1, &constLightBuffer);

	/*             Пол из 4 кубов:
		cube1World   cube4World   cube7World
		cube2World   cube5World   cube8World
		cube3World   cube6World   cube9World

	Поменяйте параматры XMMatrixTranslation, посмотрите результат.
	*/

	XMMATRIX cube1World = XMMatrixTranslation(-2.0f, 0.0f, 2.0f);
	XMMATRIX WVP = cube1World * camView * m_Projection;
	cbMatrixData cbMat;
	cbMat.World = XMMatrixTranspose(cube1World);
	cbMat.WVP = XMMatrixTranspose(WVP);
	m_pImmediateContext->UpdateSubresource(constMatrixBuffer, 0, NULL, &cbMat, 0, 0);
	m_pImmediateContext->VSSetConstantBuffers(0, 1, &constMatrixBuffer);
	shader->Draw();
	m_pImmediateContext->DrawIndexed(36, 0, 0);

	XMMATRIX cube2World = XMMatrixTranslation(-2.0f, 0.0f, 0.0f);
	WVP = cube2World * camView * m_Projection;
	cbMat.World = XMMatrixTranspose(cube2World);
	cbMat.WVP = XMMatrixTranspose(WVP);
	m_pImmediateContext->UpdateSubresource(constMatrixBuffer, 0, NULL, &cbMat, 0, 0);
	m_pImmediateContext->VSSetConstantBuffers(0, 1, &constMatrixBuffer);
	shader->Draw();
	m_pImmediateContext->DrawIndexed(36, 0, 0);

	XMMATRIX cube3World = XMMatrixTranslation(-2.0f, 0.0f, -2.0f);
	WVP = cube3World * camView * m_Projection;
	cbMat.World = XMMatrixTranspose(cube3World);
	cbMat.WVP = XMMatrixTranspose(WVP);
	m_pImmediateContext->UpdateSubresource(constMatrixBuffer, 0, NULL, &cbMat, 0, 0);
	m_pImmediateContext->VSSetConstantBuffers(0, 1, &constMatrixBuffer);
	shader->Draw();
	m_pImmediateContext->DrawIndexed(36, 0, 0);

	XMMATRIX cube4World = XMMatrixTranslation(0.0f, 0.0f, 2.0f);
	WVP = cube4World * camView * m_Projection;
	cbMat.World = XMMatrixTranspose(cube4World);
	cbMat.WVP = XMMatrixTranspose(WVP);
	m_pImmediateContext->UpdateSubresource(constMatrixBuffer, 0, NULL, &cbMat, 0, 0);
	m_pImmediateContext->VSSetConstantBuffers(0, 1, &constMatrixBuffer);
	shader->Draw();
	m_pImmediateContext->DrawIndexed(36, 0, 0);

	XMMATRIX cube10World = XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	WVP = cube10World * camView * m_Projection;
	cbMat.World = XMMatrixTranspose(cube10World);
	cbMat.WVP = XMMatrixTranspose(WVP);
	m_pImmediateContext->UpdateSubresource(constMatrixBuffer, 0, NULL, &cbMat, 0, 0);
	m_pImmediateContext->VSSetConstantBuffers(0, 1, &constMatrixBuffer);
	shader->Draw();
	m_pImmediateContext->DrawIndexed(36, 0, 0);

	XMMATRIX cube5World = XMMatrixTranslation(0.0f, 1.0f, 0.0f);
	WVP = cube5World * camView * m_Projection;
	cbMat.World = XMMatrixTranspose(cube5World);
	cbMat.WVP = XMMatrixTranspose(WVP);
	m_pImmediateContext->UpdateSubresource(constMatrixBuffer, 0, NULL, &cbMat, 0, 0);
	m_pImmediateContext->VSSetConstantBuffers(0, 1, &constMatrixBuffer);
	shader->Draw();
	m_pImmediateContext->DrawIndexed(36, 0, 0);

	XMMATRIX cube6World = XMMatrixTranslation(0.0f, 0.0f, -2.0f);
	WVP = cube6World * camView * m_Projection;
	cbMat.World = XMMatrixTranspose(cube6World);
	cbMat.WVP = XMMatrixTranspose(WVP);
	m_pImmediateContext->UpdateSubresource(constMatrixBuffer, 0, NULL, &cbMat, 0, 0);
	m_pImmediateContext->VSSetConstantBuffers(0, 1, &constMatrixBuffer);
	shader->Draw();
	m_pImmediateContext->DrawIndexed(36, 0, 0);

	XMMATRIX cube7World = XMMatrixTranslation(2.0f, 0.0f, 2.0f);
	WVP = cube7World * camView * m_Projection;
	cbMat.World = XMMatrixTranspose(cube7World);
	cbMat.WVP = XMMatrixTranspose(WVP);
	m_pImmediateContext->UpdateSubresource(constMatrixBuffer, 0, NULL, &cbMat, 0, 0);
	m_pImmediateContext->VSSetConstantBuffers(0, 1, &constMatrixBuffer);
	shader->Draw();
	m_pImmediateContext->DrawIndexed(36, 0, 0);

	XMMATRIX cube8World = XMMatrixTranslation(2.0f, 0.0f, 0.0f);
	WVP = cube8World * camView * m_Projection;
	cbMat.World = XMMatrixTranspose(cube8World);
	cbMat.WVP = XMMatrixTranspose(WVP);
	m_pImmediateContext->UpdateSubresource(constMatrixBuffer, 0, NULL, &cbMat, 0, 0);
	m_pImmediateContext->VSSetConstantBuffers(0, 1, &constMatrixBuffer);
	shader->Draw();
	m_pImmediateContext->DrawIndexed(36, 0, 0);

	XMMATRIX cube9World = XMMatrixTranslation(2.0f, 0.0f, -2.0f);
	WVP = cube9World * camView * m_Projection;
	cbMat.World = XMMatrixTranspose(cube9World);
	cbMat.WVP = XMMatrixTranspose(WVP);
	m_pImmediateContext->UpdateSubresource(constMatrixBuffer, 0, NULL, &cbMat, 0, 0);
	m_pImmediateContext->VSSetConstantBuffers(0, 1, &constMatrixBuffer);
	shader->Draw();
	m_pImmediateContext->DrawIndexed(36, 0, 0);

	return true;
}

void MyRender::Close()
{
	_CLOSE(shader);
	_CLOSE(m_font);
	_CLOSE(text1);
	_CLOSE(m_font2);
	_CLOSE(text2);
	_RELEASE(IndexBuffer);
	_RELEASE(VertBuffer);
	_RELEASE(constMatrixBuffer);
	_RELEASE(constLightBuffer);
}