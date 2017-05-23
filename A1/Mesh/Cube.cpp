#include "..\base.h"

void Cube::Init(ID3D10Device * pDevice, float scale)
{
	m_Mesh.m_iNumFaces = 12;
	m_Mesh.m_iNumVertices = 8;
	m_Mesh.m_vPositions.push_back(D3DXVECTOR3(-1.0f, -1.0f, -1.0f));
	m_Mesh.m_vPositions.push_back(D3DXVECTOR3(-1.0f, +1.0f, -1.0f));
	m_Mesh.m_vPositions.push_back(D3DXVECTOR3(+1.0f, +1.0f, -1.0f));
	m_Mesh.m_vPositions.push_back(D3DXVECTOR3(+1.0f, -1.0f, -1.0f));
	m_Mesh.m_vPositions.push_back(D3DXVECTOR3(-1.0f, -1.0f, +1.0f));
	m_Mesh.m_vPositions.push_back(D3DXVECTOR3(-1.0f, +1.0f, +1.0f));
	m_Mesh.m_vPositions.push_back(D3DXVECTOR3(+1.0f, +1.0f, +1.0f));
	m_Mesh.m_vPositions.push_back(D3DXVECTOR3(+1.0f, -1.0f, +1.0f));

	// Scale the box.
	for (DWORD i = 0; i < m_Mesh.m_iNumVertices; ++i)
		m_Mesh.m_vPositions[i] *= scale;

	m_Mesh.m_vColor = { WHITE ,BLACK ,RED ,GREEN ,BLUE ,YELLOW ,CYAN ,MAGENTA };
	m_Mesh.m_vIndices = {
		// front face
		0, 1, 2,
		0, 2, 3,

		// back face
		4, 6, 5,
		4, 7, 6,

		// left face
		4, 5, 1,
		4, 1, 0,

		// right face
		3, 2, 6,
		3, 6, 7,

		// top face
		1, 5, 6,
		1, 6, 2,

		// bottom face
		4, 0, 3,
		4, 3, 7
	};

	m_Mesh.Finalize(pDevice);

	m_Mesh.Topology = TRIANGLELIST;
	m_pDevice = pDevice;
}

void Cube::Draw()
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	m_pDevice->IASetPrimitiveTopology(m_Mesh.Topology);
	m_pDevice->IASetVertexBuffers(0, 1, &m_Mesh.m_pVB, &stride, &offset);
	if (m_Mesh.m_vIndices.size())
	{
		m_pDevice->IASetIndexBuffer(m_Mesh.m_pIB, DXGI_FORMAT_R32_UINT, 0);
		m_pDevice->DrawIndexed(m_Mesh.m_iNumFaces * 3, 0, 0 );
	}
	else
	{
		m_pDevice->Draw(m_Mesh.m_iNumVertices, 0);
	}


}
