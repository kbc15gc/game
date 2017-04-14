#pragma once

#include "fbEngine\Animation.h"
#include "SearchViewAngle.h"

class CCharacterController;
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

	enum class State { Wandering = 0,Discovery, Wait ,Translation };

	// �A�j���[�V�����f�[�^�z��̓Y�����B
	// ��0�ԂȂ�ҋ@�A�j���[�V�����A1�ԂȂ�����A�j���[�V�����B
	// �����̗񋓎q��Y�����Ƃ��āA�p����̃N���X�ŃA�j���[�V���ԍ��̃e�[�u�����쐬����B
	enum class AnimationType { Idle = 0, Walk, Dash, Max };

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
	

	// �G�l�~�[�̃A�j���[�V�����Đ��֐�(���[�v)�B
	// �����F	�A�j���[�V�����^�C�v�B
	//			��Ԏ��ԁB
	inline void PlayAnimation_Loop(const AnimationType AnimationType, const float InterpolateTime) {
		_MyComponent.Animation->PlayAnimation(_AnimationData[static_cast<unsigned int>(AnimationType)].No, InterpolateTime);
	}

	// �G�l�~�[�̃A�j���[�V�����Đ��֐�(�w��񐔃��[�v)�B
	// �����F	�A�j���[�V�����^�C�v�B
	//			��Ԏ��ԁB
	//			���[�v�񐔁B
	inline void PlayAnimation(const AnimationType AnimationType, const float InterpolateTime, const unsigned int LoopCount) {
		_MyComponent.Animation->PlayAnimation(_AnimationData[static_cast<unsigned int>(AnimationType)].No, InterpolateTime, LoopCount);
	}

	// �O������X�e�[�g�؂�ւ����s�����Ƃ���ꍇ�͂�������ĂԁB
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

	// �����ʒu�擾�B
	inline const Vector3& GetInitPos()const {
		return _InitPos;
	}

	// ���݂̃X�e�[�g���擾�B
	inline EnemyState* GetNowState()const {
		return _NowState;
	}
protected:
	// �X�e�[�g�؂�ւ��֐��B
	void _ChangeState(State next);

	// �A�j���[�V�����^�C�v�ɃA�j���[�V�����f�[�^���֘A�t����֐��B
	// �����F	�A�j���[�V�����^�C�v�̗񋓎q�B
	//			��1�����Ɋ֘A�t�������A�j���[�V�����f�[�^�B
	inline void _ConfigAnimationType(AnimationType Type, const AnimationData& Data) {
		_AnimationData[static_cast<unsigned int>(Type)] = Data;
	}

	// ���݂̃X�e�[�g�̏������I�������Ƃ��ɌĂ΂��R�[���o�b�N�֐��B
	// �����͏I�������X�e�[�g�̃^�C�v�B
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

	// �p�����[�^�[�ō��̂𐶐��B
	virtual void _BuildCollision();

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
	EnemyState* _NowState = nullptr;	// ���݂̃X�e�[�g�B

	Vector3 _InitPos;	// �����ʒu�B

	SearchViewAngle _SearchView;	// ����p����B
	float _ViewAngle = 0.0f;		// ����p(�x)�B
	float _ViewRange = 0.0f;		// �����鋗���B

	float _AttackRange = 0.0f;	// �U���\�͈́B
private:
	State _NowStateIdx;		// ���݂̃X�e�[�g�̓Y�����B
	vector<unique_ptr<EnemyState>> _MyState;	// ���̃N���X�������ׂẴX�e�[�g��o�^�B

	char _FileName[FILENAME_MAX];	// ���f���̃t�@�C�����B

	Vector3 _MoveSpeed;	// �ŏI�I�Ȉړ���(�ŏI�I�ɃL�����N�^�[�R���g���[���ɓn�����)�B

};