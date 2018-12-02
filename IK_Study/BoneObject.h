#pragma once
#include "GameObject.h"
#include "Shader.h"

class CBoneObject : public CGameObject
{
public:
	CBoneObject();
	~CBoneObject();

	D3DXMATRIX Animate(float fTimeElapsed, const D3DXMATRIX& pParentMatrix);
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext);
	D3DXMATRIX GetWorldMatrix() { return m_d3dxmtxWorld; }
};

