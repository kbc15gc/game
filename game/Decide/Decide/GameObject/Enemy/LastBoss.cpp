#include"stdafx.h"
#include "LastBoss.h"
#include "fbEngine\_Object\_Component\_3D\SkinModel.h"
#include "HFSM\EnemyTranslationState.h"
#include "HFSM\EnemyWaitState.h"
#include "fbEngine\CharacterController.h"
#include "HFSM\LastBossThroneState.h"
#include "HFSM\LastBossMagicianState.h"
#include "HFSM\LastBossHistoryTamperingState.h"
#include "HFSM\LastBossDownState.h"
#include "fbEngine\_Object\_GameObject\Particle.h"
#include "GameObject\Enemy\EnemyAttack.h"
#include "GameObject\Enemy\LastBossMagic.h"
#include "GameObject\Enemy\LaserBreath.h"
#include "GameObject\Enemy\SordShock.h"

//EnemySingleAttack Enemy::_singleAttack = EnemySingleAttack(_AnimationData[EnemyCharacter::AnimationType::Attack],);

LastBoss::LastBoss(const char* name) : EnemyCharacter(name)
{

}


LastBoss::~LastBoss()
{
}


void LastBoss::SordAttackEvent() {
	//�U���R���W�����쐬�B
	unsigned int priorty = 1;
	AttackCollision* attack = INSTANCE(GameObjectManager)->AddNew<AttackCollision>("attackCollision", priorty);
	attack->Create(_MyComponent.Parameter->GiveDamageMass(false, false), Vector3(0.0f, 0.0f, 2.0f), Quaternion::Identity, Vector3(1.0f,3.0f,2.0f), AttackCollision::CollisionMaster::Enemy, 0.25f, AttackCollision::ReactionType::Leans, transform);
	attack->RemoveParent();

	_sordAttackShot0 = INSTANCE(GameObjectManager)->AddNew<SordShock>("sordShock", 8);
	_sordAttackShot0->Create(this, Vector3(0.0f, -1.0f, 2.0f),transform->GetForward() * 25.0f,15.0f);
	_sordAttackShot0->BreathStart();
	_sordAttackShot0->BreathEnd();


	//_sordAttackLaser0 = INSTANCE(GameObjectManager)->AddNew<LaserBreath>("breath", 8);
	//_sordAttackLaser0->Create(this, Vector3(0.0f, -1.0f, 3.0f), 10.0f, transform->GetRight() ,-90.0f,"t1.png",Vector2(0.5f, 0.5f),2.0f,Color(1.5f,0.5f,1.5f,1.0f));
	//_sordAttackLaser0->BreathStart();

	//_sordAttackLaser1 = INSTANCE(GameObjectManager)->AddNew<LaserBreath>("breath", 8);
	//_sordAttackLaser1->Create(this, Vector3(0.0f, -1.0f, 4.0f), 10.0f, transform->GetRight(), -90.0f, "MurasakiHonoo.png", Vector2(0.5f, 0.5f),1.2f ,Color(1.5f, 1.0f, 0.0f, 1.0f));
	//_sordAttackLaser1->BreathStart();

	//_sordAttackLaser2 = INSTANCE(GameObjectManager)->AddNew<LaserBreath>("breath", 8);
	//_sordAttackLaser2->Create(this, Vector3(0.0f, -1.0f, 5.0f), 10.0f, transform->GetRight(), -90.0f, "MurasakiHonoo.png", Vector2(0.5f, 0.5f), 1.2f,Color(1.5f, 1.0f, 0.0f, 1.0f));
	//_sordAttackLaser2->BreathStart();


	// �U�����Đ��B
	EnemyPlaySound(EnemyCharacter::SoundIndex::Damage);
}

void LastBoss::SordAttackEvent2() {
	//_sordAttackLaser0->BreathEnd();
	//_sordAttackLaser1->BreathEnd();
	//_sordAttackLaser2->BreathEnd();
}

