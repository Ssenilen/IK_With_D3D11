#pragma once
#include "GameObject.h"

class CCubeObject : public CGameObject
{
public:
	CCubeObject();
	~CCubeObject();

	virtual void Animate(float fTimeElapsed);
};

