#include "TextureRender.h"
#include "DXTrace.h"
using namespace Microsoft::WRL;

TextureRender::TextureRender(ComPtr<ID3D11Device> device, int texWidth, int texHeight, bool generateMips)
	: m_GenerateMips(generateMips)
{
	// ******************
	// 1. ��������
	//

	ComPtr<ID3D11Texture2D> texture;
	D3D11_TEXTURE2D_DESC texDesc;
	
	texDesc.Width = texWidth;
	texDesc.Height = texHeight;
	texDesc.MipLevels = (m_GenerateMips ? 0 : 1);	// 0Ϊ����mipmap��
	texDesc.ArraySize = 1;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	// ����texture�����½�����
	HR(device->CreateTexture2D(&texDesc, nullptr, texture.ReleaseAndGetAddressOf()));

	// ******************
	// 2. ���������Ӧ����ȾĿ����ͼ
	//

	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	rtvDesc.Format = texDesc.Format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D.MipSlice = 0;

	HR(device->CreateRenderTargetView(
		texture.Get(),
		&rtvDesc,
		m_pOutputTextureRTV.GetAddressOf()));
	
	// ******************
	// 3. ���������Ӧ����ɫ����Դ��ͼ
	//

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = texDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.TextureCube.MipLevels = -1;	// ʹ�����е�mip�ȼ�

	HR(device->CreateShaderResourceView(
		texture.Get(),
		&srvDesc,
		m_pOutputTextureSRV.GetAddressOf()));

	// ******************
	// 4. ����������ȿ�ߵ����/ģ�建�����Ͷ�Ӧ����ͼ
	//

	texDesc.Width = texWidth;
	texDesc.Height = texHeight;
	texDesc.MipLevels = 0;
	texDesc.ArraySize = 1;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	ComPtr<ID3D11Texture2D> depthTex;
	device->CreateTexture2D(&texDesc, nullptr, depthTex.GetAddressOf());

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	dsvDesc.Format = texDesc.Format;
	dsvDesc.Flags = 0;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;

	HR(device->CreateDepthStencilView(
		depthTex.Get(),
		&dsvDesc,
		m_pOutputTextureDSV.GetAddressOf()));

	// ******************
	// 5. ��ʼ���ӿ�
	//
	m_OutputViewPort.TopLeftX = 0.0f;
	m_OutputViewPort.TopLeftY = 0.0f;
	m_OutputViewPort.Width = static_cast<float>(texWidth);
	m_OutputViewPort.Height = static_cast<float>(texHeight);
	m_OutputViewPort.MinDepth = 0.0f;
	m_OutputViewPort.MaxDepth = 1.0f;
}

TextureRender::~TextureRender()
{
}

void TextureRender::Begin(ComPtr<ID3D11DeviceContext> deviceContext)
{
	// ������ȾĿ������ģ����ͼ
	deviceContext->OMGetRenderTargets(1, m_pCacheRTV.GetAddressOf(), m_pCacheDSV.GetAddressOf());
	// �����ӿ�
	UINT num_Viewports = 1;
	deviceContext->RSGetViewports(&num_Viewports, &m_CacheViewPort);


	// ��ջ�����
	float black[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	deviceContext->ClearRenderTargetView(m_pOutputTextureRTV.Get(), black);
	deviceContext->ClearDepthStencilView(m_pOutputTextureDSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	// ������ȾĿ������ģ����ͼ
	deviceContext->OMSetRenderTargets(1, m_pOutputTextureRTV.GetAddressOf(), m_pOutputTextureDSV.Get());
	// �����ӿ�
	deviceContext->RSSetViewports(1, &m_OutputViewPort);
}

void TextureRender::End(ComPtr<ID3D11DeviceContext> deviceContext)
{
	// �ָ�Ĭ���趨
	deviceContext->RSSetViewports(1, &m_CacheViewPort);
	deviceContext->OMSetRenderTargets(1, m_pCacheRTV.GetAddressOf(), m_pCacheDSV.Get());

	// ��֮ǰ��ָ����Ҫmipmap����������
	if (m_GenerateMips)
	{
		deviceContext->GenerateMips(m_pOutputTextureSRV.Get());
	}
	
	// �����ʱ�������ȾĿ����ͼ�����ģ����ͼ
	m_pCacheDSV.Reset();
	m_pCacheRTV.Reset();
}

ComPtr<ID3D11ShaderResourceView> TextureRender::GetOutputTexture()
{
	return m_pOutputTextureSRV;
}
