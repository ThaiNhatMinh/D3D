#include "..\base.h"

Mesh::Mesh():m_pIB(0),m_pVB(0)
{

}

Mesh::~Mesh()
{
	R(m_pVB);
	R(m_pIB);
}


void Mesh::Finalize(ID3D10Device * pDevice)
{
	std::vector<Vertex> data;
	for (int i = 0; i < m_iNumVertices; i++)
	{
		Vertex v;
		v.color.x = m_vColor[i].r;
		v.color.y = m_vColor[i].g;
		v.color.z = m_vColor[i].b;
		v.pos = m_vPositions[i];
		data.push_back(v);
	}
	D3D10_BUFFER_DESC vbDesc;
	vbDesc.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	vbDesc.ByteWidth = sizeof(Vertex) * m_iNumVertices;
	vbDesc.CPUAccessFlags = 0;
	vbDesc.MiscFlags = 0;
	vbDesc.Usage = D3D10_USAGE_IMMUTABLE;

	D3D10_SUBRESOURCE_DATA vbSub;
	vbSub.pSysMem = &data[0];

	HR(pDevice->CreateBuffer(&vbDesc, &vbSub, &m_pVB));

	if (m_vIndices.size() > 0)
	{
		D3D10_BUFFER_DESC ibd;
		ibd.Usage = D3D10_USAGE_IMMUTABLE;
		ibd.ByteWidth = sizeof(DWORD) * m_iNumFaces * 3;
		ibd.BindFlags = D3D10_BIND_INDEX_BUFFER;
		ibd.CPUAccessFlags = 0;
		ibd.MiscFlags = 0;
		D3D10_SUBRESOURCE_DATA iinitData;
		iinitData.pSysMem = &m_vIndices[0];
		HR(pDevice->CreateBuffer(&ibd, &iinitData, &m_pIB));
	}

}
