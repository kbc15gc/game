#include"stdafx.h"
#include "BossDrarian.h"
#include "fbEngine\_Object\_Component\_3D\SkinModel.h"
#include "HFSM\EnemyTranslationState.h"
#include "HFSM\EnemyWaitState.h"
#include "fbEngine\CharacterController.h"
#include "fbEngine\_Object\_GameObject\Particle.h"
#include "fbEngine\_Object\_Component\_Physics\CapsuleColliderZ.h"
#include "GameObject\Enemy\EnemyAttack.h"

BossDrarian::BossDrarian(const char* name) : EnemyCharacter(name)
{

}


BossDrarian::~BossDrarian()
{
}

void BossDrarian::_AwakeSubClass() {
	// �g�p���郂�f���t�@�C���̃p�X��ݒ�B
	SetFileName("DRARIAN.X");
}

void BossDrarian::_StartSubClass() {

	// ����p�����B
	_ViewAngle = 100.0f;
	_ViewRange = 30.0f;

	// ���s���x�ݒ�B
	_walkSpeed = 2.5f;

	// �p�j�͈͐ݒ�B
	// ���b�菈���B
	_WanderingRange = 20.0f;

	// �ǐՔ͈͐ݒ�B
	_discoveryRange = 30.0f;

	// ����Ɉ�񂭂炢���ނ��ݒ�B
	_damageMotionRandNum = 30;

	//���f���Ƀ��C�g�ݒ�B
	_MyComponent.Model->SetLight(INSTANCE(GameObjectManager)->mainLight);
	//_MyComponent.Model->SetDitherCoefficient(1.0f);

	// �U���������`�B
	_singleAttack.reset(new EnemySingleAttack(this));
	_singleAttack->Init(6.5f,static_cast<int>(AnimationBossDrarian::Attack), 0.2f);
	_tailAttack.reset(new EnemySingleAttack(this));
	_tailAttack->Init(6.5f,static_cast<int>(AnimationBossDrarian::TailAttackRight), 0.2f);

	// �u���X�U�������B
	_breathAttack.reset(new EnemyBreathAttack(this));
	_breathAttack->Init(13.0f,static_cast<int>(AnimationBossDrarian::Breath), 0.2f);

	// �����X�e�[�g�Ɉڍs�B
	// ���b�菈���B
	_initState = State::Wandering;
	_ChangeState(_initState);
}

void BossDrarian::_UpdateSubClass() {

	if (!(_MyComponent.CharacterController->IsOnGround())) {
		// �G�l�~�[���n�ʂ��痣��Ă���B
		if (_NowStateIdx != State::Fall) {
			// ���݂̃X�e�[�g�^�C�v��ۑ��B
			_saveState = _NowStateIdx;
			// �����X�e�[�g�ɐ؂�ւ��B
			_ChangeState(State::Fall);
		}
	}
}

void BossDrarian::_LateUpdateSubClass()
{

}


EnemyAttack* BossDrarian::_AttackSelectSubClass() {
	// ���v���C���[�ƃG�l�~�[�̈ʒu�֌W�Ƃ��őJ�ڐ挈��H�B
	int rnd;

	// ���Ƃ肠�����b�菈���B
	if (Vector3(_Player->transform->GetPosition() - transform->GetPosition()).Length() <= 0.5f) {
		return _tailAttack.get();
	}
	else {
		rnd = rand() % 3;
	}

	if (rnd == 0) {
		return _singleAttack.get();
	}
	else if (rnd == 1) {
		return _breathAttack.get();
	}
	else {
		return _tailAttack.get();
	}
}

void BossDrarian::AnimationEvent_Kamituki() {
	//�U���R���W�����쐬�B
	AttackCollision* attack = CreateAttack(Vector3(0.0f, 0.25f, 5.0f), Quaternion::Identity, Vector3(1.0f, 2.0f, 2.0f), 0.25f, transform);
	attack->RemoveParent();

	// �U�����Đ��B
	EnemyPlaySound(EnemyCharacter::SoundIndex::Damage);
}

void BossDrarian::CreateAttackCollision_TailAttackSub1() 
{
	AttackCollision* attack = CreateAttack(Vector3(0.0f, -1.5f, 2.0f), Quaternion::Identity, Vector3(3.0f, 1.0f, 3.0f), 0.15f, transform,false,true,AttackCollision::ReactionType::Leans,20);
	attack->RemoveParent();
}

void BossDrarian::CreateAttackCollision_TailAttackSub2() 
{
	AttackCollision* attack = CreateAttack(Vector3(0.0f, -1.5f, 2.0f), Quaternion::Identity, Vector3(3.0f, 1.0f, 3.0f), 0.15f, transform, false, true, AttackCollision::ReactionType::Leans, 30);
	attack->RemoveParent();
}

