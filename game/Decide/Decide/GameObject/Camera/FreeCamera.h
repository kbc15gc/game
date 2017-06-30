#pragma once
#include "GameCamera.h"
#include "GameObject\Player\Player.h"

#if _DEBUG
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

	//�t���[�J�����̉�]�B
	void FreeCameraRotation();

	//�t���[�J�����̏㉺�ړ��B
	void FreeCameraMoveUpDown();

	//�v���C���[�J�����̈ʒu�ɖ߂��B
	void Return();
private:

	float _R_STICK_Y = 0.0f;					//���͂��ꂽ�E�X�e�B�b�N�̏㉺������ێ��B
	float _R_STICK_X = 0.0f;					//���͂��ꂽ�E�X�e�B�b�N�̍��E������ێ��B

	const float _CameraRotationSpeed = 10.0f;	//�J�����̉�]�X�s�[�h�B

	const float _DashSpeed = 70.0f;				//�_�b�V�����̃J�����̃X�s�[�h�B

	const float _NormalSpeed = 10.0f;			//�ʏ펞�̃J�����̃X�s�[�h�B
};

#endif // _DEBUG