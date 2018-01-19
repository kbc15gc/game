#include"stdafx.h"
#include "BossGhost.h"
#include "fbEngine\_Object\_Component\_3D\SkinModel.h"
#include "HFSM\EnemyTranslationState.h"
#include "HFSM\EnemyWaitState.h"
#include "fbEngine\CharacterController.h"
#include "fbEngine\_Object\_GameObject\Particle.h"
#include "fbEngine\_Object\_Component\_Physics\CapsuleColliderZ.h"
#include "GameObject\Enemy\EnemyAttack.h"
#include "GameObject\Enemy\HFSM\GhostPairAttackState.h"

BossGhost::PairAttackType BossGhost::_selectNowPairAttack;

BossGhost::BossGhost(const char* name) : EnemyCharacter(name)
{

}


BossGhost::~BossGhost()
{
}

void BossGhost::CreateCollision() {
	//�U���R���W�����쐬�B
	AttackCollision* attack = CreateAttack(Vector3(0.0f, 0.25f, 1.0f), Quaternion::Identity, Vector3(0.5f, 1.0f, 1.5f), 0.25f, transform);
	attack->RemoveParent();

	// �U�����Đ��B
	EnemyPlaySound(EnemyCharacter::SoundIndex::Damage);
}

void BossGhost::CreateCollision2() {
	//�U���R���W�����쐬�B
	AttackCollision* attack = CreateAttack(Vector3(0.0f, 0.25f, 1.0f), Quaternion::Identity, Vector3(0.5f, 1.0f, 1.5f), 0.25f, transform, false, false, AttackCollision::ReactionType::NotAction,85);
	attack->RemoveParent();

	// �U�����Đ��B
	EnemyPlaySound(EnemyCharacter::SoundIndex::Damage);
}

void BossGhost::LaserStartSingle()
{
	LaserBreath* laser = INSTANCE(GameObjectManager)->AddNew<LaserBreath>("laser", 3);
	laser->Create(this, Vector3::zero, 20.0f, 0.0025f, Vector3::axisY, 0.0f, "t1.png", Vector2(0.25f, 0.15f), 3.0f, Color::red,120);
	_singleLaser->BreathStart(laser);

	_MyComponent.Animation->SetAnimeSpeed(0.2f);
}

void BossGhost::LaserEndSingle() {
	_singleLaser->BreathEnd();
}

void BossGhost::LaserStart() {
	LaserBreath* laser = INSTANCE(GameObjectManager)->AddNew<LaserBreath>("laser", 3);
	laser->Create(this, Vector3::zero, 20.0f,0.0025f, Vector3::axisY,0.0f,"t1.png",Vector2(0.25f,0.15f),3.0f,Color::red,110);
	static_cast<EnemyBreathAttack*>(static_cast<EnemyWarpAttack*>(_laserComboAttack->GetOneAttack())->GetAttack())->BreathStart(laser);

	_MyComponent.Animation->SetAnimeSpeed(0.2f);
}

void BossGhost::LaserEnd() {
	static_cast<EnemyBreathAttack*>(static_cast<EnemyWarpAttack*>(_laserComboAttack->GetOneAttack())->GetAttack())->BreathEnd();
}

void BossGhost::LaserSpeed()
{
	_MyComponent.Animation->SetAnimeSpeed(0.2f);
}


void BossGhost::_AwakeSubClass() {
	// �g�p���郂�f���t�@�C���̃p�X��ݒ�B
	SetFileName("Ghost.X");
}