void LastBoss::MagicAttackStart1() {
	_magicFire1 = INSTANCE(GameObjectManager)->AddNew<LastBossMagic>("breath", 8);
	Quaternion rot;
	rot.SetRotation(Vector3::axisY, D3DXToRadian(20.0f));
	_magicFire1->Create(this, Vector3(2.0f, 0.0f, 4.0f), rot,20.0f);

	_magicAttack->BreathStart(_magicFire1);
}

void LastBoss::MagicAttackShot1() {
	_magicFire1->Shot();
	_magicAttack->BreathEnd();
}

void LastBoss::MagicAttackStart2() {
	_magicFire2 = INSTANCE(GameObjectManager)->AddNew<LastBossMagic>("breath", 8);
	Quaternion rot;
	rot.SetRotation(Vector3::axisY, D3DXToRadian(-20.0f));
	_magicFire2->Create(this, Vector3(-2.0f, 0.0f, 4.0f), rot,20.0f);

	_magicAttack->BreathStart(_magicFire2);
}

void LastBoss::MagicAttackShot2() {
	_magicFire2->Shot();
	_magicAttack->BreathEnd();
}

void LastBoss::MagicAttackStart3() {
	_magicFire3 = INSTANCE(GameObjectManager)->AddNew<LastBossMagic>("breath", 8);
	_magicFire3->Create(this, Vector3(0.0f, 0.0f, 4.0f), Quaternion::Identity,20.0f);

	_magicAttack->BreathStart(_magicFire3);
	_magicAttack->BreathEnd();
}

void LastBoss::MagicAttackShot3() {
	_magicFire3->Shot();
	_magicAttack->BreathEnd();
}

void LastBoss::BuffEvent() {
	int value[CharacterParameter::Param::MAX];
	for (int idx = 0; idx < static_cast<int>(CharacterParameter::Param::MAX); idx++) {
		value[idx] = 0;
	}

	// ���g�Ƀo�t�B
	value[static_cast<int>(CharacterParameter::Param::ATK)] = 20;
	value[static_cast<int>(CharacterParameter::Param::MAT)] = 20;
	value[static_cast<int>(CharacterParameter::Param::DEF)] = 20;
	value[static_cast<int>(CharacterParameter::Param::MDE)] = 20;
	BuffAndDebuff(value, 30.0f);
}

void LastBoss::DebuffEvent() {
	int value[CharacterParameter::Param::MAX];
	for (int idx = 0; idx < static_cast<int>(CharacterParameter::Param::MAX); idx++) {
		value[idx] = 0;
	}

	// �v���C���[�Ƀf�o�t�B
	value[static_cast<int>(CharacterParameter::Param::ATK)] = -50;
	value[static_cast<int>(CharacterParameter::Param::MAT)] = -50;
	value[static_cast<int>(CharacterParameter::Param::DEF)] = -30;
	value[static_cast<int>(CharacterParameter::Param::MDE)] = -30;
	PlayerBuffAndDebuff(value, 30.0f);
}

void LastBoss::EntourageCommand() {
	if (_NowStateIdx == static_cast<State>(LastBossState::LastBossThrone)) {
		if (!static_cast<LastBossThroneState*>(_NowState)->EntourageCommand()) {
			// ���߂��Е������Ă���B

			EncourageBuff();
			DebuffEvent();
		}
	}
}

void LastBoss::EncourageBuff() {
	if (_NowStateIdx == static_cast<State>(LastBossState::LastBossThrone)) {
		static_cast<LastBossThroneState*>(_NowState)->EncourageBuff();
	}
}

void LastBoss::_AwakeSubClass() {
	// �g�p���郂�f���t�@�C���̃p�X��ݒ�B
	SetFileName("LastBoss.X");
	//�|�W�V����
	//_InitPos = Vector3(-145.69, 190.0f, 264.72f);

	// �f�o�b�O���̑��t�߁B
	_InitPos = Vector3(-387.3f, 58.307f, -75.8f),
	transform->SetPosition(_InitPos);
}

