#include "EnemyCharacter.h"
#include "HFSM\EnemyState.h"
#include "fbEngine\SkinModel.h"
#include "fbEngine\SkinModelData.h"
#include "fbEngine\Animation.h"
#include "fbEngine\CharacterController.h"

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
	// �g�p����X�e�[�g��񋓁B
	_BuildState();
	// �Ƃ肠�����������Ȃ��X�e�[�g�Ɉڍs�B
	// ���X�e�[�g���g�p����ꍇ�͌p�����Start�֐��ŃX�e�[�g��؂�ւ���B
	_ChangeState(State::NotAction);
}

void EnemyCharacter::Start() {
	// ���̐����B
	_BuildCollision();
	// ���f�������B
	_BuildModelData();
}

void EnemyCharacter::Update() {
	// ���݂̃X�e�[�g���X�V�B
	_NowState->Update();
}


void EnemyCharacter::_BuildMyComponents() {
	// ���f������ǉ��B
	_MyComponent.Model = AddComponent<SkinModel>();
	// �A�j���[�V������ǉ��B
	_MyComponent.Animation = AddComponent<Animation>();
	// �L�����N�^�[�R���g���[����ǉ��B
	_MyComponent.CharacterController = AddComponent<CCharacterController>();
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

	float gravity = -30.0f;

	// �L�����N�^�[�R���g���[���[�쐬�B
	// ���R���C�_�[�R���|�[�l���g�͌p����Œǉ��B
	_MyComponent.CharacterController->Init(this, transform, _Radius, _Height, Vector3::zero, Collision_ID::ENEMY, _MyComponent.Collider, gravity);
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
	// �������Ȃ��X�e�[�g��ǉ��B
	_MyState.push_back(unique_ptr<EnemyState>(new EnemyState(this)));
}

void EnemyCharacter::_ChangeState(State next) {
	if (static_cast<int>(next) >= _MyState.size() || static_cast<int>(next) < 0) {
		// �n���ꂽ�������z��̗e�ʂ𒴂��Ă���B
		abort();
	}

	if (_NowState) {
		// ���݂̃X�e�[�g��null�łȂ��B
		// ���݂̃X�e�[�g����Еt���B
		_NowState->Exit(next);
	}

	// �V�����X�e�[�g�ɐ؂�ւ��B
	_NowState = _MyState[static_cast<int>(next)].get();
	_NowState->Entry();

	// ���݂̃X�e�[�g�̓Y������ۑ��B
	_NowStateIdx = next;
}