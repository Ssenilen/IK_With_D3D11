#include "stdafx.h"
#include "CCDHelper.h"

bool g_bTestFlag = false;
void CCCDHelper::ExecuteCCD(std::vector<CBoneObject*>& vBoneVector, const D3DXVECTOR3& d3dxvTargetPos)
{
	if (vBoneVector.size() <= 1)
		return;

	CBoneObject* pLastBone = *vBoneVector.rbegin();


	std::vector<CBoneObject*>::reverse_iterator boneReverseIter = vBoneVector.rbegin();

	for (int i = 0; i < vBoneVector.size(); ++i)
	{
		if (g_bTestFlag)
			break;

		CBoneObject* pCurrBone = *boneReverseIter;

		D3DXVECTOR3 d3dxvCurrBoneToTarget = d3dxvTargetPos - pCurrBone->GetPosition();
		D3DXVec3Normalize(&d3dxvCurrBoneToTarget, &d3dxvCurrBoneToTarget);

		D3DXVECTOR3 d3dxvNowBoneEndPos = pLastBone->GetPosition() + pLastBone->GetBoneDirection() * pLastBone->GetBoneLength();
		D3DXVECTOR3 d3dxvStartingPointToEndEffector = d3dxvNowBoneEndPos - pCurrBone->GetPosition();
		D3DXVec3Normalize(&d3dxvStartingPointToEndEffector, &d3dxvStartingPointToEndEffector);

		D3DXMATRIX d3dxmtxRotation;
		D3DXVECTOR3 d3dxvCrossVec;

		D3DXVec3Cross(&d3dxvCrossVec, &d3dxvStartingPointToEndEffector, &d3dxvCurrBoneToTarget);
		D3DXVec3Normalize(&d3dxvCrossVec, &d3dxvCrossVec);

		float fDot = D3DXVec3Dot(&d3dxvStartingPointToEndEffector, &d3dxvCurrBoneToTarget);
		fDot = acos(fDot);

		D3DXMatrixRotationAxis(&d3dxmtxRotation, &d3dxvCrossVec, fDot);
		pCurrBone->Rotate(d3dxmtxRotation);

		D3DXVECTOR3 d3dxvCurrBoneDir = pCurrBone->GetBoneDirection();
		D3DXVec3TransformCoord(&d3dxvCurrBoneDir, &d3dxvCurrBoneDir, &d3dxmtxRotation);
		D3DXVec3Normalize(&d3dxvCurrBoneDir, &d3dxvCurrBoneDir);
		pCurrBone->SetBoneDirection(d3dxvCurrBoneDir);

		++boneReverseIter;

		if (i == 4)
			g_bTestFlag = true;
	}

	D3DXMATRIX d3dxmtxParent;
	D3DXMatrixIdentity(&d3dxmtxParent);
	for (auto boneObject : vBoneVector)
	{
		d3dxmtxParent = boneObject->Animate(0, d3dxmtxParent);
	}
}