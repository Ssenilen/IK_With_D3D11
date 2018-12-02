#include "stdafx.h"
#include "Shader.h"

#include "BoneObject.h"
#include "MainBoneObject.h"
#include "CubeObject.h"
#include "BoneMesh.h"
#include "CubeMesh.h"



// ���� ��ȯ ����� ���� ��� ���۴� ���̴� ��ü�� ����(static) ������ ����̴�.
ID3D11Buffer *CShader::m_pd3dcbWorldMatrix = NULL;

CShader::CShader()
{
	m_ppObjects = NULL;
	m_nObjects = 0;

	m_pd3dVertexShader = NULL;
	m_pd3dVertexLayout = NULL;
	m_pd3dPixelShader = NULL;
}


CShader::~CShader()
{
	if (m_pd3dVertexShader) m_pd3dVertexShader->Release();
	if (m_pd3dVertexLayout) m_pd3dVertexLayout->Release();
	if (m_pd3dPixelShader) m_pd3dPixelShader->Release();
}


void CShader::CreateVertexShaderFromFile(ID3D11Device *pd3dDevice, WCHAR *pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderModel,
	ID3D11VertexShader **ppd3dVertexShader, D3D11_INPUT_ELEMENT_DESC *pd3dInputLayout, UINT nElements, ID3D11InputLayout **ppd3dVertexLayout)
{
	HRESULT hResult;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob *pd3dShaderBlob = NULL, *pd3dErrorBlob = NULL;

	if (SUCCEEDED(hResult = D3DX11CompileFromFile(pszFileName, NULL, NULL, pszShaderName, pszShaderModel, dwShaderFlags, 0, NULL, &pd3dShaderBlob, &pd3dErrorBlob, NULL)))
	{
		// �����ϵ� ���̴� �ڵ��� �޸� �ּҿ��� ����-���̴��� �����Ѵ�. 
		pd3dDevice->CreateVertexShader(pd3dShaderBlob->GetBufferPointer(), pd3dShaderBlob->GetBufferSize(), NULL, ppd3dVertexShader);
		// �����ϵ� ���̴� �ڵ��� �޸� �ּҿ� �Է� ���̾ƿ����� ���� ���̾ƿ��� �����Ѵ�. 
		pd3dDevice->CreateInputLayout(pd3dInputLayout, nElements, pd3dShaderBlob->GetBufferPointer(), pd3dShaderBlob->GetBufferSize(), ppd3dVertexLayout);
		pd3dShaderBlob->Release();
	}
	else
	{
		MessageBox(NULL, L"CreateVertexShaderFromFile Error", L"ERROR_VS", MB_OK);
		exit(1);
	}
}

void CShader::CreatePixelShaderFromFile(ID3D11Device *pd3dDevice, WCHAR *pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderModel, ID3D11PixelShader **ppd3dPixelShader)
{
	HRESULT hResult;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob *pd3dShaderBlob = NULL, *pd3dErrorBlob = NULL;
	// ����(pszFileName)���� ���̴� �Լ�(pszShaderName)�� �������Ͽ� �����ϵ� ���̴� �ڵ��� �޸� �ּ�(pd3dShaderBlob)�� ��ȯ�Ѵ�.
	if (SUCCEEDED(hResult = D3DX11CompileFromFile(pszFileName, NULL, NULL, pszShaderName, pszShaderModel, dwShaderFlags, 0, NULL, &pd3dShaderBlob, &pd3dErrorBlob, NULL)))
	{
		// �����ϵ� ���̴� �ڵ��� �޸� �ּҿ��� �ȼ�-���̴��� �����Ѵ�. 
		pd3dDevice->CreatePixelShader(pd3dShaderBlob->GetBufferPointer(), pd3dShaderBlob->GetBufferSize(), NULL, ppd3dPixelShader);
		pd3dShaderBlob->Release();
	}
	else
	{
		MessageBox(NULL, L"CreatePixelShaderFromFile Error", L"ERROR_PS", MB_OK);
		exit(1);
	}
}