void LastBoss::_StartSubClass() {

	// ����p�����B
	_ViewAngle = 360.0f;
	_ViewRange = 50.0f;

	// �p�j�͈͐ݒ�B
	// ���b�菈���B
	_WanderingRange = 0.0f;

	// �ǐՔ͈͐ݒ�B
	_discoveryRange = 100.0f;

	// ���s���x�ݒ�B
	_walkSpeed = 5.0f;

	// ����Ɉ�񂭂炢���ނ��ݒ�B
	_damageMotionRandNum = 1;

	//���f���Ƀ��C�g�ݒ�B
	_MyComponent.Model->SetLight(INSTANCE(GameObjectManager)->mainLight);

	// �U���������`�B
	_sordAttack.reset(new EnemySingleAttack(this));
	_sordAttack->Init(3.0f,static_cast<int>(AnimationLastBoss::SordAttack), 0.2f);
	//_sordAttack
	_magicAttack.reset(new EnemyBreathAttack(this));
	_magicAttack->Init(7.0f, static_cast<int>(AnimationLastBoss::Magic), 0.2f);
	_buffAttack.reset(new EnemySingleAttack(this));
	_buffAttack->Init(10.0f, static_cast<int>(AnimationLastBoss::Magic), 0.2f);
	_debuffAttack.reset(new EnemySingleAttack(this));
	_debuffAttack->Init(10.0f, static_cast<int>(AnimationLastBoss::MagicThrone), 0.2f);
	_commandAttack.reset(new EnemySingleAttack(this));
	_commandAttack->Init(10.0f, static_cast<int>(AnimationLastBoss::MagicThrone), 0.2f, 1.0f, 1, 1);
	_encourageBuffAttack.reset(new EnemySingleAttack(this));
	_encourageBuffAttack->Init(10.0f, static_cast<int>(AnimationLastBoss::MagicThrone), 0.2f, 1.0f, 1, 2);

	// �����X�e�[�g�Ɉڍs�B
	// ���b�菈���B
	//_initState = static_cast<State>(LastBossState::LastBossMagician);
	//_initState = static_cast<State>(LastBossState::LastBossHistory);
	_initState = State::Speak;

	//_voiceYokukitana.reset(_CreateSoundData("LastBoss_YOKUKITANA1.wav",35.0f,true));
	//_voiceYokukitana.reset(_CreateSoundData("maou_test.wav", 4.0f, true));

	_ChangeState(_initState);
}

void LastBoss::_UpdateSubClass() {
#ifdef _DEBUG
	Debug();
#endif // _DEBUG

	// �T�E���h�e�X�g�B
	//_voiceYokukitana->Play();
}

void LastBoss::_LateUpdateSubClass()
{

}


EnemyAttack* LastBoss::_AttackSelectSubClass() {
	// ���v���C���[�ƃG�l�~�[�̈ʒu�֌W�Ƃ��őJ�ڐ挈��H�B
	EnemyAttack* attack = nullptr;

	int rnd;

	if (_NowStateIdx == static_cast<State>(LastBossState::LastBossMagician)) {
		// ���p�t�X�e�[�g�B
		// �m���ōU���Ɩ����ւ̃o�t���s���B

		rnd = rand() % 2;
		////rnd = rand() % 4;

		if (rnd == 0) {
			// ���U���B

			attack = _sordAttack.get();
		}
		else if (rnd == 1) {
			// ���@�U���B

			attack = _magicAttack.get();
		}
		//else if(rnd == 2){
		//// �o�t�B

		//	attack = _buffAttack.get();
		//}
		//else {
		//	// ���U���B

		//	attack = 
		//}
	}
	else if (_NowStateIdx == static_cast<State>(LastBossState::LastBossThrone)) {
		// �ʍ��X�e�[�g�B
		// �f�o�t�������Ȃ��B

		rnd = rand() % 3;
		if (rnd == 0) {
			attack = _debuffAttack.get();
		}
		else if(rnd == 1){
			attack = _commandAttack.get();
		}
		else {
			attack = _encourageBuffAttack.get();
		}
	}

	return attack;
}

