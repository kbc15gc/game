#pragma once

class EnemyCharacter;

// ���N���X�B
// �G���ƃ{�X�̃X�e�[�g�B
class EnemyState
{
public:
	EnemyState(EnemyCharacter* Object);
	virtual ~EnemyState();

	// �؂�ւ����������B
	// �����̃X�e�[�g�ɐ؂�ւ�����ۂɌĂ΂��B
	void Entry();

	// �X�e�[�g�X�V�����B
	// �ԋp�l�͍X�V�������I���������̃t���O�B
	// �����̊֐��͋��ʏ����B
	bool Update();

	// �����͎��̃X�e�[�g�B
	// �����̃X�e�[�g�Ɉڍs����O�ɌĂ΂��B
	virtual void Exit(EnemyCharacter::State next) {};

	// ���̃X�e�[�g����ڍs�ł���X�e�[�g��I�ʂ���֐��B
	// ���f�t�H���g�ł͂��ׂẴX�e�[�g�Ɉڍs�ł���B
	// ���p����ŏ㏑�����Ď����B
	inline virtual bool IsPossibleChangeState(EnemyCharacter::State next) {
		return true;
	}


	// ���̃X�e�[�g�̏������I�����Ă��邩�̃t���O��ԋp�B
	inline bool GetIsEnd() const {
		return _IsEndState;
	}

protected:
	// ���[�J���X�e�[�g�؂�ւ��֐��B
	void _ChangeLocalState(EnemyCharacter::State next);

	// �X�e�[�g�����I���֐��B
	inline void _EndState() {
		_IsEndState = true;
	}
private:
	// �p����ł̐؂�ւ��������������B
	// ���p����ŕK���㏑�����Ďg�p�B
	virtual void _EntrySubClass() = 0;

	// �X�V�J�n�O�������B
	// ���X�V���n�܂钼�O�Ɉ�x�����Ă΂��֐��B
	// ���p����ŏ㏑�����Ďg�p�B
	virtual void _Start() {};

	// �p����ł̍X�V�����B
	// ���p����ŕK���㏑�����Ďg�p�B
	virtual void _UpdateSubClass() = 0;

	// ���݂̃��[�J���X�e�[�g���I�������ۂɌĂ΂��R�[���o�b�N�֐��B
	// �����͌��݂̃��[�J���X�e�[�g�̓Y�����B
	// ���������̂͌p����ɈϏ��B
	virtual void _EndNowLocalState_CallBack(EnemyCharacter::State now) {};
protected:
	bool _IsEndState = false;		// �X�e�[�g�̏������I���������̃t���O(true�ŏI��)�B
	EnemyCharacter* _EnemyObject = nullptr;	// ���̃X�e�[�g�����G�l�~�[�̃|�C���^�B
	EnemyState* _NowLocalState = nullptr;	// ���݂̃��[�J���X�e�[�g�B
private:
	EnemyCharacter::State _NowLocalStateIdx;		// ���݂̃��[�J���X�e�[�g�̓Y�����B
	bool _IsFirstUpdate = true;	// �X�e�[�g���؂�ւ���Ă���ŏ��̍X�V���B
};