void CShader::CreateShader(ID3D11Device *pd3dDevice)
{
	// �޽��� 2���� ���� ����(��ġ ���Ϳ� ����)�� ǥ���Ǹ�,
	// �� ���� ���۴� �Է� �������� �ٸ� ���Կ� ����Ǿ�� �Կ� ����!
	D3D11_INPUT_ELEMENT_DESC d3dInputLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT nElements = ARRAYSIZE(d3dInputLayout);
	CreateVertexShaderFromFile(pd3dDevice, L"VS.fx", "VS", "vs_5_0", &m_pd3dVertexShader, d3dInputLayout, nElements, &m_pd3dVertexLayout);
	CreatePixelShaderFromFile(pd3dDevice, L"PS.fx", "PS", "ps_5_0", &m_pd3dPixelShader);
}


void CShader::Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera)
{
	int test = 0;
	OnPrepareRender(pd3dDeviceContext);
	// ���̴� ��ü�� ��� ���� ��ü���� �������Ѵ�.
	for (int j = 0; j < m_nObjects; j++)
	{
		if (m_ppObjects[j])
		{
			if (m_ppObjects[j]->IsVisible(pCamera))
			{
				m_ppObjects[j]->Render(pd3dDeviceContext);
				test++;
			}
		}
	}
	//printf("��������������������������������\r[DEBUG] �������� ��ü ����: %d��", test);
}


void CShader::CreateShaderVariables(ID3D11Device *pd3dDevice)
{
	// ���� ��ȯ ����� ���� ��� ���۸� ����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VS_CB_WORLD_MATRIX);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	pd3dDevice->CreateBuffer(&bd, NULL, &m_pd3dcbWorldMatrix);
}


void CShader::ReleaseShaderVariables()
{
	// ���� ��ȯ ����� ���� ��� ���۸� ��ȯ
	if (m_pd3dcbWorldMatrix) m_pd3dcbWorldMatrix->Release();
}


void CShader::UpdateShaderVariable(ID3D11DeviceContext *pd3dDeviceContext, D3DXMATRIX *pd3dxmtxWorld)
{
	// ���� ��ȯ ����� ��� ���ۿ� �����Ѵ�.
	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	pd3dDeviceContext->Map(m_pd3dcbWorldMatrix, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	VS_CB_WORLD_MATRIX *pcbWorldMatrix = (VS_CB_WORLD_MATRIX *)d3dMappedResource.pData;
	D3DXMatrixTranspose(&pcbWorldMatrix->m_d3dxmtxWorld, pd3dxmtxWorld);
	pd3dDeviceContext->Unmap(m_pd3dcbWorldMatrix, 0);

	// ��� ���۸� ����̽��� ����(VS_SLOT_WORLD_MATRIX)�� �����Ѵ�.
	pd3dDeviceContext->VSSetConstantBuffers(VS_SLOT_WORLD_MATRIX, 1, &m_pd3dcbWorldMatrix);
}


void CShader::BuildObjects(ID3D11Device *pd3dDevice)
{
	CBoneMesh *pBoneMesh = new CBoneMesh(pd3dDevice, 5.0f, 20.0f, 5.0f, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));

	m_nObjects = 1;
	m_ppObjects = new CGameObject*[m_nObjects];

	const int nBoneCount = 5;

	for (int y = 0; y < m_nObjects; ++y)
	{
		CMainBoneObject* pMainBoneObject = new CMainBoneObject(nBoneCount, pBoneMesh);
		m_ppObjects[y] = pMainBoneObject;
	}
}


void CShader::ReleaseObjects()
{
	if (m_ppObjects)
	{
		for (int j = 0; j < m_nObjects; j++) if (m_ppObjects[j]) delete m_ppObjects[j];
		delete[] m_ppObjects;
	}
}


void CShader::AnimateObjects(float fTimeElapsed)
{
	for (int j = 0; j < m_nObjects; j++)
	{
		m_ppObjects[j]->Animate(fTimeElapsed);
	}
}


void CShader::OnPrepareRender(ID3D11DeviceContext *pd3dDeviceContext)
{
	pd3dDeviceContext->IASetInputLayout(m_pd3dVertexLayout);
	pd3dDeviceContext->VSSetShader(m_pd3dVertexShader, NULL, 0);
	pd3dDeviceContext->PSSetShader(m_pd3dPixelShader, NULL, 0);
}


CSceneShader::CSceneShader()
{
}

CSceneShader::~CSceneShader()
{
}

void CSceneShader::CreateShader(ID3D11Device *pd3dDevice)
{
	CShader::CreateShader(pd3dDevice);
}

