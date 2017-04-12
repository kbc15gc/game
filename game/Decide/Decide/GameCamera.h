#pragma once
#include "fbEngine/GameObject.h"
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
private:
	D3DXVECTOR3 _ToPos;
};