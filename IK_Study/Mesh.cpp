#include "stdafx.h"
#include "Mesh.h"


//
// AABB Collision Check
//////////////////////////////////////////////////////////////////////////////////////////
void AABB::Union(D3DXVECTOR3& d3dxvMinimum, D3DXVECTOR3& d3dxvMaximum)
{
	if (d3dxvMinimum.x < m_d3dxvMinimum.x) m_d3dxvMinimum.x = d3dxvMinimum.x;
	if (d3dxvMinimum.y < m_d3dxvMinimum.y) m_d3dxvMinimum.y = d3dxvMinimum.y;
	if (d3dxvMinimum.z < m_d3dxvMinimum.z) m_d3dxvMinimum.z = d3dxvMinimum.z;
	if (d3dxvMaximum.x > m_d3dxvMaximum.x) m_d3dxvMaximum.x = d3dxvMaximum.x;
	if (d3dxvMaximum.y > m_d3dxvMaximum.y) m_d3dxvMaximum.y = d3dxvMaximum.y;
	if (d3dxvMaximum.z > m_d3dxvMaximum.z) m_d3dxvMaximum.z = d3dxvMaximum.z;
}

void AABB::Union(AABB *pAABB)
{
	Union(pAABB->m_d3dxvMinimum, pAABB->m_d3dxvMaximum);
}

void AABB::Update(D3DXMATRIX *pmtxTransform)
{
	// 바운딩 박스의 최소점과 최대점은 회전을 하면 더 이상 최소점과 최대점이 되지 않는다. 
	// 그러므로 바운딩 박스의 최소점과 최대점에서 8개의 정점을 구하고 변환(회전)을 한 다음
	// 최소점과 최대점을 다시 계산한다.	
	D3DXVECTOR3 vVertices[8];
	vVertices[0] = D3DXVECTOR3(m_d3dxvMinimum.x, m_d3dxvMinimum.y, m_d3dxvMinimum.z);
	vVertices[1] = D3DXVECTOR3(m_d3dxvMinimum.x, m_d3dxvMinimum.y, m_d3dxvMaximum.z);
	vVertices[2] = D3DXVECTOR3(m_d3dxvMaximum.x, m_d3dxvMinimum.y, m_d3dxvMaximum.z);
	vVertices[3] = D3DXVECTOR3(m_d3dxvMaximum.x, m_d3dxvMinimum.y, m_d3dxvMinimum.z);
	vVertices[4] = D3DXVECTOR3(m_d3dxvMinimum.x, m_d3dxvMaximum.y, m_d3dxvMinimum.z);
	vVertices[5] = D3DXVECTOR3(m_d3dxvMinimum.x, m_d3dxvMaximum.y, m_d3dxvMaximum.z);
	vVertices[6] = D3DXVECTOR3(m_d3dxvMaximum.x, m_d3dxvMaximum.y, m_d3dxvMaximum.z);
	vVertices[7] = D3DXVECTOR3(m_d3dxvMaximum.x, m_d3dxvMaximum.y, m_d3dxvMinimum.z);
	m_d3dxvMinimum = D3DXVECTOR3(+FLT_MAX, +FLT_MAX, +FLT_MAX);
	m_d3dxvMaximum = D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

	// 8개의 정점에서 x, y, z 좌표의 최소값과 최대값을 구한다.
	for (int i = 0; i < 8; i++)
	{
		// 정점을 변환한다.
		D3DXVec3TransformCoord(&vVertices[i], &vVertices[i], pmtxTransform);
		if (vVertices[i].x < m_d3dxvMinimum.x) m_d3dxvMinimum.x = vVertices[i].x;
		if (vVertices[i].y < m_d3dxvMinimum.y) m_d3dxvMinimum.y = vVertices[i].y;
		if (vVertices[i].z < m_d3dxvMinimum.z) m_d3dxvMinimum.z = vVertices[i].z;
		if (vVertices[i].x > m_d3dxvMaximum.x) m_d3dxvMaximum.x = vVertices[i].x;
		if (vVertices[i].y > m_d3dxvMaximum.y) m_d3dxvMaximum.y = vVertices[i].y;
		if (vVertices[i].z > m_d3dxvMaximum.z) m_d3dxvMaximum.z = vVertices[i].z;
	}
}


//
// CMesh Class
//////////////////////////////////////////////////////////////////////////////////////////
CMesh::CMesh(ID3D11Device *pd3dDevice)
{
	m_nBuffers = 0;
	m_pd3dPositionBuffer = NULL;
	m_pd3dColorBuffer = NULL;
	m_ppd3dVertexBuffers = NULL;

	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	m_nSlot = 0;
	m_nStartVertex = 0;

	m_pd3dIndexBuffer = NULL;
	m_nIndices = 0;
	m_nStartIndex = 0;
	m_nBaseVertex = 0;
	m_nIndexOffset = 0;
	m_dxgiIndexFormat = DXGI_FORMAT_R32_UINT;

	m_nReferences = 0;

	m_pd3dRasterizerState = NULL;
	m_bcBoundingCube.m_d3dxvMinimum = D3DXVECTOR3(+FLT_MAX, +FLT_MAX, +FLT_MAX);
	m_bcBoundingCube.m_d3dxvMaximum = D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
}

