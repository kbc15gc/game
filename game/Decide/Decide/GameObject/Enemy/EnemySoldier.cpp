#include "stdafx.h"
#include "EnemySoldier.h"
#include "fbEngine\CharacterController.h"
#include "GameObject\Enemy\EnemyAttack.h"

EnemySoldier::EnemySoldier(const char* name) : EnemyCharacter(name)
{
}


EnemySoldier::~EnemySoldier()
{
}

void EnemySoldier::_AwakeSubClass() {
	SetFileName("enemy_02.X");
}

void EnemySoldier::_StartSubClass() {

	//�|�W�V����
	//_InitPos = Vector3(-222.0f, 60.0f, -156.0f);
	//transform->SetPosition(_InitPos);

	//����p�����B
	//_ViewAngle = 100.0f;
	//_ViewRange = 20.0f;

	//�p�j�͈͐ݒ�B
	//_WanderingRange = 10.0f;

	//�ǐՔ͈͐ݒ�B
	//_discoveryRange = 10.0f;

	//���s���x�ݒ�B
	_walkSpeed = 1.5f;

	//���f���Ƀ��C�g�ݒ�B
	_MyComponent.Model->SetLight(INSTANCE(GameObjectManager)->mainLight);

	// �U���������`�B
	_SingleAttack.reset(new EnemySingleAttack(this));
	_SingleAttack->Init(1.3f, static_cast<int>(EnemySoldierAnim::Attack01), 0.2f);

	// �����X�e�[�g�Ɉڍs�B
	_initState = State::Wandering;
	_ChangeState(_initState);
}

void EnemySoldier::CreateAttackCollsion()
{
	//�U���R���W�����쐬�B
	unsigned int priorty = 1;
	AttackCollision* attack = INSTANCE(GameObjectManager)->AddNew<AttackCollision>("attackCollision", priorty);
	attack->Create(_MyComponent.Parameter->GiveDamageMass(false, false), Vector3(0.0f, 0.0f, 1.0f), Quaternion::Identity, Vector3(1.0f, 2.0f, 1.0f), AttackCollision::CollisionMaster::Enemy, 0.15f, AttackCollision::ReactionType::NotAction, transform);
	attack->RemoveParent();

	// �U�����Đ��B
	EnemyPlaySound(EnemyCharacter::SoundIndex::Damage);
}

void EnemySoldier::_UpdateSubClass()
{
	if (_MyComponent.CharacterController->IsOnGround() == false) {
		//�����X�e�[�g�ɐ؂�ւ��B
		_ChangeState(State::Fall);
	}
}

EnemyAttack* EnemySoldier::_AttackSelectSubClass()
{
	// ���v���C���[�ƃG�l�~�[�̈ʒu�֌W�Ƃ��őJ�ڐ挈��H�B

	// ���Ƃ肠�����b�菈���B
	return _SingleAttack.get();
}

void EnemySoldier::_ConfigCollision()
{
	// �R���W�����̃T�C�Y������B
	// ���L�����N�^�[�R���g���[���[�Ŏg�p���邽�߂̂��́B
	_collisionInfo.radius = 0.3f;
	_collisionInfo.height = 0.1f;
	_collisionInfo.offset = Vector3(0.0f, -0.5f, 0.0f);
	_collisionInfo.id = Collision_ID::ENEMY;

	// �R���|�[�l���g�ɃJ�v�Z���R���C�_�[��ǉ��B
	_MyComponent.Collider = AddComponent<CCapsuleCollider>();
	// �J�v�Z���R���C�_�[���쐬�B
	static_cast<CCapsuleCollider*>(_MyComponent.Collider)->Create(_collisionInfo.radius, _collisionInfo.height);
}

void EnemySoldier::_ConfigCharacterController()
{
	// �Փ˂��鑮����ݒ�(��)�B
	_MyComponent.CharacterController->AttributeXZ_AllOn();
	_MyComponent.CharacterController->SubAttributeXZ(Collision_ID::ATTACK);
	_MyComponent.CharacterController->SubAttributeXZ(Collision_ID::SPACE);
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

void EnemySoldier::_CreateExtrudeCollision()
{
	_MyComponent.ExtrudeCollisions.push_back(_MyComponent.CharacterController->GetRigidBody());	// �L�����N�^�[�R���g���[���̍��̂����̂܂܎g�p�B
}

void EnemySoldier::_BuildAnimationSubClass(vector<double>& datas)
{
	
	// �A�j���[�V�����^�C�v�Ƀf�[�^���֘A�Â��B
	// ���G�l�~�[�͂��ׂē����X�e�[�g�N���X���g�p���邽�߁A�X�e�[�g����A�j���[�V�������Đ��ł���悤
	//   EnemyCharacter�N���X�Œ�`����Ă��邷�ׂẴG�l�~�[���ʂ̗񋓎q�Ɋ֘A�t����K�v������B
	{
		// �ҋ@��ԁB
		_ConfigAnimationType(EnemyCharacter::AnimationType::Idle, static_cast<unsigned int>(EnemySoldierAnim::Stand));
		// ���s��ԁB
		_ConfigAnimationType(EnemyCharacter::AnimationType::Walk, static_cast<unsigned int>(EnemySoldierAnim::Walk));
		// ���s��ԁB
		datas[static_cast<int>(EnemySoldierAnim::Run)] = 0.5f;
		_ConfigAnimationType(EnemyCharacter::AnimationType::Dash, static_cast<unsigned int>(EnemySoldierAnim::Run));
		// ������ԁB
		//�����郂�[�V�����������̂őҋ@�ő�p�B
		_ConfigAnimationType(EnemyCharacter::AnimationType::Fall, static_cast<unsigned int>(EnemySoldierAnim::Stand));
		// �_���[�W��ԁB
		_ConfigAnimationType(EnemyCharacter::AnimationType::Damage, static_cast<unsigned int>(EnemySoldierAnim::Damage));
		// ���S��ԁB
		_ConfigAnimationType(EnemyCharacter::AnimationType::Death, static_cast<unsigned int>(EnemySoldierAnim::Death));
	}
	
}

void EnemySoldier::_ConfigAnimationEvent()
{
	float eventFrame = 0.2f;

	_MyComponent.AnimationEventPlayer->AddAnimationEvent(static_cast<int>(EnemySoldierAnim::Attack01), eventFrame, static_cast<AnimationEvent>(&EnemySoldier::CreateAttackCollsion));
}

void EnemySoldier::_EndNowStateCallback(State EndStateType) {
	if (EndStateType == State::Wandering) {
		// �p�j�̋�����������I���B

		// �ēx�p�j�B
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
		_ChangeState(State::Wandering);
	}
	else if (EndStateType == State::Damage) {
		// �U�����󂯂��B
		// �U���J�n�B
		_ChangeState(State::StartAttack);
	}
	else if (EndStateType == State::Threat) {
		//�Њd�I���B
		//�U���J�n�B
		_ChangeState(State::StartAttack);
	}
}