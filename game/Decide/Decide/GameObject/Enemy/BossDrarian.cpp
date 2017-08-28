#include"stdafx.h"
#include "BossDrarian.h"
#include "fbEngine\_Object\_Component\_3D\SkinModel.h"
#include "HFSM\EnemyTranslationState.h"
#include "HFSM\EnemyWaitState.h"
#include "fbEngine\CharacterController.h"



BossDrarian::BossDrarian(const char* name) : EnemyCharacter(name)
{

}


BossDrarian::~BossDrarian()
{
}

void BossDrarian::_AwakeSubClass() {
	// �g�p���郂�f���t�@�C���̃p�X��ݒ�B
	SetFileName("DRARIAN.X");

	//�p�����[�^�[�������B
	_MyComponent.Parameter->ParamInit(0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,0);
}

void BossDrarian::_StartSubClass() {

	transform->SetPosition(INSTANCE(GameObjectManager)->FindObject("Player")->transform->GetPosition());
	//�p�����[�^�[�������B
	_MyComponent.Parameter->ParamInit(500, 500, 500, 500, 75, 50, 100, 20, 1, 0, 500, 100);

	// �p�����[�^�[�ݒ�B
	vector<BarColor> Color;
	Color.push_back(BarColor::Blue);
	Color.push_back(BarColor::Green);
	Color.push_back(BarColor::Yellow);
	Color.push_back(BarColor::Red);
	SetParamAll(Color, _MyComponent.Parameter->GetParams());

	// ����p�����B
	_ViewAngle = 100.0f;
	_ViewRange = 30.0f;

	// �U���\�͈͐ݒ�B
	_AttackRange = 1.3f;

	// �p�j�͈͐ݒ�B
	// ���b�菈���B
	_WanderingRange = 130.0f;

	//���f���Ƀ��C�g�ݒ�B
	_MyComponent.Model->SetLight(INSTANCE(GameObjectManager)->mainLight);

	// �U���������`�B
	_singleAttack.reset(new EnemySingleAttack(this));
	_singleAttack->Init(_AnimationData[static_cast<int>(EnemyCharacter::AnimationType::Attack1)].No, 0.2f);
	_tailAttack.reset(new EnemySingleAttack(this));
	_tailAttack->Init(static_cast<int>(AnimationDrarian::TailAttackRight), 0.2f);
	_breathAttack.reset(new EnemyBreathAttack(this));
	_breathAttack->Init(static_cast<int>(AnimationDrarian::Breath), 0.2f);

	// �U�������Ɏg�p����p�[�e�B�N���ݒ�B
	ParticleParameter param;
	param.Init();
	param.texturePath = "test.png";
	param.alphaBlendMode = 1;
	param.addVelocityRandomMargih = Vector3::zero;
	param.brightness = 7.0f;
	param.fadeTime = 0.2f;
	param.gravity = 0.0f;
	param.initAlpha = 1.0f;
	param.initPositionRandomMargin = Vector3::zero;
	param.initVelocity = Vector3::front * 10.0f;
	param.initVelocityVelocityRandomMargin = Vector3::zero;
	param.intervalTime = 0.001f;
	param.isBillboard = true;
	param.isFade = true;
	param.life = 2.0f;
	param.size = Vector2(0.5f, 0.5f);
	param.mulColor = Color::red;
	_breathAttack->ConfigParticleParameter(param);
	_breathAttack->BreathEnd();	// �Ƃ肠�����ŏ��̓p�[�e�B�N���𐶐����Ȃ��悤�ɐݒ�B

	// �����X�e�[�g�Ɉڍs�B
	// ���b�菈���B
	_ChangeState(State::Wait);

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


EnemyAttack* BossDrarian::AttackSelect() {
	// ���v���C���[�ƃG�l�~�[�̈ʒu�֌W�Ƃ��őJ�ڐ挈��H�B

	// ���Ƃ肠�����b�菈���B
	int rnd = rand() % 3;
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
	unsigned int priorty = 1;
	AttackCollision* attack = INSTANCE(GameObjectManager)->AddNew<AttackCollision>("attackCollision", priorty);
	attack->Create(_MyComponent.Parameter->GiveDamageMass(), Vector3(0.0f, 0.25f, 5.0f), Quaternion::Identity, Vector3(1.0f, 2.0f, 2.0f), AttackCollision::CollisionMaster::Enemy, 0.25f, 0.0f, transform);
	attack->RemoveParent();

	// �U�����Đ��B
	EnemyPlaySound(EnemyCharacter::SoundIndex::Attack);
}

void BossDrarian::CreateAttackCollision_TailAttack1() {
	//�U���R���W�����쐬�B
	unsigned int priorty = 1;
	AttackCollision* attack = INSTANCE(GameObjectManager)->AddNew<AttackCollision>("attackCollision", priorty);
	Quaternion rot = Quaternion::Identity;
	rot.SetRotation(Vector3::axisY, D3DXToRadian(-40.0f));
	attack->Create(_MyComponent.Parameter->GiveDamageMass(), Vector3(2.0f, 0.0f, -2.0f), rot, Vector3(2.0f, 2.0f, 5.0f), AttackCollision::CollisionMaster::Enemy, 0.15f, 0.0f, transform);
	attack->RemoveParent();
}

void BossDrarian::CreateAttackCollision_TailAttack2() {
	//�U���R���W�����쐬�B
	unsigned int priorty = 1;
	AttackCollision* attack = INSTANCE(GameObjectManager)->AddNew<AttackCollision>("attackCollision", priorty);
	Quaternion rot = Quaternion::Identity;
	rot.SetRotation(Vector3::axisY, D3DXToRadian(-60.0f));
	attack->Create(_MyComponent.Parameter->GiveDamageMass(), Vector3(3.0f, 0.0f, 0.0f), rot, Vector3(2.0f, 2.0f, 5.0f), AttackCollision::CollisionMaster::Enemy, 0.15f, 0.0f, transform);
	attack->RemoveParent();
}

void BossDrarian::CreateAttackCollision_TailAttack3() {
	//�U���R���W�����쐬�B
	unsigned int priorty = 1;
	AttackCollision* attack = INSTANCE(GameObjectManager)->AddNew<AttackCollision>("attackCollision", priorty);
	attack->Create(_MyComponent.Parameter->GiveDamageMass(), Vector3(4.0f, 0.0f, 2.0f), Quaternion::Identity, Vector3(4.0f, 2.0f, 2.0f), AttackCollision::CollisionMaster::Enemy, 0.15f, 0.0f, transform);
	attack->RemoveParent();
}

void BossDrarian::CreateAttackCollision_TailAttack4() {
	//�U���R���W�����쐬�B
	unsigned int priorty = 1;
	AttackCollision* attack = INSTANCE(GameObjectManager)->AddNew<AttackCollision>("attackCollision", priorty);
	attack->Create(_MyComponent.Parameter->GiveDamageMass(), Vector3(3.0f, 0.0f, 4.5f), Quaternion::Identity, Vector3(2.0f, 2.0f, 3.0f), AttackCollision::CollisionMaster::Enemy, 0.15f, 0.0f, transform);
	attack->RemoveParent();
}

void BossDrarian::AnimationEvent_BreathStart() {
	_breathAttack->BreathStart();
}

void BossDrarian::AnimationEvent_BreathEnd() {
	_breathAttack->BreathEnd();
}


void BossDrarian::_EndNowStateCallback(State EndStateType) {
	if (EndStateType == State::Wait) {
		// �҂��̋�����������I���B

		// �ēx�҂��B
		_ChangeState(State::Wait);
		static_cast<EnemyWaitState*>(_NowState)->SetInterval(5.0f);
	}
	else if (EndStateType == State::Discovery) {
		// �����X�e�[�g�̏��������B

		_ChangeState(State::StartAttack);
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
		// ������ԂɈڍs�B
		_ChangeState(State::Discovery);
	}
	else if (EndStateType == State::Threat) {
		// �Њd�I���B
		// ������ԂɈڍs�B
		_ChangeState(State::Discovery);
	}
}

void BossDrarian::_ConfigCollision() {

	// �R���W�����̃T�C�Y������B
	// ���L�����N�^�[�R���g���[���[�Ŏg�p���邽�߂̂��́B
	_Radius = 0.5f;
	_Height = 7.5f;

	// �R���|�[�l���g�ɃJ�v�Z���R���C�_�[��ǉ��B
	_MyComponent.Collider = AddComponent<CCapsuleCollider>();
	// �J�v�Z���R���C�_�[���쐬�B
	static_cast<CCapsuleCollider*>(_MyComponent.Collider)->Create(_Radius, _Height);
}

void BossDrarian::_ConfigCharacterController() {
	// �Փ˂��鑮����ݒ�(��)�B
	_MyComponent.CharacterController->AttributeXZ_AllOn();
	_MyComponent.CharacterController->SubAttributeXZ(Collision_ID::ATTACK);
	_MyComponent.CharacterController->SubAttributeXZ(Collision_ID::SPACE);
	// �Փ˂��鑮����ݒ�(�c)�B
	_MyComponent.CharacterController->AttributeY_AllOn();
	_MyComponent.CharacterController->SubAttributeY(Collision_ID::ATTACK);
	_MyComponent.CharacterController->SubAttributeY(Collision_ID::ENEMY);
	_MyComponent.CharacterController->SubAttributeY(Collision_ID::PLAYER);
	_MyComponent.CharacterController->SubAttributeY(Collision_ID::SPACE);
}

void BossDrarian::_BuildAnimation() {
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
		_ConfigAnimationType(EnemyCharacter::AnimationType::Idle, *Datas[static_cast<int>(AnimationDrarian::Wait)].get());
		// ���s��ԁB
		_ConfigAnimationType(EnemyCharacter::AnimationType::Walk, *Datas[static_cast<int>(AnimationDrarian::Walk)].get());
		// ���s��ԁB
		_ConfigAnimationType(EnemyCharacter::AnimationType::Dash, *Datas[static_cast<int>(AnimationDrarian::Dash)].get());
		// �i����B
		_ConfigAnimationType(EnemyCharacter::AnimationType::Threat, *Datas[static_cast<int>(AnimationDrarian::Barking)].get());
		// �U�����(���݂�)�B
		_ConfigAnimationType(EnemyCharacter::AnimationType::Attack1, *Datas[static_cast<int>(AnimationDrarian::Attack)].get());
		// �U�����(������)�B
		_ConfigAnimationType(EnemyCharacter::AnimationType::Attack2, *Datas[static_cast<int>(AnimationDrarian::TailAttackRight)].get());
		// �U�����(�u���X)�B
		_ConfigAnimationType(EnemyCharacter::AnimationType::Attack3, *Datas[static_cast<int>(AnimationDrarian::Breath)].get());
		// �_���[�W�����B
		Datas[static_cast<int>(AnimationDrarian::Damage)]->Time = 8.0f / 30.0f;
		_ConfigAnimationType(EnemyCharacter::AnimationType::Damage, *Datas[static_cast<int>(AnimationDrarian::Damage)].get());
		//// ������ԁB
		//// �����̃I�u�W�F�N�g�ɂ͗����̃A�j���[�V�������Ȃ��̂őҋ@�A�j���[�V�����ő�p�B
		//_ConfigAnimationType(EnemyCharacter::AnimationType::Fall, *Datas[static_cast<int>(AnimationProt::Stand)].get());
		// ���S��ԁB
		_ConfigAnimationType(EnemyCharacter::AnimationType::Death, *Datas[static_cast<int>(AnimationDrarian::Death)].get());
	}
}

