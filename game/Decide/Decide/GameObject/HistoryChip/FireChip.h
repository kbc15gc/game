#pragma once
#include "fbEngine\_Object\_GameObject\GameObject.h"

class FireChip : public GameObject
{
public:
	FireChip(const char* name);
	void Awake()override;
	void Update()override;
private:
};
