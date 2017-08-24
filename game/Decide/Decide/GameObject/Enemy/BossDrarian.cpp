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
	_MyComponent.Parameter->ParamInit(0, 0, 0, 0, 0, 0, 0, 0);
}

void BossDrarian::_StartSubClass() {

	Vector3 work = INSTANCE(GameObjectManager)->FindObject("Player")->transform->GetPosition();
	work.y += 3.0f;
	transform->SetPosition(work);
	//�p�����[�^�[�������B
	_MyComponent.Parameter->ParamInit(100, 100, 0, 0, 20, 0, 100, 20);

	// �p�����[�^�[�ݒ�B
	vector<BarColor> Color;
	Color.push_back(BarColor::Yellow);
	Color.push_back(BarColor::Red);
	SetParamAll(Color, _MyComponent.Parameter->GetParams());

	// ����p�����B
	_ViewAngle = 100.0f;
	_ViewRange = 30.0f;

	// �U���\�͈͐ݒ�B
	_AttackRange = 5.0f;

	// �p�j�͈͐ݒ�B
	// ���b�菈���B
	_WanderingRange = 130.0f;

	// ���s���x�ݒ�B
	_walkSpeed = 2.0f;

	//���f���Ƀ��C�g�ݒ�B
	_MyComponent.Model->SetLight(INSTANCE(GameObjectManager)->mainLight);

	// �U���������`�B
	_singleAttack.Init(_AnimationData[static_cast<int>(EnemyCharacter::AnimationType::Attack1)].No, 0.2f);
	_tailAttack.Init(static_cast<int>(AnimationDrarian::TailAttackRight), 0.2f);


	// �����X�e�[�g�Ɉڍs�B
	// ���b�菈���B
	_ChangeState(State::Wandering);

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
	return &_singleAttack;
	//return &_tailAttack;
}

void BossDrarian::CreateAttackCollision_Kamituki() {
	//�U���R���W�����쐬�B
	unsigned int priorty = 1;
	AttackCollision* attack = INSTANCE(GameObjectManager)->AddNew<AttackCollision>("attackCollision", priorty);
	attack->Create(_MyComponent.Parameter->GiveDamageMass(), Vector3(0.0f, 0.25f, 5.0f), Quaternion::Identity, Vector3(1.0f, 2.0f, 2.0f), AttackCollision::CollisionMaster::Enemy, 0.25f, 0.0f, transform);
	attack->RemoveParent();
}

