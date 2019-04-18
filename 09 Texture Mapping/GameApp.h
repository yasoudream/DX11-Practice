#ifndef GAMEAPP_H
#define GAMEAPP_H

#include "d3dApp.h"
#include "Geometry.h"
#include "LightHelper.h"
//#include "RotationTool.h"
#include <cmath>
class GameApp : public D3DApp
{
public:
	struct VSConstantBuffer
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX proj;
		DirectX::XMMATRIX worldInvTranspose;
		DirectX::XMFLOAT2 rot;
		DirectX::XMFLOAT2 pad;
	};

	struct PSConstantBuffer
	{
		DirectionalLight dirLight[10];
		PointLight pointLight[10];
		SpotLight spotLight[10];
		Material material;
		int numDirLight;
		int numPointLight;
		int numSpotLight;
		float pad;		// �����֤16�ֽڶ���
		DirectX::XMFLOAT4 eyePos;

	};

	enum class ShowMode { WoodCrate, FireAnim };

public:
	GameApp(HINSTANCE hInstance);
	~GameApp();

	bool Init();
	void OnResize();
	void UpdateScene(float dt);
	void DrawScene();


private:
	bool InitEffect();
	bool InitResource();

	template<class VertexType>
	bool ResetMesh(const Geometry::MeshData<VertexType>& meshData);


private:
	
	ComPtr<ID2D1SolidColorBrush> m_pColorBrush;				    // ��ɫ��ˢ
	ComPtr<IDWriteFont> m_pFont;								// ����
	ComPtr<IDWriteTextFormat> m_pTextFormat;					// �ı���ʽ

	ComPtr<ID3D11InputLayout> m_pVertexLayout2D;				// ����2D�Ķ������벼��
	ComPtr<ID3D11InputLayout> m_pVertexLayout3D;				// ����3D�Ķ������벼��
	ComPtr<ID3D11Buffer> m_pVertexBuffer;						// ���㻺����
	ComPtr<ID3D11Buffer> m_pIndexBuffer;						// ����������
	ComPtr<ID3D11Buffer> m_pConstantBuffers[2];				    // ����������
	UINT m_IndexCount;										    // ������������������С
	int m_CurrFrame;											// ��ǰ���涯�����ŵ��ڼ�֡
	ShowMode m_CurrMode;										// ��ǰ��ʾ��ģʽ

	ComPtr<ID3D11ShaderResourceView> m_pWoodCrate;			    // ľ������
	ComPtr<ID3D11ShaderResourceView> m_pFlareCrate;			    // ������
	ComPtr<ID3D11ShaderResourceView> m_pAlphaCrate;			    // ��֪��ʲô����
	std::vector<ComPtr<ID3D11ShaderResourceView>> m_pFireAnims; // ��������
	ComPtr<ID3D11SamplerState> m_pSamplerState;				    // ������״̬

	ComPtr<ID3D11VertexShader> m_pVertexShader3D;				// ����3D�Ķ�����ɫ��
	ComPtr<ID3D11PixelShader> m_pPixelShader3D;				    // ����3D��������ɫ��

	ComPtr<ID3D11VertexShader> m_pVertexShader3D_rot;				// ����3D�Ķ�����ɫ��
	ComPtr<ID3D11PixelShader> m_pPixelShader3D_rot;				    // ����3D��������ɫ��

	ComPtr<ID3D11VertexShader> m_pVertexShader2D;				// ����2D�Ķ�����ɫ��
	ComPtr<ID3D11PixelShader> m_pPixelShader2D;				    // ����2D��������ɫ��

	VSConstantBuffer m_VSConstantBuffer;						// �����޸�����VS��GPU�����������ı���
	PSConstantBuffer m_PSConstantBuffer;						// �����޸�����PS��GPU�����������ı���
};


#endif