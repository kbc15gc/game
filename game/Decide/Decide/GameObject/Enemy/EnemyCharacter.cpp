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
#include "fbEngine\_Object\_GameObject\SoundSource.h"

EnemyCharacter::EnemyCharacter(const char* name) :GameObject(name)
{
}

EnemyCharacter::~EnemyCharacter()
{

}


void EnemyCharacter::Awake() {
	// ���̃N���X�Ŏg�p����R���|�[�l���g��ǉ��B
	// �����L�̊֐����p����̃N���X�ŏ㏑�����Ă���ꍇ�͂����炪�Ă΂��B
	_BuildMyComponents();

	// �ʒu��񏉊����B
	transform->SetPosition(Vector3(0.0f,0.0f,0.0f));
	
	// �g�p����X�e�[�g��񋓁B
	_BuildState();

	// �p����ɂ��ς�鏈���B
	_AwakeSubClass();

}

void EnemyCharacter::Start() {
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
	_MyComponent.CharacterController->AddRigidBody();	// ���[���h�ɓo�^�����u�ԂɃo�E���f�B���O�{�b�N�X����������邽�߁A�������ݒ�̂��߂����œo�^�B
}

void EnemyCharacter::Update() {

	if (_MyComponent.Parameter->GetDeathFalg())
	{
		if (_NowStateIdx != State::Death) {
			_ChangeState(State::Death);
			static_cast<EnemyDeathState*>(_NowState)->SetWaitTime(1.0f);
		}
	}

	// �p����ɂ��ς�鏈���B
	_UpdateSubClass();

	if (_NowState == nullptr) {
		// �X�e�[�g���p����Ŏw�肵���H�B
		abort();
	}
	// ���݂̃X�e�[�g���X�V�B
	if (_NowState->Update()) {
		// �X�e�[�g�����I���B

		// �X�e�[�g���I���������Ƃ�ʒm�B
		_EndNowStateCallback(_NowStateIdx);
	}

	_MyComponent.CharacterController->SetMoveSpeed(_MoveSpeed);
	// �L�����N�^�[�R���g���[���Ŏ��ۂɃL�����N�^�[�𐧌�B
	_MyComponent.CharacterController->Execute();
}

void EnemyCharacter::LateUpdate() {
	// �p����ɂ��ς�鏈���B
	_LateUpdateSubClass();

	_MoveSpeed = Vector3::zero;	// �g���I������̂ŏ������B
}


bool EnemyCharacter::IsOutsideWandering() {
	float NowRange = Vector3(_InitPos - transform->GetPosition()).Length();
	if (NowRange > _WanderingRange) {
		// �p�j�͈͊O�ɏo���B
		return true;
	}
	return false;
}

bool EnemyCharacter::IsOutsideWandering(const Vector3& Add) {
	float NowRange = Vector3(_InitPos - (transform->GetPosition() + Add)).Length();
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
		INSTANCE(GameObjectManager)->FindObject("Player")->transform->GetPosition(),
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
}

void EnemyCharacter::_BuildCollision() {
	// �R���W�����̃p�����[�^������B
	_ConfigCollision();

	if (_Radius <= 0.0f || _Height <= 0.0f) {
		// �p����ŃT�C�Y�ݒ肵�Ă�H�B
		abort();
	}
	if (_MyComponent.Collider == nullptr) {
		// �p����ŃR���C�_�[�R���|�[�l���g�ݒ肵�Ă�H�B
		abort();
	}

	// �L�����N�^�[�R���g���[���[�쐬�B
	// ���R���C�_�[�R���|�[�l���g�͌p����Œǉ��B
	_MyComponent.CharacterController->Init(this, transform,Vector3::zero, Collision_ID::ENEMY, _MyComponent.Collider, _Gravity,false);
	
	// �L�����N�^�[�R���g���[���[�Ƀp�����[�^�[��ݒ�B
	_ConfigCharacterController();

	_MyComponent.CharacterController->SetGravity(_Gravity);
}

