#pragma once
#include "fbEngine/GameObject.h"

class Player;
//�Q�[���J����
class GameCamera :public GameObject
{
public:
	GameCamera(const char* name) :GameObject(name) {}
	void Awake()override;
	void Update()override;
	void LateUpdate()override;
	//�J��������]
	void RotTransversal(float roty);
	//�J�������c�ɉ�]�B
	void RotLongitudinal(float rotx);
private:
	Player*		_Player;
	D3DXVECTOR3 _ToPos;
};