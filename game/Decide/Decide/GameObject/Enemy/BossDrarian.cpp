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
	_MyComponent.Parameter->ParamInit(0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0);
}

void BossDrarian::_StartSubClass() {

	transform->SetPosition(INSTANCE(GameObjectManager)->FindObject("Player")->transform->GetPosition());
	//�p�����[�^�[�������B
	_MyComponent.Parameter->ParamInit(500, 500, 500, 500, 75, 50, 100, 20,1,0,500);

	// �p�����[�^�[�ݒ�B
	vector<BarColor> Color;
	Color.push_back(BarColor::Blue);
	Color.push_back(BarColor::Green);
	Color.push_back(BarColor::Yellow);
	Color.push_back(BarColor::Red);
	SetParamAll(Color, _MyComponent.Parameter->GetParams());

	// ����p�����B
	_ViewAngle = 140.0f;
	_ViewRange = 20.0f;

	// �U���\�͈͐ݒ�B
	_AttackRange = 1.3f;

	// �p�j�͈͐ݒ�B
	// ���b�菈���B
	_WanderingRange = 130.0f;

	//���f���Ƀ��C�g�ݒ�B
	_MyComponent.Model->SetLight(INSTANCE(GameObjectManager)->mainLight);


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


EnemyCharacter::State BossDrarian::AttackSelect() {
	// ���v���C���[�ƃG�l�~�[�̈ʒu�֌W�Ƃ��őJ�ڐ挈��H�B

	// ���Ƃ肠�����b�菈���B
	return State::Attack;
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

		// �v���C���[�Ƃ̈ʒu�֌W�Ē����B
		_ChangeState(State::Discovery);
	}
	else if (EndStateType == State::Fall) {
		// �����X�e�[�g�I���B

		// ���O�̃X�e�[�g�ɐ؂�ւ��B
		_ChangeState(_saveState);
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
	//	// ���s��ԁB
	//	_ConfigAnimationType(EnemyCharacter::AnimationType::Walk, *Datas[static_cast<int>(AnimationProt::Walk)].get());
	//	// ���s��ԁB
	//	// �����̃I�u�W�F�N�g�ɂ̓_�b�V���̃A�j���[�V�������Ȃ��̂ŕ����A�j���[�V�����ő�p�B
	//	_ConfigAnimationType(EnemyCharacter::AnimationType::Dash, *Datas[static_cast<int>(AnimationProt::Walk)].get());
	//	// �U����ԁB
	//	_ConfigAnimationType(EnemyCharacter::AnimationType::Attack, *Datas[static_cast<int>(AnimationProt::Attack)].get());
	//	// ������ԁB
	//	// �����̃I�u�W�F�N�g�ɂ͗����̃A�j���[�V�������Ȃ��̂őҋ@�A�j���[�V�����ő�p�B
	//	_ConfigAnimationType(EnemyCharacter::AnimationType::Fall, *Datas[static_cast<int>(AnimationProt::Stand)].get());
	//	// ���S��ԁB
	//	_ConfigAnimationType(EnemyCharacter::AnimationType::Death, *Datas[static_cast<int>(AnimationProt::Death)].get());
	}
}

void BossDrarian::_ConfigAnimationEvent() {
	//int eventFrame = 30;

	//// �U���A�j���[�V�����ɃR���W���������C�x���g�ǉ��B
	//AnimationEvent::AttackEventInfo info(transform, true);
	//info.damage = _MyComponent.Parameter->GiveDamageMass();
	//info.master = AttackCollision::CollisionMaster::Enemy;
	//info.pos = Vector3(1.5f, 0.5f, 0.0f);
	//info.rot = Quaternion::Identity;
	//info.size = Vector3::one;
	//info.life = 0.25f;
	//_MyComponent.AnimationEvent->AddAnimationEvent(static_cast<int>(AnimationDrarian::Attack), eventFrame, info);
}
