#pragma once
#include "fbEngine\_Object\_GameObject\GameObject.h"

class RockCave : public GameObject
{
public:
	RockCave(const char* name);
	void Awake()override;
	void Start()override;
	void Update()override;
};
