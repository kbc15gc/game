#include"stdafx.h"
#include "Enemy.h"
#include "fbEngine\_Object\_Component\_3D\SkinModel.h"
#include "HFSM\EnemyTranslationState.h"
#include "HFSM\EnemyWaitState.h"
#include "fbEngine\CharacterController.h"


//EnemySingleAttack Enemy::_singleAttack = EnemySingleAttack(_AnimationData[EnemyCharacter::AnimationType::Attack],);

Enemy::Enemy(const char* name) : EnemyCharacter(name)
{

}


Enemy::~Enemy()
{
}

void Enemy::CreateAttackCollision() {
	//�U���R���W�����쐬�B
	unsigned int priorty = 1;
	AttackCollision* attack = INSTANCE(GameObjectManager)->AddNew<AttackCollision>("attackCollision", priorty);
	attack->Create(_MyComponent.Parameter->GiveDamageMass(false, false), Vector3(0.0f, 0.5f, 1.5f), Quaternion::Identity, Vector3::one, AttackCollision::CollisionMaster::Enemy, 0.15f, 0.0f, transform);
	attack->RemoveParent();

	// �U�����Đ��B
	EnemyPlaySound(EnemyCharacter::SoundIndex::Attack1);
}

void Enemy::_AwakeSubClass() {
	// �g�p���郂�f���t�@�C���̃p�X��ݒ�B
	SetFileName("enemy_00.X");
}

void Enemy::_StartSubClass(){
	// ����p�����B
	_ViewAngle = 90.0f;
	_ViewRange = 10.0f;

	// �U���\�͈͐ݒ�B
	_AttackRange = 1.3f;

	// �p�j�͈͐ݒ�B
	// ���b�菈���B
	_WanderingRange = 10.0f;

	// �ǐՔ͈͐ݒ�B
	_discoveryRange = 10.0f;

	// ���s���x�ݒ�B
	_walkSpeed = 1.0f;

	//���f���Ƀ��C�g�ݒ�B
	_MyComponent.Model->SetLight(INSTANCE(GameObjectManager)->mainLight);

	// �U���������`�B
	_singleAttack.reset(new EnemySingleAttack(this));
	_singleAttack->Init(_AnimationData[static_cast<int>(EnemyCharacter::AnimationType::Attack1)].No,0.2f);

	// �����X�e�[�g�Ɉڍs�B
	// ���b�菈���B
	_ChangeState(State::Wandering);

}

