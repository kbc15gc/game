#include"stdafx.h"
#include "../EnemyCharacter.h"
#include "EnemyWanderingState.h"
#include "EnemyTranslationState.h"
#include "EnemyWaitState.h"

EnemyWanderingState::EnemyWanderingState(EnemyCharacter* Object) : EnemyState(Object)
{
}


EnemyWanderingState::~EnemyWanderingState()
{
}

void EnemyWanderingState::_EntrySubClass() {
	// �ŏ��̃��[�J���X�e�[�g�Ɉڍs�B
	_ChangeLocalState(EnemyCharacter::State::Translation);	// �ړ��X�e�[�g�Ɉڍs�B

	if (_isOutsideRange) {
		// �O�̜p�j�����Ŕ͈͊O�ɏo���B

		Vector3 dir = _EnemyObject->transform->GetForward();
		dir = dir * -1.0f;	// �������]�B
		// �G�l�~�[����]������B
		_EnemyObject->LookAtDirectionInterpolate(dir,0.5f);

		// �t���O�������B
		_isOutsideRange = false;
	}
	else {
		// �O�̜p�j�����Ŕ͈͓��ɂ����B

		// �����_���Ői�ޕ��������肷��B
		int selectNum = 4;
		float rad = D3DXToRadian(360.0f);
		int rnd = rand() % selectNum;
		rad = rad / (rnd + 1);
		_EnemyObject->RotationAxisInterpolate(Vector3::up, rad,0.5f);
	}

	// �p�����[�^�ݒ�B
	float length = 5.0f;
	float speed = _EnemyObject->GetWalkSpeed();
	_TranslationPalam(_EnemyObject->transform->GetForward(), speed * 5.0f, speed);
}

void EnemyWanderingState::_StartSubClass () {
	// �̂�����ݒ�B
	_EnemyObject->ConfigDamageReaction(true, 1);
}

void EnemyWanderingState::_UpdateSubClass() {
	// ���G���Ȃ̂Ŏ���p������s���B
	_EnemyObject->SearchView();
}

void EnemyWanderingState::_EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType) {
	// ���݂̃��[�J���X�e�[�g�̏������I�������B
	if (EndLocalStateType == EnemyCharacter::State::Translation) {
		// �ړ��X�e�[�g�I���B

		//if (_NowLocalState) {
		// ���݂̃X�e�[�g�͂��łɔj�����ꂽ��Ȃ̂ŁA�z�񂩂�擾����B

		_isOutsideRange = static_cast<EnemyTranslationState*>(_EnemyObject->GetMyState()[static_cast<int>(EnemyCharacter::State::Translation)].get())->GetIsOutsideRange();
		// �ҋ@�X�e�[�g�Ɉڍs�B
		_ChangeLocalState(EnemyCharacter::State::Wait);
		// �p�����[�^�ݒ�B
		static_cast<EnemyWaitState*>(_NowLocalState)->SetInterval(4.5f);
		//}
	}
	else if (EndLocalStateType == EnemyCharacter::State::Wait) {
		// �ҋ@�X�e�[�g�I���B

		// �p�j�X�e�[�g��������I���B
		_EndState();
	}
}

void EnemyWanderingState::_TranslationPalam(const Vector3& dir, float length, float speed) {
	static_cast<EnemyTranslationState*>(_NowLocalState)->SetDir(dir);
	static_cast<EnemyTranslationState*>(_NowLocalState)->SetLength(length);
	static_cast<EnemyTranslationState*>(_NowLocalState)->SetMoveSpeed(speed);
	static_cast<EnemyTranslationState*>(_NowLocalState)->SetIsWandering(true);
}