void BossDrarian::_ConfigAnimationEvent() {
	// ���݂��U���B
	{
		int eventFrame = 15;
		_MyComponent.AnimationEventPlayer->AddAnimationEvent(static_cast<int>(AnimationDrarian::Attack), eventFrame, static_cast<AnimationEvent>(&BossDrarian::AnimationEvent_Kamituki));
	}
	// �����ۍU���B
	{

		int eventFrame = 60;
		_MyComponent.AnimationEventPlayer->AddAnimationEvent(static_cast<int>(AnimationDrarian::TailAttackRight), eventFrame, static_cast<AnimationEvent>(&BossDrarian::CreateAttackCollision_TailAttack1));

		eventFrame = 62;
		_MyComponent.AnimationEventPlayer->AddAnimationEvent(static_cast<int>(AnimationDrarian::TailAttackRight), eventFrame, static_cast<AnimationEvent>(&BossDrarian::CreateAttackCollision_TailAttack2));

		eventFrame = 65;
		_MyComponent.AnimationEventPlayer->AddAnimationEvent(static_cast<int>(AnimationDrarian::TailAttackRight), eventFrame, static_cast<AnimationEvent>(&BossDrarian::CreateAttackCollision_TailAttack3));

		eventFrame = 70;
		_MyComponent.AnimationEventPlayer->AddAnimationEvent(static_cast<int>(AnimationDrarian::TailAttackRight), eventFrame, static_cast<AnimationEvent>(&BossDrarian::CreateAttackCollision_TailAttack4));
	}
	// �u���X�U���B
	{
		int eventFrame = 80.0f;
		_MyComponent.AnimationEventPlayer->AddAnimationEvent(static_cast<int>(AnimationDrarian::Breath), eventFrame, static_cast<AnimationEvent>(&BossDrarian::AnimationEvent_BreathStart));

		eventFrame = 120.0f;
		_MyComponent.AnimationEventPlayer->AddAnimationEvent(static_cast<int>(AnimationDrarian::Breath), eventFrame, static_cast<AnimationEvent>(&BossDrarian::AnimationEvent_BreathEnd));
	}
}

void BossDrarian::_BuildSoundTable() {
	// �U�����o�^�B
	_ConfigSoundData(EnemyCharacter::SoundIndex::Attack, "Damage_01.wav", false, false);
}


// EnemyBreathAttack�B

EnemyBreathAttack::EnemyBreathAttack(EnemyCharacter* object) : EnemyAttack(object){
	_player = INSTANCE(GameObjectManager)->FindObject("Player");
	_particleEmitter = INSTANCE(GameObjectManager)->AddNew<ParticleEmitter>("BreathEmitter", 8);
	_particleEmitter->transform->SetParent(_enemyObject->transform);
	_particleEmitter->transform->SetLocalPosition(Vector3(0.0f,0.0f,5.0f));
}

bool EnemyBreathAttack::Update() {
	if (_particleEmitter->GetEmitFlg()) {

	}
	if (!_isPlaying) {
		// ���[�V�����Đ��I���B
		return true;
	}
	return false;
}