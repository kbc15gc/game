#include"stdafx.h"
#include "Enemy.h"
#include "fbEngine\_Object\_Component\_3D\SkinModel.h"
#include "HFSM\EnemyTranslationState.h"
#include "HFSM\EnemyWaitState.h"
#include "fbEngine\CharacterController.h"



Enemy::Enemy(const char* name) : EnemyCharacter(name)
{

}


Enemy::~Enemy()
{
}

void Enemy::_AwakeSubClass() {
	// �g�p���郂�f���t�@�C���̃p�X��ݒ�B
	SetFileName("enemy_00.X");
}

void Enemy::_StartSubClass(){
	// ����p�����B
	_ViewAngle = 90.0f;
	_ViewRange = 5.0f;

	// �U���\�͈͐ݒ�B
	_AttackRange = 1.3f;

	//���f���Ƀ��C�g�ݒ�
	_MyComponent.Model->SetLight(INSTANCE(GameObjectManager)->mainLight);

	// �ʒu���ݒ�B
	_InitPos = Vector3(0.0f, 10.0f, 0.0f);
	transform->SetLocalPosition(_InitPos);

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


EnemyCharacter::State Enemy::AttackSelect() {
	// ���v���C���[�ƃG�l�~�[�̈ʒu�֌W�Ƃ��őJ�ڐ挈��H�B

	// ���Ƃ肠�����b�菈���B
	return State::Attack;
}

void Enemy::_EndNowStateCallback(State EndStateType) {
	if (EndStateType == State::Discovery) {
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

void Enemy::_ConfigCollision() {

	// �R���W�����̃T�C�Y������B
	// ���L�����N�^�[�R���g���[���[�Ŏg�p���邽�߂̂��́B
	_Radius = 0.5f;
	_Height = 1.5f;

	// �R���|�[�l���g�ɃJ�v�Z���R���C�_�[��ǉ��B
	_MyComponent.Collider = AddComponent<CCapsuleCollider>();
	// �J�v�Z���R���C�_�[���쐬�B
	static_cast<CCapsuleCollider*>(_MyComponent.Collider)->Create(_Radius,_Height);
}

void Enemy::_BuildAnimation() {
	vector<unique_ptr<AnimationData>> Datas;
	for (int idx = 0; idx < _MyComponent.Animation->GetNumAnimationSet(); idx++) {
		// �A�j���[�V�����Z�b�g�̔ԍ��ƍĐ����Ԃ��Z�b�g�ɂ����f�[�^���쐬�B
		unique_ptr<AnimationData> data(new AnimationData);
		data->No = idx;
		data->Time = -1.0f;	// ���ׂ�1�b�ȏ�̃A�j���[�V�����Ȃ̂ŁA���Ԃ͐ݒ肵�Ȃ��B
		// �e�A�j���[�V�����̏I�����Ԃ�ݒ肵�Ă����B
		//_MyComponent.Animation->SetAnimationEndTime(data->No, data->Time);

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
		_ConfigAnimationType(EnemyCharacter::AnimationType::Attack, *Datas[static_cast<int>(AnimationProt::Attack)].get());
		// ������ԁB
		// �����̃I�u�W�F�N�g�ɂ͗����̃A�j���[�V�������Ȃ��̂őҋ@�A�j���[�V�����ő�p�B
		_ConfigAnimationType(EnemyCharacter::AnimationType::Fall, *Datas[static_cast<int>(AnimationProt::Stand)].get());
	}
}

