#pragma once
#include "fbEngine\_Object\_GameObject\GameObject.h"

class Dungeon : public GameObject
{
public:
	Dungeon(const char* name);
	void Awake()override;
	void Start()override;
	void Update()override;
}; 