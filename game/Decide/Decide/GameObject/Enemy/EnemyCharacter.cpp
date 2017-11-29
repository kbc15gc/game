#include"stdafx.h"
#include "EnemyCharacter.h"
#include "HFSM\EnemyState.h"
#include "fbEngine\_Object\_Component\_3D\SkinModel.h"
#include "HFSM\EnemyTranslationState.h"
#include "HFSM\EnemyWanderingState.h"
#include "HFSM\EnemyWaitState.h"
#include "HFSM\EnemyDiscoveryState.h"
#include "HFSM\EnemyAttackState.h"
#include "HFSM\EnemyStartAttackState.h"
#include "HFSM\EnemyFallState.h"
#include "HFSM\EnemyDeathState.h"
#include "HFSM\EnemyDamageReactionState.h"
#include "HFSM\EnemyThreatState.h"
#include "HFSM\EnemySpeakState.h"
#include "HFSM\EnemyChaceState.h"
#include "HFSM\LastBossThroneState.h"
#include "HFSM\LastBossMagicianState.h"
#include "HFSM\LastBossHistoryTamperingState.h"
#include "HFSM\LastBossDownState.h"
#include "HFSM\EnemyBackStepState.h"
#include "fbEngine\_Object\_GameObject\SoundSource.h"
#include "GameObject\Component\ParticleEffect.h"
#include "GameObject\Component\BuffDebuffICon.h"


EnemyCharacter::NearEnemyInfo EnemyCharacter::nearEnemyInfo = NearEnemyInfo(FLT_MAX,nullptr);

EnemyCharacter::EnemyCharacter(const char* name) :GameObject(name)
{

}

EnemyCharacter::~EnemyCharacter()
{

}


void EnemyCharacter::Awake() {
	_Type = vector<vector<int>>(static_cast<int>(Item::ItemCodeE::Max), vector<int>(LoadEnemyInfo::dropMax, -1));

	// ���̃N���X�Ŏg�p����R���|�[�l���g��ǉ��B
	// �����L�̊֐����p����̃N���X�ŏ㏑�����Ă���ꍇ�͂����炪�Ă΂��B
	_BuildMyComponents();

	// �ʒu��񏉊����B
	transform->SetPosition(Vector3(0.0f,0.0f,0.0f));
	
	// �p����ɂ��ς�鏈���B
	_AwakeSubClass();
}

void EnemyCharacter::Start() {
	// �g�p����X�e�[�g��񋓁B
	_BuildState();
	// ���̐����B
	_BuildCollision();
	// ���f�������B
	_BuildModelData();
	// �A�j���[�V�����e�[�u���쐬�B
	_BuildAnimation();
	// �A�j���[�V�����C�x���g�ݒ�B
	_ConfigAnimationEvent();
	// �T�E���h�e�[�u���쐬�B
	_BuildSoundTable();

	_MoveSpeed = Vector3::zero;	// �������B
	
	// �p����ɂ��ς�鏈���B
	_StartSubClass();

	// �����ʒu�ݒ�B
	_InitPos = transform->GetPosition();

	// �p����ŏ����ʒu���ݒ肳�ꂽ�\�������邽�ߍX�V�B
	_MyComponent.CharacterController->Execute();
	//_MyComponent.CharacterController->AddRigidBody();	// ���[���h�ɓo�^�����u�ԂɃo�E���f�B���O�{�b�N�X����������邽�߁A�������ݒ�̂��߂����œo�^�B
	
	//�v���C���[�B
	_Player = (Player*)INSTANCE(GameObjectManager)->FindObject("Player");

}

