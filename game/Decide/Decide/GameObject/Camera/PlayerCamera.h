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
private:
	Vector3 _GetPlayerPos();
	//�ʏ펞�̃J��������
	void _StandardBehavior();

	//�v���C���[�̕����������B
	void _LookAtPlayer();

	//�J������Y���ɉ�](��)�B
	void _RotateHorizon(float roty);

	//�J������X���ɉ�](�c)�B
	void _RotateVertical(float rotx);

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
};