#pragma once
#include "fbEngine/GameObject.h"
#include "fbEngine/CharacterController.h"
#include "PlayerState/PlayerStateRun.h"
#include "PlayerState/PlayerStateIdol.h"
#include "PlayerState/PlayerStateAttack.h"

class SkinModel;
class Animation;


class Player : public GameObject
{
public:
	//�v���C���[�̃X�e�[�g
	enum class State
	{
		Idol = 0,			//�A�C�h��
		Run,				//����
		Attack,				//�U��
	};
	//�A�j���[�V�����̃i���o�[
	enum class AnimationNo
	{
		AnimationInvalid = -1,		//����
		AnimationDeath,				//���S
		AnimationAttack02,			//�U��02
		AnimationAttack01,			//�U��01
		AnimationJump,				//�W�����v
		AnimationRun,				//����
		AnimationWalk,				//����
		AnimationIdol,				//�A�C�h��	
		AnimationNum,				//�A�j���[�V�����̐�
	};
	Player(const char* name);
	void Awake()override;
	void Start()override;
	void Update()override;
	//��ԕύX
	// nextstate ���̃X�e�[�g
	void ChangeState(State nextstate);
	//�A�j���[�V�����Đ�
	// animno �A�j���[�V�����̃i���o�[
	// interpolatetime ��Ԏ���
	// loopnum ���[�v�� (�f�t�H���g��-1)
	void PlayAnimation(AnimationNo animno, float interpolatetime , int loopnum = -1);
	//�A�j���[�V�����R���g���[��
	void AnimationControl();
	//�Z�b�g�X�e�[�g
	void SetState(State state)
	{
		_State = state;
	}
	//�Q�b�g�X�e�[�g
	State GetState()
	{
		return _State;
	}
	//�L�����N�^�[�R���g���[���[�Q�b�g
	CCharacterController& GetCharaCon() const
	{
		return* _CharacterController;
	}
	//�A�j���[�V�����Đ����t���O�Q�b�g
	const bool GetAnimIsPlay() const;
private:
	friend class PlayerStateAttack;

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
	//�A�j���[�V�����̏I������
	double _AnimationEndTime[(int)AnimationNo::AnimationNum];
	//�L�����N�^�[�R���g���[���[
	CCharacterController* _CharacterController;
	//���݂̃v���C���[�̃X�e�[�g
	PlayerState*	_CurrentState;
	//�v���C���[�X�e�[�g����
	PlayerStateRun	_RunState;
	//�v���C���[�X�e�[�g�A�C�h��
	PlayerStateIdol	_IdolState;
	//�v���C���[�X�e�[�g�A�^�b�N
	PlayerStateAttack _AttackState;
};