void BossDrarian::CreateAttackCollision_TailAttackSub3()
{
	AttackCollision* attack = CreateAttack(Vector3(0.0f, -1.5f, 2.0f), Quaternion::Identity, Vector3(3.0f, 1.0f, 3.0f), 0.15f, transform, false, true, AttackCollision::ReactionType::Leans, 35);
	attack->RemoveParent();
}

void BossDrarian::CreateAttackCollision_TailAttackSub4()
{
	AttackCollision* attack = CreateAttack(Vector3(0.0f, -1.5f, 2.0f), Quaternion::Identity, Vector3(3.0f, 1.0f, 3.0f), 0.15f, transform, false, true, AttackCollision::ReactionType::Leans, 40);
	attack->RemoveParent();
}


void BossDrarian::CreateAttackCollision_TailAttack1() {
	//�U���R���W�����쐬�B
	Quaternion rot = Quaternion::Identity;
	rot.SetRotation(Vector3::axisY, D3DXToRadian(-40.0f));
	AttackCollision* attack = CreateAttack(Vector3(2.0f, 0.0f, 0.0f), rot, Vector3(2.0f, 2.0f, 10.0f), 0.15f, transform, false, false, AttackCollision::ReactionType::Blown,150);
	attack->RemoveParent();

	// �U�����Đ��B
	EnemyPlaySound(EnemyCharacter::SoundIndex::Buoon);
}

void BossDrarian::CreateAttackCollision_TailAttack2() {
	//�U���R���W�����쐬�B
	Quaternion rot = Quaternion::Identity;
	rot.SetRotation(Vector3::axisY, D3DXToRadian(-60.0f));
	AttackCollision* attack = CreateAttack(Vector3(3.0f, 0.0f, 0.0f), rot, Vector3(2.0f, 2.0f, 5.0f), 0.15f, transform, false, false, AttackCollision::ReactionType::Blown);
	attack->RemoveParent();
}

void BossDrarian::CreateAttackCollision_TailAttack3() {
	//�U���R���W�����쐬�B
	AttackCollision* attack = CreateAttack(Vector3(4.0f, 0.0f, 2.0f), Quaternion::Identity, Vector3(4.0f, 2.0f, 2.0f), 0.15f, transform, false, false, AttackCollision::ReactionType::Blown);
	attack->RemoveParent();
}

void BossDrarian::CreateAttackCollision_TailAttack4() {
	//�U���R���W�����쐬�B
	AttackCollision* attack = CreateAttack(Vector3(3.0f, 0.0f, 4.5f), Quaternion::Identity, Vector3(2.0f, 2.0f, 3.0f), 0.15f, transform, false, false, AttackCollision::ReactionType::Blown);
	attack->RemoveParent();
}

void BossDrarian::AnimationEvent_BreathStart() {
	LaserBreath* breath = INSTANCE(GameObjectManager)->AddNew<LaserBreath>("breath", 8);
	breath->Create(this, Vector3(0.0f, 0.0f, 5.0f), 10.0f,0.01f,Vector3::axisY,0.0f);

	_breathAttack->BreathStart(breath);
	// �U�����Đ��B
	EnemyPlaySound(EnemyCharacter::SoundIndex::Buoon);
}

void BossDrarian::AnimationEvent_BreathEnd() {
	_breathAttack->BreathEnd();
}

void BossDrarian::CreateAttackCollision_BreathAttackSub1()
{
	AttackCollision* attack = CreateAttack(Vector3(0.0f, -1.5f, 2.0f), Quaternion::Identity, Vector3(3.0f, 1.0f, 3.0f), 0.25f, transform, false, true, AttackCollision::ReactionType::Leans, 30);
	attack->RemoveParent();
}
void BossDrarian::CreateAttackCollision_BreathAttackSub2() 
{
	AttackCollision* attack = CreateAttack(Vector3(0.0f, -1.5f, 2.0f), Quaternion::Identity, Vector3(3.0f, 1.0f, 3.0f), 0.25f, transform, false, true, AttackCollision::ReactionType::Leans, 30);
	attack->RemoveParent();
}

void BossDrarian::_EndNowStateCallback(State EndStateType) {
	if (EndStateType == State::Wandering) {
		// �p�j�X�e�[�g�I���B
		_ChangeState(State::Wandering);
	}
	else if (EndStateType == State::StartAttack) {
		// ��x�U�����I�������B

		// ������x�U���J�n�B
		_ChangeState(State::StartAttack);
	}
	else if (EndStateType == State::Fall) {
		// �����X�e�[�g�I���B

		// ���O�̃X�e�[�g�ɐ؂�ւ��B
		_ChangeState(_saveState);
	}
	else if (EndStateType == State::Damage) {
		// �U�����󂯂��B
		// �U���J�n�B
		_ChangeState(State::StartAttack);
	}
	else if (EndStateType == State::Threat) {
		// �Њd�I���B
		// �U���J�n�B
		_ChangeState(State::StartAttack);
	}
}

