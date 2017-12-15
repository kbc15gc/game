#pragma once

#include "EnemyState.h"

class Player;

// �G�l�~�[�ǐՃX�e�[�g�B
class EnemyChaceState : public EnemyState {
public:
	EnemyChaceState(EnemyCharacter* Object);
	~EnemyChaceState();

	inline void SetEndRange(float range) {
		_endRange = range;
	}


private:
	void _EntrySubClass()override;

	void _StartSubClass()override;

	void _UpdateSubClass()override;

	void _ExitSubClass(EnemyCharacter::State next)override;

	void _EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType);

	bool IsPossibleChangeState(EnemyCharacter::State next)override;


private:
	Player* _Player = nullptr;
	float _Speed = 0.0f;	// �ړ����x�B
	bool _isOutside = false;
	float _endRange = 0.0f;	// �ǐՏI�������B
	bool _isRotationinterpolate = false;// ��]���Ԃ��邩�B
};