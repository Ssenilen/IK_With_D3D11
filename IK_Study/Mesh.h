#pragma once

class AABB
{
public:
	D3DXVECTOR3 m_d3dxvMinimum;
	D3DXVECTOR3 m_d3dxvMaximum;

public:
	AABB() { m_d3dxvMinimum = D3DXVECTOR3(+FLT_MAX, +FLT_MAX, +FLT_MAX); m_d3dxvMaximum = D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX); }
	AABB(D3DXVECTOR3 d3dxvMinimum, D3DXVECTOR3 d3dxvMaximum) { m_d3dxvMinimum = d3dxvMinimum; m_d3dxvMaximum = d3dxvMaximum; }

	// 두 개의 바운딩 박스를 합한다.
	void Union(D3DXVECTOR3& d3dxvMinimum, D3DXVECTOR3& d3dxvMaximum);
	void Union(AABB *pAABB);
	// 바운딩 박스의 8개의 꼭지점을 행렬로 변환하고 최소점과 최대점을 다시 계산한다.
	void Update(D3DXMATRIX *pd3dxmtxTransform);
};


class CVertex
{
private:
	D3DXVECTOR3 m_d3dxvPosition;

public:
	CVertex() { m_d3dxvPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f); }
	CVertex(D3DXVECTOR3 d3dxPosition) { m_d3dxvPosition = d3dxPosition; }
	~CVertex() {};
};


class CDiffusedVertex
{
	D3DXVECTOR3 m_d3dxvPosition;
	D3DXCOLOR m_d3dxcDiffuse;
public:
	CDiffusedVertex(float x, float y, float z, D3DXCOLOR d3dxcDiffuse) { m_d3dxvPosition = D3DXVECTOR3(x, y, z); m_d3dxcDiffuse = d3dxcDiffuse; }
	CDiffusedVertex(D3DXVECTOR3 d3dxvPosition, D3DXCOLOR d3dxcDiffuse) { m_d3dxvPosition = d3dxvPosition; m_d3dxcDiffuse = d3dxcDiffuse; }
	CDiffusedVertex() { m_d3dxvPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f); m_d3dxcDiffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f); }
	~CDiffusedVertex() { }
};


class CMesh
{
private:
	int m_nReferences;
protected:
	AABB m_bcBoundingCube;

	D3D11_PRIMITIVE_TOPOLOGY m_d3dPrimitiveTopology;

	// 정점의 위치, 색상점의 위치 벡터와 색상을 저장하기 위한 버퍼에 대한 인터페이스 포인터
	ID3D11Buffer *m_pd3dPositionBuffer;
	ID3D11Buffer *m_pd3dColorBuffer;
		
	UINT m_nSlot;		// 버퍼들을 입력조립기에 연결하기 위한 시작 슬롯 번호이다. 
	// 인스턴싱을 위한 정점 버퍼 = 메쉬의 정점 데이터 + 인스턴싱 데이터(위치, 방향)
	// 따라서, 인스턴싱을 위한 정점 버퍼는 하나가 아니라 버퍼들의 배열이 된다.
	ID3D11Buffer **m_ppd3dVertexBuffers;
	int m_nBuffers;		// 정점을 조립하기 위해 필요한 버퍼의 개수

	int m_nVertices;
	UINT m_nStartVertex;
	UINT *m_pnVertexStrides; // 버퍼의 원소 바이트 수를 나타내는 배열
	UINT *m_pnVertexOffsets; // 버퍼들의 시작 위치를 나타내는 배열

	ID3D11Buffer *m_pd3dIndexBuffer; // 인덱스 버퍼에 대한 인터페이스 포인터
	UINT m_nIndices;
	UINT m_nStartIndex;		// 인덱스 버퍼에서 메쉬 표현을 위한 시작 인덱스
	int m_nBaseVertex;		// 같 인덱스에 더해질 인덱스
	UINT m_nIndexOffset;	
	DXGI_FORMAT	m_dxgiIndexFormat; // 각 인덱스의 형식(DXGI_FORMAT_R32_UINT 또는 DXGI_FORMAT_R16_UINT) 

	ID3D11RasterizerState *m_pd3dRasterizerState;

public:
	CMesh(ID3D11Device *pd3dDevice);
	virtual ~CMesh();

	void AddRef() { m_nReferences++; }
	void Release() { if (--m_nReferences <= 0) delete this; }
	AABB GetBoundingCube() { return(m_bcBoundingCube); }

	// 메쉬의 정점 버퍼들을 배열로 조립 
	void AssembleToVertexBuffer(int nBuffers = 0, ID3D11Buffer **m_pd3dBuffers = NULL, UINT *pnBufferStrides = NULL, UINT *pnBufferOffsets = NULL);

	virtual void CreateRasterizerState(ID3D11Device *pd3dDevice);
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext);
	virtual void RenderInstanced(ID3D11DeviceContext *pd3dDeviceContext, int nInstances = 0, int nStartInstance = 0);
};