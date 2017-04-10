#pragma once
#include "fbEngine/GameObject.h"
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
private:
	D3DXVECTOR3 _ToPos;
};