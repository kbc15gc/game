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
		_FuturePos = _GetPlayerPos() + (_Player->transform->GetForward() * _Dist);
		transform->SetPosition(_FuturePos);
		_Camera->Update();

		_ToCameraDir = (_Player->transform->GetForward());
		_Height = _ToCameraDir.y * _Dist;
	}
	//�v���C���[�̕����������B
	void LookAtTarget();

private:
	//�v���C���[�̍��W���擾�B
	Vector3 _GetPlayerPos();

	//�J���������̐L�k�B
	void _UpdateDist();

	//�ʏ펞�̃J��������
	void _StandardBehavior();

	//�J�����⏕�B
	void _AutoSupport();

	//�J������Y���ɉ�](��)�B
	void _RotateHorizon(float roty);

	//�J������X���ɉ�](�c)�B
	void _RotateVertical(float rotx);

	//���C���΂��āA�J�����̈ړ�����m�F�B
	Vector3 _ClosetRay(Vector3 from, Vector3 to);

	//�o�l�̗l�ɒǐՁB
	Vector3 _SpringChaseMove(const Vector3& now, const Vector3& target, float spring, float damping,
		Vector3 &velocity, float time, float speed);

	//�v���C���[�̕����������B
	void _LookAtTarget();

	//�J�������ړ������鏈���B
	void _Move()override {};

	// ���̃J�����ɐ؂�ւ�������ɌĂ΂��R�[���o�b�N�B
	virtual void ChangeCameraReAction() {
		//�v���C���[�̍X�V���~�߂�B
		_Player->SetIsStopUpdate(false);
	}
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

	//�ŏI�I�ɓ��B������W�B
	Vector3 _FuturePos = Vector3::zero, _FutureTarget = Vector3::zero;

	//�^�[�Q�b�g����J�����ւ̌����x�N�g���B
	Vector3 _ToCameraDir = Vector3::zero;
	//�J�����̍����B
	float _Height = 0.0f;
	//�J�����ړ��̉����x�B
	Vector3 _MoveV, _LookV;

	//���Z�b�g�t���O�B
	bool _Reset;
	Vector3 _ResetDir;


	//�o�l�̐L�ы�B
	float _SpringM = 20.0f, _SpringL = 30.0f;
	//�o�l�̏k�܂鋭���B
	float _DampingM = 3.0f, _DampingL = 4.0f;
	//�o�l�ړ��̑��x�B
	float _SMoveSpeedM = 5.0f, _SMoveSpeedL = 5.0f;
	//�J�����̉�]���x
	float CAMERA_ROTSPEED = 3.5f;
};