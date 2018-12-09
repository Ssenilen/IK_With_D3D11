#include "stdafx.h"
#include "FabrikHelper.h"


void CFabrikHelper::ExecuteFabrik(std::vector<CBoneObject*>& vBoneVector, const D3DXVECTOR3& d3dxvTargetPos)
{
	std::vector<CBoneObject*>::reverse_iterator boneReverseIter = vBoneVector.rbegin();
	std::vector<CBoneObject*>::reverse_iterator boneReverseIterEnd = vBoneVector.rend();
	std::vector<CBoneObject*>::iterator boneIter = vBoneVector.begin();
	std::vector<CBoneObject*>::iterator boneIterEnd = vBoneVector.end();

	CFabrikHelper::ExecuteForwardReaching(boneReverseIter, boneReverseIterEnd, d3dxvTargetPos);
	D3DXVECTOR3 d3dxvBackwordPos(0.0f, 0.0f, 0.0f);
	CFabrikHelper::ExecuteBackwardReaching(boneIter, boneIterEnd, d3dxvBackwordPos);
}

void CFabrikHelper::ExecuteForwardReaching(std::vector<CBoneObject*>::reverse_iterator& BoneReverceIter, std::vector<CBoneObject*>::reverse_iterator& BoneReverceIterEnd, const D3DXVECTOR3& d3dxvTargetPos)
{
	if (BoneReverceIter == BoneReverceIterEnd)
		return;

	CBoneObject* pBoneObject = *BoneReverceIter;

	D3DXVECTOR3 d3dxvCurrBonePosition = pBoneObject->GetPosition();
	D3DXVECTOR3 d3dxvTargetDir = d3dxvTargetPos - d3dxvCurrBonePosition;

	d3dxvTargetDir *= -1;
	D3DXVec3Normalize(&d3dxvTargetDir, &d3dxvTargetDir);

	D3DXVECTOR3	d3dxvReverseBone = d3dxvTargetDir * pBoneObject->GetBoneLength();
	D3DXVECTOR3 d3dxvNextTargetPos = d3dxvTargetPos + d3dxvReverseBone;

	d3dxvTargetDir *= -1;
	D3DXVec3Normalize(&d3dxvTargetDir, &d3dxvTargetDir);

	D3DXMATRIX d3dxmtxRotation;
	D3DXVECTOR3 d3dxvCrossVec;

	D3DXVec3Cross(&d3dxvCrossVec, &D3DXVECTOR3(0.0f, 1.0f, 0.0f), &d3dxvTargetDir);
	D3DXVec3Normalize(&d3dxvCrossVec, &d3dxvCrossVec);

	float fDot = D3DXVec3Dot(&D3DXVECTOR3(0.0f, 1.0f, 0.0f), &d3dxvTargetDir);
	fDot = acos(fDot);
	
	D3DXMatrixRotationAxis(&d3dxmtxRotation, &d3dxvCrossVec, fDot);
	
	// 반드시 이 순서로 해야 한다. (Rot 행렬 만들고, 좌표를 넣어주는 식.)
	pBoneObject->Rotate(d3dxmtxRotation);
	pBoneObject->SetPosition(d3dxvNextTargetPos);
	pBoneObject->FabrikAnimate();

	ExecuteForwardReaching(++BoneReverceIter, BoneReverceIterEnd, d3dxvNextTargetPos);
}

void CFabrikHelper::ExecuteBackwardReaching(std::vector<CBoneObject*>::iterator& BoneIter, std::vector<CBoneObject*>::iterator& BoneIterEnd, const D3DXVECTOR3& d3dxvBackwardPos)
{
	if (BoneIter == BoneIterEnd)
		return;

	CBoneObject* pBoneObject = *BoneIter;
	pBoneObject->SetPosition(d3dxvBackwardPos);
	pBoneObject->FabrikAnimate();

	D3DXVECTOR3 d3dxvNextBonePos = pBoneObject->GetPosition() + pBoneObject->GetUp() * pBoneObject->GetBoneLength();

	ExecuteBackwardReaching(++BoneIter, BoneIterEnd, d3dxvNextBonePos);
}