void CSceneShader::BuildObjects(ID3D11Device *pd3dDevice)
{
	CCubeMesh *pCubeMesh = new CCubeMesh(pd3dDevice, 12.0f, 12.0f, 12.0f, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

	int xObjects = 5, yObjects = 5, zObjects = 5, i = 0;
	m_nObjects = (xObjects * 2 + 1)*(yObjects * 2 + 1)*(zObjects * 2 + 1);
	m_ppObjects = new CGameObject*[m_nObjects];

	float fxPitch = 12.0f * 2.5f;
	float fyPitch = 12.0f * 2.5f;
	float fzPitch = 12.0f * 2.5f;
	CCubeObject *pCubeObject = NULL;
	for (int x = -xObjects; x <= xObjects; x++)
	{
		for (int y = -yObjects; y <= yObjects; y++)
		{
			for (int z = -zObjects; z <= zObjects; z++)
			{
				pCubeObject = new CCubeObject();
				pCubeObject->SetMesh(pCubeMesh);
				// pObject->SetShader(pShader);
				pCubeObject->SetPosition(fxPitch*x, fyPitch*y, fzPitch*z);
				m_ppObjects[i++] = pCubeObject;
			}
		}
	}
}

CPlayerShader::CPlayerShader()
{
}

CPlayerShader::~CPlayerShader()
{
}

void CPlayerShader::CreateShader(ID3D11Device *pd3dDevice)
{
	CShader::CreateShader(pd3dDevice);
}

void CPlayerShader::BuildObjects(ID3D11Device *pd3dDevice)
{
	m_nObjects = 1;
	m_ppObjects = new CGameObject*[m_nObjects];

	ID3D11DeviceContext *pd3dDeviceContext = NULL;
	pd3dDevice->GetImmediateContext(&pd3dDeviceContext);

	CCameraPlayer *pCameraPlayer = new CCameraPlayer(pd3dDevice);
	pCameraPlayer->SetMesh(NULL);
	pCameraPlayer->CreateShaderVariables(pd3dDevice);
	pCameraPlayer->ChangeCamera(pd3dDevice, THIRD_PERSON_CAMERA, 0.0f);

	CCamera *pCamera = pCameraPlayer->GetCamera();
	pCamera->SetViewport(pd3dDeviceContext, 0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT, 0.0f, 1.0f);
	pCamera->GenerateProjectionMatrix(1.01f, 50000.0f, ASPECT_RATIO, 60.0f);
	pCamera->GenerateViewMatrix();

	pCameraPlayer->SetPosition(D3DXVECTOR3(0.0f, 10.0f, -50.0f));
	m_ppObjects[0] = pCameraPlayer;

	if (pd3dDeviceContext) pd3dDeviceContext->Release();
}



//
// CInstancingShader
//////////////////////////////////////////////////////////////////////////////////////////
CInstancingShader::CInstancingShader()
{
	m_pd3dCubeInstanceBuffer = NULL;
}

CInstancingShader::~CInstancingShader()
{
	if (m_pd3dCubeInstanceBuffer) m_pd3dCubeInstanceBuffer->Release();
}

void CInstancingShader::CreateShader(ID3D11Device *pd3dDevice)
{
	// �ν��Ͻ� ���۴� 1���� ����(���+����)�� �����Ѵ�. ���� ��ȣ�� '2'
	D3D11_INPUT_ELEMENT_DESC d3dInputLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "INSTANCEPOS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 2, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "INSTANCEPOS", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 2, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "INSTANCEPOS", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 2, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "INSTANCEPOS", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 2, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "INSTANCECOLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 2, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 }
	};
	UINT nElements = ARRAYSIZE(d3dInputLayout);
	CreateVertexShaderFromFile(pd3dDevice, L"VS.fx", "VSInstancedDiffusedColor", "vs_5_0", &m_pd3dVertexShader, d3dInputLayout, nElements, &m_pd3dVertexLayout);
	CreatePixelShaderFromFile(pd3dDevice, L"PS.fx", "PSInstancedDiffusedColor", "ps_5_0", &m_pd3dPixelShader);
}

