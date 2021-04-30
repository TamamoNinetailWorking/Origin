#include "XFile.h"
#include "../../../System/Utility/Memory.h"

//三角形頂点情報をワールド座標に変換
void CXFile::TraiangleTransforms(const D3DXMATRIX& matrix)
{
	//ワールド座標系に変換するためのワーク用配列
	XFile::Vertex* TransformWork;
	D3DXVECTOR3 temp;

	int idx0, idx1, idx2;
	D3DXVECTOR3 normal;
	D3DXVECTOR3 p0p1;
	D3DXVECTOR3 p0p2;

	//生成された三角形情報があれば削除しておく
	if (m_pTriangle != nullptr)
	{
		DeleteArray(m_pTriangle);
	}

	//頂点数を取得しておく
	const int VertexNum = GetVertexNum();
	//座標変換用ワークの生成
	TransformWork = new XFile::Vertex[VertexNum];

	//頂点をすべて取り出してワールド座標に変換する
	for (int count = 0; count < VertexNum; count++)
	{
		TransformWork[count] = GetVertex(count);

		temp.x = GetVertex(count).m_Pos.x;
		temp.y = GetVertex(count).m_Pos.y;
		temp.z = GetVertex(count).m_Pos.z;

		D3DXVec3TransformCoord(&temp, &temp, &matrix);

		TransformWork[count].m_Pos.x = temp.x;
		TransformWork[count].m_Pos.y = temp.y;
		TransformWork[count].m_Pos.z = temp.z;
	}

	//面数を取得しておく
	const int FaceNum = GetFaceNum();
	//三角形の面数分オブジェクト生成
	m_pTriangle = new XFile::Triangle[FaceNum];
	//受け渡し用のトライアングル
	m_pTrianglePlane = new XFile::TrianglePlane[FaceNum];

	for (int count = 0; count < FaceNum; count++)
	{
		idx0 = m_connect[count].index[0];
		idx1 = m_connect[count].index[1];
		idx2 = m_connect[count].index[2];

		m_pTriangle[count].vertex[0] = TransformWork[idx0];
		m_pTriangle[count].color[0] = D3DCOLOR_ARGB(255, 255, 255, 255);

		m_pTriangle[count].vertex[1] = TransformWork[idx1];
		m_pTriangle[count].color[1] = D3DCOLOR_ARGB(255, 255, 255, 255);

		m_pTriangle[count].vertex[2] = TransformWork[idx2];
		m_pTriangle[count].color[2] = D3DCOLOR_ARGB(255, 255, 255, 255);


		m_pTrianglePlane[count].vertex[0].Pos = m_pTriangle[count].vertex[0].m_Pos;
		m_pTrianglePlane[count].vertex[1].Pos = m_pTriangle[count].vertex[1].m_Pos;
		m_pTrianglePlane[count].vertex[2].Pos = m_pTriangle[count].vertex[2].m_Pos;

		m_pTrianglePlane[count].vertex[0].Tex = m_pTriangle[count].vertex[0].m_tex;
		m_pTrianglePlane[count].vertex[1].Tex = m_pTriangle[count].vertex[1].m_tex;
		m_pTrianglePlane[count].vertex[2].Tex = m_pTriangle[count].vertex[2].m_tex;


		m_pTrianglePlane[count].vertex[0].Color = D3DXVECTOR4(1, 1, 1, 1);
		m_pTrianglePlane[count].vertex[1].Color = D3DXVECTOR4(1, 1, 1, 1);
		m_pTrianglePlane[count].vertex[2].Color = D3DXVECTOR4(1, 1, 1, 1);



		p0p1.x = m_pTriangle[count].vertex[1].m_Pos.x - m_pTriangle[count].vertex[0].m_Pos.x;
		p0p1.y = m_pTriangle[count].vertex[1].m_Pos.y - m_pTriangle[count].vertex[0].m_Pos.y;
		p0p1.z = m_pTriangle[count].vertex[1].m_Pos.z - m_pTriangle[count].vertex[0].m_Pos.z;

		p0p2.x = m_pTriangle[count].vertex[2].m_Pos.x - m_pTriangle[count].vertex[0].m_Pos.x;
		p0p2.y = m_pTriangle[count].vertex[2].m_Pos.y - m_pTriangle[count].vertex[0].m_Pos.y;
		p0p2.z = m_pTriangle[count].vertex[2].m_Pos.z - m_pTriangle[count].vertex[0].m_Pos.z;

		//法線情報計算
		D3DXVec3Cross(&normal, &p0p1, &p0p2);
		D3DXVec3Normalize(&normal, &normal);
		m_pTriangle[count].normal = normal;

		//重心計算
		m_pTriangle[count].center_position.x = (m_pTriangle[count].vertex[0].m_Pos.x + m_pTriangle[count].vertex[1].m_Pos.x + m_pTriangle[count].vertex[2].m_Pos.x) / 3;
		m_pTriangle[count].center_position.y = (m_pTriangle[count].vertex[0].m_Pos.y + m_pTriangle[count].vertex[1].m_Pos.y + m_pTriangle[count].vertex[2].m_Pos.y) / 3;
		m_pTriangle[count].center_position.z = (m_pTriangle[count].vertex[0].m_Pos.z + m_pTriangle[count].vertex[1].m_Pos.z + m_pTriangle[count].vertex[2].m_Pos.z) / 3;

		//頂点座標を、原点を基準とした座標に変換する
		m_pTriangle[count].vertex[0].m_Pos.x -= m_pTriangle[count].center_position.x;
		m_pTriangle[count].vertex[0].m_Pos.y -= m_pTriangle[count].center_position.y;
		m_pTriangle[count].vertex[0].m_Pos.z -= m_pTriangle[count].center_position.z;

		m_pTriangle[count].vertex[1].m_Pos.x -= m_pTriangle[count].center_position.x;
		m_pTriangle[count].vertex[1].m_Pos.y -= m_pTriangle[count].center_position.y;
		m_pTriangle[count].vertex[1].m_Pos.z -= m_pTriangle[count].center_position.z;

		m_pTriangle[count].vertex[2].m_Pos.x -= m_pTriangle[count].center_position.x;
		m_pTriangle[count].vertex[2].m_Pos.y -= m_pTriangle[count].center_position.y;
		m_pTriangle[count].vertex[2].m_Pos.z -= m_pTriangle[count].center_position.z;
	}

	DeleteArray(TransformWork);
	return;
}

