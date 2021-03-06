#include "stdafx.h"
#include "GameObject\Enemy\BreathObject.h"
#include "GameObject\Enemy\EnemyCharacter.h"
#include "../Village/EventManager.h"

void BreathObject::LateUpdate() {
	if (_enemyObject) {
		if (_enemyObject->GetNowStateIndex() == EnemyCharacter::State::Death) {
			// エネミー死亡。
			_enemyObject = nullptr;
			BreathPlay();
		}
		else {
			if (_enemyObject->GetIsStopUpdate() || !_enemyObject->GetActive()) {
				BreathStop();
			}
			else {
				BreathPlay();
			}
		}
	}
	else {
		if (INSTANCE(EventManager)->IsEvent()) {
			BreathStop();
		}
		else {
			BreathPlay();
		}
	}
};