void BossGhost::_StartSubClass() {

	// ����p�����B
	_ViewAngle = 120.0f;
	//_ViewRange = 10.0f;


	// �p�j�͈͐ݒ�B
	// ���b�菈���B
	//_WanderingRange = 20.0f;

	// �ǐՔ͈͐ݒ�B
	//_discoveryRange = 30.0f;

	// ���s���x�ݒ�B
	_walkSpeed = 3.0f;

	// ����Ɉ�񂭂炢���ނ��ݒ�B
	_damageMotionRandNum = 7;

	//���f���Ƀ��C�g�ݒ�B
	_MyComponent.Model->SetLight(INSTANCE(GameObjectManager)->mainLight);
	_MyComponent.Model->SetModelEffect(ModelEffectE::ALPHA);

	// �U���������`�B
	{
		_singleAttack.reset(new EnemySingleAttack(this));
		_singleAttack->Init(1.25f, static_cast<int>(AnimationBossGhost::Attack), 0.2f,0.7f);

		_singleLaser.reset(new EnemyBreathAttack(this));
		_singleLaser->Init(3.0f, static_cast<int>(AnimationBossGhost::Attack), 0.2f, 0.7f, 1, 2);

		_comboAttack.reset(new EnemyComboAttack(this));
		EnemyAttack* singleAttack = new EnemySingleAttack(this);
		singleAttack->Init(1.0f, static_cast<int>(AnimationBossGhost::Attack), 0.2f,0.7f,1,3);
		EnemyAttack* warpAttack = new EnemyWarpAttack(this);
		static_cast<EnemyWarpAttack*>(warpAttack)->Init(13.0f, singleAttack);
		_comboAttack->Init(13.0f, warpAttack);

		_laserComboAttack.reset(new EnemyComboAttack(this));
		EnemyAttack* singleAttack2 = new EnemyBreathAttack(this);
		singleAttack2->Init(7.0f, static_cast<int>(AnimationBossGhost::Attack), 0.2f, 0.7f, 1, 1);
		EnemyAttack* warpAttack2 = new EnemyWarpAttack(this);
		static_cast<EnemyWarpAttack*>(warpAttack2)->Init(13.0f, singleAttack2);
		_laserComboAttack->Init(13.0f, warpAttack2);
	}

	_pairAttackArray.push_back(_comboAttack.get());
	_pairAttackArray.push_back(_laserComboAttack.get());

	_isPairAttackReady = false;

	// �����X�e�[�g�Ɉڍs�B
	// ���b�菈���B
	_initState = State::Wait;
	_ChangeState(_initState);
	static_cast<EnemyWaitState*>(_NowState)->CustamParameter(EnemyCharacter::AnimationType::Idle,0.2f,2,0,1.5f)/*->SetInterval(5.0f)*/;
}

void BossGhost::_UpdateSubClass() {
	if (_pairGhost) {
		if (_NowStateIdx == static_cast<EnemyCharacter::State>(BossGhostState::BossGhostPairAttack)) {
			// �����U����ԁB

			if (!_isPairAttackReady) {
				_isPairAttackReady = static_cast<GhostPairAttackState*>(_NowState)->GetIsEndWarp();
			}

			if (_isPairAttackReady) {
				// �����̏������������Ă���B
				if (static_cast<BossGhostState>(_pairGhost->GetNowStateIndex()) == BossGhostState::BossGhostPairAttack) {
					if (_pairGhost->GetIsPairAttackReady()) {
						// �y�A�̏������������Ă���B

						static_cast<GhostPairAttackState*>(_NowState)->SetIsStartAttack(true);	// �U�����J�n����B
					}
				}
			}
		}
	}
	else if(_NowStateIdx == static_cast<EnemyCharacter::State>(BossGhostState::BossGhostPairAttack)){
		// �����U�����Ƀy�A�֌W���������ꂽ�B

		static_cast<GhostPairAttackState*>(_NowState)->SetIsStartAttack(true);	// �U�����J�n����B
	}
}

void BossGhost::_LateUpdateSubClass()
{

}


EnemyAttack* BossGhost::_AttackSelectSubClass() {
	// ���v���C���[�ƃG�l�~�[�̈ʒu�֌W�Ƃ��őJ�ڐ挈��H�B

	EnemyAttack* attack = nullptr;

	if (static_cast<BossGhostState>(_NowStateIdx) == BossGhostState::BossGhostPairAttack) {
		attack = _pairAttackArray[_selectNowPairAttack];
	}
	else {
		// ���Ƃ肠�����b�菈���B
		int rnd = rand() % 4;
		if (rnd == 0) {
			attack = _singleAttack.get();
		}
		else if (rnd == 1) {
			attack = _singleLaser.get();
		}
		else if (rnd == 2) {
			attack = _comboAttack.get();
		}
		else {
			attack = _laserComboAttack.get();
		}
	}
	return attack;
}

