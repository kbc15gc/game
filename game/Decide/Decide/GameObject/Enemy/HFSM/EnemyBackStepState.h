#pragma once

#include "EnemyState.h"

// �G�l�~�[�o�b�N�X�e�b�v�X�e�[�g�B
class EnemyBackStepState : public EnemyState {
public:
	EnemyBackStepState(EnemyCharacter* Object);
	~EnemyBackStepState();

	// �ړ�����������w��B
	inline void SetDirection(const Vector3& dir) {
		_Direction = dir;
		// ���K���B
		_Direction.Normalize();
	}
	inline void SetAddPower(float add) {
		_addPower = add;
	}
	inline void SetMaxPower(float max) {
		_maxPower = max;
	}

private:
	void _EntrySubClass()override;

	void _StartSubClass()override;

	void _UpdateSubClass()override;

	void _ExitSubClass(EnemyCharacter::State next)override {};

	void _EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType);




private:
	float _interval = 0.0f;// �o�b�N�X�e�b�v�J�n�܂ł̑҂�����(�A�j���[�V�����ȂǂŃ^�C�~���O������邱�Ƃ����邽��)�B
	Vector3 _Direction;	// �i�ޕ����B
	bool _isBack = false;
	bool _isAddPower = false;	// ���Z���������B
	float _nowBackPower = 0.0f;	// ��ގ��̃o�l�p���[�B
	float _addPower = 6.0;
	float _maxPower = 3.0f;

	float _counter = 0.0f;
};