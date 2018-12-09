#include "stdafx.h"
#include "AnalyticIKHelper.h"

int g_testCount = 0;
void CAnalyticIKHelper::ExecuteAnalyticIKHelper(std::vector<CBoneObject*>& vBoneVector, const D3DXVECTOR3& d3dxvTargetPos)
{
	// 두 개의 본에 대해서만 계산한다.
	if (vBoneVector.size() != 2)
		return;

	// (1) 제 2 코사인 법칙을 활용하여 JointBone의 방향을 결정
	CBoneObject* pRootBone = vBoneVector[0];
	CBoneObject* pJointBone = vBoneVector[1];

	D3DXVECTOR3 d3dxvFootDir = d3dxvTargetPos - pRootBone->GetPosition();

	float fA = pRootBone->GetBoneLength();
	float fB = pJointBone->GetBoneLength();
	float fC = D3DXVec3Length(&d3dxvFootDir);

	// 여기서의 B는 뼈대를 꺾어주기 위한 Joint의 반대편 각도(pi - B)를 의미한다.
	
	// C^2 = A^2 + B^2 - 2AB * cos(pi - B)
	// C^2 = A^2 + B^2 + 2AB * cos(B)
	// cos(B) = (C^2 - A^2 - B^2) / 2AB ... 이므로
	float fCosB = (fC*fC - fA*fA - fB*fB) / (2 * fA*fB);

	// 왜 바뀌는 축에 적용하면 문제가 생기지... 일단은 강제로 0,0,1을 기준으로 회전하게 해서 해결했다.
	D3DXVECTOR3 d3dxvJointBoneAxis = pJointBone->GetRight(); 

	fCosB = max(-1.0f, min(1.0, fCosB)); // clamp
	float fArcCosB = acos(fCosB);


	D3DXMATRIX d3dxmtxRotate;
	D3DXMatrixIdentity(&d3dxmtxRotate);
	D3DXMatrixRotationAxis(&d3dxmtxRotate, &D3DXVECTOR3(0.0f, 0.0f, 1.0f), fArcCosB);

	D3DXMATRIX d3dxmtxParent;
	pJointBone->Rotate(d3dxmtxRotate);
	D3DXMatrixIdentity(&d3dxmtxParent);
	for (auto boneObject : vBoneVector)
	{
		d3dxmtxParent = boneObject->Animate(0, d3dxmtxParent);
	}

	// (2) 이제 맞춰보자... TargetPos의 방향으로.
	D3DXVECTOR3 d3dxvEndEffectorDir((pJointBone->GetPosition() + pJointBone->GetBoneLength() * pJointBone->GetUp()) - pRootBone->GetPosition());
	D3DXVec3Normalize(&d3dxvEndEffectorDir, &d3dxvEndEffectorDir);
	D3DXVec3Normalize(&d3dxvFootDir, &d3dxvFootDir);

	D3DXVECTOR3 d3dxvRotateAxis;
	D3DXVec3Cross(&d3dxvRotateAxis, &d3dxvEndEffectorDir, &d3dxvFootDir);
	D3DXVec3Normalize(&d3dxvRotateAxis, &d3dxvRotateAxis);

	if (D3DXVec3LengthSq(&d3dxvRotateAxis) == 0)
		return;

	float fDot = D3DXVec3Dot(&d3dxvEndEffectorDir, &d3dxvFootDir);

	fDot = max(-1.0f, min(1.0, fDot)); // clamp
	fDot = acos(fDot);
	D3DXMatrixIdentity(&d3dxmtxRotate);
	D3DXMatrixRotationAxis(&d3dxmtxRotate, &d3dxvRotateAxis, fDot);
	std::cout << fDot << std::endl;

	pRootBone->Rotate(d3dxmtxRotate);
	
	D3DXMatrixIdentity(&d3dxmtxParent);
	for (auto boneObject : vBoneVector)
	{
		d3dxmtxParent = boneObject->Animate(0, d3dxmtxParent);
	}

	D3DXVECTOR3 test((pJointBone->GetPosition() + pJointBone->GetBoneLength() * pJointBone->GetUp()) - pRootBone->GetPosition());
	float fLength = D3DXVec3Length(&test);
	std::cout << fLength << std::endl;
}