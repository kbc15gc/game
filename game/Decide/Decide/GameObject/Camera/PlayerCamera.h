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
		GameCamera(name)
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

	//�ړ��֐�
	void Move()override;
	void SetIsMove(bool value)
	{
		_IsMove = value;
	}
private:

	/**
	* �J��������].
	*
	* @param roty	��]�p�x.
	*/
	void _RotTransversal(float roty);

	/**
	* �J�������c�ɉ�].
	*
	* @param roty	��]�p�x.
	*/
	void _RotLongitudinal(float rotx);

	//�ʏ펞�̃J��������
	void _StandardBehavior();

	// ���̃J�����ɐ؂�ւ�������ɌĂ΂��R�[���o�b�N�B
	virtual void ChangeCameraReAction() {
		//�v���C���[�̍X�V���~�߂�B
		_Player->SetIsStopUpdate(false);
	}



private:

	//���C�̌`��
	SphereCollider* _Sphere;
	//�v���C���[�֌������x�N�g��
	D3DXVECTOR3 _ToPlayerDir;
	//����
	float _Dist;
	//�����蔻��̔��a
	float _Radius;

	//���j���I�u�W�F�N�g�B
	HistoryBook* _HistoryBook = nullptr;

	/** �ړ��\�t���O. */
	bool _IsMove = true;

};