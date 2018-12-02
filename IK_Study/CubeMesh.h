#pragma once
#include "Mesh.h"

class CCubeMesh : public CMesh
{
public:
	CCubeMesh::CCubeMesh(ID3D11Device *pd3dDevice, float fWidth, float fHeight, float fDepth, D3DXCOLOR d3dxColor);
	~CCubeMesh();

	void CreateRasterizerState(ID3D11Device *pd3dDevice);
	void Render(ID3D11DeviceContext *pd3dDeviceContext);
};