void LastBoss::_EndNowStateCallback(State EndStateType) {
	if (EndStateType == State::Speak) {
		// ��b�I���B
		
		// �ʍ��X�e�[�g�Ɉڍs�B
		_ChangeState(static_cast<State>(LastBossState::LastBossThrone));
	}
	else if (static_cast<LastBossState>(EndStateType) == LastBossState::LastBossThrone) {
		// �ʍ��X�e�[�g�I���B

		// �ʍ��Ȃ��̏�ԂɃ��[�V������ύX�B
		_ConfigAnimationType(AnimationType::Idle, static_cast<int>(AnimationLastBoss::Wait));
		_ConfigAnimationType(AnimationType::Walk, static_cast<int>(AnimationLastBoss::Move));
		_ConfigAnimationType(AnimationType::Dash, static_cast<int>(AnimationLastBoss::Move));
		_ConfigAnimationType(AnimationType::BackStep, static_cast<int>(AnimationLastBoss::Move));

		// ���p�t�X�e�[�g�Ɉڍs�B
		_ChangeState(static_cast<State>(LastBossState::LastBossMagician));
	}
	else if (static_cast<LastBossState>(EndStateType) == LastBossState::LastBossMagician) {
		// ���p�t�X�e�[�g�I���B

		// ���j��₃X�e�[�g�Ɉڍs�B
		_ChangeState(static_cast<State>(LastBossState::LastBossHistory));
	}
	else if (static_cast<LastBossState>(EndStateType) == LastBossState::LastBossHistory) {
		// ���j��₃X�e�[�g�I���B

		// ���p�t�X�e�[�g�Ɉڍs�B
		_ChangeState(static_cast<State>(LastBossState::LastBossMagician));
	}
	else if (EndStateType == EnemyCharacter::State::Damage) {
		// �_���[�W�X�e�[�g�I���B

		// ��u�O�̃X�e�[�g�Ɉڍs�B
		_ChangeState(static_cast<EnemyCharacter::State>(_saveState));
	}
}

void LastBoss::_ChangeStateCallback(State prev, State next) {
	if (next == State::Speak) {
		// �����X�e�[�g�ɖ߂����B

		// ��b�X�e�[�g�̒��ɂ͏��������Ȃ����A���[�V�����ύX�͍s�������̂ł�����ŏ���������B
		// �ʍ�����̏�ԂɃ��[�V������ύX�B
		_ConfigAnimationType(AnimationType::Idle, static_cast<int>(AnimationLastBoss::WaitThrone));
		_ConfigAnimationType(AnimationType::Walk, static_cast<int>(AnimationLastBoss::MoveThrone));
		_ConfigAnimationType(AnimationType::Dash, static_cast<int>(AnimationLastBoss::MoveThrone));
		_ConfigAnimationType(AnimationType::BackStep, static_cast<int>(AnimationLastBoss::MoveThrone));
	}
}

void LastBoss::_ConfigCollision() {

	// �R���W�����̃T�C�Y������B
	// ���L�����N�^�[�R���g���[���[�Ŏg�p���邽�߂̂��́B
	_collisionInfo.radius = 0.5f;
	_collisionInfo.height = 3.6f;
	_collisionInfo.offset = Vector3(0.0f, 0.46f, 0.0f);
	_collisionInfo.id = Collision_ID::BOSS;

	// �R���|�[�l���g�ɃJ�v�Z���R���C�_�[��ǉ��B
	_MyComponent.Collider = AddComponent<CCapsuleCollider>();
	// �J�v�Z���R���C�_�[���쐬�B
	static_cast<CCapsuleCollider*>(_MyComponent.Collider)->Create(_collisionInfo.radius, _collisionInfo.height);

	_Gravity = 0.0f;
}

