#pragma once
#include "fbEngine/GameObject.h"

class Player;
//ƒQ[ƒ€‚Åg‚¤‰e‚ğì‚é‚½‚ß‚ÌƒJƒƒ‰
class GameShadowCamera :public GameObject
{
public:
	GameShadowCamera(const char* name) :GameObject(name) {}
	void Awake()override;
	void Update()override;
private:
	Player* _Player;
};