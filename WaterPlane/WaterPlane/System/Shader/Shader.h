#pragma once

#include <d3d11.h>


//モデル毎に共通して制作。
//モデル単位でシェーダーを持たせる事によって、
//同じシェーダー作成を防止するマネージャー作成を推奨。
class CShader
{
private:

	//デバイスの受け取り用
	ID3D11Device * m_pDevice;
	ID3D11DeviceContext* m_pImmediateContext;

	//頂点シェーダー
	ID3D11VertexShader* m_pVertexShader;
	//頂点レイアウト
	ID3D11InputLayout* m_pVertexLayout;
	//ピクセルシェーダー
	ID3D11PixelShader* m_pPixelShader;
	//ジオメトリシェーダー
	ID3D11GeometryShader* m_pGeometryShader;

	//頂点シェーダーリスト
	

	//シェーダーコンパイラー
	HRESULT CompileShaderFromFile(char* szFilename, LPCSTR szEntryPoint, LPCSTR szShaderModel,ID3DBlob** ppBlobOut);

public:

	explicit CShader(ID3D11Device* pdevice,ID3D11DeviceContext* pdevicecontext)
		:m_pVertexShader(nullptr),
		m_pVertexLayout(nullptr),
		m_pPixelShader(nullptr),
		m_pGeometryShader(nullptr)
	{
		m_pDevice = pdevice;
		m_pImmediateContext = pdevicecontext;
		return;
	};

	virtual ~CShader() 
	{
		//Exit();
		return;
	};

	//頂点シェーダーオブジェクトの生成
	bool CreateVertexShader(char* szFileName, LPCSTR szFuncName, D3D11_INPUT_ELEMENT_DESC* layout, unsigned int numElements);

	//ジオメトリシェーダーオブジェクトの生成
	bool CreateGeometryShader(char* szFileName, LPCSTR szFuncName);
	
	//ピクセルシェーダーオブジェクトの生成
	bool CreatePixelShader(char*szFileName, LPCSTR szFuncName);

	//コンスタントバッファの作成
	static bool CreateConstantBuffer(unsigned int ByteSize,ID3D11Buffer** ppConstantBuffer);

	//頂点バッファの作成
	static bool CreateVertexBuffer(unsigned int stride, unsigned int VertexNum, void* pVertex,ID3D11Buffer** ppVertexBuffer);

	//インデックスバッファの作成
	static bool CreateIndexBuffer(unsigned int IndexNum, void* IndexData, ID3D11Buffer** ppIndexBuffer);

	//テクスチャサンプラーの作成
	static bool CreateSamplerState(ID3D11SamplerState*& pSamplerState);

	//シェーダーリソースビューの作成
	static bool CreateShaderResourceView(ID3D11Buffer* pBuffer, ID3D11ShaderResourceView** ppSRV);

	//TGAファイルをリソースビューとして作成
	static bool CreateSRVfromTGAFile(const char* filename, ID3D11ShaderResourceView** ppSRV);

	//WICファイルをリソースビューとして作成
	static bool CreateSRVfromWICFile(const char* filename, ID3D11ShaderResourceView** pSRV);

	//Getter
	ID3D11VertexShader* GetVertexShader() 
	{ return m_pVertexShader; };
	ID3D11PixelShader* GetPixelShader() 
	{ return m_pPixelShader; };
	ID3D11InputLayout* GetVertexLayout() 
	{ return m_pVertexLayout; };
	ID3D11GeometryShader* GetGeometryShader() 
	{ return m_pGeometryShader; };

	//Setter
	void SetVertexShader(ID3D11VertexShader*& pVertexShader)
	{ m_pVertexShader = pVertexShader; };
	void SetPixelShader(ID3D11PixelShader*& pPixelShader)
	{ m_pPixelShader = pPixelShader; };
	void SetGeometryShader(ID3D11GeometryShader*& pGeometryShader)
	{ m_pGeometryShader = pGeometryShader; };
	void SetVertexLayout(ID3D11InputLayout*& pInputLayout)
	{ m_pVertexLayout = pInputLayout; };

	//メンバー変数の解放
	void Exit();
};