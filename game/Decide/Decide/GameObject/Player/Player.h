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
#include "GameObject\Component\ParameterBar.h"
#include "GameObject\Component\OutputData.h"


class SkinModel;
class Animation;
class ParameterBar;
class HistoryBook;

namespace
{
	const int MAXLV = 100;

	struct ExperiencePointTableInfo
	{
		int ExperiencePoint;	//���x���A�b�v���ɕK�v�Ȍo���l
		int HP;					//���x�����Ƃ�HP
		int MP;					//���x�����Ƃ�MP
		int ATK;				//���x�����Ƃ�ATK
		int DEF;				//���x�����Ƃ�DEF
		int DEX;				//���x�����Ƃ�DEX
		int AGI;				//���x�����Ƃ�AGI
	};

	const Support::DATARECORD ExperiencePointTableInfoData[] =
	{
		{ "ExperiencePoint",Support::DataTypeE::INT, offsetof(struct ExperiencePointTableInfo,ExperiencePoint),sizeof(int) },
		{ "HP", Support::DataTypeE::INT,offsetof(struct ExperiencePointTableInfo,HP),sizeof(int)},
		{ "MP", Support::DataTypeE::INT,offsetof(struct ExperiencePointTableInfo,MP),sizeof(int)},
		{ "ATK", Support::DataTypeE::INT,offsetof(struct ExperiencePointTableInfo,ATK),sizeof(int)},
		{ "DEF", Support::DataTypeE::INT,offsetof(struct ExperiencePointTableInfo,DEF),sizeof(int)},
		{ "DEX", Support::DataTypeE::INT,offsetof(struct ExperiencePointTableInfo,DEX),sizeof(int)},
		{ "AGI", Support::DataTypeE::INT,offsetof(struct ExperiencePointTableInfo,AGI),sizeof(int)},
	};
}

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
		AnimationAttack03,							//�U���R
		AnimationAttack04,							//�U��04
		AnimationAttack05,							//�U��03
		AnimationAttackEnd = AnimationAttack05,
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

	// �������������������̈ȊO�̍U���R���W�����ƏՓ˂����u�ԌĂ΂��R�[���o�b�N�B
	// �������͏Փ˂����U���R���W�����B
	// �����������Ȃ�������inline�̂܂܂ł�����(��������3�s�ȏ�̏���������悤�ɂȂ�܂�)�B
	inline virtual void HitAttackCollisionEnter(AttackCollision* hitCollision);

	// �������������������̈ȊO�̍U���R���W�����ɏՓ˂Ă���ԌĂ΂��R�[���o�b�N�B
	// �������͏Փ˂����U���R���W�����B
	// �����������Ȃ�������inline�̂܂܂ł�����(��������3�s�ȏ�̏���������悤�ɂȂ�܂�)�B
	inline virtual void HitAttackCollisionStay(AttackCollision* hitCollision) {
		OutputDebugString("Stay");
	}

	// �������������������̈ȊO�̍U���R���W�����Ƃ̏Փ˂���O�ꂽ��Ă΂��R�[���o�b�N�B
	// �������͏Փ˂����U���R���W�����B
	// �����������Ȃ�������inline�̂܂܂ł�����(��������3�s�ȏ�̏���������悤�ɂȂ�܂�)�B
	inline virtual void HitAttackCollisionExit(AttackCollision* hitCollision) {
		OutputDebugString("Exit");
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
	//���f����\�����邩�t���O���Z�b�g�B
	void SetEnable(bool is)
	{
		_Model->enable = is;
	}
	//�v���C���[���
	void Releace();
	//�G�����Ƃ�����(�o���l�A����)���󂯎��B
	void TakeDrop(int dropexp,int money)
	{
		_PlayerParam->AddParam(CharacterParameter::EXP, dropexp);
		_PlayerParam->AddParam(CharacterParameter::MONEY, money);
	}

	int* GetParamPt(CharacterParameter::Param param)
	{
		return _PlayerParam->GetParamPt(param);
	}

private:
	//�v���C���[���_���[�W���󂯂鏈��
	void _Damage();
	//�o���l�e�[�u����CSV���烍�[�h�B
	void _LoadEXPTable();
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
	//�ŏ��̃|�W�V����
	Vector3 _StartPos;
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
	//���x���A�b�v���̉�
	SoundSource* _LevelUP = nullptr;
	//�v���C���[�̃p�����[�^�[
	CharacterParameter* _PlayerParam = nullptr;
	// ��]�B
	ObjectRotation* _Rotation = nullptr;
	// HP�o�[�B
	ParameterBar* _HPBar = nullptr;
	// MP�o�[�B
	ParameterBar* _MPBar = nullptr;
	//�q�X�g���[�u�b�N
	HistoryBook* _HistoryBook = nullptr;
#ifdef _DEBUG
	// �f�o�b�O�p�f�[�^�o�̓R���|�[�l���g�B
	OutputData* _outputData = nullptr;
#endif
	//�f�o�b�O
	bool _Debug = false;

	//�o���l�e�[�u��
	std::vector<int> _EXPTable;
	//���x�����Ƃ�HP
	std::vector<int> _HPTable;
	//���x�����Ƃ�MP
	std::vector<int> _MPTable;
	//���x�����Ƃ�ATK
	std::vector<int> _ATKTable;
	//���x�����Ƃ�DEF
	std::vector<int> _DEFTable;
	//���x�����Ƃ�DEX
	std::vector<int> _DEXTable;
	//���x�����Ƃ�AGI
	std::vector<int> _AGITable;
};