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
#include "AttackCollision.h"


EnemyCharacter::EnemyCharacter(const char* name) :GameObject(name)
{
}

EnemyCharacter::~EnemyCharacter()
{

}


void EnemyCharacter::Awake() {
	// �p����ɂ��ς�鏈���B
	_AwakeSubClass();

	// ���̃N���X�Ŏg�p����R���|�[�l���g��ǉ��B
	// �����L�̊֐����p����̃N���X�ŏ㏑�����Ă���ꍇ�͂����炪�Ă΂��B
	_BuildMyComponents();
	// �g�p����X�e�[�g��񋓁B
	_BuildState();
	// ���̐����B
	_BuildCollision();
	// ���f�������B
	_BuildModelData();
	// �A�j���[�V�����e�[�u���쐬�B
	_BuildAnimation();
}

void EnemyCharacter::Start() {
	_MoveSpeed = Vector3::zero;	// �������B
	
	// �ʒu���ݒ�B
	_InitPos = Vector3(560, 69, -1000);
	transform->SetLocalPosition(_InitPos);

	// �p����ɂ��ς�鏈���B
	_StartSubClass();
}

void EnemyCharacter::Update() {

	if (_MyComponent.Parameter->HP <= 0)
	{
		INSTANCE(GameObjectManager)->AddRemoveList(this);
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


void EnemyCharacter::CreateAttackCollision(const int eventFrame, const Vector3& pos, const Vector3& angle, const Vector3& size) {
	//���݂̃t���[���擾
	const int nowFrame = _MyComponent.Animation->NowFrame();
	//�t���[����10�̎������蔻��쐬
	if (nowFrame == eventFrame)
	{
		//�U���R���W�����쐬
		unsigned int priorty = 1;
		AttackCollision* attack = INSTANCE(GameObjectManager)->AddNew<AttackCollision>("attack_enemy", priorty);
		Quaternion rot = Quaternion::Identity;
		attack->Create(_MyComponent.Parameter->AttackDamageMass(1), pos, rot, size, AttackCollision::CollisionMaster::Enemy, -1.0f);
	}
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

		// �����X�e�[�g�Ɉڍs�B
		_ChangeState(State::Discovery);
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
	_MyComponent.CharacterController->Init(this, transform, _Radius, _Height, Vector3::zero, Collision_ID::ENEMY, _MyComponent.Collider, _Gravity);
	
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