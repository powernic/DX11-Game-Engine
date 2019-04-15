#include "MyRender.h"
#include <xnamath.h>
#include <d3dcompiler.h>

struct SimpleVertex
{
	XMFLOAT3 Pos;
};

MyRender::MyRender()
{
	m_pVertexShader = nullptr;
	m_pPixelShader = nullptr;
	m_pVertexLayout = nullptr;
	m_pVertexBuffer = nullptr;
}

HRESULT MyRender::m_compileshaderfromfile(WCHAR* FileName, LPCSTR EntryPoint, LPCSTR ShaderModel, ID3DBlob** ppBlobOut)
{
	HRESULT hr = S_OK;

	hr = D3DX11CompileFromFile(FileName, NULL, NULL,
		EntryPoint, ShaderModel, 0,
		0, NULL, ppBlobOut, NULL, NULL);

	return hr;
}

bool MyRender::Init(HWND hwnd)
{
	HRESULT hr = S_OK;

	// Компиляция вершинного шейдера
	ID3DBlob* pVSBlob = NULL;
	hr = m_compileshaderfromfile(L"shader.fx", "VS", "vs_4_0", &pVSBlob);
	if (FAILED(hr))
	{
		Log::Get()->Err("Невозможно скомпилировать файл shader.fx. Пожалуйста, запустите данную программу из папки, содержащей этот файл");
		return false;
	}

	// Создание вершинного шейдера
	hr = m_pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &m_pVertexShader);
	if (FAILED(hr))
	{
		Log::Get()->Err("Не удалось создать вершинный шейдер");
		_RELEASE(pVSBlob);
		return false;
	}

	// определение входного формата
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = ARRAYSIZE(layout);

	// создание входного формата
	hr = m_pd3dDevice->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &m_pVertexLayout);

	_RELEASE(pVSBlob);
	if (FAILED(hr))
		return false;

	// установка входного формата
	m_pImmediateContext->IASetInputLayout(m_pVertexLayout);

	// Компиляция пиксельного шейдера
	ID3DBlob* pPSBlob = NULL;
	hr = m_compileshaderfromfile(L"shader.fx", "PS", "ps_4_0", &pPSBlob);
	if (FAILED(hr))
	{
		Log::Get()->Err("Невозможно скомпилировать файл shader.fx. Пожалуйста, запустите данную программу из папки, содержащей этот файл");
		return false;
	}

	// Создание пиксельного шейдера
	hr = m_pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &m_pPixelShader);
	_RELEASE(pPSBlob);
	if (FAILED(hr))
	{
		Log::Get()->Err("Не удалось создать пиксельный шейдер");
		_RELEASE(pPSBlob);
		return false;
	}

	// Создание буфера вершин
	SimpleVertex vertices[] =
	{	
		XMFLOAT3(0.0f, 0.5f, 0.5f),
		XMFLOAT3(0.5f, -0.5f, 0.5f),
		XMFLOAT3(-0.5f, -0.5f, 0.5f),
	};

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex) * 3;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA Data;
	ZeroMemory(&Data, sizeof(Data));
	Data.pSysMem = vertices;

	hr = m_pd3dDevice->CreateBuffer(&bd, &Data, &m_pVertexBuffer);
	if (FAILED(hr))
		return false;

	// Установка вершинного буфера
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	m_pImmediateContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	// установка топологии примитива
	m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return true;
}

bool MyRender::Draw()
{
	m_pImmediateContext->VSSetShader(m_pVertexShader, NULL, 0);
	m_pImmediateContext->PSSetShader(m_pPixelShader, NULL, 0);
	m_pImmediateContext->Draw(3, 0);

	return true;
}

void MyRender::Close()
{
	_RELEASE(m_pVertexBuffer);
	_RELEASE(m_pVertexLayout);
	_RELEASE(m_pVertexShader);
	_RELEASE(m_pPixelShader);
}