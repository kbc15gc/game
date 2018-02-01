#pragma once

#include "GameObject\Enemy\EnemyCharacter.h"
#include "GameObject\Enemy\EnemyManager.h"

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

	virtual void LateUpdate() {};

	// �����͎��̃X�e�[�g�B
	// �����̃X�e�[�g�Ɉڍs����ۂɌĂ΂��B
	void Exit(EnemyCharacter::State next);

	// ���̃X�e�[�g����ڍs�ł���X�e�[�g��I�ʂ���֐��B
	// ���f�t�H���g�ł͂��ׂẴX�e�[�g�Ɉڍs�ł���B
	// ���p����ŏ㏑�����Ď����B
	inline virtual bool IsPossibleChangeState(EnemyCharacter::State next) {
		return true;
	}

	// ���̃X�e�[�g���Ƀ_���[�W��^�����邩�B
	// ���f�t�H���g�̓_���[�W��^������B
	// ���p����ŏ㏑�����Ď����B
	inline virtual bool IsPossibleDamage() {
		return true;
	}

	// �X�e�[�g�����I���v���֐��B
	// ���O�����狭���I�ɃX�e�[�g�������I�����������ꍇ�͂�������ĂԁB
	inline void _EndStateRequest() {
		_EndState();
	}


	// ���̃X�e�[�g�̏������I�����Ă��邩�̃t���O��ԋp�B
	inline bool GetIsEnd() const {
		return _IsEndState;
	}

	// �w�肵���l�Ŏg�p����A�j���[�V�������Ԏ��Ԃ�����������B
	inline void CustamParameter(EnemyCharacter::AnimationType type, float interpolate,int loopNum,int eventNo,float speed) {
		_playAnimation = type;
		_interpolate = interpolate;
		_loopNum = loopNum;
		_eventNo = eventNo;
		_playSpeed = speed;
	}

	EnemyCharacter::AnimationType GetPlayAnimationType()const {
		return _playAnimation;
	}

	float GetInterpolate()const {
		return _interpolate;
	}

	int GetEventNo()const {
		return _eventNo;
	}

	int GetLoopNum()const {
		return _loopNum;
	}

	float GetPlaySpeed()const {
		return _playSpeed;
	}

	virtual void SetActive(bool flg) {
		_isActive = flg;
	};

protected:
	// ���[�J���X�e�[�g�؂�ւ��֐��B
	void _ChangeLocalState(EnemyCharacter::State next);

	// �X�e�[�g�����I���֐��B
	inline void _EndState() {
		_IsEndState = true;
	}

	void _PlayAnimation() {
		_saveSpeed = _EnemyObject->GetAnimationSpeed();
		_EnemyObject->SetAnimationSpeed(_playSpeed);
		_EnemyObject->PlayAnimation(_playAnimation, _interpolate, _loopNum, _eventNo);
	}

private:
	// �p����ł̐؂�ւ��������������B
	// ���p����ŕK���㏑�����Ďg�p�B
	virtual void _EntrySubClass() = 0;

	void Start() {
		_PlayAnimation();
		_StartSubClass();
	}

	// �X�V�J�n�O�������B
	// ���X�V���n�܂钼�O�Ɉ�x�����Ă΂��֐��B
	// ���p����ŏ㏑�����Ďg�p�B
	virtual void _StartSubClass() {};

	// �p����ł̍X�V�����B
	// ���p����ŕK���㏑�����Ďg�p�B
	virtual void _UpdateSubClass() = 0;

	// �����͎��̃X�e�[�g�B
	// �����̃X�e�[�g�Ɉڍs����ۂɌĂ΂��B
	// ���p����Ŏ����B
	virtual void _ExitSubClass(EnemyCharacter::State next) = 0;

	// ���݂̃��[�J���X�e�[�g���I�������ۂɌĂ΂��R�[���o�b�N�֐��B
	// �����͌��݂̃��[�J���X�e�[�g�̓Y�����B
	// �����̊֐��͊e�X�e�[�g�������I�ɏI�������ꍇ�ɂ̂݌Ăяo�����B
	// ���������̂͌p����ɈϏ��B
	virtual void _EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType) {};


protected:

	EnemyCharacter::AnimationType _playAnimation = EnemyCharacter::AnimationType::None;
	float _interpolate = 0.2f;
	int _loopNum = 1;
	int _eventNo = 0;
	float _playSpeed = 1.0f;
	float _saveSpeed;

	bool _IsEndState = false;		// �X�e�[�g�̏������I���������̃t���O(true�ŏI��)�B
	EnemyCharacter* _EnemyObject = nullptr;	// ���̃X�e�[�g�����G�l�~�[�̃|�C���^�B
	EnemyState* _NowLocalState = nullptr;	// ���݂̃��[�J���X�e�[�g�B
	EnemyCharacter::State _NowLocalStateIdx;		// ���݂̃��[�J���X�e�[�g�̓Y�����B
private:

	bool _IsFirstUpdate = true;	// �X�e�[�g���؂�ւ���Ă���ŏ��̍X�V���B

	bool _isActive = true;
};