CMesh::~CMesh()
{
	if (m_pd3dRasterizerState) m_pd3dRasterizerState->Release();
	if (m_pd3dPositionBuffer) m_pd3dPositionBuffer->Release();
	if (m_pd3dColorBuffer) m_pd3dColorBuffer->Release();
	if (m_pd3dIndexBuffer) m_pd3dIndexBuffer->Release();

	if (m_ppd3dVertexBuffers) delete[] m_ppd3dVertexBuffers;
	if (m_pnVertexStrides) delete[] m_pnVertexStrides;
	if (m_pnVertexOffsets) delete[] m_pnVertexOffsets;
}

void CMesh::Render(ID3D11DeviceContext *pd3dDeviceContext)
{
	pd3dDeviceContext->IASetVertexBuffers(m_nSlot, m_nBuffers, m_ppd3dVertexBuffers, m_pnVertexStrides, m_pnVertexOffsets);
	pd3dDeviceContext->IASetIndexBuffer(m_pd3dIndexBuffer, m_dxgiIndexFormat, m_nIndexOffset);
	pd3dDeviceContext->IASetPrimitiveTopology(m_d3dPrimitiveTopology);
	pd3dDeviceContext->RSSetState(m_pd3dRasterizerState);

	if (m_pd3dIndexBuffer)
		pd3dDeviceContext->DrawIndexed(m_nIndices, m_nStartIndex, m_nBaseVertex);
	else
		pd3dDeviceContext->Draw(m_nVertices, m_nStartVertex);
}


void CMesh::RenderInstanced(ID3D11DeviceContext *pd3dDeviceContext, int nInstances, int nStartInstance)
{
	// 인스턴싱의 경우 입력 조립기에 메쉬의 정점 버퍼와 인스턴스 정점 버퍼가 연결된다.
	pd3dDeviceContext->IASetVertexBuffers(m_nSlot, m_nBuffers, m_ppd3dVertexBuffers, m_pnVertexStrides, m_pnVertexOffsets);
	pd3dDeviceContext->IASetIndexBuffer(m_pd3dIndexBuffer, m_dxgiIndexFormat, m_nIndexOffset);
	pd3dDeviceContext->IASetPrimitiveTopology(m_d3dPrimitiveTopology);
	pd3dDeviceContext->RSSetState(m_pd3dRasterizerState);

	if (m_pd3dIndexBuffer)
		pd3dDeviceContext->DrawIndexedInstanced(m_nIndices, nInstances, m_nStartIndex, m_nBaseVertex, nStartInstance);
	else
		pd3dDeviceContext->DrawInstanced(m_nVertices, nInstances, m_nStartVertex, nStartInstance);
}


void CMesh::CreateRasterizerState(ID3D11Device *pd3dDevice)
{
	// Default
	D3D11_RASTERIZER_DESC d3dRasterizerDesc;
	ZeroMemory(&d3dRasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
	d3dRasterizerDesc.CullMode = D3D11_CULL_BACK;
	d3dRasterizerDesc.FillMode = D3D11_FILL_SOLID;
	pd3dDevice->CreateRasterizerState(&d3dRasterizerDesc, &m_pd3dRasterizerState);
}


void CMesh::AssembleToVertexBuffer(int nBuffers, ID3D11Buffer **ppd3dBuffers, UINT *pnBufferStrides, UINT *pnBufferOffsets)
{
	ID3D11Buffer **ppd3dNewVertexBuffers = new ID3D11Buffer*[m_nBuffers + nBuffers];
	UINT *pnNewVertexStrides = new UINT[m_nBuffers + nBuffers];
	UINT *pnNewVertexOffsets = new UINT[m_nBuffers + nBuffers];

	if (m_nBuffers > 0)
	{
		for (int i = 0; i < m_nBuffers; i++)
		{
			ppd3dNewVertexBuffers[i] = m_ppd3dVertexBuffers[i];
			pnNewVertexStrides[i] = m_pnVertexStrides[i];
			pnNewVertexOffsets[i] = m_pnVertexOffsets[i];
		}
		if (m_ppd3dVertexBuffers) delete[] m_ppd3dVertexBuffers;
		if (m_pnVertexStrides) delete[] m_pnVertexStrides;
		if (m_pnVertexOffsets) delete[] m_pnVertexOffsets;
	}

	for (int i = 0; i < nBuffers; i++)
	{
		ppd3dNewVertexBuffers[m_nBuffers + i] = ppd3dBuffers[i];
		pnNewVertexStrides[m_nBuffers + i] = pnBufferStrides[i];
		pnNewVertexOffsets[m_nBuffers + i] = pnBufferOffsets[i];
	}

	m_nBuffers += nBuffers;
	m_ppd3dVertexBuffers = ppd3dNewVertexBuffers;
	m_pnVertexStrides = pnNewVertexStrides;
	m_pnVertexOffsets = pnNewVertexOffsets;
}
