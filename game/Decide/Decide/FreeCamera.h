#pragma once
#include "GameCamera.h"

//�t���[�J�����N���X�̒�`�B
class FreeCamera : public GameCamera
{
public:
	//�R���X�g���N�^�B
	FreeCamera(const char* name) :
		GameCamera(name)
	{
	}

	//�f�X�g���N�^�B
	~FreeCamera();

	//�R���X�g���N�^��̏������B
	void Awake()override;

	//�������B
	void Start()override;

	//�X�V�B
	void Update()override;

private:
	//�ړ��֐�
	void _Move();

	//���̃J���������C���J�����ɐ؂�ւ���B
	void SetMainCamera()
	{
		
	}
private:
	//�J�����R���|�[�l���g
	Camera* _FreeCamera;

	bool _FreeCameraFlag = false;

	Vector3 _toPosition = Vector3::zero;
};