void EnemyCharacter::Update() {

	if (_MyComponent.Parameter->GetDeathFlg())
	{
		if (_NowStateIdx != State::Death) {
			_ChangeState(State::Death);
			static_cast<EnemyDeathState*>(_NowState)->SetWaitTime(2.0f);
		}
	}
	else {
		// ����łȂ��B

		float work = Vector3(_Player->transform->GetPosition() - transform->GetPosition()).Length();
		if (work < nearEnemyInfo.length) {
			// �v���C���[�Ƃ̋������ŒZ�����������B
			nearEnemyInfo.length = work;
			nearEnemyInfo.object = this;
		}
	}

	_BarRenderUpdate();

	// �p����ɂ��ς�鏈���B
	_UpdateSubClass();

	// �G�l�~�[�̃G�t�F�N�g�X�V�B
	EffectUpdate();

	if (_NowState) {
		// ���݂̃X�e�[�g���X�V�B
		if (_NowState->Update()) {
			// �X�e�[�g�����I���B

			EnemyCharacter::State work = _NowStateIdx;	// �I�������X�e�[�g��ۑ��B
			// �Ƃ肠�����X�e�[�g��؂�ւ��ĕK���I���������ĂԁB
			_ChangeState(EnemyCharacter::State::None);

			// �X�e�[�g���I���������Ƃ�ʒm�B
			_EndNowStateCallback(work);
		}
	}
	else {
		// �������Ȃ��X�e�[�g�ɓ����Ă���̂ŏ�ɃX�e�[�g�I���R�[���o�b�N���ĂԁB
		_EndNowStateCallback(State::None);
	}

	_MyComponent.CharacterController->SetMoveSpeed(_MoveSpeed);
	// �L�����N�^�[�R���g���[���Ŏ��ۂɃL�����N�^�[�𐧌�B
	_MyComponent.CharacterController->Execute();
	// �ړ��������ʁA�w�肵�������̂��̂��Փ˂��Ă���Ή����o���B
	_MyComponent.CharacterExtrude->Extrude(_MyComponent.CharacterController->GetmoveSpeedExcute());
}

void EnemyCharacter::LateUpdate() {
	// �p����ɂ��ς�鏈���B
	_LateUpdateSubClass();

	_MoveSpeed = Vector3::zero;	// �g���I������̂ŏ������B

	// �ŒZ�G�l�~�[�̏������Z�b�g�B
	nearEnemyInfo = NearEnemyInfo(FLT_MAX, nullptr);
}


bool EnemyCharacter::IsOutsideDiscovery() {
	Vector3 work = _InitPos - transform->GetPosition();
	work.y = 0.0f;
	float NowRange = work.Length();
	if (NowRange > _discoveryRange) {
		// �ǐՔ͈͊O�ɏo���B
		return true;
	}
	return false;
}

bool EnemyCharacter::IsOutsideWandering(const Vector3& Add) {
	Vector3 work = _InitPos - (transform->GetPosition() + Add);
	work.y = 0.0f;
	float NowRange = work.Length();
	if (NowRange > _WanderingRange) {
		// �p�j�͈͊O�ɏo���B
		return true;
	}
	return false;
}


void EnemyCharacter::SearchView() {
	// ����p����B
	if (_SearchView.IsDiscovery(
		transform->GetPosition(),
		_Player->transform->GetPosition(),
		transform->GetForward(),
		_ViewAngle,
		_ViewRange))
	{
		// �����ɓ����Ă���B

		// �Њd�X�e�[�g�Ɉڍs�B
		_ChangeState(State::Threat);
	}
}

void EnemyCharacter::ConfigDamageReaction(bool isMotion, unsigned short probability) {
	_isDamageMotion = isMotion;
	if (isMotion) {
		if (probability <= 1) {
			_isDamageMotionRandom = false;
		}
		else {
			_isDamageMotionRandom = true;
			_damageMotionProbability = probability;
		}
	}
}


void EnemyCharacter::_BarRenderUpdate() {
	if (_MyComponent.HPBar) {
		float distance = 60.0f;
		if (Vector3(_Player->transform->GetPosition() - transform->GetPosition()).Length() <= distance) {
			_MyComponent.HPBar->RenderEnable();
			_MyComponent.BuffDebuffICon->RenderEnable();
		}
		else {
			_MyComponent.HPBar->RenderDisable();
			_MyComponent.BuffDebuffICon->RenderDisable();

		}
	}
}

