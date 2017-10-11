#pragma once
#include "fbEngine\_Object\_GameObject\GameObject.h"
#include "fbEngine/CharacterController.h"
#include "PlayerState/PlayerStateRun.h"
#include "PlayerState/PlayerStateIdol.h"
#include "PlayerState/PlayerStateAttack.h"
#include "PlayerState\/PlayerStateDeath.h"
#include "PlayerState\PlayerStateStop.h"
#include "PlayerState\PlayerStateImpact.h"
#include "AttackCollision.h"
#include "fbEngine\_Object\_GameObject\SoundSource.h"
#include "fbEngine\_Object\_GameObject\TextObject.h"
#include "GameObject\Component\CharacterParameter.h"
#include"GameObject\Component\ObjectRotation.h"
#include "GameObject\Component\ParameterBar.h"
#include "GameObject\Component\OutputData.h"
#include "GameObject\Inventory\Inventory.h"
#include "GameObject\ItemManager\HoldItem\HoldEquipment.h"
#include "GameObject\ItemManager\HoldItem\HoldArmor.h"
#include "GameObject\ItemManager\HoldItem\HoldWeapon.h"
#include "GameObject\TextImage\LevelUpImage.h"
#include "GameObject\Component\BuffDebuffICon.h"
#include "GameObject\Village\NPC.h"
#include "GameObject\Component\AnimationEvent.h"

class SkinModel;
class Animation;
class ParameterBar;
class ItemManager;
class ParticleEffect;
class BuffDebuffICon;
class HistoryManager;

namespace
{
	const int MAXLV = 100;

	struct ExperiencePointTableInfo
	{
		int ExperiencePoint;	//���x���A�b�v���ɕK�v�Ȍo���l
		int param[CharacterParameter::Param::MAX];	// �e��p�����[�^�B
	};

	const Support::DATARECORD ExperiencePointTableInfoData[] =
	{
		{ "ExperiencePoint",Support::DataTypeE::INT, offsetof(struct ExperiencePointTableInfo,ExperiencePoint),sizeof(int) },
		{ "param",	Support::DataTypeE::INTARRAY, offsetof(struct ExperiencePointTableInfo,param),	sizeof(ExperiencePointTableInfo::param) },
	};

}


namespace {
	//�v���C���[�����𑕔����Ă���̂����܂Ƃ߂��\����(�h��ƕ���)�B
	struct PlayerEquipment
	{
		HoldArmor* armor = nullptr;		//�h��B
		HoldWeapon* weapon = nullptr;	//����B
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
		Impact,				//�_���[�W���󂯂�
		Death,				//���S
		Stop,				//�~�܂�
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
		AnimationImpact,							//�_���[�W���󂯂�
		AnimationDeath,								//���S
		AnimationNum,								//�A�j���[�V�����̐�
	};
	// �v���C���[�̃p�����[�^�[
	enum class Parameter {
		EXP = 0,
		HP,
		MP,
		ATK,
		DEF,
		DEX,
		AGI
	};
	//�v���C���[�U���{�C�X
	enum class AttackBoice
	{
		Start = 0,
		Attack1 = Start,
		Attack2,
		Attack3,
		End = Attack3
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
	void PlayAnimation(AnimationNo animno, float interpolatetime, int loopnum = -1);
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
		_Model->SetEnable(is);
	}
	//�v���C���[���
	void Releace();
	//�G�����Ƃ�����(�o���l�A����)���󂯎��B
	void TakeDrop(int dropexp, int money)
	{
		_nowEXP += dropexp;
		// �����̓C���x���g���Ɋi�[�B
		INSTANCE(Inventory)->AddPlayerMoney(money);
	}

	inline void HeelHP() {

	}

	int GetParam(CharacterParameter::Param param)
	{
		return _PlayerParam->GetParam(param);
	}
	int GetPigmentParam(CharacterParameter::Param param) {
		return _PlayerParam->GetPigmentParam(param);
	}
	int GetBuffParam(CharacterParameter::Param param) {
		return _PlayerParam->GetBuffParam(param);
	}
	int GetDebuffParam(CharacterParameter::Param param) {
		return _PlayerParam->GetDebuffParam(param);
	}

	int GetMaxHP() {
		return _PlayerParam->GetMaxHP();
	}
	int GetMaxMP() {
		return _PlayerParam->GetMaxMP();
	}
	// ���݂̌o���l�ʂ��擾�B
	int GetExp() const {
		return _nowEXP;
	}
	// ���x���A�b�v�ɕK�v�Ȍo���l���擾�B
	int GetNextLevelExp()const {
		return _EXPTable[_PlayerParam->GetParam(CharacterParameter::LV) - 1];
	}
	//�v���C���[���X�g�b�v������t���O�B
	void PlayerStopEnable()
	{
		ChangeState(State::Stop);
	}
	//�v���C���[�X�g�b�v����߂��֐��B
	//�X�e�[�g�A�C�h���ɖ߂�܂��B
	void PlayerStopDisable()
	{
		ChangeState(State::Idol);
	}

