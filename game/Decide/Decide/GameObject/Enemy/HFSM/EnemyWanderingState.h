#pragma once
#include "EnemyState.h"

// �G�l�~�[�p�j�X�e�[�g�B
class EnemyWanderingState : public EnemyState {
public:
	EnemyWanderingState(EnemyCharacter* Object);
	~EnemyWanderingState();
private:
	void _EntrySubClass()override;

	void _StartSubClass()override;

	void _UpdateSubClass()override;

	void _ExitSubClass(EnemyCharacter::State next)override {};

	void _EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType);

	// �ړ��X�e�[�g�p�����[�^�̕ێ��B
	// �����F	�����B
	//			�ړ��ʁB
	//			�ړ����x�B
	void _TranslationPalam(const Vector3& dir,float length,float speed);

private:
	bool _isOutsideRange = false;	// �͈͊O�ɏo�����B
};