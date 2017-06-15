#pragma once
#include "fbEngine\_Object\_GameObject\GameObject.h"
#include "fbEngine/CharacterController.h"
#include "PlayerState/PlayerStateRun.h"
#include "PlayerState/PlayerStateIdol.h"
#include "PlayerState/PlayerStateAttack.h"
#include "PlayerState\/PlayerStateDeath.h"
#include "AttackCollision.h"
#include "fbEngine\_Object\_GameObject\SoundSource.h"
#include "fbEngine\_Object\_GameObject\TextObject.h"
#include "GameObject\Component\CharacterParameter.h"
#include"GameObject\Component\ObjectRotation.h"

class SkinModel;
class Animation;
class ParameterBar;

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
		AnimationInvalid = -1,						//����
		AnimationIdol,								//�A�C�h��	
		AnimationWalk,								//����
		AnimationRun,								//����
		AnimationJump,								//�W�����v
		AnimationAttackStart,			
		AnimationAttack01 = AnimationAttackStart,	//�U��01
		AnimationAttack02,							//�U��02
		AnimationAttack03,							//�U��03
		AnimationAttackEnd = AnimationAttack03,
		AnimationDeath,								//���S
		AnimationNum,								//�A�j���[�V�����̐�
	};
	Player(const char* name);
	~Player();
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
	// �G�l�~�[��������U���B
	void HitAttackCollision(AttackCollision* hitCollision) {
		OutputDebugString("�Ƃ肠�����u���C�N�|�C���g�ݒ�ł���悤�ɂ���ˁB");
		if (hitCollision->GetMaster() == AttackCollision::CollisionMaster::Enemy)
		{
			_PlayerParam->SubParam(CharacterParameter::Param::HP,1);	//�_���[�W���󂯂�(�Ƃ肠�����A���C�t���P�����炷)
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
		return*_CharacterController;
	}
private:
	friend class PlayerStateAttack;
	friend class PlayerStateDeath;
	friend class PlayerStateIdol;
	friend class PlayerStateRun;

	//�R���|�[�l���g�Ƃ��A�h���X�̕ێ����K�v�Ȃ��̂���
	//���f��
	SkinModel* _Model = nullptr;
	//�A�j���[�V����
	Animation* _Anim = nullptr;
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
	//�A�j���[�V�����̏I������
	double _AnimationEndTime[(int)AnimationNo::AnimationNum];
	//�L�����N�^�[�R���g���[���[
	CCharacterController* _CharacterController = nullptr;
	//���݂̃v���C���[�̃X�e�[�g
	PlayerState*	_CurrentState = nullptr;
	//���݂̍U���A�j���[�V�����X�e�[�g
	AnimationNo	_NowAttackAnimNo;
	//���̍U���A�j���[�V�����X�e�[�g
	AnimationNo _NextAttackAnimNo;
	//�v���C���[�X�e�[�g����
	PlayerStateRun	_RunState;
	//�v���C���[�X�e�[�g�A�C�h��
	PlayerStateIdol	_IdolState;
	//�v���C���[�X�e�[�g�A�^�b�N
	PlayerStateAttack _AttackState;
	//�v���C���[�X�e�[�g�f�X
	PlayerStateDeath _DeathState;
	//�v���C���[���_���[�W�󂯂�����SE
	SoundSource* _DamageSE = nullptr;
	//�v���C���[�̃p�����[�^�[
	CharacterParameter* _PlayerParam = nullptr;
	//�v���C���[�̃��x��
	int _Level;
	//HP�̃e�L�X�g�\��
	TextObject* _HPText;
	//MP�̃e�L�X�g�\��
	TextObject* _MPText;
	// ��]�B
	ObjectRotation* _Rotation = nullptr;
	// HP�o�[�B
	ParameterBar* _HPBar = nullptr;
	// MP�o�[�B
	ParameterBar* _MPBar = nullptr;
};