#pragma once
#include "fbEngine/GameObject.h"
#include "fbEngine/CharacterController.h"

class SkinModel;
class Animation;


class Player : public GameObject
{
public:
	enum class PlayerState
	{
		Wait,
		Run,
		Jump,
		Attack,
	};
	Player(const char* name);
	void Awake()override;
	void Start()override;
	void Update()override;
	//�ړ�
	void Move();
	//�W�����v
	void Jump();
	//�U��
	void Attack();
private:
	//�d�͏���
	void _GravityCheck(const float& movey);
	//���̂����蔻��
	void _WallCheck(const Vector3& move);
private:
	//�R���|�[�l���g�Ƃ��A�h���X�̕ێ����K�v�Ȃ��̂���
	SkinModel* _Model;
	Animation* _Anim;
	RigidBody* _Rigid;
	btRigidBody* _RB;
	//���W�b�g�{�f�B�̍���
	float _Height;
	//���W�b�g�{�f�B�̕�
	float _Radius;
	//�v���C���[�̐F
	Color _PlayerColor;
	//�ŏI�I�Ȉړ���
	Vector3 _MoveSpeed;
	//�i�s
	Vector3 _Dir;
	//�d��
	float _Gravity;
	//�v���C���[�̏��
	PlayerState _State;
	//�v���C���[�̃��x��
	int _Level;
	//�W�����v�t���O true=�W�����v�� , false=�W�����v���ĂȂ�
	bool _Jump;
	//�L�����N�^�[�R���g���[���[
	CCharacterController* _CharacterController;
};