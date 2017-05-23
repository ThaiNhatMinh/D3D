#pragma once
#include "..\base.h"

struct Vertex
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 color;
};

 

#define	POINTLIST  D3D10_PRIMITIVE_TOPOLOGY_POINTLIST
#define	LINELIST  D3D10_PRIMITIVE_TOPOLOGY_LINELIST
#define	LINESTRIP  D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP
#define	TRIANGLELIST  D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST

class Mesh
{
private:
	
public:

	DWORD m_iNumVertices;
	DWORD m_iNumFaces;

	std::vector<D3DXVECTOR3> m_vPositions;
	std::vector<D3DXCOLOR> m_vColor;
	std::vector<unsigned int> m_vIndices;

	D3D10_PRIMITIVE_TOPOLOGY Topology;
	//ID3D10Device* m_pDevice;	// D3D10 Device
	ID3D10Buffer* m_pVB;		// vertex buffer
	ID3D10Buffer* m_pIB;		// index buffer
public:
	Mesh();
	~Mesh();

	virtual void Finalize(ID3D10Device* pDevice);
	
};