void BossDrarian::_ConfigCollision() {
	// �U������p�̃R���W�����B
	{
		// �����B
		{
			RigidBody* coll = AddComponent<RigidBody>();	// �L�����N�^�[�R���g���[���Ƃ͕ʂɐV�����쐬(�v���C���[���L�����R���̌`��ŉ����o�������Ȃ�����)�B

			RigidBodyInfo info;
			info.coll = AddComponent<BoxCollider>();
			static_cast<BoxCollider*>(info.coll)->Create(Vector3(3.0f, 3.0f,1.0f));
			info.id = Collision_ID::BOSS;
			info.mass = 0.0f;
			info.physicsType = Collision::PhysicsType::Kinematick;
			info.offset = Vector3(0.0f,0.0f,2.0f);
			info.rotation = Quaternion::Identity;
			coll->Create(info, true);

			_MyComponent.ExtrudeCollisions.push_back(coll);	// ���łɉ����o���悤�R���W�����ɒǉ����Ă����B
		}
		// ���́B
		{
			RigidBody* coll = AddComponent<RigidBody>();	// �L�����N�^�[�R���g���[���Ƃ͕ʂɐV�����쐬(�v���C���[���L�����R���̌`��ŉ����o�������Ȃ�����)�B

			RigidBodyInfo info;
			info.coll = AddComponent<CCapsuleColliderZ>();
			static_cast<CCapsuleColliderZ*>(info.coll)->Create(0.75f, 8.1f);
			info.id = Collision_ID::BOSS;
			info.mass = 0.0f;
			info.physicsType = Collision::PhysicsType::Kinematick;
			info.offset = Vector3(0.0f,1.0f,0.0f);
			info.rotation = Quaternion::Identity;
			coll->Create(info, true);

			_MyComponent.ExtrudeCollisions.push_back(coll);	// ���łɉ����o���悤�R���W�����ɒǉ����Ă����B
		}
	}

	// �L�����N�^�[�R���g���[���p�B
	{
		// �R���W�����̃T�C�Y������B
		// ���L�����N�^�[�R���g���[���[�Ŏg�p���邽�߂̂��́B
		_collisionInfo.radius = 1.8f;
		_collisionInfo.height = 6.0f;
		_collisionInfo.offset = Vector3(0.0f, 0.125f, 0.0f);
		_collisionInfo.id = Collision_ID::CHARACTER_GHOST;

		// �d�͐ݒ�B
		_Gravity = -9.8f;

		// �R���|�[�l���g�ɃJ�v�Z���R���C�_�[Z��ǉ��B
		_MyComponent.Collider = AddComponent<CCapsuleColliderZ>();
		// �J�v�Z���R���C�_�[���쐬�B
		static_cast<CCapsuleColliderZ*>(_MyComponent.Collider)->Create(_collisionInfo.radius, _collisionInfo.height); 
	}
}

