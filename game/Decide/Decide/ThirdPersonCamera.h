#pragma once
#include "GameCamera.h"
//�ӂ���J�����N���X�̒�`�B
class ThirdPersonCamera :	public GameCamera
{
public:
	//�R���X�g���N�^�B
	ThirdPersonCamera(const char* name) :
		GameCamera(name)
	{
	}
	~ThirdPersonCamera();

	//�R���X�g���N�^��̏������B
	void Awake()override;

	//�������B
	void Start()override;

	//�X�V�B
	void UpdateSubClass()override;
private:
	//�ړ��֐��B
	void _Move();

	//���̃J���������C���J�����ɐ؂�ւ���B
	void SetMainCamera()
	{
	}
private:
	Camera* _ThirdPersonCamera = nullptr;
	bool _ThirdPersonCameraFlag = false;
};