void BossDrarian::_EndNowStateCallback(State EndStateType) {
	if (EndStateType == State::Wandering) {
		// �p�j������������I���B

		// �ēx�p�j�B
		_ChangeState(State::Wandering);
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
	// �R���|�[�l���g�ɃJ�v�Z���R���C�_�[��ǉ��B
	_MyComponent.Collider = AddComponent<CCapsuleCollider>();
	// �J�v�Z���R���C�_�[���쐬�B
	// �R���W�����̃T�C�Y������B
	_Radius = 0.5f;
	_Height = 2.4f;
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
		////�U���R���W�����쐬�B
		//unsigned int priorty = 1;
		//AttackCollision* attack = INSTANCE(GameObjectManager)->AddNew<AttackCollision>("attackCollision", priorty);
		//attack->Create(_MyComponent.Parameter->GiveDamageMass(), Vector3(0.0f, 0.25f, 5.0f), Quaternion::Identity, Vector3(1.0f, 2.0f, 2.0f), AttackCollision::CollisionMaster::Enemy, 0.25f, 0.0f, transform);
		//attack->RemoveParent(); 

		int eventFrame = 15;
		AnimationEvent p = static_cast<AnimationEvent>(&BossDrarian::CreateAttackCollision_Kamituki);
		_MyComponent.AnimationEventPlayer->AddAnimationEvent(static_cast<int>(AnimationDrarian::Attack), eventFrame, this,p);

		//	// �U���A�j���[�V�����ɃR���W���������C�x���g�ǉ��B
	//	AnimationEvent::AttackEventInfo info(transform, true);
	//	info.damage = _MyComponent.Parameter->GiveDamageMass();
	//	info.master = AttackCollision::CollisionMaster::Enemy;
	//	info.pos = Vector3(0.0f, 0.25f, 5.0f);
	//	info.rot = Quaternion::Identity;
	//	//info.rot.SetRotation(Vector3::axisY, D3DXToRadian(40.0f));
	//	info.size = Vector3(1.0f, 2.0f, 2.0f);
	//	info.life = 0.25f;
	//	_MyComponent.AnimationEvent->AddAnimationEvent(static_cast<int>(AnimationDrarian::Attack), eventFrame, info);
	//}
	//// �����ۍU���B
	//{

	//	int eventFrame = 60;
	//	// �U���A�j���[�V�����ɃR���W���������C�x���g�ǉ��B
	//	AnimationEvent::AttackEventInfo info(transform, true);
	//	info.damage = _MyComponent.Parameter->GiveDamageMass();
	//	info.master = AttackCollision::CollisionMaster::Enemy;
	//	info.pos = Vector3(2.0f, 0.0f, -2.0f);
	//	info.rot = Quaternion::Identity;
	//	info.rot.SetRotation(Vector3::axisY, D3DXToRadian(-40.0f));
	//	info.size = Vector3(2.0f, 2.0f, 5.0f);
	//	info.life = 0.15f;
	//	_MyComponent.AnimationEvent->AddAnimationEvent(static_cast<int>(AnimationDrarian::TailAttackRight), eventFrame, info);

	//	eventFrame = 62;
	//	// �U���A�j���[�V�����ɃR���W���������C�x���g�ǉ��B
	//	AnimationEvent::AttackEventInfo info2(transform, true);
	//	info2.damage = _MyComponent.Parameter->GiveDamageMass();
	//	info2.master = AttackCollision::CollisionMaster::Enemy;
	//	info2.pos = Vector3(3.0f, 0.0f, 0.0f);
	//	info2.rot = Quaternion::Identity;
	//	info2.rot.SetRotation(Vector3::axisY, D3DXToRadian(-60.0f));
	//	info2.size = Vector3(2.0f, 2.0f, 5.0f);
	//	info2.life = 0.15f;
	//	_MyComponent.AnimationEvent->AddAnimationEvent(static_cast<int>(AnimationDrarian::TailAttackRight), eventFrame, info2);

	//	eventFrame = 65;
	//	// �U���A�j���[�V�����ɃR���W���������C�x���g�ǉ��B
	//	AnimationEvent::AttackEventInfo info3(transform, true);
	//	info3.damage = _MyComponent.Parameter->GiveDamageMass();
	//	info3.master = AttackCollision::CollisionMaster::Enemy;
	//	info3.pos = Vector3(4.0f, 0.0f, 2.0f);
	//	info3.rot = Quaternion::Identity;
	//	//info.rot.SetRotation(Vector3::axisY, D3DXToRadian(40.0f));
	//	info3.size = Vector3(4.0f, 2.0f, 2.0f);
	//	info3.life = 0.15f;
	//	_MyComponent.AnimationEvent->AddAnimationEvent(static_cast<int>(AnimationDrarian::TailAttackRight), eventFrame, info3);

	//	eventFrame = 70;
	//	// �U���A�j���[�V�����ɃR���W���������C�x���g�ǉ��B
	//	AnimationEvent::AttackEventInfo info4(transform, true);
	//	info4.damage = _MyComponent.Parameter->GiveDamageMass();
	//	info4.master = AttackCollision::CollisionMaster::Enemy;
	//	info4.pos = Vector3(3.0f, 0.0f, 4.5f);
	//	info4.rot = Quaternion::Identity;
	//	//info.rot.SetRotation(Vector3::axisY, D3DXToRadian(40.0f));
	//	info4.size = Vector3(2.0f, 2.0f, 3.0f);
	//	info4.life = 0.15f;
	//	_MyComponent.AnimationEvent->AddAnimationEvent(static_cast<int>(AnimationDrarian::TailAttackRight), eventFrame, info4);

	}
}

void BossDrarian::_BuildSoundTable() {
	// �U�����o�^�B
	_ConfigSoundData(EnemyCharacter::SoundIndex::Attack, "Damage_01.wav", false, false);
}


// EnemyBreathAttack�B

bool EnemyBreathAttack::Update() {
	return true;
}
