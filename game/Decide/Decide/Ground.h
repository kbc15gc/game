#pragma once
#include "fbEngine\_Object\_GameObject\GameObject.h"

class Ground : public GameObject
{
public:
	Ground(const char* name);
	void Awake()override;
	void Update()override;
private:
};