void EnemyCharacter::_BuildMyComponents() {
	// ���f������ǉ��B
	_MyComponent.Model = AddComponent<SkinModel>();
	// �A�j���[�V������ǉ��B
	_MyComponent.Animation = AddComponent<Animation>();
	// �L�����N�^�[�R���g���[����ǉ��B
	_MyComponent.CharacterController = AddComponent<CCharacterController>();
	// ��]�N���X��ǉ��B
	_MyComponent.RotationAction = AddComponent<ObjectRotation>();
	// �o�[�R���|�[�l���g��ǉ��B
	_MyComponent.HPBar = AddComponent<ParameterBar>();
	//�p�����[�^�[�̃R���|�[�l���g�ǉ��B
	_MyComponent.Parameter = AddComponent<CharacterParameter>();
	// �X�|�i�[�R���|�[�l���g�ǉ��B
	_MyComponent.Spawner = AddComponent<ObjectSpawn>();
	// �A�j���[�V�����C�x���g�R���|�[�l���g�ǉ��B
	_MyComponent.AnimationEventPlayer = AddComponent<AnimationEventPlayer>();

	// �p�[�e�B�N���G�t�F�N�g�R���|�[�l���g�ǉ��B
	_MyComponent.ParticleEffect = AddComponent<ParticleEffect>();

	// �o�t�f�o�t�A�C�R���R���|�[�l���g�ǉ��B
	_MyComponent.BuffDebuffICon = AddComponent<BuffDebuffICon>();
	// ���̉����o���R���|�[�l���g�ǉ��B
	_MyComponent.CharacterExtrude = AddComponent<CharacterExtrude>();
}

void EnemyCharacter::_BuildCollision() {
	// �R���W�����̃p�����[�^������B
	_ConfigCollision();

	if (_collisionInfo.radius <= 0.0f || _collisionInfo.height <= 0.0f) {
		// �p����ŃT�C�Y�ݒ肵�Ă�H�B
		abort();
	}
	if (_MyComponent.Collider == nullptr) {
		// �p����ŃR���C�_�[�R���|�[�l���g�ݒ肵�Ă�H�B
		abort();
	}

	// �L�����N�^�[�R���g���[���[�쐬�B
	// ���R���C�_�[�R���|�[�l���g�͌p����Œǉ��B
	_MyComponent.CharacterController->Init(_collisionInfo.offset, _collisionInfo.id, _MyComponent.Collider, _Gravity,true);
	
	// �L�����N�^�[�R���g���[���[�Ƀp�����[�^�[��ݒ�B
	_ConfigCharacterController();

	_MyComponent.CharacterController->SetGravity(_Gravity);

	// �L�����N�^�[�R���g���[�������o���R���|�[�l���g�쐬�B
	_CreateExtrudeCollision();

	int Attribute = Collision_ID::NOT_ID;
	if (_MyComponent.ExtrudeCollisions.size() > 0) {
		_MyComponent.CharacterExtrude->Init(_MyComponent.ExtrudeCollisions, Attribute);
	}
	else {
		// �L�����N�^�[�R���g���[�������o���p�̍��̂�ݒ肵�ĂȂ��B

		abort();
	}

	_ConfigCharacterExtrude();
}

void EnemyCharacter::_BuildModelData() {
	//�X�L�����f���f�[�^�쐬�B
	SkinModelData* modeldata = new SkinModelData();
	// ���f���f�[�^�ǂݍ��݁B
	modeldata->CloneModelData(SkinModelManager::LoadModel(_FileName), _MyComponent.Animation);
	//���f���R���|�[�l���g�Ƀ��f���f�[�^��ݒ�B
	_MyComponent.Model->SetModelData(modeldata);
	//_MyComponent.Model->SetModelEffect(ModelEffectE::SPECULAR, false);

	_MyComponent.Model->SetAtomosphereFunc(AtmosphereFunc::enAtomosphereFuncObjectFromAtomosphere);
	_MyComponent.Model->SetModelEffect(ModelEffectE::DITHERING, true);

	_MyComponent.AnimationEventPlayer->Init(_MyComponent.Animation->GetNumAnimationSet());
}

