#pragma once

class AABB
{
public:
	D3DXVECTOR3 m_d3dxvMinimum;
	D3DXVECTOR3 m_d3dxvMaximum;

public:
	AABB() { m_d3dxvMinimum = D3DXVECTOR3(+FLT_MAX, +FLT_MAX, +FLT_MAX); m_d3dxvMaximum = D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX); }
	AABB(D3DXVECTOR3 d3dxvMinimum, D3DXVECTOR3 d3dxvMaximum) { m_d3dxvMinimum = d3dxvMinimum; m_d3dxvMaximum = d3dxvMaximum; }

	// �� ���� �ٿ�� �ڽ��� ���Ѵ�.
	void Union(D3DXVECTOR3& d3dxvMinimum, D3DXVECTOR3& d3dxvMaximum);
	void Union(AABB *pAABB);
	// �ٿ�� �ڽ��� 8���� �������� ��ķ� ��ȯ�ϰ� �ּ����� �ִ����� �ٽ� ����Ѵ�.
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

	// ������ ��ġ, �������� ��ġ ���Ϳ� ������ �����ϱ� ���� ���ۿ� ���� �������̽� ������
	ID3D11Buffer *m_pd3dPositionBuffer;
	ID3D11Buffer *m_pd3dColorBuffer;
		
	UINT m_nSlot;		// ���۵��� �Է������⿡ �����ϱ� ���� ���� ���� ��ȣ�̴�. 
	// �ν��Ͻ��� ���� ���� ���� = �޽��� ���� ������ + �ν��Ͻ� ������(��ġ, ����)
	// ����, �ν��Ͻ��� ���� ���� ���۴� �ϳ��� �ƴ϶� ���۵��� �迭�� �ȴ�.
	ID3D11Buffer **m_ppd3dVertexBuffers;
	int m_nBuffers;		// ������ �����ϱ� ���� �ʿ��� ������ ����

	int m_nVertices;
	UINT m_nStartVertex;
	UINT *m_pnVertexStrides; // ������ ���� ����Ʈ ���� ��Ÿ���� �迭
	UINT *m_pnVertexOffsets; // ���۵��� ���� ��ġ�� ��Ÿ���� �迭

	ID3D11Buffer *m_pd3dIndexBuffer; // �ε��� ���ۿ� ���� �������̽� ������
	UINT m_nIndices;
	UINT m_nStartIndex;		// �ε��� ���ۿ��� �޽� ǥ���� ���� ���� �ε���
	int m_nBaseVertex;		// �� �ε����� ������ �ε���
	UINT m_nIndexOffset;	
	DXGI_FORMAT	m_dxgiIndexFormat; // �� �ε����� ����(DXGI_FORMAT_R32_UINT �Ǵ� DXGI_FORMAT_R16_UINT) 

	ID3D11RasterizerState *m_pd3dRasterizerState;

public:
	CMesh(ID3D11Device *pd3dDevice);
	virtual ~CMesh();

	void AddRef() { m_nReferences++; }
	void Release() { if (--m_nReferences <= 0) delete this; }
	AABB GetBoundingCube() { return(m_bcBoundingCube); }

	// �޽��� ���� ���۵��� �迭�� ���� 
	void AssembleToVertexBuffer(int nBuffers = 0, ID3D11Buffer **m_pd3dBuffers = NULL, UINT *pnBufferStrides = NULL, UINT *pnBufferOffsets = NULL);

	virtual void CreateRasterizerState(ID3D11Device *pd3dDevice);
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext);
	virtual void RenderInstanced(ID3D11DeviceContext *pd3dDeviceContext, int nInstances = 0, int nStartInstance = 0);
};