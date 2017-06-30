#pragma once
#include "GameCamera.h"
#include "GameObject\Player\Player.h"

//�ӂ���J�����N���X�̒�`�B
class ThirdPersonCamera :	public GameCamera
{
public:

	//�ӂ���J�����̍����B
	enum class Camera_Height
	{
		Invalid = -1,
		Low,
		Middle,
		Height
	};
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

	void Move()override;
private:
	
	//enmu�̉��Z�B
	Camera_Height& Add (Camera_Height& height)
	{
		//���̍�������i�K��ɏグ��B
		height = static_cast<Camera_Height>(static_cast<int>(height) + 1);

		//�v�Z���ꂽ�l����ԏ�̍����ȏ�Ȃ��ԏ�̍����ɏC���B
		if (height >= Camera_Height::Height)
		{
			height = Camera_Height::Height;
			//�����������獂�ɂȂ����̂ŃJ�����̈ʒu��ۑ��B
			_PrevCameraPos = transform->GetPosition();
			return height;
		}

		//�v�Z���ꂽ������Ԃ��B
		return height;
	}

	//enmu�̌��Z�B
	Camera_Height& Subtract(Camera_Height& height)
	{
		//���̍�������i�K���ɉ�����B
		height = static_cast<Camera_Height>(static_cast<int>(height) - 1);

		//�v�Z���ꂽ�l����ԉ��̍����ȉ��Ȃ��ԉ��̍����ɏC���B
		if (height == Camera_Height::Invalid)
		{
			height = Camera_Height::Low;
			return height;
		}

		//���������ɂȂ����̂ŕۑ����Ă����ʒu��K�p�B
		if (height==Camera_Height::Middle)
		{
			//�؂�ւ��O�̒��̃J�����ʒu��ݒ�B
			transform->SetPosition(_PrevCameraPos);
		}

		//�v�Z���ꂽ������Ԃ��B
		return height;
	}

	//�J�����̍�����ύX�B
	void ChangeHeight();

	// ���̃J�����ɐ؂�ւ�������ɌĂ΂��R�[���o�b�N�B
	virtual void ChangeCameraReAction() {
		transform->SetPosition(_PlayerPos->x, 0.0f, _PlayerPos->z);
		//�v���C���[�̍X�V���~�߂�B
		_Player->SetIsStopUpdate(true);
	}

	//�v���C���[�J�����̈ʒu�ɖ߂��B
	void Return();
private:
	
	Camera_Height _NowHeight = Camera_Height::Low;		//���̍����B
	const float _LowCameraSpeed = 9.0f;					//�Ⴂ�����ł̈ړ��X�s�[�h�B
	const float _MiddleCameraSpeed = 25.0f;				//���̍����ł̈ړ��X�s�[�h�B
	const float _MiddleCameraDashSpeed = 100.0f;		//���̍����ł̃_�b�V���X�s�[�h�B
	Vector3 _PrevCameraPos = Vector3::zero;				//�؂�ւ��O�̃J�����̈ʒu��ێ��B
	Vector3 _HeightPos = Vector3(0.0f, 5000.0f, 0.0f);	//���̍����B
};