void CXFile::ExplosionUpdate()
{
	D3DXMATRIX mat1, mat2, mat3;
	D3DXMATRIX matTra;
	static float counter = 0.0f;
	const int increment_correction = 10;

	for (int i = 0; i < GetFaceNum(); i++) {

		// 回転角度から行列を作成する
		D3DXMatrixRotationX(&mat1, m_pTriangle[i].angle.x);
		D3DXMatrixRotationY(&mat2, m_pTriangle[i].angle.y);
		D3DXMatrixRotationZ(&mat3, m_pTriangle[i].angle.z);

		m_pTriangle[i].matrix = mat1 * mat2*mat3;

		m_pTriangle[i].matrix._41 = m_pTriangle[i].center_position.x;
		m_pTriangle[i].matrix._42 = m_pTriangle[i].center_position.y;
		m_pTriangle[i].matrix._43 = m_pTriangle[i].center_position.z;

		// 回転角度の足しこみ
		// ここをコーディング
		// ――――― start -----------
		m_pTriangle[i].angle.x += m_pTriangle[i].derivative_angle.x;
		m_pTriangle[i].angle.y += m_pTriangle[i].derivative_angle.y;
		m_pTriangle[i].angle.z += m_pTriangle[i].derivative_angle.z;

		// ――――― end -----------

		// 法線方向への移動
		// ここをコーディング
		// ――――― start -----------
		/*D3DXMatrixTranslation(&matTra, g_pTriangle[i].nx * counter, g_pTriangle[i].ny * counter, g_pTriangle[i].nz * counter);
		g_pTriangle[i].mat = g_pTriangle[i].mat * matTra;*/
		m_pTriangle[i].center_position.x += m_pTriangle[i].normal.x / increment_correction;
		m_pTriangle[i].center_position.y += m_pTriangle[i].normal.y / increment_correction;
		m_pTriangle[i].center_position.z += m_pTriangle[i].normal.z / increment_correction;
		// ――――― end -----------

		for (int j = 0; j<3; j++) {
			unsigned char alpha = (unsigned char)((m_pTriangle[i].color[j] & 0xff000000) >> 24);
			if (alpha > 0) {
				m_pTriangle[i].color[j] -= 0x01000000;	// アルファ値の変更
			}
			else {
				m_pTriangle[i].color[j] = 0x00ffffff;	// アルファ値が0以下なら0に
			}
			m_pTrianglePlane[i].vertex[j].Color.z = (float)alpha/ 255;
			//m_pTrianglePlane[i].vertex[j].Color.z = 1.0f;
		}

	}
	counter += 0.1f;
}