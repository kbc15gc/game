#pragma once
#include "GameCamera.h"

//�ӂ���J�����N���X�̒�`�B
class ThirdPersonCamera :	public GameCamera
{
public:

	//�ӂ���J�����̍����B
	enum class Camera_Height
	{
		Invalid = -1,
		Low = 0,
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

		//�v�Z���ꂽ������Ԃ��B
		return height;
	}

private:
	
	Camera_Height _NowHeight = Camera_Height::Low;//���̍����B

};