void EnemyCharacter::_BuildState() {
	// �p�j�X�e�[�g��ǉ��B
	_MyState.push_back(unique_ptr<EnemyState>(new EnemyWanderingState(this)));
	// �����X�e�[�g��ǉ��B
	_MyState.push_back(unique_ptr<EnemyDiscoveryState>(new EnemyDiscoveryState(this)));
	// �ǐՃX�e�[�g��ǉ��B
	_MyState.push_back(unique_ptr<EnemyChaceState>(new EnemyChaceState(this)));
	// �o�b�N�X�e�b�v�X�e�[�g��ǉ��B
	_MyState.push_back(unique_ptr<EnemyBackStepState>(new EnemyBackStepState(this)));
	// �Њd�X�e�[�g��ǉ��B
	_MyState.push_back(unique_ptr<EnemyThreatState>(new EnemyThreatState(this)));
	// ��b�X�e�[�g��ǉ��B
	_MyState.push_back(unique_ptr<EnemySpeakState>(new EnemySpeakState(this)));
	// �U���J�n�X�e�[�g��ǉ��B
	_MyState.push_back(unique_ptr<EnemyStartAttackState>(new EnemyStartAttackState(this)));
	// �U���X�e�[�g��ǉ��B
	_MyState.push_back(unique_ptr<EnemyAttackState>(new EnemyAttackState(this)));
	// �ҋ@�X�e�[�g��ǉ��B
	_MyState.push_back(unique_ptr<EnemyWaitState>(new EnemyWaitState(this)));
	// ���i�X�e�[�g��ǉ��B
	_MyState.push_back(unique_ptr<EnemyTranslationState>(new EnemyTranslationState(this)));
	// �����X�e�[�g��ǉ��B
	_MyState.push_back(unique_ptr<EnemyFallState>(new EnemyFallState(this)));
	// �_���[�W�����X�e�[�g��ǉ��B
	_MyState.push_back(unique_ptr<EnemyDamageReactionState>(new EnemyDamageReactionState(this)));
	// ���S�X�e�[�g��ǉ��B
	_MyState.push_back(unique_ptr<EnemyDeathState>(new EnemyDeathState(this)));

	_BuildStateSubClass();
}

void EnemyCharacter::_BuildAnimation() {

	// �e�[�u���������B
	for (int idx = 0; idx < static_cast<int>(AnimationType::Max); idx++) {
		_AnimationNo[idx] = -1;
	}

	// �A�j���[�V�����̍Đ����ԃe�[�u�����쐬�B
	vector<double> Datas = vector<double>(_MyComponent.Animation->GetNumAnimationSet(), -1.0);

	// �p����ŃA�j���[�V�����̏I�����ԂƃG�l�~�[���ʂ̃A�j���[�V�����񋓎q�ւ̊֘A�t�����s���B
	_BuildAnimationSubClass(Datas);

	// �A�j���[�V�����̏I�����Ԃ��R���|�[�l���g�ɒʒm�B
	for (int idx = 0; idx < static_cast<int>(Datas.size()); idx++) {
		_MyComponent.Animation->SetAnimationEndTime(static_cast<UINT>(idx), Datas[idx]);
	}
};

void EnemyCharacter::Drop() {
	_DropSubClass();
	for (int idx = 0; idx < static_cast<int>(Item::ItemCodeE::Max); idx++)
	{
		for (int i = 0; i < LoadEnemyInfo::dropMax; i++)
		{
			//���Ƃ��A�C�e�������`�F�b�N�B
			if (_Type[idx][i] != -1)
			{
				DropItem* item = INSTANCE(GameObjectManager)->AddNew<DropItem>("DropItem", 9);
				//���Ƃ��A�C�e����id�ƃR�[�h���w��B
				item->Create(_Type[idx][i], idx, transform->GetPosition(), 2);
			}
		}
	}
	_Player->TakeDrop(GetDropEXP(), GetDropMoney());
}

