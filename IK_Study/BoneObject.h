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
	// �� �κ��� CCD�� ���ԵǸ� ���� �и�����. (���� ����� �ٸ���.)
	virtual void SetPosition(float x, float y, float z);
	virtual void SetPosition(D3DXVECTOR3 d3dxvPosition);

	void Rotate(D3DXMATRIX d3dmtxRotate, bool bMatrixMultiply = false);

private:
	float m_fBoneLength;
};