ID3D11Buffer *CInstancingShader::CreateInstanceBuffer(ID3D11Device *pd3dDevice, int nObjects, UINT nBufferStride, void *pBufferData)
{
	ID3D11Buffer *pd3dInstanceBuffer = NULL;
	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	// ������ �ʱ�ȭ �����Ͱ� ������ ���� ���۷� �����Ѵ�.
	// (���߿� ������ �Ͽ� ������ ä��ų� ����)
	d3dBufferDesc.Usage = (pBufferData) ? D3D11_USAGE_DEFAULT : D3D11_USAGE_DYNAMIC;
	d3dBufferDesc.ByteWidth = nBufferStride * nObjects;
	d3dBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	d3dBufferDesc.CPUAccessFlags = (pBufferData) ? 0 : D3D11_CPU_ACCESS_WRITE;
	D3D11_SUBRESOURCE_DATA d3dBufferData;
	ZeroMemory(&d3dBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3dBufferData.pSysMem = pBufferData;
	pd3dDevice->CreateBuffer(&d3dBufferDesc, (pBufferData) ? &d3dBufferData : NULL, &pd3dInstanceBuffer);
	return pd3dInstanceBuffer;
}

void CInstancingShader::BuildObjects(ID3D11Device *pd3dDevice)
{
	m_nInstanceBufferStride = sizeof(VS_VB_INSTANCE);
	m_nInstanceBufferOffset = 0;

	int xObjects = 10, yObjects = 10, zObjects = 10, i = 0;
	int nCubeObjects = (xObjects * 2 + 1) * (yObjects * 2 + 1) * (zObjects * 2 + 1);

	m_nObjects = nCubeObjects;
	m_ppObjects = new CGameObject*[m_nObjects];

	CCubeMesh *pCubeMesh = new CCubeMesh(pd3dDevice, 12.0f, 12.0f, 12.0f, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));

	float fxPitch = 12.0f * 2.5f;
	float fyPitch = 12.0f * 2.5f;
	float fzPitch = 12.0f * 2.5f;
	CCubeObject *pCubeObject = NULL;
	for (int x = -xObjects; x <= xObjects; x++)
	{
		for (int y = -yObjects; y <= yObjects; y++)
		{
			for (int z = -zObjects; z <= zObjects; z++)
			{
				pCubeObject = new CCubeObject();
				pCubeObject->SetMesh(pCubeMesh);
				pCubeObject->SetPosition(fxPitch*x, fyPitch*y, fzPitch*z);
				m_ppObjects[i++] = pCubeObject;
			}
		}
	}
	m_pd3dCubeInstanceBuffer = CreateInstanceBuffer(pd3dDevice, nCubeObjects, m_nInstanceBufferStride, NULL);
	pCubeMesh->AssembleToVertexBuffer(1, &m_pd3dCubeInstanceBuffer, &m_nInstanceBufferStride, &m_nInstanceBufferOffset);
}


void CInstancingShader::Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera)
{
	OnPrepareRender(pd3dDeviceContext);

	int test = 0;
	int nCubeObjects = m_nObjects;
	CMesh *pCubeMesh = m_ppObjects[0]->GetMesh();

	int nCubeInstances = 0;
	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	pd3dDeviceContext->Map(m_pd3dCubeInstanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	VS_VB_INSTANCE *pCubeInstances = (VS_VB_INSTANCE *)d3dMappedResource.pData;
	for (int j = 0; j < nCubeObjects; j++)
	{
		if (m_ppObjects[j])
		{
			// ������ü ��ü�� ī�޶� ���� ��(������ü ��ü�� �ٿ�� �ڽ��� ����ü�� ������ ��) 
			// �ν��Ͻ� �����͸� �ν��Ͻ� ���۷� ����
			if (m_ppObjects[j]->IsVisible(pCamera))
			{
				D3DXMatrixTranspose(&pCubeInstances[nCubeInstances].m_d3dxTransform, &m_ppObjects[j]->m_d3dxmtxWorld);
				pCubeInstances[nCubeInstances++].m_d3dxColor = (j % 2) ? D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f) : RANDOM_COLOR;
				test++;
			}
		}
	}
	//printf("��������������������������������\r[DEBUG] �������� ��ü ����: %d��", test);
	pd3dDeviceContext->Unmap(m_pd3dCubeInstanceBuffer, 0);

	// ī�޶� ���̴� ������ü�鸸 �ν��Ͻ��� �Ѵ�.
	pCubeMesh->RenderInstanced(pd3dDeviceContext, nCubeInstances, 0);
}