/**
* �A�C�e������.
*/
bool EnemyCharacter::ItemEffect(Item::ItemInfo * info)
{
	//�߂�l.
	bool returnValue = false;

	returnValue = BuffAndDebuff(info->effectValue,info->time);

//	for (int idx = static_cast<int>(CharacterParameter::Param::ATK); idx < CharacterParameter::MAX; idx++) {
//		if (_MyComponent.Parameter)
//		{
//			if (_MyComponent.ParticleEffect) {
//				_MyComponent.ParticleEffect->DeBuffEffect();
//			}
//#ifdef  _DEBUG
//			else {
//				// �G�t�F�N�g�R���|�[�l���g�Ȃ���B
//				abort();
//			}
//#endif //  _DEBUG
//			_MyComponent.Parameter->Debuff(static_cast<CharacterParameter::Param>(idx), static_cast<unsigned short>(abs(value)), info->time);
//			_MyComponent.BuffDebuffICon->DebuffIconCreate(static_cast<BuffDebuffICon::Param>(idx));
//		}
//	}
//
//	return true;

//	//�߂�l.
//	bool returnValue = false;
//
//	for (int idx = static_cast<int>(CharacterParameter::Param::ATK); idx < CharacterParameter::MAX; idx++) {
//		int value = info->effectValue[idx];
//		if (value > 0) {
//			// �o�t�B
//			if (_MyComponent.ParticleEffect) {
//				_MyComponent.ParticleEffect->BuffEffect();
//			}
//#ifdef  _DEBUG
//			else {
//				// �G�t�F�N�g�R���|�[�l���g�Ȃ���B
//				abort();
//			}
//#endif //  _DEBUG
//
//			_MyComponent.Parameter->Buff(static_cast<CharacterParameter::Param>(idx), static_cast<unsigned short>(value), info->time);
//			_MyComponent.BuffDebuffICon->SelectUseIconType_Enemy();
//			_MyComponent.BuffDebuffICon->BuffIconCreate(static_cast<CharacterParameter::Param>(idx));
//			returnValue = true;
//		}
//		else if (value < 0) {
//			// �f�o�t(�f�����b�g)�B
//			if (_MyComponent.ParticleEffect) {
//				_MyComponent.ParticleEffect->DeBuffEffect();
//			}
//#ifdef  _DEBUG
//			else {
//				// �G�t�F�N�g�R���|�[�l���g�Ȃ���B
//				abort();
//			}
//#endif //  _DEBUG
//			_MyComponent.Parameter->Debuff(static_cast<CharacterParameter::Param>(idx), static_cast<unsigned short>(abs(value)), info->time);
//			_MyComponent.BuffDebuffICon->SelectUseIconType_Enemy();
//			_MyComponent.BuffDebuffICon->DebuffIconCreate(static_cast<CharacterParameter::Param>(idx));
//			returnValue = true;
//		}
//	}
	return returnValue;
}

bool EnemyCharacter::BuffAndDebuff(int effectValue[CharacterParameter::Param::MAX], float time) {
	bool ret = false;
	for (int idx = static_cast<int>(CharacterParameter::Param::ATK); idx < CharacterParameter::MAX; idx++) {
		int value = effectValue[idx];
		if (value > 0) {
			// �o�t�B
			if (_MyComponent.ParticleEffect) {
				_MyComponent.ParticleEffect->BuffEffect();
			}
#ifdef  _DEBUG
			else {
				// �G�t�F�N�g�R���|�[�l���g�Ȃ���B
				abort();
			}
#endif //  _DEBUG

			_MyComponent.Parameter->Buff(static_cast<CharacterParameter::Param>(idx), value, time);
			_MyComponent.BuffDebuffICon->SelectUseIconType_Enemy();
			_MyComponent.BuffDebuffICon->BuffIconCreate(static_cast<CharacterParameter::Param>(idx));

			EnemyPlaySound(EnemyCharacter::SoundIndex::StatusUp);

			ret = true;
		}
		else if (value < 0) {
			// �f�o�t(�f�����b�g)�B
			if (_MyComponent.ParticleEffect) {
				_MyComponent.ParticleEffect->DeBuffEffect();
			}
#ifdef  _DEBUG
			else {
				// �G�t�F�N�g�R���|�[�l���g�Ȃ���B
				abort();
			}
#endif //  _DEBUG
			_MyComponent.Parameter->Debuff(static_cast<CharacterParameter::Param>(idx), abs(value), time);
			_MyComponent.BuffDebuffICon->SelectUseIconType_Enemy();
			_MyComponent.BuffDebuffICon->DebuffIconCreate(static_cast<CharacterParameter::Param>(idx));

			EnemyPlaySound(EnemyCharacter::SoundIndex::StatusDown);

			ret = true;
		}
	}
	return ret;
}

