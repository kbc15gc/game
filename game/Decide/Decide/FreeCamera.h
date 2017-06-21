#pragma once
#include "fbEngine\_Object\_GameObject\GameObject.h"

//�t���[�J�����N���X�̒�`�B
class FreeCamera : public GameObject
{
public:
	//�R���X�g���N�^�B
	FreeCamera(const char* name) :
		GameObject(name)
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
		//�ӂ���J�����ɂ���O�̃J�������Q�b�g�B
		_GameCamera = INSTANCE(GameObjectManager)->mainCamera;

		//�ӂ���J�����ɐ؂�ւ��B
		INSTANCE(GameObjectManager)->mainCamera = _FreeCamera;

		_FreeCameraFlag = true;
	}
private:
	//�J�����R���|�[�l���g
	Camera* _FreeCamera;

	//�Q�[���J�����B
	Camera* _GameCamera = nullptr;

	bool _FreeCameraFlag = false;

	Vector3 _toPosition = Vector3::zero;
};

