#pragma once
#include "Mesh.h"
#include "Camera.h"

class CGameObject
{
private:
	int m_nReferences;

protected:
	float m_fRotationSpeed;

public:
	CGameObject();
	virtual ~CGameObject();

	void AddRef();
	void Release();

	D3DXMATRIX m_d3dxmtxLocal;
	D3DXMATRIX m_d3dxmtxWorld;
	D3DXMATRIX m_d3dxmtxRotate;
	D3DXMATRIX m_d3dxmtxScale;
	D3DXMATRIX m_d3dxmtxTranlate;

	float m_fRotationAngle;

	CMesh *m_pMesh;

	virtual void SetMesh(CMesh *pMesh);
	virtual void Animate(float fTimeElapsed);
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext);

	virtual void SetPosition(float x, float y, float z);
	virtual void SetPosition(D3DXVECTOR3 d3dxvPosition);

	D3DXVECTOR3 GetPosition();

	void MoveRight(float fDistance = 1.0f);
	void MoveUp(float fDistance = 1.0f);
	void MoveForward(float fDistance = 1.0f);

	void Rotate(float fPitch = 10.0f, float fYaw = 10.0f, float fRoll = 10.0f);
	void Rotate(D3DXVECTOR3* pd3dAxis, float fAngle);

	D3DXVECTOR3 GetLookAt();
	D3DXVECTOR3 GetUp();
	D3DXVECTOR3 GetRight();
	CMesh *GetMesh() { return m_pMesh; }

	virtual bool IsVisible(CCamera *pCamera = NULL);

	// 객체 렌더링 전에 호출할 함수
	virtual void OnPrepareRender() {}
};