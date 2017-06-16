#pragma once

#include "fbEngine\_Object\_Component\_3D\Animation.h"
#include "SearchViewAngle.h"
#include "../Component/ObjectRotation.h"
#include "fbEngine\CharacterController.h"
#include "AttackCollision.h"
#include "GameObject\Component\CharacterParameter.h"
#include "GameObject\Component\ParameterBar.h"

class SkinModel;
class Animation;
class EnemyState;

// ���N���X�B
// �G�l�~�[�̃L�����N�^�[�B
class EnemyCharacter :
	public GameObject
{
public:
	// �X�e�[�g�z��̓Y������񋓁B
	// ���p����Ŏg�p������̂��܂߂Ă��ׂẴX�e�[�g�������ɗ񋓂���B
	// ���ǉ�����ۂ͂��̃N���X��_BuildState�֐��ɋL�q�������ԂɂȂ��Ă��邩����������m�F���邱�ƁB
	// ���X�e�[�g��ǉ������ۂ͂��������łȂ����̃N���X����_BuildState�֐����X�V���邱�ƁB

	enum class State { Wandering = 0,Discovery, StartAttack, Attack ,Wait ,Translation, Fall };

	// �A�j���[�V�����f�[�^�z��̓Y�����B
	// ��0�ԂȂ�ҋ@�A�j���[�V�����A1�ԂȂ�����A�j���[�V�����B
	// �����̗񋓎q��Y�����Ƃ��āA�p����̃N���X�ŃA�j���[�V���ԍ��̃e�[�u�����쐬����B
	enum class AnimationType { Idle = 0, Walk, Dash, Attack, Fall, Max };

	// �A�j���[�V�����f�[�^�\���́B
	struct AnimationData {
		unsigned int No;	// �A�j���[�V�����ԍ��B
		float Time;	// �Đ����ԁB
	};

private:
	// ���̃N���X�Ŏg�p����R���|�[�l���g�B
	// ���R���|�[�l���g�͏���ɍ폜����邽�߁A���̃N���X�ł͍폜���Ȃ��B
	// �����̃N���X�Ǝ��̃����o�ϐ��ƏZ�ݕ������s�����߂ɍ\���̉�����B
	struct Components {
		SkinModel* Model = nullptr;	// ���̃I�u�W�F�N�g�̃��f���B
		Animation* Animation = nullptr;	// ���̃I�u�W�F�N�g�̃A�j���[�V�����B
		Collider* Collider = nullptr;	// �R���W�����`��B
		CCharacterController* CharacterController = nullptr;		// �L�����N�^�[�R���g���[���B
		ObjectRotation* RotationAction = nullptr;	// �I�u�W�F�N�g����]������N���X�B
		CharacterParameter* Parameter = nullptr;//�G�l�~�[�̃p�����[�^�[�B
		ParameterBar* HPBar = nullptr;			// �Q�[�WHP�p�B
	};

public:
	// �����̓I�u�W�F�N�g�̖��O�B
	EnemyCharacter(const char* name);
	~EnemyCharacter();

	// ������͊�{�I�Ɍp�����Ȃ��B
	// ���p����Ǝ��̎����́Aprivate�ɂ���_AwakeSubClass�֐����p�����Ď������邱�ƁB
	void Awake()override;

	// ������͊�{�I�Ɍp�����Ȃ��B
	// ���p����Ǝ��̎����́Aprivate�ɂ���_StartSubClass�֐����p�����Ď������邱�ƁB
	void Start()override;

	// ������͊�{�I�Ɍp�����Ȃ��B
	// ���p����Ǝ��̎����́Aprivate�ɂ���_UpdateSubClass�֐����p�����Ď������邱�ƁB
	void Update()override;

	// ������͊�{�I�Ɍp�����Ȃ��B
	// ���p����Ǝ��̎����́Aprivate�ɂ���_LateUpdateSubClass�֐����p�����Ď������邱�ƁB
	void LateUpdate()override;
	

	// �G�l�~�[���w�肵���I�u�W�F�N�g�Ɍ������鏈��(��ԂȂ�)�B
	// �����F	�������I�u�W�F�N�g�B
	inline void LookAtObject(const GameObject& Object) {
		_MyComponent.RotationAction->RotationToObject_XZ(Object);
	}

	// �G�l�~�[���w�肵�������Ɍ������鏈��(��ԂȂ�)�B
	// �����F	�����x�N�g���B
	inline void LookAtDirection(const Vector3& dir) {
		_MyComponent.RotationAction->RotationToDirection_XZ(dir);
	}

	// �G�l�~�[���w�肵�����Ɗp�x��p���ĉ�]����֐��B
	// �����F	��(�P�ʃx�N�g��)�B
	//			��]�p�x(���W�A��)�B
	inline void RotationAxis(const Vector3& axis, float angle) {
		_MyComponent.RotationAction->RotationAxis(axis, angle);
	}

	// �U�����������肷��֐��B
	// ���p����Ŏ����B
	// �������U���p�^�[��������ꍇ�͂����Ń��[�J���X�e�[�g�ɑJ�ڂ���ۂɕ��򂳂���B
	virtual EnemyCharacter::State AttackSelect() = 0;

	// �U������p�R���W�����쐬�֐��B
	// �����F	�R���W�����𔭐�������t���[���B
	//			�R���W���������ʒu�B
	//			�R���W������]�B
	//			�R���W�����̃T�C�Y�B
	// �����������R���W�����`��̓{�b�N�X�ł��B
	void CreateAttackCollision(const int eventFrame, const Vector3& pos, const Vector3& angle, const Vector3& size);

	// �G�l�~�[�̃A�j���[�V�����Đ��֐�(���[�v)�B
	// �����F	�A�j���[�V�����^�C�v�B
	//			��Ԏ��ԁB
	inline void PlayAnimation_Loop(const AnimationType animationType, const float interpolateTime) {
		_MyComponent.Animation->PlayAnimation(_AnimationData[static_cast<unsigned int>(animationType)].No, interpolateTime);
	}

	// �G�l�~�[�̃A�j���[�V�����Đ��֐�(�w��񐔃��[�v)�B
	// �����F	�A�j���[�V�����^�C�v�B
	//			��Ԏ��ԁB
	//			���[�v�񐔁B
	inline void PlayAnimation(const AnimationType animationType, const float interpolateTime, const int loopCount) {
		_MyComponent.Animation->PlayAnimation(_AnimationData[static_cast<unsigned int>(animationType)].No, interpolateTime, loopCount);
	}

	// �G�l�~�[���A�j���[�V�������Đ����Ă��邩�B
	inline bool GetIsPlaying() {
		return _MyComponent.Animation->GetPlaying();
	}

	// �G�l�~�[���p�j�͈͊O�ɏo��������B
	bool IsOutsideWandering();

	// �����̒l�����Z�������ʁA�G�l�~�[���p�j�͈͊O�ɏo��������B
	bool IsOutsideWandering(const Vector3& Add);

	// �������������������̈ȊO�̍U���R���W�����ƏՓ˂����u�ԌĂ΂��R�[���o�b�N�B
	// �������͏Փ˂����U���R���W�����B
	// �����������Ȃ�������inline�̂܂܂ł�����(��������3�s�ȏ�̏���������悤�ɂȂ�܂�)�B
	inline virtual void HitAttackCollisionEnter(AttackCollision* hitCollision) {
		if (hitCollision->GetMaster() == AttackCollision::CollisionMaster::Player)
		{
			_MyComponent.Parameter->DamageMass(hitCollision->GetDamage(), _MyComponent.Parameter->GetParam(CharacterParameter::DEF));
		}
	}

	// �������������������̈ȊO�̍U���R���W�����ɏՓ˂Ă���ԌĂ΂��R�[���o�b�N�B
	// �������͏Փ˂����U���R���W�����B
	// �����������Ȃ�������inline�̂܂܂ł�����(��������3�s�ȏ�̏���������悤�ɂȂ�܂�)�B
	inline virtual void HitAttackCollisionStay(AttackCollision* hitCollision) {

	}

	// �������������������̈ȊO�̍U���R���W�����Ƃ̏Փ˂���O�ꂽ��Ă΂��R�[���o�b�N�B
	// �������͏Փ˂����U���R���W�����B
	// �����������Ȃ�������inline�̂܂܂ł�����(��������3�s�ȏ�̏���������悤�ɂȂ�܂�)�B
	inline virtual void HitAttackCollisionExit(AttackCollision* hitCollision) {

	}


	// �X�e�[�g�������ɃX�e�[�g�����荞�݂Ő؂�ւ���֐��B
	// ���O����X�e�[�g������X�e�[�g�؂�ւ����s�����Ƃ���ꍇ�͂�������ĂԁB
	inline void ChangeStateRequest(State next) {
		_ChangeState(next);
	}

	// ����p����֐��B
	// ����p������s���X�e�[�g����Ăяo���B
	void SearchView();


	// ���f���t�@�C���̃p�X��ݒ�B
	inline void SetFileName(const char* name) {
		if (strlen(name) >= FILENAME_MAX) {
			// �t�@�C�����������I�[�o�[�B
			abort();
		}
		strcpy(_FileName, name);
	}

	// �X�e�[�g�z���ǂݎ���p�ŕԂ��֐��B
	inline const vector<unique_ptr<EnemyState>>& GetMyState() const {
		return _MyState;
	}

	// �ړ��ʂ�ݒ�B
	inline void SetMoveSpeed(const Vector3& speed) {
		_MoveSpeed = speed;
	}

	// �ړ��ʉ��Z�B
	inline void AddMoveSpeed(const Vector3& speed) {
		_MoveSpeed = _MoveSpeed + speed;
	}

	// ����p�ݒ�(�x)�B
	inline void SetViewAngle(float angle) {
		_ViewAngle = angle;
	}

	// �����鋗���ݒ�B
	inline void SetViewRange(float range) {
		_ViewRange = range;
	}

	// �U���\�͈͎擾�B
	inline float GetAttackRange()const {
		return _AttackRange;
	}

	// �p�j�͈͕ԋp�B
	inline float GetWanderingRange()const {
		return _WanderingRange;
	}

	// �����ʒu���猻�݂̈ʒu�̈ړ��ʂ�ԋp(Y��������)�B
	inline float GetInitToNowPosRange_XZ()const {
		Vector3 vec(_InitPos - transform->GetPosition());
		vec.y = 0.0f;
		return vec.Length();
	}

	// �����ʒu���猻�݈ʒu�ւ̃x�N�g����ԋp�B
	inline const Vector3& GetInitToNowPosVec() const {
		return transform->GetPosition() - _InitPos;
	}

	// �����ʒu�擾�B
	inline const Vector3& GetInitPos()const {
		return _InitPos;
	}

	// ���݂̃X�e�[�g���擾�B
	inline EnemyState* GetNowState()const {
		return _NowState;
	}

	inline bool GetIsGround() const {
		return _MyComponent.CharacterController->IsOnGround();
	}
protected:
	// �X�e�[�g�؂�ւ��֐��B
	void _ChangeState(State next);

	// �A�j���[�V�����^�C�v�ɃA�j���[�V�����f�[�^���֘A�t����֐��B
	// �����F	�A�j���[�V�����^�C�v�̗񋓎q�B
	//			��1�����Ɋ֘A�t�������A�j���[�V�����f�[�^�B
	// �����̊֐��ɓn���ꂽ�A�j���[�V�����̏I�����Ԃ��ݒ肳��܂��B
	inline void _ConfigAnimationType(AnimationType Type, const AnimationData& Data) {
		_AnimationData[static_cast<unsigned int>(Type)] = Data;
		// �A�j���[�V�����R���|�[�l���g�ɃA�j���[�V�����̏I�����Ԑݒ�B
		_MyComponent.Animation->SetAnimationEndTime(Data.No,Data.Time);
	}

	// ���݂̃X�e�[�g�̏������I�������Ƃ��ɌĂ΂��R�[���o�b�N�֐��B
	// �����͏I�������X�e�[�g�̃^�C�v�B
	// �����̊֐��͊e�X�e�[�g�������I�ɏI�������ꍇ�ɂ̂݌Ăяo�����B
	// �����̃N���X��ChangeStateRequest�ŏI�������ꍇ�͌Ă΂�Ȃ��B
	// �����[�J���X�e�[�g�̏I�����ɂ��Ă΂�Ȃ��B
	virtual void _EndNowStateCallback(State EndStateType) {};

private:

	// �p����ł̍X�V�����B
	// ���p����ŏ㏑�����Ďg�p�B
	virtual void _AwakeSubClass() = 0;

	// �p����ł̃V�[���؂�ւ��ɂ�鏉���������B
	// ���p����ŏ㏑�����Ďg�p�B
	virtual void _StartSubClass() {};

	// �p����ł̍X�V�����B
	// ���p����ŏ㏑�����Ďg�p�B
	virtual void _UpdateSubClass() = 0;

	// �p����ł̍X�V�����I����ɌĂ΂�鏈���B
	// ���p����ŏ㏑�����Ďg�p�B
	virtual void _LateUpdateSubClass() {};


	// ���̃N���X���g�p����R���|�[�l���g��ǉ�����֐��B
	virtual void _BuildMyComponents();

	// �R���W������`�֐��B
	// �R���W�����̌`���p�����[�^��ݒ肷��֐��B
	// ���������̂͌p����ɈϏ��B
	virtual void _ConfigCollision() = 0;

	// �L�����N�^�[�R���g���[���[�̃p�����[�^�[��ݒ肷��֐��B
	// �Փ˂���R���W����������d�͂̒l�Ȃǂ������Őݒ肷��B
	// ���������̂͌p����ɈϏ��B
	virtual void _ConfigCharacterController() = 0;

	// �p�����[�^�[�ō��̂𐶐��B
	void _BuildCollision();

	// ���f���f�[�^�������֐��B
	void _BuildModelData();

	// �p����Ŏg�p���邷�ׂẴX�e�[�g��o�^����֐��B
	// ���X�e�[�g��ǉ�������K�����̊֐����ɋL�q��ǉ�����
	// ���ǉ�����ۂ͂��̃N���X��State�񋓑̂ɑΉ����鏇�ԂɂȂ��Ă��邩����������m�F���邱�ƁB
	// ���X�e�[�g��ǉ������ۂ͂��������łȂ����̃N���X����State�񋓑̂��X�V���邱�ƁB
	void _BuildState();

	// �p����ŃA�j���[�V�����ԍ��̃e�[�u�����쐬�B
	// ���Y�����ɂ͂��̃N���X��`����AnimationType�񋓑̂��g�p�B
	virtual void _BuildAnimation() = 0;
protected:
	Components _MyComponent;	// ���̃N���X�Ŏg�p����R���|�[�l���g�B
	float _Radius = 0.0f;	// �R���W�����T�C�Y(��)�B
	float _Height = 0.0f;	// �R���W�����T�C�Y(����)�B
	AnimationData _AnimationData[static_cast<int>(AnimationType::Max)];	// �e�A�j���[�V�����^�C�v�̃A�j���[�V�����ԍ��ƍĐ����Ԃ̔z��B
	State _NowStateIdx;		// ���݂̃X�e�[�g�̓Y�����B
	EnemyState* _NowState = nullptr;	// ���݂̃X�e�[�g�B

	float _Gravity = -50.0f;	// �d�́B

	Vector3 _InitPos;	// �����ʒu�B

	SearchViewAngle _SearchView;	// ����p����B
	float _ViewAngle = 0.0f;		// ����p(�x)�B
	float _ViewRange = 0.0f;		// �����鋗���B

	float _AttackRange = 0.0f;	// �U���\�͈́B

	float _WanderingRange = 0.0f;	// �p�j�͈́B
private:
	vector<unique_ptr<EnemyState>> _MyState;	// ���̃N���X�������ׂẴX�e�[�g��o�^�B

	char _FileName[FILENAME_MAX];	// ���f���̃t�@�C�����B

	Vector3 _MoveSpeed;	// �ŏI�I�Ȉړ���(�ŏI�I�ɃL�����N�^�[�R���g���[���ɓn�����)�B
};