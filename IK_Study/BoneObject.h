#pragma once
#include "GameObject.h"
#include "Shader.h"

class CBoneObject : public CGameObject
{
	CBoneObject* m_pParentBone;
	CBoneObject* m_pChildBone;

public:
	CBoneObject();
	~CBoneObject();

	void SetChildBone(CBoneObject* pChild) { m_pChildBone = pChild; }
	void SetParentBone(CBoneObject* pParent) { m_pParentBone = pParent; }

	virtual void Animate(float fTimeElapsed, bool bUseLocalMatrix);
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext);
	D3DXMATRIX GetWorldMatrix() { return m_d3dxmtxWorld; }

	bool bTestFlag;
};

