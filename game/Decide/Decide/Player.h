#pragma once
#include "fbEngine/GameObject.h"
#include "fbEngine/CharacterController.h"
#include "PlayerStateRun.h"

class SkinModel;
class Animation;


class Player : public GameObject
{
public:
	enum class State
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
	//��ԕύX
	//���̃X�e�[�g
	void ChangeState(State nextstate);
	//�L�����N�^�[�R���g���[���[�Q�b�g
	CCharacterController& GetCharaCon() const
	{
		return* _CharacterController;
	}
private:
	//�d�͏���
	void _GravityCheck(const float& movey);
	//���̂����蔻��
	void _WallCheck(const Vector3& move);
private:
	//�R���|�[�l���g�Ƃ��A�h���X�̕ێ����K�v�Ȃ��̂���
	SkinModel* _Model;
	Animation* _Anim;
	//����
	float _Height;
	//���a
	float _Radius;
	//�ŏI�I�Ȉړ���
	Vector3 _MoveSpeed;
	//�i�s
	Vector3 _Dir;
	//�d��
	float _Gravity;
	//�v���C���[�̏��
	State _State;
	//�v���C���[�̃��x��
	int _Level;
	//�W�����v�t���O true=�W�����v�� , false=�W�����v���ĂȂ�
	bool _Jump;
	//�L�����N�^�[�R���g���[���[
	CCharacterController* _CharacterController;
	//���݂̃v���C���[�̃X�e�[�g
	PlayerState*	_CurrentState;
	//�v���C���[�X�e�[�g����
	PlayerStateRun	_RunState;
};