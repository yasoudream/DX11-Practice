//***************************************************************************************
// TextureRender.h by X_Jun(MKXJun) (C) 2018-2019 All Rights Reserved.
// Licensed under the MIT License.
//
// ��Ⱦ��������
// Render-To-Texture class.
//***************************************************************************************

#ifndef TEXTURERENDER_H
#define TEXTURERENDER_H

#include <d3d11_1.h>
#include <wrl/client.h>

class TextureRender
{
public:
	template<class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;


	TextureRender(ComPtr<ID3D11Device> device,
		int texWidth, 
		int texHeight,
		bool generateMips = false);
	~TextureRender();

	// ��ʼ�Ե�ǰ���������Ⱦ
	void Begin(ComPtr<ID3D11DeviceContext> deviceContext);
	// �����Ե�ǰ�������Ⱦ����ԭ״̬
	void End(ComPtr<ID3D11DeviceContext> deviceContext);
	// ��ȡ��Ⱦ�õ�����
	ComPtr<ID3D11ShaderResourceView> GetOutputTexture();

private:
	ComPtr<ID3D11ShaderResourceView>	m_pOutputTextureSRV;	// ����������Ӧ����ɫ����Դ��ͼ
	ComPtr<ID3D11RenderTargetView>		m_pOutputTextureRTV;	// ����������Ӧ����ȾĿ����ͼ
	ComPtr<ID3D11DepthStencilView>		m_pOutputTextureDSV;	// ����������õ����/ģ����ͼ
	D3D11_VIEWPORT						m_OutputViewPort;	// ������õ��ӿ�

	ComPtr<ID3D11RenderTargetView>		m_pCacheRTV;		// ��ʱ����ĺ󱸻�����
	ComPtr<ID3D11DepthStencilView>		m_pCacheDSV;		// ��ʱ��������/ģ�建����
	D3D11_VIEWPORT						m_CacheViewPort;	// ��ʱ������ӿ�

	bool								m_GenerateMips;	// �Ƿ�����mipmap��
};

#endif