void LastBoss::_ConfigCharacterController() {
	// �Փ˂��鑮����ݒ�(��)�B
	_MyComponent.CharacterController->AttributeXZ_AllOn();
	_MyComponent.CharacterController->SubAttributeXZ(Collision_ID::ATTACK);
	_MyComponent.CharacterController->SubAttributeXZ(Collision_ID::SPACE);
	_MyComponent.CharacterController->SubAttributeXZ(Collision_ID::ENEMY);
	_MyComponent.CharacterController->SubAttributeXZ(Collision_ID::PLAYER);	// �v���C���[�͉����o���̂ŉ����߂���Ȃ��悤�ɂ���B
	_MyComponent.CharacterController->SubAttributeXZ(Collision_ID::DROPITEM);
	// �Փ˂��鑮����ݒ�(�c)�B
	_MyComponent.CharacterController->AttributeY_AllOn();
	_MyComponent.CharacterController->SubAttributeY(Collision_ID::ATTACK);
	_MyComponent.CharacterController->SubAttributeY(Collision_ID::ENEMY);
	_MyComponent.CharacterController->SubAttributeY(Collision_ID::PLAYER);
	_MyComponent.CharacterController->SubAttributeY(Collision_ID::SPACE);
	_MyComponent.CharacterController->SubAttributeY(Collision_ID::DROPITEM);
}

void LastBoss::_CreateExtrudeCollision() {
	_MyComponent.ExtrudeCollisions.push_back(_MyComponent.CharacterController->GetRigidBody());	// �L�����N�^�[�R���g���[���̍��̂����̂܂܎g�p�B
}

void LastBoss::_BuildStateSubClass() {
	// ���X�{�X�ʍ��X�e�[�g�ǉ��B
	_MyState.push_back(unique_ptr<LastBossThroneState>(new LastBossThroneState(this)));
	// ���X�{�X���p�t�X�e�[�g�ǉ��B
	_MyState.push_back(unique_ptr<LastBossMagicianState>(new LastBossMagicianState(this)));
	// ���X�{�X���j��₃X�e�[�g�ǉ��B
	_MyState.push_back(unique_ptr<LastBossHistoryTamperingState>(new LastBossHistoryTamperingState(this)));
	// ���X�{�X�_�E���X�e�[�g�ǉ��B
	_MyState.push_back(unique_ptr<LastBossDownState>(new LastBossDownState(this)));
}

void LastBoss::_BuildAnimationSubClass(vector<double>& datas) {

	// �A�j���[�V�����^�C�v�Ƀf�[�^���֘A�Â��B
	// ���G�l�~�[�͂��ׂē����X�e�[�g�N���X���g�p���邽�߁A�X�e�[�g����A�j���[�V�������Đ��ł���悤
	//   EnemyCharacter�N���X�Œ�`����Ă��邷�ׂẴG�l�~�[���ʂ̗񋓎q�Ɋ֘A�t����K�v������B
	{
		// �ҋ@��ԁB
		_ConfigAnimationType(EnemyCharacter::AnimationType::Idle, static_cast<unsigned int>(AnimationLastBoss::WaitThrone));
		// ���s��ԁB
		_ConfigAnimationType(EnemyCharacter::AnimationType::Walk, static_cast<unsigned int>(AnimationLastBoss::MoveThrone));
		//���s��ԁB
		 //�����̃I�u�W�F�N�g�ɂ̓_�b�V���̃A�j���[�V�������Ȃ��̂ŕ����A�j���[�V�����ő�p�B
		_ConfigAnimationType(EnemyCharacter::AnimationType::Dash, static_cast<unsigned int>(AnimationLastBoss::MoveThrone));
	//	// ������ԁB
	//	// �����̃I�u�W�F�N�g�ɂ͗����̃A�j���[�V�������Ȃ��̂őҋ@�A�j���[�V�����ő�p�B
	//	_ConfigAnimationType(EnemyCharacter::AnimationType::Fall, *Datas[static_cast<int>(AnimationProt::Stand)].get());
		// �o�b�N�X�e�b�v�B
		_ConfigAnimationType(EnemyCharacter::AnimationType::BackStep, static_cast<unsigned int>(AnimationLastBoss::Move));
		// �_���[�W��ԁB
		_ConfigAnimationType(EnemyCharacter::AnimationType::Damage, static_cast<unsigned int>(AnimationLastBoss::Damage));
		// ���S��ԁB
		_ConfigAnimationType(EnemyCharacter::AnimationType::Death, static_cast<unsigned int>(AnimationLastBoss::Death));
	}
}