void BossDrarian::_ConfigCharacterController() {
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

void BossDrarian::_CreateExtrudeCollision() {
}

void BossDrarian::_BuildAnimationSubClass(vector<double>& datas) {

	// �A�j���[�V�����^�C�v�Ƀf�[�^���֘A�Â��B
	// ���G�l�~�[�͂��ׂē����X�e�[�g�N���X���g�p���邽�߁A�X�e�[�g����A�j���[�V�������Đ��ł���悤
	//   EnemyCharacter�N���X�Œ�`����Ă��邷�ׂẴG�l�~�[���ʂ̗񋓎q�Ɋ֘A�t����K�v������B
	{
		// �ҋ@��ԁB
		_ConfigAnimationType(EnemyCharacter::AnimationType::Idle, static_cast<unsigned int>(AnimationBossDrarian::Wait));
		// ���s��ԁB
		_ConfigAnimationType(EnemyCharacter::AnimationType::Walk, static_cast<unsigned int>(AnimationBossDrarian::Walk));
		// ���s��ԁB
		_ConfigAnimationType(EnemyCharacter::AnimationType::Dash, static_cast<unsigned int>(AnimationBossDrarian::Dash));
		// �i����B
		_ConfigAnimationType(EnemyCharacter::AnimationType::Threat, static_cast<unsigned int>(AnimationBossDrarian::Barking));
		// �_���[�W�����B
		datas[static_cast<int>(AnimationBossDrarian::Damage)] = 8.0f / 30.0f;
		_ConfigAnimationType(EnemyCharacter::AnimationType::Damage, static_cast<unsigned int>(AnimationBossDrarian::Damage));
		//// ������ԁB
		//// �����̃I�u�W�F�N�g�ɂ͗����̃A�j���[�V�������Ȃ��̂őҋ@�A�j���[�V�����ő�p�B
		//_ConfigAnimationType(EnemyCharacter::AnimationType::Fall, *Datas[static_cast<int>(AnimationProt::Stand)].get());
		// ���S��ԁB
		_ConfigAnimationType(EnemyCharacter::AnimationType::Death, static_cast<unsigned int>(AnimationBossDrarian::Death));
	}
}

void BossDrarian::_ConfigAnimationEvent() {
	// ���݂��U���B
	{
		float eventFrame = 0.3f;
		_MyComponent.AnimationEventPlayer->AddAnimationEvent(static_cast<int>(AnimationBossDrarian::Attack), eventFrame, static_cast<AnimationEvent>(&BossDrarian::AnimationEvent_Kamituki));
	}
	// �����ۍU���B
	{
		float eventFrame = 0.3f;
		_MyComponent.AnimationEventPlayer->AddAnimationEvent(static_cast<int>(AnimationBossDrarian::TailAttackRight), eventFrame, static_cast<AnimationEvent>(&BossDrarian::CreateAttackCollision_TailAttackSub1));

		eventFrame = 0.6f;
		_MyComponent.AnimationEventPlayer->AddAnimationEvent(static_cast<int>(AnimationBossDrarian::TailAttackRight), eventFrame, static_cast<AnimationEvent>(&BossDrarian::CreateAttackCollision_TailAttackSub2));

		eventFrame = 1.1f;
		_MyComponent.AnimationEventPlayer->AddAnimationEvent(static_cast<int>(AnimationBossDrarian::TailAttackRight), eventFrame, static_cast<AnimationEvent>(&BossDrarian::CreateAttackCollision_TailAttackSub3));

		eventFrame = 2.0f;
		_MyComponent.AnimationEventPlayer->AddAnimationEvent(static_cast<int>(AnimationBossDrarian::TailAttackRight), eventFrame, static_cast<AnimationEvent>(&BossDrarian::CreateAttackCollision_TailAttack1));

		eventFrame = 2.1f;
		_MyComponent.AnimationEventPlayer->AddAnimationEvent(static_cast<int>(AnimationBossDrarian::TailAttackRight), eventFrame, static_cast<AnimationEvent>(&BossDrarian::CreateAttackCollision_TailAttack2));

		eventFrame = 2.2f;
		_MyComponent.AnimationEventPlayer->AddAnimationEvent(static_cast<int>(AnimationBossDrarian::TailAttackRight), eventFrame, static_cast<AnimationEvent>(&BossDrarian::CreateAttackCollision_TailAttack3));

		eventFrame = 2.3f;
		_MyComponent.AnimationEventPlayer->AddAnimationEvent(static_cast<int>(AnimationBossDrarian::TailAttackRight), eventFrame, static_cast<AnimationEvent>(&BossDrarian::CreateAttackCollision_TailAttack4));

		eventFrame = 3.6f;
		_MyComponent.AnimationEventPlayer->AddAnimationEvent(static_cast<int>(AnimationBossDrarian::TailAttackRight), eventFrame, static_cast<AnimationEvent>(&BossDrarian::CreateAttackCollision_TailAttackSub4));

	}
	// �u���X�U���B
	{
		float eventFrame = 0.6f;
		_MyComponent.AnimationEventPlayer->AddAnimationEvent(static_cast<int>(AnimationBossDrarian::Breath), eventFrame, static_cast<AnimationEvent>(&BossDrarian::CreateAttackCollision_BreathAttackSub1));

		eventFrame = 1.3f;
		_MyComponent.AnimationEventPlayer->AddAnimationEvent(static_cast<int>(AnimationBossDrarian::Breath), eventFrame, static_cast<AnimationEvent>(&BossDrarian::CreateAttackCollision_BreathAttackSub2));

		eventFrame = 2.7f;
		_MyComponent.AnimationEventPlayer->AddAnimationEvent(static_cast<int>(AnimationBossDrarian::Breath), eventFrame, static_cast<AnimationEvent>(&BossDrarian::AnimationEvent_BreathStart));

		eventFrame = 4.0f;
		_MyComponent.AnimationEventPlayer->AddAnimationEvent(static_cast<int>(AnimationBossDrarian::Breath), eventFrame, static_cast<AnimationEvent>(&BossDrarian::AnimationEvent_BreathEnd));
	
		eventFrame = 4.9f;
		_MyComponent.AnimationEventPlayer->AddAnimationEvent(static_cast<int>(AnimationBossDrarian::Breath), eventFrame, static_cast<AnimationEvent>(&BossDrarian::CreateAttackCollision_TailAttackSub1));

		eventFrame = 5.3f;
		_MyComponent.AnimationEventPlayer->AddAnimationEvent(static_cast<int>(AnimationBossDrarian::Breath), eventFrame, static_cast<AnimationEvent>(&BossDrarian::CreateAttackCollision_TailAttackSub2));
	}
}

