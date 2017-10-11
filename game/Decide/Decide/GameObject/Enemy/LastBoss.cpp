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


//EnemySingleAttack Enemy::_singleAttack = EnemySingleAttack(_AnimationData[EnemyCharacter::AnimationType::Attack],);

LastBoss::LastBoss(const char* name) : EnemyCharacter(name)
{

}


LastBoss::~LastBoss()
{
}

void LastBoss::CreateAttackCollision() {
	//�U���R���W�����쐬�B
	unsigned int priorty = 1;
	AttackCollision* attack = INSTANCE(GameObjectManager)->AddNew<AttackCollision>("attackCollision", priorty);
	attack->Create(_MyComponent.Parameter->GiveDamageMass(false, false), Vector3(0.0f, 0.5f, 1.5f), Quaternion::Identity, Vector3::one, AttackCollision::CollisionMaster::Enemy, 0.15f, 0.0f, transform);
	attack->RemoveParent();

	// �U�����Đ��B
	EnemyPlaySound(EnemyCharacter::SoundIndex::Attack1);
}

void LastBoss::_AwakeSubClass() {
	// �g�p���郂�f���t�@�C���̃p�X��ݒ�B
	SetFileName("LastBoss.X");
}

void LastBoss::_StartSubClass() {

	//�|�W�V����
	_InitPos = Vector3(-202.0f, 60.0f, -156.0f);
	transform->SetPosition(_InitPos);

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

	//���f���Ƀ��C�g�ݒ�B
	_MyComponent.Model->SetLight(INSTANCE(GameObjectManager)->mainLight);

	// �U���������`�B
	_singleAttack.reset(new EnemySingleAttack(this));
	_singleAttack->Init(2.0f,_AnimationData[static_cast<int>(EnemyCharacter::AnimationType::Attack1)].No, 0.2f);

	// �����X�e�[�g�Ɉڍs�B
	// ���b�菈���B
	_initState = State::Speak;
	_ChangeState(_initState);
}

void LastBoss::_UpdateSubClass() {
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

void LastBoss::_LateUpdateSubClass()
{

}


EnemyAttack* LastBoss::_AttackSelectSubClass() {
	// ���v���C���[�ƃG�l�~�[�̈ʒu�֌W�Ƃ��őJ�ڐ挈��H�B

	// ���Ƃ肠�����b�菈���B
	return _singleAttack.get();
}

void LastBoss::_EndNowStateCallback(State EndStateType) {
	if (EndStateType == State::Speak) {
		// ��b�I���B
		
		// �ʍ��X�e�[�g�Ɉڍs�B
		_ChangeState(static_cast<State>(LastBossState::LastBossThrone));
	}
	else if (static_cast<LastBossState>(EndStateType) == LastBossState::LastBossThrone) {
		// �ʍ��X�e�[�g�I���B

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
}

void LastBoss::_ConfigCollision() {

	// �R���W�����̃T�C�Y������B
	// ���L�����N�^�[�R���g���[���[�Ŏg�p���邽�߂̂��́B
	_collisionInfo.radius = 0.5f;
	_collisionInfo.height = 3.6f;
	_collisionInfo.offset = Vector3(0.0f, 0.46f, 0.0f);
	_collisionInfo.id = Collision_ID::ENEMY;

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
	_MyComponent.CharacterController->SubAttributeXZ(Collision_ID::PLAYER);	// �v���C���[�͉����o���̂ŉ����߂���Ȃ��悤�ɂ���B
	// �Փ˂��鑮����ݒ�(�c)�B
	_MyComponent.CharacterController->AttributeY_AllOn();
	_MyComponent.CharacterController->SubAttributeY(Collision_ID::ATTACK);
	_MyComponent.CharacterController->SubAttributeY(Collision_ID::ENEMY);
	_MyComponent.CharacterController->SubAttributeY(Collision_ID::PLAYER);
	_MyComponent.CharacterController->SubAttributeY(Collision_ID::SPACE);
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

void LastBoss::_BuildAnimation() {
	vector<unique_ptr<AnimationData>> Datas;
	for (int idx = 0; idx < _MyComponent.Animation->GetNumAnimationSet(); idx++) {
		// �A�j���[�V�����Z�b�g�̔ԍ��ƍĐ����Ԃ��Z�b�g�ɂ����f�[�^���쐬�B
		unique_ptr<AnimationData> data(new AnimationData);
		data->No = idx;
		data->Time = -1.0f;	// ���ׂ�1�b�ȏ�̃A�j���[�V�����Ȃ̂ŁA���Ԃ͐ݒ肵�Ȃ��B
							// �z��ɒǉ��B
		Datas.push_back(move(data));
	}

	//// �A�j���[�V�����^�C�v�Ƀf�[�^���֘A�Â��B
	//// ���G�l�~�[�͂��ׂē����X�e�[�g�N���X���g�p���邽�߁A�X�e�[�g����A�j���[�V�������Đ��ł���悤
	////   EnemyCharacter�N���X�Œ�`����Ă��邷�ׂẴG�l�~�[���ʂ̗񋓎q�Ɋ֘A�t����K�v������B
	//{
	//	// �ҋ@��ԁB
	//	_ConfigAnimationType(EnemyCharacter::AnimationType::Idle, *Datas[static_cast<int>(AnimationProt::Stand)].get());
	//	// ���s��ԁB
	//	_ConfigAnimationType(EnemyCharacter::AnimationType::Walk, *Datas[static_cast<int>(AnimationProt::Walk)].get());
	//	// ���s��ԁB
	//	// �����̃I�u�W�F�N�g�ɂ̓_�b�V���̃A�j���[�V�������Ȃ��̂ŕ����A�j���[�V�����ő�p�B
	//	_ConfigAnimationType(EnemyCharacter::AnimationType::Dash, *Datas[static_cast<int>(AnimationProt::Walk)].get());
	//	// �U����ԁB
	//	_ConfigAnimationType(EnemyCharacter::AnimationType::Attack1, *Datas[static_cast<int>(AnimationProt::Attack)].get());
	//	// ������ԁB
	//	// �����̃I�u�W�F�N�g�ɂ͗����̃A�j���[�V�������Ȃ��̂őҋ@�A�j���[�V�����ő�p�B
	//	_ConfigAnimationType(EnemyCharacter::AnimationType::Fall, *Datas[static_cast<int>(AnimationProt::Stand)].get());
	//	// �_���[�W��ԁB
	//	_ConfigAnimationType(EnemyCharacter::AnimationType::Damage, *Datas[static_cast<int>(AnimationProt::Damage)].get());
	//	// ���S��ԁB
	//	_ConfigAnimationType(EnemyCharacter::AnimationType::Death, *Datas[static_cast<int>(AnimationProt::Death)].get());
	//}
}

void LastBoss::_ConfigAnimationEvent() {
	//float eventFrame = 1.0f;

	//_MyComponent.AnimationEventPlayer->AddAnimationEvent(_AnimationData[static_cast<int>(EnemyCharacter::AnimationType::Attack1)].No, eventFrame, static_cast<AnimationEvent>(&LastBoss::CreateAttackCollision));
}

void LastBoss::_BuildSoundTable() {
	// �U�����o�^�B
	_ConfigSoundData(EnemyCharacter::SoundIndex::Attack1, "Damage_01.wav", false, false);
}
