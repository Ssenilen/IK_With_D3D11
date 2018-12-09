#pragma once
#include "BoneObject.h"

class CAnalyticIKHelper
{
public:
	static void ExecuteAnalyticIKHelper(std::vector<CBoneObject*>& vBoneVector, const D3DXVECTOR3& d3dxvTargetPos);
};

