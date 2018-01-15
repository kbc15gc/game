#include"stdafx.h"
#include "CodeNameD.h"
#include "fbEngine\_Object\_Component\_3D\SkinModel.h"
#include "HFSM\EnemyTranslationState.h"
#include "HFSM\EnemyWaitState.h"
#include "fbEngine\CharacterController.h"
#include "fbEngine\_Object\_GameObject\Particle.h"
#include "fbEngine\_Object\_Component\_Physics\CapsuleColliderZ.h"
#include "GameObject\Enemy\EnemyAttack.h"

BossD::BossD(const char* name) : EnemyCharacter(name)
{

}

BossD::~BossD()
{
}

void BossD::_AwakeSubClass() {
	SetFileName("BossD.X");
}
void BossD::_StartSubClass() {

	//�|�W�V����
	//_InitPos = Vector3(-202.0f, 120.0f, -156.0f);
	//transform->SetPosition(_InitPos);

	// ����p�����B
	_ViewAngle = 100.0f;
	//_ViewRange = 30.0f;

	// ���s���x�ݒ�B
	_walkSpeed = 2.5f;

	// �p�j�͈͐ݒ�B
	// ���b�菈���B
	//_WanderingRange = 20.0f;

	// �ǐՔ͈͐ݒ�B
	//_discoveryRange = 30.0f;

	// ����Ɉ�񂭂炢���ނ��ݒ�B
	_damageMotionRandNum = 30;

	//���f���Ƀ��C�g�ݒ�B
	_MyComponent.Model->SetLight(INSTANCE(GameObjectManager)->mainLight);
	//�U�������B
	_singleAttack.reset(new EnemySingleAttack(this));
	_singleAttack->Init(6.5f, static_cast<int>(AnimationBossD::Attack), 0.2f);

	_kick.reset(new EnemySingleAttack(this));
	_kick->Init(6.5f, static_cast<int>(AnimationBossD::Kick), 0.2f);

	_kiriage.reset(new EnemySingleAttack(this));
	_kiriage->Init(6.5f, static_cast<int>(AnimationBossD::Kiriage), 0.2f);

	_360Attack.reset(new EnemySingleAttack(this));
	_360Attack->Init(6.5f, static_cast<int>(AnimationBossD::Attack360), 0.2f);

	// �����X�e�[�g�Ɉڍs�B
	// ���b�菈���B
	_initState = State::Wandering;
	_ChangeState(_initState);
}