	//�v���C���[�ɑ������Z�b�g(���ŃA�C�e���R�[�h�����ĕ��킩�h����Z�b�g)�B
	void SetEquipment(HoldItemBase* equi);

	//�Q�[���J�n���ɃC���x���g�����瑕�����Ă��镐���T�������������B
	void Re_SetEquipment();

	//�v���C���[�̑����\���̂��擾�B
	inline PlayerEquipment* GetEquipment() {
		return _Equipment;
	}

	//�v���C���[��HpBar���擾�B
	inline ParameterBar* GetPlayerHpBar() {
		return _HPBar;
	}

	//�v���C���[��MpBar���擾�B
	inline ParameterBar* GetPlayerMpBar() {
		return _MPBar;
	}

	//�o�t�f�o�t�A�C�R�����擾�B
	inline BuffDebuffICon* GetBuffDebuffICon() {
		return _BuffDebuffICon;
	}


	/**
	* �A�C�e�����g�p���ꂽ.
	*/
	bool ItemEffect(Item::ItemInfo* info);
	
	/**
	* �G�t�F�N�g�p�X�V.
	*/
	void EffectUpdate();

	//�A�j���[�V�����C�x���g
	void AnimationEventControl();
	//�U��1
	void Attack1();
	//�U��2
	void Attack2();
	//�U��3
	void Attack3();
	//�U��4
	void Attack4();
	//�U��5
	void Attack5();

private:
	//�v���C���[���_���[�W���󂯂鏈��
	void _Damage();
	//�o���l�e�[�u����CSV���烍�[�h�B
	void _LoadEXPTable();
	// ���x���A�b�v�B
	// �����F		���x���A�b�v�ɕK�v�Ȍo���l�̒l�B
	void _LevelUP();
	//�b��
	void Speak();

#ifdef _DEBUG
	//�f�o�b�O�@�\
	void _DebugPlayer();
	//�f�o�b�O���x��
	void _DebugLevel(int lv);
#endif // _DEBUG

private:
	friend class PlayerStateAttack;
	friend class PlayerStateDeath;
	friend class PlayerStateIdol;
	friend class PlayerStateRun;
	friend class PlayerStateImpact;

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
	//�v���C���[�X�e�[�g�C���p
	PlayerStateImpact _ImpactState;
	//�v���C���[�X�e�[�g�f�X
	PlayerStateDeath _DeathState;
	//�v���C���[�X�e�[�g�X�g�b�v
	PlayerStateStop _StopState;
	//�v���C���[�̃p�����[�^�[
	CharacterParameter* _PlayerParam = nullptr;
	// ��]�B
	ObjectRotation* _Rotation = nullptr;
	// HP�o�[�B
	ParameterBar* _HPBar = nullptr;
	// MP�o�[�B
	ParameterBar* _MPBar = nullptr;
#ifdef _DEBUG
	// �f�o�b�O�p�f�[�^�o�̓R���|�[�l���g�B
	OutputData* _outputData = nullptr;
#endif
	//�f�o�b�O
	bool _Debug = false;
	//���x���A�b�v�ɕK�v�Ȍo���l�̃e�[�u��(LV - 1���Y����)�B
	std::vector<int> _EXPTable;
	int _nowEXP = 0;	// ���݂̌o���l�B
	// ���x�����Ƃ̃p�����[�^�[�e�[�u���B
	vector<vector<int>> _ParamTable = vector<vector<int>>(MAXLV,vector<int>(CharacterParameter::MAX,0));
	//�v���C���[�̑����B
	PlayerEquipment* _Equipment = nullptr;
	//�p�[�e�B�N���G�t�F�N�g�B
	ParticleEffect*	_ParticleEffect = nullptr;
	//�o�t�f�o�t�A�C�R���B
	BuffDebuffICon* _BuffDebuffICon = nullptr;
	//���x���A�b�v�C���[�W
	LevelUpImage* _LevelUpImage;
	//���j��
	HistoryManager* _HistoryManager = nullptr;
	//NPC�Ƙb���Ƃ��W�����v���Ȃ�����
	bool _Speak;
	//�v���C���[���_���[�W�󂯂�����SE
	SoundSource* _DamageSound = nullptr;
	//���x���A�b�v���̉�
	SoundSource* _LevelUpSound = nullptr;
	//�񕜃T�E���h
	SoundSource* _HeelSound = nullptr;
	//�X�e�[�^�X�A�b�v�T�E���h
	SoundSource* _StatusUpSound = nullptr;
	//�X�e�[�^�X�_�E���T�E���h
	SoundSource* _StatusDownSound = nullptr;
	//�v���C���[���S�T�E���h
	SoundSource* _DeathSound = nullptr;
	//�U������SE
	SoundSource* _AttackSoound = nullptr;
	//�U���{�C�X
	vector<SoundSource*> _AttackBoiceSound;
	//�U���{�C�XENUM
	AttackBoice	_AttackBoice;

	//�A�j���[�V�����C�x���g
	AnimationEventPlayer* _AnimationEventPlayer = nullptr;
};