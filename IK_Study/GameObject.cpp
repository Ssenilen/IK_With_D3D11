#include "stdafx.h"
#include "GameObject.h"
#include "Shader.h"

CGameObject::CGameObject()
{
	D3DXMatrixIdentity(&m_d3dxmtxWorld);
	D3DXMatrixIdentity(&m_d3dxmtxLocal);
	D3DXMatrixIdentity(&m_d3dxmtxRotate);
	D3DXMatrixIdentity(&m_d3dxmtxScale);
	D3DXMatrixIdentity(&m_d3dxmtxTranlate); 
	m_fRotationAngle = 0.0f;

	m_pMesh = NULL;

	m_nReferences = 0;
}


CGameObject::~CGameObject()
{
	if (m_pMesh) m_pMesh->Release();
}


void CGameObject::AddRef()
{
	m_nReferences++;
}


void CGameObject::Release()
{
	if (m_nReferences > 0) m_nReferences--;
	if (m_nReferences <= 0) delete this;
}


void CGameObject::SetMesh(CMesh *pMesh)
{
	if (m_pMesh) m_pMesh->Release();
	m_pMesh = pMesh;
	if (m_pMesh) m_pMesh->AddRef();
}


void CGameObject::Animate(float fTimeElapsed)
{
}


void CGameObject::Render(ID3D11DeviceContext *pd3dDeviceContext)
{
	OnPrepareRender();
	CShader::UpdateShaderVariable(pd3dDeviceContext, &m_d3dxmtxWorld);
	if (m_pMesh) m_pMesh->Render(pd3dDeviceContext);
}


void CGameObject::SetPosition(float x, float y, float z)
{
	m_d3dxmtxTranlate._41 = x;
	m_d3dxmtxTranlate._42 = y;
	m_d3dxmtxTranlate._43 = z;
	//m_d3dxmtxWorld._41 = x;
	//m_d3dxmtxWorld._42 = y;
	//m_d3dxmtxWorld._43 = z;
}


void CGameObject::SetPosition(D3DXVECTOR3 d3dxvPosition)
{
	m_d3dxmtxTranlate._41 = d3dxvPosition.x;
	m_d3dxmtxTranlate._42 = d3dxvPosition.y;
	m_d3dxmtxTranlate._43 = d3dxvPosition.z;
}


D3DXVECTOR3 CGameObject::GetPosition()
{
	return D3DXVECTOR3(m_d3dxmtxTranlate._41, m_d3dxmtxTranlate._42, m_d3dxmtxTranlate._43);
}

D3DXVECTOR3 CGameObject::GetLookAt()
{
	D3DXVECTOR3 d3dxvLookAt(m_d3dxmtxWorld._31, m_d3dxmtxWorld._32, m_d3dxmtxWorld._33);
	D3DXVec3Normalize(&d3dxvLookAt, &d3dxvLookAt);
	return d3dxvLookAt;
} 

D3DXVECTOR3 CGameObject::GetUp()
{
	D3DXVECTOR3 d3dxvUp(m_d3dxmtxWorld._21, m_d3dxmtxWorld._22, m_d3dxmtxWorld._23);
	D3DXVec3Normalize(&d3dxvUp, &d3dxvUp);
	return d3dxvUp;
}

D3DXVECTOR3 CGameObject::GetRight()
{
	D3DXVECTOR3 d3dxvRight(m_d3dxmtxWorld._11, m_d3dxmtxWorld._12, m_d3dxmtxWorld._13);
	D3DXVec3Normalize(&d3dxvRight, &d3dxvRight);
	return d3dxvRight;
}

void CGameObject::MoveRight(float fDistance)
{
	D3DXVECTOR3 d3dxvPosition = GetPosition();
	D3DXVECTOR3 d3dxvRight = GetRight();
	d3dxvPosition += fDistance * d3dxvRight;
	CGameObject::SetPosition(d3dxvPosition);
}

void CGameObject::MoveUp(float fDistance)
{
	D3DXVECTOR3 d3dxvPosition = GetPosition();
	D3DXVECTOR3 d3dxvUp = GetUp();
	d3dxvPosition += fDistance * d3dxvUp;
	CGameObject::SetPosition(d3dxvPosition);
}

void CGameObject::MoveForward(float fDistance)
{
	D3DXVECTOR3 d3dxvPosition = GetPosition();
	D3DXVECTOR3 d3dxvLookAt = GetLookAt();
	d3dxvPosition += fDistance * d3dxvLookAt;
	CGameObject::SetPosition(d3dxvPosition);
}

void CGameObject::Rotate(float fPitch, float fYaw, float fRoll)
{
	D3DXMATRIX mtxRotate;
	D3DXMatrixRotationYawPitchRoll(&mtxRotate, (float)D3DXToRadian(fYaw), (float)D3DXToRadian(fPitch), (float)D3DXToRadian(fRoll));
	m_d3dxmtxWorld = mtxRotate * m_d3dxmtxWorld;
}

void CGameObject::Rotate(D3DXVECTOR3 *pd3dxvAxis, float fAngle)
{
	D3DXMATRIX mtxRotate;
	D3DXMatrixRotationAxis(&mtxRotate, pd3dxvAxis, (float)D3DXToRadian(fAngle));
	m_d3dxmtxWorld = mtxRotate * m_d3dxmtxWorld;
}

bool CGameObject::IsVisible(CCamera *pCamera)
{
	bool bIsVisible = true;
	AABB bcBoundingCube;
	if (m_pMesh)
	{
		bcBoundingCube = m_pMesh->GetBoundingCube();
		// 객체의 메쉬의 바운딩박스(모델 좌표계)를 객체의 월드 변환 행렬로 변환,
		// 이후 새로운 바운딩 박스를 계산
		bcBoundingCube.Update(&m_d3dxmtxWorld);
	}
	// 바운딩 박스(월드 좌표계)가 카메라의 절두체에 포함되는 가를 검사한다. 
	if (pCamera) bIsVisible = pCamera->IsInFrustum(&bcBoundingCube);
	return bIsVisible;
}