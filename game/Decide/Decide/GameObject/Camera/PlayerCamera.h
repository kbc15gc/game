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

	//�J�����⏕�B
	void _CameraSupport();

	//���C���΂��āA�J�����̈ړ�����m�F�B
	Vector3 _ClosetRay();

	//�J�������ړ������鏈���B
	void _Move()override;

	//�o�l�̗l�ɒǐՁB
	Vector3 _SpringChaseMove(const Vector3& now, const Vector3& target, const float& spring, const float& damping, const float& time);

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
	SphereCollider* _Sphere;
	//����
	float _Dist;
	//�����蔻��̔��a
	float _Radius;

	//�J�������ŏI�I�ɓ��B����ڕW���W�B
	Vector3 _DestinationPos;
	//�J�����ړ��̉����x�B
	Vector3 _Velocity;
	//�_���s���O�萔�B
	float _Damping;
	//�o�l�萔�B
	float _Spring;

	//�^�[�Q�b�g����J�����ւ̌����x�N�g���B
	Vector3 _ToCameraDir;
	//�v���C���[�����Ă�������B
	const Vector3* _PForward;

	//�J�������Z�b�g�t���O�B
	bool _Reset = false;
	float _Timer = 0.0f;
	Vector3 tmp;
};