#pragma once

#include <d3d11.h>


//���f�����ɋ��ʂ��Đ���B
//���f���P�ʂŃV�F�[�_�[���������鎖�ɂ���āA
//�����V�F�[�_�[�쐬��h�~����}�l�[�W���[�쐬�𐄏��B
class CShader
{
private:

	//�f�o�C�X�̎󂯎��p
	ID3D11Device * m_pDevice;
	ID3D11DeviceContext* m_pImmediateContext;

	//���_�V�F�[�_�[
	ID3D11VertexShader* m_pVertexShader;
	//���_���C�A�E�g
	ID3D11InputLayout* m_pVertexLayout;
	//�s�N�Z���V�F�[�_�[
	ID3D11PixelShader* m_pPixelShader;
	//�W�I���g���V�F�[�_�[
	ID3D11GeometryShader* m_pGeometryShader;

	//���_�V�F�[�_�[���X�g
	

	//�V�F�[�_�[�R���p�C���[
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

	//���_�V�F�[�_�[�I�u�W�F�N�g�̐���
	bool CreateVertexShader(char* szFileName, LPCSTR szFuncName, D3D11_INPUT_ELEMENT_DESC* layout, unsigned int numElements);

	//�W�I���g���V�F�[�_�[�I�u�W�F�N�g�̐���
	bool CreateGeometryShader(char* szFileName, LPCSTR szFuncName);
	
	//�s�N�Z���V�F�[�_�[�I�u�W�F�N�g�̐���
	bool CreatePixelShader(char*szFileName, LPCSTR szFuncName);

	//�R���X�^���g�o�b�t�@�̍쐬
	static bool CreateConstantBuffer(unsigned int ByteSize,ID3D11Buffer** ppConstantBuffer);

	//���_�o�b�t�@�̍쐬
	static bool CreateVertexBuffer(unsigned int stride, unsigned int VertexNum, void* pVertex,ID3D11Buffer** ppVertexBuffer);

	//�C���f�b�N�X�o�b�t�@�̍쐬
	static bool CreateIndexBuffer(unsigned int IndexNum, void* IndexData, ID3D11Buffer** ppIndexBuffer);

	//�e�N�X�`���T���v���[�̍쐬
	static bool CreateSamplerState(ID3D11SamplerState*& pSamplerState);

	//�V�F�[�_�[���\�[�X�r���[�̍쐬
	static bool CreateShaderResourceView(ID3D11Buffer* pBuffer, ID3D11ShaderResourceView** ppSRV);

	//TGA�t�@�C�������\�[�X�r���[�Ƃ��č쐬
	static bool CreateSRVfromTGAFile(const char* filename, ID3D11ShaderResourceView** ppSRV);

	//WIC�t�@�C�������\�[�X�r���[�Ƃ��č쐬
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

	//�����o�[�ϐ��̉��
	void Exit();
};