#pragma once

#include "EnemyState.h"

class EnemyCharacter;

// �G�l�~�[���i�X�e�[�g�B
class EnemyTranslationState : public EnemyState {
public:
	EnemyTranslationState(EnemyCharacter* Object);
	~EnemyTranslationState();


	void Exit(EnemyCharacter::State next)override;


	// �ړ�����������w��B
	inline void SetDir(const Vector3& dir) {
		_Direction = dir;
		// ���K���B
		_Direction.Normalize();
	}

	// �ړ��ʐݒ�B
	inline void SetLength(const float length) {
		_MoveLength = length;
	}

	// �ړ����x�ݒ�(���b)�B
	inline void SetMoveSpeed(const float speed) {
		_MoveSpeed = speed;
	}

	// ���̊֐���true��ݒ肷��ƁA�p�j�͈͊O�ɏo����X�e�[�g���I������悤�ɂȂ�B
	inline void SetIsWandering(bool flg) {
		_isWandering = flg;
	}

	// �p�j�͈͊O�ɏo�����ԋp
	inline bool GetIsOutsideRange() const {
		return _isOutsideRange;
	}
private:
	void _EntrySubClass()override;

	void _Start()override;

	void _UpdateSubClass()override;

	void _EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType);

private:
	Vector3 _Direction;	// �i�ޕ����B
	float _MoveLength;	// �ړ��ʁB
	float _MoveSpeed;	// �ړ�����X�s�[�h(���b)�B
	bool _isOutsideRange = false;
	// �ȉ���Ɨp�B
	Vector3 _InitPos;	// �����ʒu�B
	bool _isWandering = false;	// �p�j���̒��i���B
};