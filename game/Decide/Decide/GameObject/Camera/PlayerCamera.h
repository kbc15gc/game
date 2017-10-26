#pragma once
#include "GameCamera.h"
#include "fbEngine\_Object\_GameObject\GameObject.h"
#include "GameObject\Player\Player.h"
class HistoryBook;


class PlayerCamera : public GameCamera
{
public:
	//�R���X�g���N�^�B
	PlayerCamera(const char* name) :
		GameCamera(name),
		_Spring(2.0f),
		_Damping(1.0f)
	{
	}
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

	//�J������Y���ɉ�](��)�B
	void _RotateHorizon(float roty);

	//�J������X���ɉ�](�c)�B
	void _RotateVertical(float rotx);

	//���C���΂��āA�J�����̈ړ�����m�F�B
	Vector3 _ClosetRay();

	//�v���C���[�̕����������B
	void _LookAtPlayer();

	//�J�������ړ������鏈���B
	void _Move()override;

	//�o�l�̗l�ɒǐՁB
	void _SpringChaseMove(float time);

	// ���̃J�����ɐ؂�ւ�������ɌĂ΂��R�[���o�b�N�B
	virtual void ChangeCameraReAction() {
		//�v���C���[�̍X�V���~�߂�B
		_Player->SetIsStopUpdate(false);
	}
private:
	//�����蔻����Ƃ邽�߂̃��C�̌`��
	SphereCollider* _Sphere;
	//�v���C���[�֌������x�N�g��
	D3DXVECTOR3 _ToPlayerDir;
	//�J�����ւ̃x�N�g���B
	D3DXVECTOR3 _ToCamera;
	//����
	float _Dist;
	//�����蔻��̔��a
	float _Radius;

	//���j���I�u�W�F�N�g�B
	HistoryBook* _HistoryBook = nullptr;

	/** �ړ��\�t���O. */
	bool _IsMove = true;

	//�O�t���[���̃|�W�V�����B
	Vector3 _PrevPosition = Vector3::zero;

	//�J�������I�u�W�F�N�g�ƏՓ˂��Ă��邩�H
	bool _IsHitObject;

	//�J�������ŏI�I�ɓ��B����ڕW���W�B
	Vector3 _DestinationPos;
	//�J�����ړ��̉����x�B
	Vector3 _Velocity;
	//�_���s���O�萔�B
	float _Damping;
	//�o�l�萔�B
	float _Spring;
};