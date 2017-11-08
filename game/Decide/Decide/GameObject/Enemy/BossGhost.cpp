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
	EnemyPlaySound(EnemyCharacter::SoundIndex::Attack1);
}

void BossGhost::_AwakeSubClass() {
	// �g�p���郂�f���t�@�C���̃p�X��ݒ�B
	SetFileName("Ghost.X");
}

void BossGhost::_StartSubClass() {

	// ����p�����B
	_ViewAngle = 160.0f;
	_ViewRange = 30.0f;

	// ���s���x�ݒ�B
	_walkSpeed = 2.5f;

	// �p�j�͈͐ݒ�B
	// ���b�菈���B
	_WanderingRange = 20.0f;

	// �ǐՔ͈͐ݒ�B
	_discoveryRange = 30.0f;

	// ����Ɉ�񂭂炢���ނ��ݒ�B
	_damageMotionRandNum = 1;

	//���f���Ƀ��C�g�ݒ�B
	_MyComponent.Model->SetLight(INSTANCE(GameObjectManager)->mainLight);
	_MyComponent.Model->SetModelEffect(ModelEffectE::ALPHA);

	// �U���������`�B
	_comboAttack.reset(new GhostComboAttack(this));
	_comboAttack->Init(13.0f, static_cast<int>(AnimationBossGhost::Attack), 0.2f);
	_breathAttack.reset(new EnemyBreathAttack(this));
	_breathAttack->Init(13.0f, static_cast<int>(AnimationBossGhost::Attack), 0.2f);

	// �����X�e�[�g�Ɉڍs�B
	// ���b�菈���B
	_initState = State::StartAttack;
	_ChangeState(_initState);
}

void BossGhost::_UpdateSubClass() {

	//if (!(_MyComponent.CharacterController->IsOnGround())) {
	//	// �G�l�~�[���n�ʂ��痣��Ă���B
	//	if (_NowStateIdx != State::Fall) {
	//		// ���݂̃X�e�[�g�^�C�v��ۑ��B
	//		_saveState = _NowStateIdx;
	//		// �����X�e�[�g�ɐ؂�ւ��B
	//		_ChangeState(State::Fall);
	//	}
	//}
}

void BossGhost::_LateUpdateSubClass()
{

}


EnemyAttack* BossGhost::_AttackSelectSubClass() {
	// ���v���C���[�ƃG�l�~�[�̈ʒu�֌W�Ƃ��őJ�ڐ挈��H�B

	// ���Ƃ肠�����b�菈���B
	//int rnd = rand() % 3;
	//if (rnd == 0) {
		return _comboAttack.get();
	//}
}

void BossGhost::_EndNowStateCallback(State EndStateType) {

	// �Ƃ肠�����A���ŋ����U�����Ȃ��悤�ɂ���B
	if (EndStateType == static_cast<EnemyCharacter::State>(BossGhostState::BossGhostPairAttack)) {
		_ChangeState(State::StartAttack);
	}
	else if (_isCommand) {
		// ���X�{�X���疽�߂��󂯂��B

		// �����U���J�n�B
		_ChangeState(static_cast<EnemyCharacter::State>(BossGhostState::BossGhostPairAttack));
		_isCommand = false;
	}
	else if (EndStateType == State::Damage) {
		// �U�����󂯂��B
		// �U���J�n�B
		_ChangeState(State::StartAttack);
	}
	else if (EndStateType == State::StartAttack) {
		// ��x�U�����I�������B

		// ������x�U���J�n�B
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
	
	// �R���{�U���B
	{
		eventFrame = 0.5f;
		_MyComponent.AnimationEventPlayer->AddAnimationEvent(static_cast<int>(AnimationBossGhost::Attack), eventFrame, static_cast<AnimationEvent>(&BossGhost::CreateCollision));
	}
}

void BossGhost::_BuildSoundTable() {
	// �U�����o�^�B
	_ConfigSoundData(EnemyCharacter::SoundIndex::Attack1, "Damage_01.wav", false, false);
	_ConfigSoundData(EnemyCharacter::SoundIndex::Attack2, "Buoonn.wav", false, false);
	_ConfigSoundData(EnemyCharacter::SoundIndex::Attack3, "Buoonn.wav", false, false);
}

