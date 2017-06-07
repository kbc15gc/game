#pragma once
#include "fbEngine\_Object\_GameObject\GameObject.h"
#include "fbEngine/CharacterController.h"
#include "PlayerState/PlayerStateRun.h"
#include "PlayerState/PlayerStateIdol.h"
#include "PlayerState/PlayerStateAttack.h"
#include "PlayerState\/PlayerStateDeath.h"
#include "AttackCollision.h"
#include "fbEngine\_Object\_GameObject\SoundSource.h"

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
		Death,				//���S
		StateNum,
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

	// �������������������̈ȊO�̍U���R���W�����ɏՓ˂�����Ă΂��R�[���o�b�N�B
	// �������͏Փ˂����U���R���W�����B
	// �����������Ȃ�������inline�̂܂܂ł�����(��������3�s�ȏ�̏���������悤�ɂȂ�܂�)�B
	void HitAttackCollision(AttackCollision* hitCollision) {
		OutputDebugString("�Ƃ肠�����u���C�N�|�C���g�ݒ�ł���悤�ɂ���ˁB");
		if (hitCollision->GetMaster() == AttackCollision::CollisionMaster::Enemy)
		{
			_Life--;	//�Ƃ肠�����A���C�t���P�����炷�B
			_DamageSE->Play(false);//�_���[�W���󂯂��Ƃ���SE
		}
	}

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
	friend class PlayerStateDeath;

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
	//�v���C���[�̃��C�t
	int _Life;
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
	//�v���C���[�X�e�[�g�f�X
	PlayerStateDeath _DeathState;
	//�v���C���[���_���[�W�󂯂�����SE
	SoundSource* _DamageSE;
};