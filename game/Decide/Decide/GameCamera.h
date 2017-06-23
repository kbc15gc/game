#pragma once

#include "fbEngine\_Object\_GameObject\GameObject.h"
#include "GameObject\Player\Player.h"

class Player;

//�J�����̊��N���X�B
class GameCamera :public GameObject
{
public:
	//�R���X�g���N�^�B
	GameCamera(const char* name) :
		GameObject(name)
	{
	}

	~GameCamera();

	//�R���X�g���N�^��̏������B
	void Awake()override;

	//�������B
	void Start()override;

	//�X�V�B
	void Update()override;

	// �p����̍X�V�����B
	virtual void UpdateSubClass() = 0;

	//���̃J�������w��B
	void SetNextCamera(GameCamera* next);

	void ChangeRequest();

	//���̃J���������C���J�����Ƃ��Ďg�p����B
	void ActiveCamera()
	{
		Camera* cam = INSTANCE(GameObjectManager)->mainCamera;	//GameObjectManager�ɓo�^���Ă���ꍇ�A���O���čX�V���~����B
		if (cam) {
			static_cast<GameCamera*>(cam->gameObject)->UnActivateFlg();
		}
		INSTANCE(GameObjectManager)->mainCamera = this->_Camera;
		_Player = static_cast<Player*>(INSTANCE(GameObjectManager)->FindObject("Player"));
		_Player->SetIsStopUpdate(false);
		_isActivate = true;
	}
	
	void LateUpdate()override {
		if (_isActivate) {
			ActivateFlg();
			_isActivate = false;
		}
	}
	// �p����̍X�V�����B
	virtual void Move() = 0;

protected:
	void ActivateFlg() {
		_isActive = true;
	}
	void UnActivateFlg() {
		_isActive = false;
		_isActivate = false;
	}

	//�J�����̈ړ����x��ݒ�B
	void SetCameraSpeed(float speed)
	{
		_MoveSpeed = speed;
	}

protected:
	//�J�����R���|�[�l���g�B
	Camera* _Camera = nullptr;

	//���̃J�����B
	GameCamera* _NextCamera = nullptr;

	//���̃J�������X�V�������s�����B
	bool _isActive = true;	

	//���̃J�������A�N�e�B�u�����邩�B
	bool _isActivate = false;

	//�v���C���[�I�u�W�F�N�g
	Player* _Player = nullptr;

	//�v���C���[�̃|�W�V�����ւ̎Q��
	const Vector3* _PlayerPos = &Vector3::zero;

	//�J�����̈ړ����x�B
	float _MoveSpeed = 3.0f;

};