void EnemyCharacter::_ChangeState(State next) {
	if (next != EnemyCharacter::State::None && static_cast<int>(next) >= static_cast<int>(_MyState.size())) {
		// �n���ꂽ�������z��̗e�ʂ𒴂��Ă���B
		abort();
	}

	if (_NowState) {
		// ���݂̃X�e�[�g��null�łȂ��B
		if (_NowState->GetIsEnd() || _NowState->IsPossibleChangeState(next)) {
			// ���݂̃X�e�[�g���玟�̃X�e�[�g�ւ̈ڍs��������Ă���A�܂��͌��݂̃X�e�[�g���I�����Ă���B
			// ���݂̃X�e�[�g����Еt���B
			_NowState->Exit(next);
		}
		else {
			// ���݂̃X�e�[�g���玟�̃X�e�[�g�ւ̈ڍs��������Ă��Ȃ��B
			return;
		}
	}

	if (next == State::None) {
		// ���̃X�e�[�g���w�肳��Ă��Ȃ��B
		_NowState = nullptr;
	}
	else {
		// �V�����X�e�[�g�ɐ؂�ւ��B
		_NowState = _MyState[static_cast<int>(next)].get();
		_NowState->Entry();
	}

	// �X�e�[�g�؂�ւ����̃R�[���o�b�N�Ăяo���B
	_ChangeStateCallback(_NowStateIdx, next);

	// ���݂̃X�e�[�g�̓Y������ۑ��B
	_NowStateIdx = next;
}

void EnemyCharacter::_BuildSoundTable() {
	// �ėp���ʉ��o�^�B
	//_SoundData = vector<unique_ptr<SoundData>>(static_cast<int>(SoundIndex::Max));

	// �U�����o�^�B
	_ConfigSoundData(EnemyCharacter::SoundIndex::Damage, "Damage_01.wav", 1.0f);
	_ConfigSoundData(EnemyCharacter::SoundIndex::Buoon, "Buoonn.wav",1.0f);
	_ConfigSoundData(EnemyCharacter::SoundIndex::AttackGolem, "EnemyGolemAttack01.wav", 1.0f);

	// ���񂾎��̐��o�^�B
	_ConfigSoundData(EnemyCharacter::SoundIndex::Death, "EnemyGolemDie.wav",1.0f);

	// �_���[�W���󂯂����̐��o�^�B
	_ConfigSoundData(EnemyCharacter::SoundIndex::DamageGolem, "EnemyGolemDamage.wav", 1.0f);

	_ConfigSoundData(EnemyCharacter::SoundIndex::StatusUp, "Player/statusup.wav", 1.0f);
	_ConfigSoundData(EnemyCharacter::SoundIndex::StatusDown, "Player/statusdown.wav", 1.0f);

}

void EnemyCharacter::_ConfigSoundData(SoundIndex idx, char* filePath, bool is3D, bool isLoop) {
	if (idx >= SoundIndex::Max) {
		// �p����Ǝ��̌��ʉ��B

		_SoundData.resize(static_cast<int>(idx) + 1);
	}
	_SoundData[static_cast<int>(idx)].reset(_CreateSoundData(filePath, is3D, isLoop));
}

