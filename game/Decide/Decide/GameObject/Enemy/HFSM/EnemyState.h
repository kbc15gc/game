#pragma once

class EnemyCharacter;

// ���N���X�B
// �G���ƃ{�X�̃X�e�[�g�B
class EnemyState
{
public:
	EnemyState(EnemyCharacter* Object);
	virtual ~EnemyState();

	// �������B
	// �����̃X�e�[�g�ɐ؂�ւ�����ۂɌĂ΂��B
	virtual void Entry();

	// �X�e�[�g�X�V�����B
	// �ԋp�l�͍X�V�������I���������̃t���O�B
	// �����̊֐��͋��ʏ����B
	bool Update();

	// �����͎��̃X�e�[�g�B
	// �����̃X�e�[�g�Ɉڍs����O�ɌĂ΂��B
	virtual void Exit(EnemyCharacter::State next);

protected:
	// ���[�J���X�e�[�g�؂�ւ��֐��B
	void _ChangeLocalState(EnemyCharacter::State next);

private:
	// �p����ł̍X�V�����B
	// ���p����ŏ㏑�����Ďg�p�B
	virtual void _UpdateSubClass() = 0;

	// ���݂̃��[�J���X�e�[�g���I�������ۂɌĂ΂��R�[���o�b�N�֐��B
	// �����͌��݂̃��[�J���X�e�[�g�̓Y�����B
	// ���������̂͌p����ɈϏ��B
	virtual void _EndNowLocalState_CallBack(EnemyCharacter::State now) {};
protected:
	bool _IsEndState = false;		// �X�e�[�g�̏������I���������̃t���O(true�ŏI��)�B
	EnemyCharacter* _EnemyObject = nullptr;	// ���̃X�e�[�g�����G�l�~�[�̃|�C���^�B
private:

	EnemyState* _NowLocalState = nullptr;	// ���݂̃��[�J���X�e�[�g�B
	EnemyCharacter::State _NowLocalStateIdx;		// ���݂̃��[�J���X�e�[�g�̓Y�����B
};

