#pragma once

class CCharacterController;
class SkinModel;
class Animation;
class EnemyState;

// ���N���X�B
// �G�l�~�[�̃L�����N�^�[�B
class EnemyCharacter :
	public GameObject
{
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
	// �X�e�[�g�z��̓Y������񋓁B
	// ���p����Ŏg�p������̂��܂߂Ă��ׂẴX�e�[�g�������ɗ񋓂���B
	enum class State{ NotAction };

public:
	// �����̓I�u�W�F�N�g�̖��O�B
	EnemyCharacter(const char* name);
	~EnemyCharacter();


	virtual void Awake()override;

	virtual void Start()override;

	virtual void Update()override;


	// ���f���t�@�C���̃p�X��ݒ�B
	inline void SetFileName(const char* name) {
		if (strlen(name) >= FILENAME_MAX) {
			// �t�@�C�����������I�[�o�[�B
			abort();
		}
		strcpy(_FileName, name);
	}

	// �X�e�[�g�z���ǂݎ���p�ŕԂ��֐��B
	inline const vector<unique_ptr<EnemyState>>& GetMyState() const{
		return _MyState;
	}

protected:
	// �X�e�[�g�؂�ւ��֐��B
	void _ChangeState(State next);

private:
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
	void _BuildState();

protected:
	Components _MyComponent;	// ���̃N���X�Ŏg�p����R���|�[�l���g�B
	float _Radius = 0.0f;	// �R���W�����T�C�Y(��)�B
	float _Height = 0.0f;	// �R���W�����T�C�Y(����)�B
private:
	EnemyState* _NowState = nullptr;	// ���݂̃X�e�[�g�B
	State _NowStateIdx;			// ���݂̃X�e�[�g�̓Y�����B
	vector<unique_ptr<EnemyState>> _MyState;	// ���̃N���X�������ׂẴX�e�[�g��o�^�B

	char _FileName[FILENAME_MAX];	// ���f���̃t�@�C�����B
};