#pragma once
#include "fbEngine/GameObject.h"

class Player;
//ƒQ[ƒ€ƒJƒƒ‰
class GameCamera :public GameObject
{
public:
	GameCamera(const char* name) :GameObject(name) {}
	void Awake()override;
	void Update()override;
	void LateUpdate()override;
	//ƒJƒƒ‰‰¡‰ñ“]
	void RotTransversal(float roty);
	//ƒJƒƒ‰‚ªc‚É‰ñ“]B
	void RotLongitudinal(float rotx);
private:
	Player*		_Player;
	D3DXVECTOR3 _ToPos;
};