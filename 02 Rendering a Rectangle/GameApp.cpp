#include "GameApp.h"
#include "d3dUtil.h"
#include "DXTrace.h"
using namespace DirectX;
using namespace std::experimental;

const D3D11_INPUT_ELEMENT_DESC GameApp::VertexPosColor::inputLayout[2] = {
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

GameApp::GameApp(HINSTANCE hInstance)
	: D3DApp(hInstance)
{
}

GameApp::~GameApp()
{
}

bool GameApp::Init()
{
	if (!D3DApp::Init())
		return false;

	if (!InitEffect())
		return false;

	if (!InitResource())
		return false;

	return true;
}

void GameApp::OnResize()
{
	D3DApp::OnResize();
}

void GameApp::UpdateScene(float dt)
{

}

void GameApp::DrawScene()
{
	assert(m_pd3dImmediateContext);
	assert(m_pSwapChain);

	static float black[4] = { 0.0f, 0.0f, 0.0f, 1.0f };	// RGBA = (0,0,0,255)
	m_pd3dImmediateContext->ClearRenderTargetView(m_pRenderTargetView.Get(), black);
	m_pd3dImmediateContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// ���ƾ���
	m_pd3dImmediateContext->Draw(4, 0);
	HR(m_pSwapChain->Present(0, 0));
}

bool GameApp::InitEffect()
{
	ComPtr<ID3DBlob> blob;

	// ����������ɫ��
	HR(CreateShaderFromFile(L"HLSL\\Triangle_VS.cso", L"HLSL\\Triangle_VS.hlsl", "VS", "vs_5_0", blob.ReleaseAndGetAddressOf()));
	HR(m_pd3dDevice->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, m_pVertexShader.GetAddressOf()));
	// �������󶨶��㲼��
	HR(m_pd3dDevice->CreateInputLayout(VertexPosColor::inputLayout, ARRAYSIZE(VertexPosColor::inputLayout),
		blob->GetBufferPointer(), blob->GetBufferSize(), m_pVertexLayout.GetAddressOf()));

	// ����������ɫ��
	HR(CreateShaderFromFile(L"HLSL\\Triangle_PS.cso", L"HLSL\\Triangle_PS.hlsl", "PS", "ps_5_0", blob.ReleaseAndGetAddressOf()));
	HR(m_pd3dDevice->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, m_pPixelShader.GetAddressOf()));

	return true;
}

bool GameApp::InitResource()
{
	// ���þ��ζ���
	VertexPosColor vertices[] =
	{
		{ XMFLOAT3(0.5f, 0.5f, 0.5f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(0.5f, -0.5f, 0.5f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(-0.5f, 0.5f, 0.5f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(-0.5f, -0.5f, 0.5f), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f) }
	};
	// ���ö��㻺��������
	D3D11_BUFFER_DESC vbd;
	ZeroMemory(&vbd, sizeof(vbd));
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof vertices;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	// �½����㻺����
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = vertices;
	HR(m_pd3dDevice->CreateBuffer(&vbd, &InitData, m_pVertexBuffer.GetAddressOf()));


	// ******************
	// ����Ⱦ���߸����׶ΰ󶨺�������Դ

	// ����װ��׶εĶ��㻺��������
	UINT stride = sizeof(VertexPosColor);	// ��Խ�ֽ���
	UINT offset = 0;						// ��ʼƫ����

	m_pd3dImmediateContext->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), &stride, &offset);
	// ����ͼԪ���ͣ��趨���벼��
	m_pd3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	m_pd3dImmediateContext->IASetInputLayout(m_pVertexLayout.Get());
	// ����ɫ���󶨵���Ⱦ����
	m_pd3dImmediateContext->VSSetShader(m_pVertexShader.Get(), nullptr, 0);
	m_pd3dImmediateContext->PSSetShader(m_pPixelShader.Get(), nullptr, 0);




	return true;
}