void EnemyCharacter::_BuildModelData() {
	//�X�L�����f���f�[�^�쐬�B
	SkinModelData* modeldata = new SkinModelData();
	// ���f���f�[�^�ǂݍ��݁B
	modeldata->CloneModelData(SkinModelManager::LoadModel(_FileName), _MyComponent.Animation);
	//���f���R���|�[�l���g�Ƀ��f���f�[�^��ݒ�B
	_MyComponent.Model->SetModelData(modeldata);
	//_MyComponent.Model->SetModelEffect(ModelEffectE::SPECULAR, false);
}

void EnemyCharacter::_BuildState() {
	// �p�j�X�e�[�g��ǉ��B
	_MyState.push_back(unique_ptr<EnemyState>(new EnemyWanderingState(this)));
	// �����X�e�[�g��ǉ��B
	_MyState.push_back(unique_ptr<EnemyDiscoveryState>(new EnemyDiscoveryState(this)));
	// �Њd�X�e�[�g��ǉ��B
	_MyState.push_back(unique_ptr<EnemyThreatState>(new EnemyThreatState(this)));
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
}

void EnemyCharacter::_ChangeState(State next) {
	if (static_cast<int>(next) >= _MyState.size() || static_cast<int>(next) < 0) {
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

	// �V�����X�e�[�g�ɐ؂�ւ��B
	_NowState = _MyState[static_cast<int>(next)].get();
	_NowState->Entry();

	// ���݂̃X�e�[�g�̓Y������ۑ��B
	_NowStateIdx = next;
}

	void EnemyCharacter::_ConfigSoundData(SoundIndex idx, char* filePath, bool is3D, bool isLoop) {
	strcpy(_SoundData[static_cast<int>(idx)].Path, filePath);
	_SoundData[static_cast<int>(idx)].Is3D = is3D;
	_SoundData[static_cast<int>(idx)].IsLoop = isLoop;
	_SoundData[static_cast<int>(idx)].Source = INSTANCE(GameObjectManager)->AddNew<SoundSource>(filePath, 0);
	string work = "Asset/Sound/";
	work = work + filePath;
	_SoundData[static_cast<int>(idx)].Source->Init(work.c_str(), is3D);
}

void EnemyCharacter::HitAttackCollisionEnter(AttackCollision* hitCollision) {
	if (hitCollision->GetMaster() == AttackCollision::CollisionMaster::Player)
	{
		if (_MyComponent.Parameter->GetParam(CharacterParameter::HP) >= 0)
		{
			_MyComponent.HPBar->SubValue(_MyComponent.Parameter->ReciveDamage(hitCollision->GetDamage()));
			GiveDamage(hitCollision->GetDamage());
		}
	}
}

void EnemyCharacter::GiveDamage(int damage) {
	int _damage;
	if (_NowState->IsPossibleDamage()) {
		// �_���[�W��^������X�e�[�g�������B

		// �_���[�W�l�����ƂɃp�����[�^�[�X�V�B
		_MyComponent.HPBar->SubValue(_MyComponent.Parameter->ReciveDamage(damage));
		_damage = damage;

		if (_isDamageMotion) {
			// �̂����邩�B

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
	else {
		_damage = 0;
	}

	AttackValue2D* attackvalue = INSTANCE(GameObjectManager)->AddNew<AttackValue2D>("AttackValue2D", 5);
	attackvalue->Init(Vector3::zero,_damage, 1.5f, Vector3(0.0f, _Height, 0.0f));
	attackvalue->transform->SetParent(transform);
}


// EnemyAttack�B

void EnemyAttack::Init(int animType, float interpolate, int animLoopNum) {
	_animType = animType;
	_interpolate = interpolate;
	_animLoopNum = animLoopNum;
}



// EnemySingleAttack�B

bool EnemySingleAttack::Update(){
	if (!_isPlaying) {
		// �U�����[�V������x�I���B
		return true;
	}
	return false;
}
=======
}
>>>>>>> c38421cd1207074941b60b76756a5dbe764cc97d
