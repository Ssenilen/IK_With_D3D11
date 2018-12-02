#pragma once
#include "GameObject.h"
#include "BoneObject.h"
#include "BoneMesh.h"

class CMainBoneObject : public CGameObject
{
public:
	CMainBoneObject(int nBoneCount, CBoneMesh* pBoneMesh);
	~CMainBoneObject();

	D3DXVECTOR3 GetEndEffector();

	virtual void Animate(float fTimeElapsed);
	virtual void Render(ID3D11DeviceContext* pd3dDeviceContext);
	virtual bool IsVisible(CCamera *pCamera);



private:
	void BuildObject(int nBoneCount, CBoneMesh* pBoneMesh);
	void AttachBoneObject(CBoneObject* pBoneObject);

	std::vector<CBoneObject*> m_vpBoneObjects;
};

