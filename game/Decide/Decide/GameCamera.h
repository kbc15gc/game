/**
* �Q�[���J�����N���X�̒�`.
*/
#pragma once

#include "fbEngine\_Object\_GameObject\GameObject.h"
/**
* �J�����̒��ۃN���X�B
*/
class GameCamera :public GameObject
{
public:
	//�R���X�g���N�^�B
	GameCamera(const char* name) :
		GameObject(name)
	{
	}

	//�J�������Z�b�g�B
	void SetCamera(Camera* camera)
	{
		_Camera = camera;
	}

	//�J�������Q�b�g�B
	Camera* GetCamera()
	{
		return _Camera;
	}
protected:
	//�J�����R���|�[�l���g�B
	Camera* _Camera = nullptr;
};