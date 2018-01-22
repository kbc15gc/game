#pragma once
#include "fbEngine\_Object\_GameObject\GameObject.h"
#include "fbEngine\_Object\_Component\_3D\Light.h"

class RockCave : public GameObject
{
public:
	RockCave(const char* name);
	void Awake()override;
	void Start()override;
	void Update()override;
private:
	CharacterLight MyLight_;
};
