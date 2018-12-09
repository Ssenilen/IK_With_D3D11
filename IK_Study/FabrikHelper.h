#pragma once
#include "BoneObject.h"

class CFabrikHelper
{
public:
	static void ExecuteFabrik(std::vector<CBoneObject*>& vBoneVector, const D3DXVECTOR3& d3dxvTargetPos);
	static void ExecuteForwardReaching(std::vector<CBoneObject*>::reverse_iterator& BoneReverceIter, std::vector<CBoneObject*>::reverse_iterator& BoneReverceIterEnd, const D3DXVECTOR3& d3dxvTargetPos);
	static void ExecuteBackwardReaching(std::vector<CBoneObject*>::iterator& BoneIter, std::vector<CBoneObject*>::iterator& BoneIterEnd, const D3DXVECTOR3& d3dxvBackwardPos);
};

