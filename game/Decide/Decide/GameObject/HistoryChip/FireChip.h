#pragma once
#include "fbEngine/GameObject.h"

class FireChip : public GameObject
{
public:
	FireChip(const char* name);
	void Awake()override;
	void Update()override;
private:
};