void Enemy::_UpdateSubClass() {
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

void Enemy::_LateUpdateSubClass()
{
	
}


EnemyAttack* Enemy::AttackSelect() {
	// ���v���C���[�ƃG�l�~�[�̈ʒu�֌W�Ƃ��őJ�ڐ挈��H�B

	// ���Ƃ肠�����b�菈���B
	return _singleAttack.get();
}

void Enemy::_EndNowStateCallback(State EndStateType) {
	if (EndStateType == State::Wandering) {
		// �p�j�̋�����������I���B

		// �ēx�p�j�B
		_ChangeState(State::Wandering);
	}
	else if (EndStateType == State::Discovery) {
		// �����X�e�[�g�̏��������B

		_ChangeState(State::StartAttack);
	}
	else if (EndStateType == State::StartAttack) {
		// ��x�U�����I�������B

		// �v���C���[�Ƃ̈ʒu�֌W�Ē����B
		_ChangeState(State::Discovery);
	}
	else if (EndStateType == State::Fall) {
		// �����X�e�[�g�I���B
		
		// ���O�̃X�e�[�g�ɐ؂�ւ��B
		_ChangeState(_saveState);
	}
	else if (EndStateType == State::Damage) {
		// �U�����󂯂��B
		// ������ԂɈڍs�B
		_ChangeState(State::Discovery);
	}
	else if (EndStateType == State::Threat) {
		// �Њd�I���B
		// ������ԂɈڍs�B
		_ChangeState(State::Discovery);
	}

}

void Enemy::_ConfigCollision() {

	// �R���W�����̃T�C�Y������B
	// ���L�����N�^�[�R���g���[���[�Ŏg�p���邽�߂̂��́B
	_collisionInfo.radius = 0.325f;
	_collisionInfo.height = 0.3f;
	_collisionInfo.offset = Vector3(0.0f, 0.46f, 0.0f);

	// �R���|�[�l���g�ɃJ�v�Z���R���C�_�[��ǉ��B
	_MyComponent.Collider = AddComponent<CCapsuleCollider>();
	// �J�v�Z���R���C�_�[���쐬�B
	static_cast<CCapsuleCollider*>(_MyComponent.Collider)->Create(_collisionInfo.radius, _collisionInfo.height);
}

void Enemy::_ConfigCharacterController() {
	// �Փ˂��鑮����ݒ�(��)�B
	_MyComponent.CharacterController->AttributeXZ_AllOn();
	_MyComponent.CharacterController->SubAttributeXZ(Collision_ID::ATTACK);
	_MyComponent.CharacterController->SubAttributeXZ(Collision_ID::SPACE);
	_MyComponent.CharacterController->SubAttributeXZ(Collision_ID::PLAYER);
	//_MyComponent.CharacterController->SubAttributeXZ(Collision_ID::BUILDING);
	// �Փ˂��鑮����ݒ�(�c)�B
	_MyComponent.CharacterController->AttributeY_AllOn();
	_MyComponent.CharacterController->SubAttributeY(Collision_ID::ATTACK);
	_MyComponent.CharacterController->SubAttributeY(Collision_ID::ENEMY);
	_MyComponent.CharacterController->SubAttributeY(Collision_ID::PLAYER);
	_MyComponent.CharacterController->SubAttributeY(Collision_ID::SPACE);
}

void Enemy::_BuildAnimation() {
	vector<unique_ptr<AnimationData>> Datas;
	for (int idx = 0; idx < _MyComponent.Animation->GetNumAnimationSet(); idx++) {
		// �A�j���[�V�����Z�b�g�̔ԍ��ƍĐ����Ԃ��Z�b�g�ɂ����f�[�^���쐬�B
		unique_ptr<AnimationData> data(new AnimationData);
		data->No = idx;
		data->Time = -1.0f;	// ���ׂ�1�b�ȏ�̃A�j���[�V�����Ȃ̂ŁA���Ԃ͐ݒ肵�Ȃ��B
		// �z��ɒǉ��B
		Datas.push_back(move(data));
	}

	// �A�j���[�V�����^�C�v�Ƀf�[�^���֘A�Â��B
	// ���G�l�~�[�͂��ׂē����X�e�[�g�N���X���g�p���邽�߁A�X�e�[�g����A�j���[�V�������Đ��ł���悤
	//   EnemyCharacter�N���X�Œ�`����Ă��邷�ׂẴG�l�~�[���ʂ̗񋓎q�Ɋ֘A�t����K�v������B
	{
		// �ҋ@��ԁB
		_ConfigAnimationType(EnemyCharacter::AnimationType::Idle, *Datas[static_cast<int>(AnimationProt::Stand)].get());
		// ���s��ԁB
		_ConfigAnimationType(EnemyCharacter::AnimationType::Walk, *Datas[static_cast<int>(AnimationProt::Walk)].get());
		// ���s��ԁB
		// �����̃I�u�W�F�N�g�ɂ̓_�b�V���̃A�j���[�V�������Ȃ��̂ŕ����A�j���[�V�����ő�p�B
		_ConfigAnimationType(EnemyCharacter::AnimationType::Dash, *Datas[static_cast<int>(AnimationProt::Walk)].get());
		// �U����ԁB
		_ConfigAnimationType(EnemyCharacter::AnimationType::Attack1, *Datas[static_cast<int>(AnimationProt::Attack)].get());
		// ������ԁB
		// �����̃I�u�W�F�N�g�ɂ͗����̃A�j���[�V�������Ȃ��̂őҋ@�A�j���[�V�����ő�p�B
		_ConfigAnimationType(EnemyCharacter::AnimationType::Fall, *Datas[static_cast<int>(AnimationProt::Stand)].get());
		// �_���[�W��ԁB
		_ConfigAnimationType(EnemyCharacter::AnimationType::Damage, *Datas[static_cast<int>(AnimationProt::Damage)].get());
		// ���S��ԁB
		_ConfigAnimationType(EnemyCharacter::AnimationType::Death, *Datas[static_cast<int>(AnimationProt::Death)].get());
	}
}

void Enemy::_ConfigAnimationEvent() {
	int eventFrame = 30;
	
	_MyComponent.AnimationEventPlayer->AddAnimationEvent(_AnimationData[static_cast<int>(EnemyCharacter::AnimationType::Attack1)].No, eventFrame, static_cast<AnimationEvent>(&Enemy::CreateAttackCollision));
}

void Enemy::_BuildSoundTable() {
	// �U�����o�^�B
	_ConfigSoundData(EnemyCharacter::SoundIndex::Attack1,"Damage_01.wav",false,false);
}