EnemyCharacter::SoundData* EnemyCharacter::_CreateSoundData(char* filePath, float volume, bool isLoop, bool is3D) {
	EnemyCharacter::SoundData* data = new EnemyCharacter::SoundData;
	strcpy(data->Path, filePath);
	data->volume = volume;
	data->Is3D = is3D;
	data->IsLoop = isLoop;
	data->Source = INSTANCE(GameObjectManager)->AddNew<SoundSource>(filePath, 0);
	string work = "Asset/Sound/";
	work = work + filePath;
	data->Source->Init(work.c_str(), is3D);
	return data;
}

void EnemyCharacter::HitAttackCollisionEnter(AttackCollision* hitCollision) {
	if (hitCollision->GetMaster() == AttackCollision::CollisionMaster::Player)
	{
		if (_MyComponent.Parameter->GetParam(CharacterParameter::HP) > 0)
		{
			GiveDamage(*hitCollision->GetDamageInfo(),hitCollision->GetReactionType());
		}
	}
}

void EnemyCharacter::GiveDamage(const CharacterParameter::DamageInfo& info, AttackCollision::ReactionType reaction) {
	int _damage;
	if ((_NowState && _NowState->IsPossibleDamage()) || _NowState == nullptr) {
		// �_���[�W��^������X�e�[�g�������B

		_damage = _MyComponent.Parameter->ReciveDamage(info);

		// �_���[�W�l�����ƂɃp�����[�^�[�X�V�B
		_MyComponent.HPBar->SubValue(static_cast<float>(_damage));

		//�󂯂��_���[�W�ʂ�\���B
		Color c;
		if (_damage == 0)
		{
			c = Color::white * 0.3f;
		}
		else
		{
			c = Color::red;
		}
		AttackValue2D* attackvalue = INSTANCE(GameObjectManager)->AddNew<AttackValue2D>("AttackValue2D", 5);
		attackvalue->Init(transform, _damage, info.isCritical,1.5f, Vector3(0.0f, 1.0f, 0.0f), c);

		if (_isDamageMotion) {
			// �_���[�W���Ƀ��[�V�������Đ����邩�B

			if (reaction != AttackCollision::ReactionType::NotAction) {
				// �������̍U���ł͂Ȃ������B

				// �G�l�~�[�͂Ђ�݃��[�V���������Ȃ��̂łƂ肠�����Ђ�܂���B

				if (_isDamageMotionRandom) {
					// �̂����胂�[�V�����̍Đ��������_���ɂ���B

					if (rand() % _damageMotionProbability == 0) {
						// �̂�����B

						// �_���[�W�����X�e�[�g�Ɉڍs�\�B
						_ChangeState(State::Damage);
					}
				}
				else {
					// �m��ł̂����胂�[�V�������Đ�����B

					// �_���[�W�����X�e�[�g�Ɉڍs�\�B
					_ChangeState(State::Damage);
				}
			}
		}
	}
	else {
		_damage = 0;
	}
}

float EnemyCharacter::GetNowSelectAttackRange()const {
	if (_nowAttack) {
		return _nowAttack->GetAttackRange();
	}
	return 0.0f;
}


/**
* �G�t�F�N�g�p�X�V.
*/
void EnemyCharacter::EffectUpdate() {
	bool isBuffEffect = false;
	bool isDeBuffEffect = false;
	for (int idx = static_cast<int>(CharacterParameter::Param::ATK); idx < CharacterParameter::Param::DEX; idx++) {

		if (_MyComponent.Parameter->GetBuffParam_Percentage((CharacterParameter::Param)idx) > 0)
		{
			isBuffEffect = true;
		}
		else
		{
			_MyComponent.BuffDebuffICon->DeleteBuffIcon((CharacterParameter::Param)idx);
		}
		if (_MyComponent.Parameter->GetDebuffParam_Percentage((CharacterParameter::Param)idx) > 0)
		{
			isDeBuffEffect = true;
		}
		else
		{
			_MyComponent.BuffDebuffICon->DeleteDebuffIcon((CharacterParameter::Param)idx);
		}
	}
	_MyComponent.ParticleEffect->SetBuffEffectFlag(isBuffEffect);
	_MyComponent.ParticleEffect->SetDebuffEffectFlag(isDeBuffEffect);
}



