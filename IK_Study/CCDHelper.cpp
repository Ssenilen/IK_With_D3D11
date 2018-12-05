#include "stdafx.h"
#include "CCDHelper.h"

bool g_bTestFlag = false;
void CCCDHelper::ExecuteCCD(std::vector<CBoneObject*>& vBoneVector, const D3DXVECTOR3& d3dxvTargetPos)
{
	if (vBoneVector.size() <= 1)
		return;

	CBoneObject* pLastBone = *vBoneVector.rbegin();


	std::vector<CBoneObject*>::reverse_iterator boneReverseIter = vBoneVector.rbegin();

	for (int i = vBoneVector.size() - 1; i >= 0; --i)
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

		D3DXMATRIX d3dxmtxParent;
		D3DXMatrixIdentity(&d3dxmtxParent);

		for (int j = i - 1; j < vBoneVector.size(); ++j)
		{
			if (j < 0) continue;

			d3dxmtxParent = vBoneVector[j]->GetWorldMatrix();
			pCurrBone->Animate(0, d3dxmtxParent);
		}

		D3DXVECTOR3 d3dxvLastBoneDir; // = pLastBone->GetBoneDirection();
		D3DXVec3TransformCoord(&d3dxvLastBoneDir, &D3DXVECTOR3(0.0f, 1.0f, 0.0f), &d3dxmtxParent);
		D3DXVec3Normalize(&d3dxvLastBoneDir, &d3dxvLastBoneDir);
		pLastBone->SetBoneDirection(d3dxvLastBoneDir);

		//for (int j  = i - 1; j < vBoneVector.size(); ++j);
		//{
		//	if (j < 0) continue; // Root의 부모는 처리할 필요가 읎다.

		//	const D3DXMATRIX& d3dxmtxParentMatrix = vBoneVector[j]->GetWorldMatrix();
		//	pCurrBone->Rotate(d3dxmtxRotation);
		//	pCurrBone->Animate(0, d3dxmtxParentMatrix);
		//}}

		++boneReverseIter;

		if (i == 3)
			g_bTestFlag = true;
	}

	D3DXMATRIX d3dxmtxParent;
	D3DXMatrixIdentity(&d3dxmtxParent);
	for (auto boneObject : vBoneVector)
	{
		d3dxmtxParent = boneObject->Animate(0, d3dxmtxParent);
	}
}