#include "stdafx.h"
#include "EnemyGolem.h"
#include "fbEngine\CharacterController.h"
#include "GameObject\Enemy\EnemyAttack.h"

EnemyGolem::EnemyGolem(const char* name) : EnemyCharacter(name)
{
}


EnemyGolem::~EnemyGolem()
{
}

void EnemyGolem::_AwakeSubClass() {
	SetFileName("enemy_01.X");
}

void EnemyGolem::_StartSubClass() {
	//����p�����B
	_ViewAngle = 100.0f;
	_ViewRange = 20.0f;

	//�p�j�͈͐ݒ�B
	_WanderingRange = 10.0f;

	//�ǐՔ͈͐ݒ�B
	_discoveryRange = 10.0f;

	//���s���x�ݒ�B
	_walkSpeed = 1.5f;

	//���f���Ƀ��C�g�ݒ�B
	_MyComponent.Model->SetLight(INSTANCE(GameObjectManager)->mainLight);

	_MyComponent.Model->SetModelEffect(ModelEffectE::SPECULAR, true);

	// �U���������`�B
	_SingleAttack.reset(new EnemySingleAttack(this));
	_SingleAttack->Init(1.5f, static_cast<int>(EnemyGolemAnim::Attack01), 0.0f,0.5f);

	// �����X�e�[�g�Ɉڍs�B
	_initState = State::Wandering;
	_ChangeState(_initState);
}

void EnemyGolem::CreateAttackCollsion()
{
	//�U���R���W�����쐬�B
	unsigned int priorty = 1;
	AttackCollision* attack = INSTANCE(GameObjectManager)->AddNew<AttackCollision>("attackCollision", priorty);
	attack->Create(_MyComponent.Parameter->GiveDamageMass(false, false), Vector3(0.0f, 0.5f, 1.5f), Quaternion::Identity, Vector3(2.0f,3.0f,1.0f), AttackCollision::CollisionMaster::Enemy, 0.15f, AttackCollision::ReactionType::NotAction, transform);
	attack->RemoveParent();

	//�U�����Đ��B
	EnemyPlaySound(EnemyCharacter::SoundIndex::DamageGolem);
}

void EnemyGolem::_UpdateSubClass() 
{
	if (_MyComponent.CharacterController->IsOnGround()==false) {
		//�����X�e�[�g�ɐ؂�ւ��B
		_ChangeState(State::Fall);
	}
}

EnemyAttack* EnemyGolem::_AttackSelectSubClass() 
{
	// ���v���C���[�ƃG�l�~�[�̈ʒu�֌W�Ƃ��őJ�ڐ挈��H�B

	// ���Ƃ肠�����b�菈���B
	return _SingleAttack.get();
}

void EnemyGolem::_ConfigCollision()
{
	// �R���W�����̃T�C�Y������B
	// ���L�����N�^�[�R���g���[���[�Ŏg�p���邽�߂̂��́B
	_collisionInfo.radius = 0.6f;
	_collisionInfo.height = 0.3f;
	_collisionInfo.offset = Vector3(0.0f, 0.66f, 0.0f);
	_collisionInfo.id = Collision_ID::ENEMY;

	// �R���|�[�l���g�ɃJ�v�Z���R���C�_�[��ǉ��B
	_MyComponent.Collider = AddComponent<CCapsuleCollider>();
	// �J�v�Z���R���C�_�[���쐬�B
	static_cast<CCapsuleCollider*>(_MyComponent.Collider)->Create(_collisionInfo.radius, _collisionInfo.height);
}

void EnemyGolem::_ConfigCharacterController() 
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

void EnemyGolem::_CreateExtrudeCollision() 
{
	_MyComponent.ExtrudeCollisions.push_back(_MyComponent.CharacterController->GetRigidBody());	// �L�����N�^�[�R���g���[���̍��̂����̂܂܎g�p�B
}

void EnemyGolem::_BuildAnimationSubClass(vector<double>& datas)
{

	// �A�j���[�V�����^�C�v�Ƀf�[�^���֘A�Â��B
	// ���G�l�~�[�͂��ׂē����X�e�[�g�N���X���g�p���邽�߁A�X�e�[�g����A�j���[�V�������Đ��ł���悤
	//   EnemyCharacter�N���X�Œ�`����Ă��邷�ׂẴG�l�~�[���ʂ̗񋓎q�Ɋ֘A�t����K�v������B
	{
		// �ҋ@��ԁB
		_ConfigAnimationType(EnemyCharacter::AnimationType::Idle, static_cast<unsigned int>(EnemyGolemAnim::Stand));
		// ���s��ԁB
		_ConfigAnimationType(EnemyCharacter::AnimationType::Walk, static_cast<unsigned int>(EnemyGolemAnim::Walk));
		// ���s��ԁB
		_ConfigAnimationType(EnemyCharacter::AnimationType::Dash, static_cast<unsigned int>(EnemyGolemAnim::Run));
		// ������ԁB
		_ConfigAnimationType(EnemyCharacter::AnimationType::Fall, static_cast<unsigned int>(EnemyGolemAnim::Fall));
		// �_���[�W��ԁB
		_ConfigAnimationType(EnemyCharacter::AnimationType::Damage, static_cast<unsigned int>(EnemyGolemAnim::Damage));
		// ���S��ԁB
		_ConfigAnimationType(EnemyCharacter::AnimationType::Death, static_cast<unsigned int>(EnemyGolemAnim::Death));
	}


}

void EnemyGolem::_ConfigAnimationEvent()
{
	float eventFrame = 0.4f;

	_MyComponent.AnimationEventPlayer->AddAnimationEvent(static_cast<int>(EnemyGolemAnim::Attack01), eventFrame, static_cast<AnimationEvent>(&EnemyGolem::CreateAttackCollsion));
}

void EnemyGolem::_EndNowStateCallback(State EndStateType) {
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