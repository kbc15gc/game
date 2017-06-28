#pragma once
#include "GameCamera.h"
#include "GameObject\Player\Player.h"

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
	void UpdateSubClass()override;

	void Move()override;

private:
	// ���̃J�����ɐ؂�ւ�������ɌĂ΂��R�[���o�b�N�B
	virtual void ChangeCameraReAction() {
		//�v���C���[�̍X�V���~�߂�B
		_Player->SetIsStopUpdate(true);
	}
private:
	bool _FreeCameraFlag = false;

	Vector3 _toPosition = Vector3::zero;
};