void BossD::_UpdateSubClass() {

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

void BossD::_LateUpdateSubClass()
{

}

EnemyAttack* BossD::_AttackSelectSubClass() {
	// ���v���C���[�ƃG�l�~�[�̈ʒu�֌W�Ƃ��őJ�ڐ挈��H�B

	 //���Ƃ肠�����b�菈���B
	int rnd = rand() % 4;
	if (rnd == 0) {
		return _singleAttack.get();
	}
	else if(rnd == 1)
	{
		return _kiriage.get();
	}
	else if (rnd == 2)
	{
		return _360Attack.get();
	}
	else {
		return _kick.get();
	}
}
void BossD::AnimationEvent_Attack(){
	AttackCollision* attack = CreateAttack(Vector3(0.0f, 0.25f, 1.5f), Quaternion::Identity, Vector3(2.5f, 5.0f, 2.5f), 0.25f, transform);
	attack->RemoveParent();
}
void BossD::AnimationEvent_Kick(){
	AttackCollision* attack = CreateAttack(Vector3(0.0f, 0.25f, 2.0f), Quaternion::Identity, Vector3(2.5f, 3.0f, 3.0f), 0.25f, transform);
	attack->RemoveParent();
}
void BossD::AnimationEvent_Kiriage(){
	AttackCollision* attack = CreateAttack(Vector3(0.0f, 0.25f, 1.5f), Quaternion::Identity, Vector3(3.0f, 5.0f, 2.0f), 0.25f, transform);
	attack->RemoveParent();
}
void BossD::AnimationEvent_360Attack(){
	AttackCollision* attack = CreateAttack(Vector3(0.0f, 0.25f, 2.0f), Quaternion::Identity, Vector3(5.0f, 3.0f, 2.5f), 0.25f, transform);
	attack->RemoveParent();
}
void BossD::_EndNowStateCallback(State EndStateType) {

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

void BossD::_ConfigCollision() {
	
	// �L�����N�^�[�R���g���[���p�B
	{
		// �R���W�����̃T�C�Y������B
		// ���L�����N�^�[�R���g���[���[�Ŏg�p���邽�߂̂��́B
		_collisionInfo.radius = 0.5f;
		_collisionInfo.height = 1.1f;
		_collisionInfo.offset = Vector3(0.0f, 1.0f, 0.0f);
		_collisionInfo.id = Collision_ID::BOSS;

		// �d�͐ݒ�B
		_Gravity = -9.8f;

		// �R���|�[�l���g�ɃJ�v�Z���R���C�_�[��ǉ��B
		_MyComponent.Collider = AddComponent<CCapsuleCollider>();
		// �J�v�Z���R���C�_�[���쐬�B
		static_cast<CCapsuleCollider*>(_MyComponent.Collider)->Create(_collisionInfo.radius, _collisionInfo.height);

	}
}

void BossD::_ConfigCharacterController() {
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

void BossD::_CreateExtrudeCollision() {
	_MyComponent.ExtrudeCollisions.push_back(_MyComponent.CharacterController->GetRigidBody());	// ���łɉ����o���悤�R���W�����ɒǉ����Ă����B
}

void BossD::_BuildAnimationSubClass(vector<double>& datas) {

	// �A�j���[�V�����^�C�v�Ƀf�[�^���֘A�Â��B
	// ���G�l�~�[�͂��ׂē����X�e�[�g�N���X���g�p���邽�߁A�X�e�[�g����A�j���[�V�������Đ��ł���悤
	//   EnemyCharacter�N���X�Œ�`����Ă��邷�ׂẴG�l�~�[���ʂ̗񋓎q�Ɋ֘A�t����K�v������B
	{
		// �ҋ@��ԁB
		_ConfigAnimationType(EnemyCharacter::AnimationType::Idle, static_cast<unsigned int>(AnimationBossD::Idle));
		// ���s��ԁB
		_ConfigAnimationType(EnemyCharacter::AnimationType::Walk, static_cast<unsigned int>(AnimationBossD::Walk));
		// ���s��ԁB
		_ConfigAnimationType(EnemyCharacter::AnimationType::Dash, static_cast<unsigned int>(AnimationBossD::Run));
		// �i����B
		_ConfigAnimationType(EnemyCharacter::AnimationType::Threat, static_cast<unsigned int>(AnimationBossD::DrawingaSword));
		// �_���[�W�����B
		_ConfigAnimationType(EnemyCharacter::AnimationType::Damage, static_cast<unsigned int>(AnimationBossD::Damage));
		// ������ԁB
		_ConfigAnimationType(EnemyCharacter::AnimationType::Fall, static_cast<unsigned int>(AnimationBossD::Idle));
		// ���S��ԁB
		_ConfigAnimationType(EnemyCharacter::AnimationType::Death, static_cast<unsigned int>(AnimationBossD::Damage));
	}
	datas[static_cast<int>(AnimationBossD::Run)] = 44.0f / 60.0f;
}

void BossD::_ConfigAnimationEvent() {
	//�a��
	{
		float eventFrame = 0.9f;
		_MyComponent.AnimationEventPlayer->AddAnimationEvent(static_cast<int>(AnimationBossD::Attack), eventFrame, static_cast<AnimationEvent>(&BossD::AnimationEvent_Attack));
	}
	//�L�b�N
	{
		float eventFrame = 0.9f;
		_MyComponent.AnimationEventPlayer->AddAnimationEvent(static_cast<int>(AnimationBossD::Kick), eventFrame, static_cast<AnimationEvent>(&BossD::AnimationEvent_Kick));
	}
	//�a��グ
	{
		float eventFrame = 0.9f;
		_MyComponent.AnimationEventPlayer->AddAnimationEvent(static_cast<int>(AnimationBossD::Kiriage), eventFrame, static_cast<AnimationEvent>(&BossD::AnimationEvent_Kiriage));
	}
	//��]�a��
	{
		float eventFrame = 0.9f;
		_MyComponent.AnimationEventPlayer->AddAnimationEvent(static_cast<int>(AnimationBossD::Attack360), eventFrame, static_cast<AnimationEvent>(&BossD::AnimationEvent_360Attack));
	}
}

