#include "stdafx.h"
#include "CCDHelper.h"

int g_nCount = 0;
bool g_bTestFlag = false;
float g_fEpsilon = 0.001f;
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

		D3DXVECTOR3 d3dxvEndEffector = pLastBone->GetPosition() + pLastBone->GetUp() * pLastBone->GetBoneLength();
		D3DXVECTOR3 d3dxvStartingPointToEndEffector = d3dxvEndEffector - pCurrBone->GetPosition();
		D3DXVec3Normalize(&d3dxvStartingPointToEndEffector, &d3dxvStartingPointToEndEffector);

		D3DXMATRIX d3dxmtxRotation;
		D3DXVECTOR3 d3dxvCrossVec;

		D3DXVec3Cross(&d3dxvCrossVec, &d3dxvStartingPointToEndEffector, &d3dxvCurrBoneToTarget);
		D3DXVec3Normalize(&d3dxvCrossVec, &d3dxvCrossVec);

		float fDot = D3DXVec3Dot(&d3dxvStartingPointToEndEffector, &d3dxvCurrBoneToTarget);
		fDot = max(-1.0f, min(1.0, fDot)); // clamp
		fDot = acos(fDot);
		std::cout << fDot << std::endl;

		D3DXMatrixRotationAxis(&d3dxmtxRotation, &d3dxvCrossVec, fDot);

		pCurrBone->Rotate(d3dxmtxRotation, true);

		D3DXMATRIX d3dxmtxParent;
		D3DXMatrixIdentity(&d3dxmtxParent);

		if (i - 1 >= 0)
		{
			d3dxmtxParent = vBoneVector[i - 1]->GetWorldMatrix();
		}

		for (int j = i; j < vBoneVector.size(); ++j)
		{
			d3dxmtxParent = vBoneVector[j]->Animate(0, d3dxmtxParent);
		}

		++boneReverseIter;
		++g_nCount;

		d3dxvEndEffector = pLastBone->GetPosition() + pLastBone->GetUp() * pLastBone->GetBoneLength();
		if (D3DXVec3LengthSq(&(d3dxvTargetPos - d3dxvEndEffector)) < g_fEpsilon*g_fEpsilon)
			g_bTestFlag = true;
		if (g_nCount == 2)
			g_bTestFlag = true;


		Sleep(10.0f);
		std::cout << g_nCount << " - length: " <<
			D3DXVec3LengthSq(&(d3dxvTargetPos - d3dxvEndEffector)) <<
			" / epsilon: " << g_fEpsilon*g_fEpsilon << std::endl;
	}
}