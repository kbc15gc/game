#include"stdafx.h"
#include "CodeNameD.h"
#include "fbEngine\_Object\_Component\_3D\SkinModel.h"
#include "HFSM\EnemyTranslationState.h"
#include "HFSM\EnemyWaitState.h"
#include "fbEngine\CharacterController.h"
#include "fbEngine\_Object\_GameObject\Particle.h"
#include "fbEngine\_Object\_Component\_Physics\CapsuleColliderZ.h"
#include "GameObject\Enemy\EnemyAttack.h"

BossD::BossD(const char* name) : EnemyCharacter(name)
{

}

BossD::~BossD()
{
}

void BossD::_AwakeSubClass() {
	SetFileName("doragon.X");
}
void BossD::_StartSubClass() {

	//�|�W�V����
	_InitPos = Vector3(-202.0f, 60.0f, -156.0f);
	transform->SetPosition(_InitPos);

	// ����p�����B
	_ViewAngle = 100.0f;
	_ViewRange = 30.0f;

	// ���s���x�ݒ�B
	_walkSpeed = 2.5f;

	// �p�j�͈͐ݒ�B
	// ���b�菈���B
	_WanderingRange = 20.0f;

	// �ǐՔ͈͐ݒ�B
	_discoveryRange = 30.0f;

	// ����Ɉ�񂭂炢���ނ��ݒ�B
	_damageMotionRandNum = 30;

	//���f���Ƀ��C�g�ݒ�B
	_MyComponent.Model->SetLight(INSTANCE(GameObjectManager)->mainLight);
	//�U�������B
	
	// �����X�e�[�g�Ɉڍs�B
	// ���b�菈���B
	_initState = State::Wandering;
	_ChangeState(_initState);
}

void BossD::_UpdateSubClass() {

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

void BossD::_LateUpdateSubClass()
{

}

EnemyAttack* BossD::_AttackSelectSubClass() {
	// ���v���C���[�ƃG�l�~�[�̈ʒu�֌W�Ƃ��őJ�ڐ挈��H�B

	// ���Ƃ肠�����b�菈���B
	int rnd = rand() % 2;
	if (rnd == 0) {
		return _singleAttack.get();
	}
	else {
		return _singleAttackSecondPattern.get();
	}

}


void BossD::_EndNowStateCallback(State EndStateType) {

	if (EndStateType == State::Wandering) {
		// �p�j�X�e�[�g�I���B
		_ChangeState(State::Wandering);
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
		// �U���J�n�B
		_ChangeState(State::StartAttack);
	}
	else if (EndStateType == State::Threat) {
		// �Њd�I���B
		// �U���J�n�B
		_ChangeState(State::StartAttack);
	}
}

void BossD::_ConfigCollision() {
	// �U������p�̃R���W�����B
	{
		// ���́B
		{
			RigidBody* coll = AddComponent<RigidBody>();	// �L�����N�^�[�R���g���[���Ƃ͕ʂɐV�����쐬(�v���C���[���L�����R���̌`��ŉ����o�������Ȃ�����)�B

			RigidBodyInfo info;
			info.coll = AddComponent<CCapsuleColliderZ>();
			static_cast<CCapsuleColliderZ*>(info.coll)->Create(0.75f, 5.1f);
			info.id = Collision_ID::BOSS;
			info.mass = 0.0f;
			info.physicsType = Collision::PhysicsType::Kinematick;
			info.offset = Vector3(0.0f, 0.0f, 0.0f);
			info.rotation = Quaternion::Identity;
			coll->Create(info, true);

			_MyComponent.ExtrudeCollisions.push_back(coll);	// ���łɉ����o���悤�R���W�����ɒǉ����Ă����B
		}
	}

	// �L�����N�^�[�R���g���[���p�B
	{
		// �R���W�����̃T�C�Y������B
		// ���L�����N�^�[�R���g���[���[�Ŏg�p���邽�߂̂��́B
		_collisionInfo.radius = 1.8;
		_collisionInfo.height = 6.0f;
		_collisionInfo.offset = Vector3(0.0f, 1.0f, 0.0f);
		_collisionInfo.id = Collision_ID::CHARACTER_GHOST;

		// �d�͐ݒ�B
		_Gravity = -9.8f;

		// �R���|�[�l���g�ɃJ�v�Z���R���C�_�[Z��ǉ��B
		_MyComponent.Collider = AddComponent<CCapsuleColliderZ>();
		// �J�v�Z���R���C�_�[���쐬�B
		static_cast<CCapsuleColliderZ*>(_MyComponent.Collider)->Create(_collisionInfo.radius, _collisionInfo.height);
	}
}

void BossD::_ConfigCharacterController() {
	// �Փ˂��鑮����ݒ�(��)�B
	_MyComponent.CharacterController->AttributeXZ_AllOn();
	_MyComponent.CharacterController->SubAttributeXZ(Collision_ID::ATTACK);
	_MyComponent.CharacterController->SubAttributeXZ(Collision_ID::SPACE);
	_MyComponent.CharacterController->SubAttributeXZ(Collision_ID::PLAYER);
	_MyComponent.CharacterController->SubAttributeXZ(Collision_ID::ENEMY);
	_MyComponent.CharacterController->SubAttributeXZ(Collision_ID::DROPITEM);
	// �Փ˂��鑮����ݒ�(�c)�B
	_MyComponent.CharacterController->AttributeY_AllOn();
	_MyComponent.CharacterController->SubAttributeY(Collision_ID::ATTACK);
	_MyComponent.CharacterController->SubAttributeY(Collision_ID::ENEMY);
	_MyComponent.CharacterController->SubAttributeY(Collision_ID::PLAYER);
	_MyComponent.CharacterController->SubAttributeY(Collision_ID::SPACE);
	_MyComponent.CharacterController->SubAttributeY(Collision_ID::DROPITEM);

}

void BossD::_CreateExtrudeCollision() {
}

void BossD::_BuildAnimationSubClass(vector<double>& datas) {

	// �A�j���[�V�����^�C�v�Ƀf�[�^���֘A�Â��B
	// ���G�l�~�[�͂��ׂē����X�e�[�g�N���X���g�p���邽�߁A�X�e�[�g����A�j���[�V�������Đ��ł���悤
	//   EnemyCharacter�N���X�Œ�`����Ă��邷�ׂẴG�l�~�[���ʂ̗񋓎q�Ɋ֘A�t����K�v������B
	
}

void BossD::_ConfigAnimationEvent() {
	
}

void BossD::_BuildSoundTable() {
	// �U�����o�^�B

}