void BossGhost::_EndNowStateCallback(State EndStateType) {

	// �Ƃ肠�����A���ŋ����U�����Ȃ��悤�ɂ���B
	if (EndStateType == static_cast<EnemyCharacter::State>(BossGhostState::BossGhostPairAttack)) {
		_isPairAttackReady = false;
		_ChangeState(State::Wait);
		static_cast<EnemyWaitState*>(_NowState)->SetInterval(3.0f);
	}
	else if (_pairGhost && _isCommand) {
		// ���X�{�X���疽�߂��󂯂��B

		// �����U���J�n�B
		_ChangeState(static_cast<EnemyCharacter::State>(BossGhostState::BossGhostPairAttack));
		static_cast<GhostPairAttackState*>(_NowState)->SetStartAttackInterval(_intervalStartPairAttack);
		_isCommand = false;
	}
	else if (EndStateType == State::Damage) {
		// �U�����󂯂��B
		// �U���J�n�B
		_ChangeState(State::StartAttack);
	}
	else if (EndStateType == State::StartAttack) {
		// ��x�U�����I�������B

		if (!_isEntourage) {
			// ������x�U���J�n�B
			_ChangeState(State::StartAttack);
		}
		else {
			_ChangeState(State::Wait);
			static_cast<EnemyWaitState*>(_NowState)->SetInterval(3.0f);
		}
	}
	else if (EndStateType == State::Wait) {
		if (_isEntourage) {
			_ChangeState(State::StartAttack);
		}
		else {
			_ChangeState(State::Wait);
		}
	}
	else if (EndStateType == State::Threat && !_isEntourage) {
		_ChangeState(State::StartAttack);
	}
}

void BossGhost::_ConfigCollision() {
	// �L�����N�^�[�R���g���[���p�B
	{
		// �R���W�����̃T�C�Y������B
		// ���L�����N�^�[�R���g���[���[�Ŏg�p���邽�߂̂��́B
		_collisionInfo.radius = 0.3f;
		_collisionInfo.height = 0.5f;
		_collisionInfo.offset = Vector3(0.0f, 0.0f, 0.0f);
		_collisionInfo.id = Collision_ID::BOSS;

		// �d�͐ݒ�B
		_Gravity = 0.0f;

		// �R���|�[�l���g�ɃJ�v�Z���R���C�_�[Z��ǉ��B
		_MyComponent.Collider = AddComponent</*CCapsuleCollider*/BoxCollider>();
		// �J�v�Z���R���C�_�[���쐬�B
		//static_cast<CCapsuleCollider*>(_MyComponent.Collider)->Create(_collisionInfo.radius, _collisionInfo.height);
		static_cast<BoxCollider*>(_MyComponent.Collider)->Create(Vector3(0.6f, 1.7f,0.6f));
	}
}

void BossGhost::_ConfigCharacterController() {
	// �Փ˂��鑮����ݒ�(��)�B
	_MyComponent.CharacterController->AttributeXZ_AllOn();
	_MyComponent.CharacterController->SubAttributeXZ(Collision_ID::ATTACK);
	_MyComponent.CharacterController->SubAttributeXZ(Collision_ID::SPACE);
	_MyComponent.CharacterController->SubAttributeXZ(Collision_ID::PLAYER);
	_MyComponent.CharacterController->SubAttributeXZ(Collision_ID::ENEMY);
	_MyComponent.CharacterController->SubAttributeXZ(Collision_ID::DROPITEM);
	// �Փ˂��鑮����ݒ�(�c)�B
	_MyComponent.CharacterController->AttributeY_AllOn();
	_MyComponent.CharacterController->SubAttributeY(Collision_ID::ATTACK);
	_MyComponent.CharacterController->SubAttributeY(Collision_ID::ENEMY);
	_MyComponent.CharacterController->SubAttributeY(Collision_ID::PLAYER);
	_MyComponent.CharacterController->SubAttributeY(Collision_ID::SPACE);
	_MyComponent.CharacterController->SubAttributeY(Collision_ID::DROPITEM);

}