void LastBoss::_ConfigAnimationEvent() {

	float eventFrame;

	// ���U���B
	{
		eventFrame = 1.6f;
		_MyComponent.AnimationEventPlayer->AddAnimationEvent(static_cast<int>(AnimationLastBoss::SordAttack), eventFrame, static_cast<AnimationEvent>(&LastBoss::SordAttackEvent));

		eventFrame += 1.0f;
		_MyComponent.AnimationEventPlayer->AddAnimationEvent(static_cast<int>(AnimationLastBoss::SordAttack), eventFrame, static_cast<AnimationEvent>(&LastBoss::SordAttackEvent2));

	}

	// ���@�U���B
	{
		eventFrame = 1.0f;
		_MyComponent.AnimationEventPlayer->AddAnimationEvent(static_cast<int>(AnimationLastBoss::Magic), eventFrame, static_cast<AnimationEvent>(&LastBoss::MagicAttackStart1));

		eventFrame += 0.3f;
		_MyComponent.AnimationEventPlayer->AddAnimationEvent(static_cast<int>(AnimationLastBoss::Magic), eventFrame, static_cast<AnimationEvent>(&LastBoss::MagicAttackStart2));

		eventFrame += 0.3f;
		_MyComponent.AnimationEventPlayer->AddAnimationEvent(static_cast<int>(AnimationLastBoss::Magic), eventFrame, static_cast<AnimationEvent>(&LastBoss::MagicAttackStart3));


		eventFrame = 2.01f;
		_MyComponent.AnimationEventPlayer->AddAnimationEvent(static_cast<int>(AnimationLastBoss::Magic), eventFrame, static_cast<AnimationEvent>(&LastBoss::MagicAttackShot1));
		eventFrame += 0.1f;
		_MyComponent.AnimationEventPlayer->AddAnimationEvent(static_cast<int>(AnimationLastBoss::Magic), eventFrame, static_cast<AnimationEvent>(&LastBoss::MagicAttackShot2));
		eventFrame += 0.1f;
		_MyComponent.AnimationEventPlayer->AddAnimationEvent(static_cast<int>(AnimationLastBoss::Magic), eventFrame, static_cast<AnimationEvent>(&LastBoss::MagicAttackShot3));
	}

	// �o�t�B
	{
		//eventFrame = 1.0f;
		//_MyComponent.AnimationEventPlayer->AddAnimationEvent(static_cast<int>(AnimationLastBoss::Magic), eventFrame, static_cast<AnimationEvent>(&LastBoss::BuffEvent));
	}

	// �f�o�t�B
	{
		eventFrame = 1.0f;
		_MyComponent.AnimationEventPlayer->AddAnimationEvent(static_cast<int>(AnimationLastBoss::MagicThrone), eventFrame, static_cast<AnimationEvent>(&LastBoss::DebuffEvent));
	}

	// ���ߖ���(���߂���̂ɂȂ����ꍇ�̓v���C���[�ւ̃f�o�t�Ƒ��߂ւ̃o�t)�B
	{
		eventFrame = 1.0f;
		_MyComponent.AnimationEventPlayer->AddAnimationEvent(static_cast<int>(AnimationLastBoss::MagicThrone), eventFrame, static_cast<AnimationEvent>(&LastBoss::EntourageCommand),1);
	}

	// ���߃o�t�B
	{
		eventFrame = 1.0f;
		_MyComponent.AnimationEventPlayer->AddAnimationEvent(static_cast<int>(AnimationLastBoss::MagicThrone), eventFrame, static_cast<AnimationEvent>(&LastBoss::EncourageBuff), 2);
	}
}

#ifdef _DEBUG
void LastBoss::Debug() {
	if(KeyBoardInput->isPressed(DIK_X)&&KeyBoardInput->isPush(DIK_B))
	{
		this->SetActive(false, true);
	}

	if (KeyBoardInput->isPressed(DIK_X) && KeyBoardInput->isPush(DIK_V))
	{
		this->SetActive(true, true);
	}
}
#endif // _DEBUG

