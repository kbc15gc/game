#pragma once
#include "GameCamera.h"
#include "fbEngine\_Object\_GameObject\GameObject.h"
#include "GameObject\Player\Player.h"
class HistoryBook;


class PlayerCamera : public GameCamera
{
public:
	//�R���X�g���N�^�B
	PlayerCamera(const char* name);
	~PlayerCamera();

	/**
	* �R���X�g���N�^��̏�����.
	*/
	void Awake()override;

	/**
	* ������.
	*/
	void Start()override;

	/**
	* �X�V.
	*/
	void UpdateSubClass()override;

	void SetIsMove(bool value)
	{
		_IsMove = value;
	}

	/**
	* �J�����̍��W�Ȃǂ̏�����.
	*/
	inline void Init()
	{
		_Camera->SetTarget(_GetPlayerPos());
		_DestinationPos = _GetPlayerPos() + (_Player->transform->GetForward() * _Dist);
		transform->SetPosition(_DestinationPos);
		_Camera->Update();

		_ToCameraDir = (_Player->transform->GetForward());
	}
	//�v���C���[�̕����������B
	void LookAtTarget()
	{
		auto next = _GetPlayerPos();
		_Camera->SetTarget(next);
		_DestinationPos = _ClosetRay();
		transform->SetPosition(_DestinationPos);
		transform->LockAt(next);
	}

private:
	Vector3 _GetPlayerPos();
	//�ʏ펞�̃J��������
	void _StandardBehavior();

	//�v���C���[�̕����������B
	void _LookAtTarget();

	//�J������Y���ɉ�](��)�B
	void _RotateHorizon(float roty);

	//�J������X���ɉ�](�c)�B
	void _RotateVertical(float rotx);

	//�J���������̐L�k�B
	void _UpdateDist();

	//�J�����⏕�B
	void _CameraSupport();

	//���C���΂��āA�J�����̈ړ�����m�F�B
	Vector3 _ClosetRay();

	//�J�������ړ������鏈���B
	void _Move()override;

	/**
	* �ǔ��J����.
	*/
	void _Tracking();
	
	// ���̃J�����ɐ؂�ւ�������ɌĂ΂��R�[���o�b�N�B
	virtual void ChangeCameraReAction() {
		//�v���C���[�̍X�V���~�߂�B
		_Player->SetIsStopUpdate(false);
	}

	//�J�������Z�b�g�B
	void CameraReset();
private:
	//���j���I�u�W�F�N�g�B
	HistoryBook* _HistoryBook = nullptr;
	/** �ړ��\�t���O. */
	bool _IsMove = true;

	//�����蔻����Ƃ邽�߂̃��C�̌`��
	SphereCollider* _Sphere = nullptr;
	//����
	float _Dist = 0.0f;
	//�����蔻��̔��a
	float _Radius = 0.0f;

	//�J�������ŏI�I�ɓ��B����ڕW���W�B
	Vector3 _DestinationPos = Vector3::zero;

	//�^�[�Q�b�g����J�����ւ̌����x�N�g���B
	Vector3 _ToCameraDir = Vector3::zero;
	//�v���C���[�����Ă�������B
	const Vector3* _PForward = nullptr;

	//�J�������Z�b�g�t���O�B
	bool _Reset = false;
	float _Timer = 0.0f;
	Vector3 tmp = Vector3::zero;

	Vector3 _PurposePos = Vector3::zero;
	Vector3 _PurposeTarget = Vector3::zero;

};