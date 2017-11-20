#pragma once
#include "EnemyState.h"

// ���X�{�X���j���(���j�������A���䑕�u�ғ��A�喂�@�����A�������G���)�X�e�[�g�B
class LastBossHistoryTamperingState : public EnemyState {
public:
	LastBossHistoryTamperingState(EnemyCharacter* Object);
	~LastBossHistoryTamperingState();
private:
	void _EntrySubClass()override;

	void _StartSubClass()override;

	void _UpdateSubClass()override;

	void _ExitSubClass(EnemyCharacter::State next)override;

	void _EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType);

private:
	bool _isOutsideRange = false;	// �͈͊O�ɏo�����B

	vector<GameObject*> _historyBuildings;	// ���䑕�u�B
};