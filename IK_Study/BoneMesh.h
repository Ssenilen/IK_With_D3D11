#pragma once
#include "Mesh.h"

class CBoneMesh : public CMesh
{
public:
	CBoneMesh::CBoneMesh(ID3D11Device *pd3dDevice, float fWidth, float fHeight, float fDepth, D3DXCOLOR d3dxColor);
	~CBoneMesh();

	void CreateRasterizerState(ID3D11Device *pd3dDevice);
	void Render(ID3D11DeviceContext *pd3dDeviceContext);
};

