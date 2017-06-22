#pragma once
#include "fbEngine\_Object\_GameObject\GameObject.h"

//�ӂ���J�����N���X�̒�`�B
class ThirdPersonCamera :	public GameObject
{
public:
	//�R���X�g���N�^�B
	ThirdPersonCamera(const char* name) :
		GameObject(name)
	{
	}
	~ThirdPersonCamera();

	//�R���X�g���N�^��̏������B
	void Awake()override;

	//�������B
	void Start()override;

	//�X�V�B
	void Update()override;
private:
	//�ړ��֐��B
	void _Move();

	//���̃J���������C���J�����ɐ؂�ւ���B
	void SetMainCamera()
	{
		//�ӂ���J�����ɂ���O�̃J�������Q�b�g�B
		_GameCamera = INSTANCE(GameObjectManager)->mainCamera;

		//�ӂ���J�����ɐ؂�ւ��B
		INSTANCE(GameObjectManager)->mainCamera = _ThirdPersonCamera;

		_ThirdPersonCameraFlag = true;
	}
private:
	//�ӂ���J�����R���|�[�l���g�B
	Camera* _ThirdPersonCamera = nullptr;

	//�Q�[���J�����B
	Camera* _GameCamera = nullptr;

	bool _ThirdPersonCameraFlag = false;
};

