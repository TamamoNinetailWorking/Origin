#pragma once

template <typename VertexBuffer,typename IndexBuffer,typename MaterialBuffer,typename MaterialIndexBuffer,typename Matrix>
class CDrawer3D
{
protected:

	VertexBuffer* m_pVertexBuffer;
	IndexBuffer* m_pIndexBuffer;
	MaterialBuffer* m_pMaterialBuffer;
	MaterialIndexBuffer* m_pMaterialIndexBuffer;

	void CommonInit() {
		m_pVertexBuffer = nullptr;
		m_pIndexBuffer = nullptr;
		m_pMaterialBuffer = nullptr;
		m_pMaterialIndexBuffer = nullptr;
	}

public:

	CDrawer3D() {
		CommonInit();
	};
	virtual ~CDrawer3D() {};

	VertexBuffer*& GetVertexBuffer() { return m_pVertexBuffer; };
	IndexBuffer*& GetIndexBuffer() { return m_pIndexBuffer; };
	MaterialBuffer*& GetMaterialBuffer() { return m_pMaterialBuffer; };
	MaterialIndexBuffer* GetMaterialIndexBuffer() { return m_pMaterialIndexBuffer; };

	virtual void Draw(const Matrix& matrix) {};

};