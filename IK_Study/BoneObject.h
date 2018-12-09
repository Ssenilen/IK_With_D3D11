#pragma once
#include "GameObject.h"
#include "Shader.h"

class CBoneObject : public CGameObject
{
public:
	CBoneObject();
	~CBoneObject();

	D3DXMATRIX Animate(float fTimeElapsed, const D3DXMATRIX& pParentMatrix);
	void FabrikAnimate();
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext);
	D3DXMATRIX GetWorldMatrix() { return m_d3dxmtxWorld; }

	void SetBoneLength(const float fBoneLength) { m_fBoneLength = fBoneLength; }
	float GetBoneLength() { return m_fBoneLength; }
	virtual D3DXVECTOR3 GetPosition();
	// 이 부분은 CCD가 도입되면 따로 분리하자. (둘은 방식이 다르다.)
	virtual void SetPosition(float x, float y, float z);
	virtual void SetPosition(D3DXVECTOR3 d3dxvPosition);

	void Rotate(D3DXMATRIX d3dmtxRotate, bool bMatrixMultiply = false);

private:
	float m_fBoneLength;
};

