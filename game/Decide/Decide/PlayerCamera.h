#pragma once
#include "GameCamera.h"
#include "fbEngine\_Object\_GameObject\GameObject.h"

class Player;
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

	//�ړ��֐�
	void _Move();
	//�ʏ펞�̃J��������
	void _StandardBehavior();

private:

	//���C�̌`��
	SphereCollider* _Sphere;
	//�v���C���[�I�u�W�F�N�g
	Player*		_Player;
	//�v���C���[�̃|�W�V�����ւ̎Q��
	const Vector3* _PlayerPos;
	//�v���C���[�֌������x�N�g��
	D3DXVECTOR3 _ToPlayerDir;
	//����
	float _Dist;
	//�����蔻��̔��a
	float _Radius;

	//���j���I�u�W�F�N�g�B
	HistoryBook* _HistoryBook = nullptr;

};