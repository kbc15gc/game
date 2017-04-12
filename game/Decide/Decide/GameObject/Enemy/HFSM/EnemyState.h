#pragma once

class EnemyCharacter;

// ���N���X�B
// �G���ƃ{�X�̃X�e�[�g�B
class EnemyState
{
public:
	EnemyState(EnemyCharacter* Object);
	virtual ~EnemyState();

	virtual void Entry();

	// �ԋp�l�͍X�V�������I���������̃t���O�B
	virtual bool Update();

	// �����͎��̃X�e�[�g�B
	virtual void Exit(EnemyCharacter::State next);

protected:
	// ���[�J���X�e�[�g�؂�ւ��֐��B
	void _ChangeLocalState(EnemyCharacter::State next);

private:
	// ���݂̃��[�J���X�e�[�g���I�������ۂɌĂ΂��R�[���o�b�N�֐��B
	// �����͌��݂̃��[�J���X�e�[�g�̓Y�����B
	// ���������̂͌p����ɈϏ��B
	virtual void _EndNowLocalState_CallBack(EnemyCharacter::State now) {};
private:
	EnemyCharacter* _EnemyObject = nullptr;	// ���̃X�e�[�g�����G�l�~�[�̃|�C���^�B

	EnemyState* _NowLocalState = nullptr;	// ���݂̃��[�J���X�e�[�g�B
	EnemyCharacter::State _NowLocalStateIdx;		// ���݂̃��[�J���X�e�[�g�̓Y�����B

	bool _IsEndState = false;		// �X�e�[�g�̏������I���������̃t���O(true�ŏI��)�B
};

