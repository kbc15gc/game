#pragma once
#include "EnemyState.h"

class BossGhost;

// ���X�{�X�ʍ�(���ߏ����A�v���C���[�Ƀf�o�t�A�������G���)�X�e�[�g�B
class LastBossThroneState : public EnemyState {
public:
	LastBossThroneState(EnemyCharacter* Object);
	~LastBossThroneState();
private:
	void _EntrySubClass()override;

	void _Start()override;

	void _UpdateSubClass()override;

	void _ExitSubClass(EnemyCharacter::State next)override;

	void _EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType);

private:
	bool _isOutsideRange = false;	// �͈͊O�ɏo�����B
	float _timeCounter;

	static const int _entourageNum;	// ���߂̐��B
	vector<BossGhost*> _entourageEnemys;	// ���߁B
};