void BossGhost::_CreateExtrudeCollision() {
	_MyComponent.ExtrudeCollisions.push_back(_MyComponent.CharacterController->GetRigidBody());	// �����o���悤�R���W�����ɒǉ����Ă����B
}

void BossGhost::_BuildStateSubClass() {
	// �S�[�X�g�����U���X�e�[�g�ǉ��B
	_MyState.push_back(unique_ptr<GhostPairAttackState>(new GhostPairAttackState(this)));
}


void BossGhost::_BuildAnimationSubClass(vector<double>& datas) {

	// �A�j���[�V�����^�C�v�Ƀf�[�^���֘A�Â��B
	// ���G�l�~�[�͂��ׂē����X�e�[�g�N���X���g�p���邽�߁A�X�e�[�g����A�j���[�V�������Đ��ł���悤
	//   EnemyCharacter�N���X�Œ�`����Ă��邷�ׂẴG�l�~�[���ʂ̗񋓎q�Ɋ֘A�t����K�v������B
	{
		// �ҋ@��ԁB
		_ConfigAnimationType(EnemyCharacter::AnimationType::Idle, static_cast<unsigned int>(AnimationBossGhost::Wait));
		// ���s��ԁB
		_ConfigAnimationType(EnemyCharacter::AnimationType::Walk, static_cast<unsigned int>(AnimationBossGhost::Walk));
		// ���s��ԁB
		_ConfigAnimationType(EnemyCharacter::AnimationType::Dash, static_cast<unsigned int>(AnimationBossGhost::Walk));
		// �_���[�W�����B
		_ConfigAnimationType(EnemyCharacter::AnimationType::Damage, static_cast<unsigned int>(AnimationBossGhost::Damage));
		// ���S��ԁB
		_ConfigAnimationType(EnemyCharacter::AnimationType::Death, static_cast<unsigned int>(AnimationBossGhost::Death));
	}

	datas[static_cast<int>(AnimationBossGhost::Damage)] = 10.0f / 30.0f;
	datas[static_cast<int>(AnimationBossGhost::Attack)] = 20.0f / 30.0f;
}

void BossGhost::_ConfigAnimationEvent() {
	float eventFrame = 0.1f;
	
	// �p���`�U���B
	{
		eventFrame = 0.5f;
		_MyComponent.AnimationEventPlayer->AddAnimationEvent(static_cast<int>(AnimationBossGhost::Attack), eventFrame, static_cast<AnimationEvent>(&BossGhost::CreateCollision));
	}

	// ��p���`�U���B
	{
		eventFrame = 0.5f;
		_MyComponent.AnimationEventPlayer->AddAnimationEvent(static_cast<int>(AnimationBossGhost::Attack), eventFrame, static_cast<AnimationEvent>(&BossGhost::CreateCollision2),3);
	}

	// ���[�U�[�U��(�P��)�B
	{
		eventFrame = 0.5f;
		_MyComponent.AnimationEventPlayer->AddAnimationEvent(static_cast<int>(AnimationBossGhost::Attack), eventFrame, static_cast<AnimationEvent>(&BossGhost::LaserStartSingle), 2);
		eventFrame = 1.5f;
		_MyComponent.AnimationEventPlayer->AddAnimationEvent(static_cast<int>(AnimationBossGhost::Attack), eventFrame, static_cast<AnimationEvent>(&BossGhost::LaserEndSingle), 2);
	}

	// ���[�U�[�U��(�R���{)�B
	{
		eventFrame = 0.5f;
		_MyComponent.AnimationEventPlayer->AddAnimationEvent(static_cast<int>(AnimationBossGhost::Attack), eventFrame, static_cast<AnimationEvent>(&BossGhost::LaserStart),1);
		eventFrame = 1.5f;
		_MyComponent.AnimationEventPlayer->AddAnimationEvent(static_cast<int>(AnimationBossGhost::Attack), eventFrame, static_cast<AnimationEvent>(&BossGhost::LaserEnd),1);
	}
}

