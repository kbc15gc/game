#pragma once
#include "fbEngine/GameObject.h"

class Player;
//ゲームカメラ
class GameCamera :public GameObject
{
public:
	GameCamera(const char* name) :GameObject(name) {}
	void Awake()override;
	void Update()override;
	void LateUpdate()override;
	//カメラ横回転
	void RotTransversal(float roty);
	//カメラが縦に回転。
	void RotLongitudinal(float rotx);
private:
	Player*		_Player;
	D3DXVECTOR3 _ToPos;
};