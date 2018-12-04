#pragma once
#include "BoneObject.h"

class CCCDHelper
{
public:
	static void ExecuteCCD(std::vector<CBoneObject*>& vBoneVector, const D3DXVECTOR3& d3dxvTargetPos);
};

