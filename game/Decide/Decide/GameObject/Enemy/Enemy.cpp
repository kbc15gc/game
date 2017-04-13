#include "Enemy.h"
#include "fbEngine\SkinModel.h"
#include "fbEngine\CapsuleCollider.h"

Enemy::Enemy(const char* name) : EnemyCharacter(name)
{

}


Enemy::~Enemy()
{
}

void Enemy::Awake() {
	// �g�p���郂�f���t�@�C���̃p�X��ݒ�B
	SetFileName("enemy_00.X");
	EnemyCharacter::Awake();
}

void Enemy::Start(){
	//���f���Ƀ��C�g�ݒ�
	_MyComponent.Model->SetLight(GameObjectManager::mainLight);
	// �ʒu���ݒ�B
	transform->SetLocalPosition(Vector3(0.0f, 10.0f, 0.0f));
	// �����X�e�[�g�Ɉڍs�B
	// ���b�菈���B
	_ChangeState(State::Translation);
	EnemyCharacter::Start();
}

void Enemy::Update() {
	EnemyCharacter::Update();
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
		Datas.push_back(move(data));
		// �����ɃA�j���[�V�����I�����Ԑݒ�֐����L�q�B

	}

	// �A�j���[�V�����^�C�v�Ƀf�[�^���֘A�Â��B
	{
		// �ҋ@��ԁB
		_ConfigAnimationType(EnemyCharacter::AnimationType::Idle, *Datas[static_cast<int>(AnimationProt::Stand)].get());
		// ���s��ԁB
		_ConfigAnimationType(EnemyCharacter::AnimationType::Walk, *Datas[static_cast<int>(AnimationProt::Walk)].get());
	}
}