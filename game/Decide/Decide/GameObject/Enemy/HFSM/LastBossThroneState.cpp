#include"stdafx.h"
#include "../EnemyCharacter.h"
#include "LastBossThroneState.h"
#include "EnemyTranslationState.h"
#include "EnemyWaitState.h"
#include "GameObject\Enemy\BossGhost.h"
#include "GameObject\Enemy\HFSM\GhostPairAttackState.h"

//const int LastBossThroneState::_entourageNum = 2;

// テスト。
const int LastBossThroneState::_entourageNum = 2;

LastBossThroneState::LastBossThroneState(EnemyCharacter* Object) : EnemyState(Object)
{
}


LastBossThroneState::~LastBossThroneState()
{
}

void LastBossThroneState::_EntrySubClass() {
	_EnemyObject->PlayAnimation_Loop(EnemyCharacter::AnimationType::Idle, 0.1f);

	// 側近召喚。
	float dir = 1.0f;
	float startAttackOffset = 1.0f;	// 共同攻撃開始タイミングをずらす処理。
	for (int idx = 0; idx < _entourageNum; idx++) {
		_entourageEnemys.push_back(INSTANCE(GameObjectManager)->AddNew<BossGhost>("_entourageEnemy", 1));
		vector<BarColor> color;
		color.push_back(BarColor::Red);
		vector<int> param = vector<int>(CharacterParameter::Param::MAX,0);
		param[static_cast<int>(CharacterParameter::Param::HP)] = _EnemyObject->GetMaxHP() / 4;
		param[static_cast<int>(CharacterParameter::Param::MP)] = _EnemyObject->GetMaxMP()/ 4;

		for (int i = CharacterParameter::Param::ATK; i < CharacterParameter::Param::MAX;i++) {
			param[i] = _EnemyObject->GetPigmentParam(static_cast<CharacterParameter::Param>(i)) / 4;
		}

		// テスト。
		//param[static_cast<int>(CharacterParameter::Param::HP)] = 10;

		_entourageEnemys[idx]->SetParamAll(color, param);
		_entourageEnemys[idx]->transform->SetPosition(_EnemyObject->transform->GetPosition() + (_EnemyObject->transform->GetForward() * 3.0f * dir));
		_entourageEnemys[idx]->SetIntervalStartPairAttack(startAttackOffset);
		startAttackOffset += 0.97f;
		if (idx >= 1) {
			// 側近を二体とも生成した。

			// お互いをペアとして登録。
			_entourageEnemys[idx]->SetPairGhost(_entourageEnemys[idx - 1]);
			_entourageEnemys[idx - 1]->SetPairGhost(_entourageEnemys[idx]);
		}
		dir *= -1.0f;
	}

	_isCommand = false;
	_timeCounter = 0.0f;
}

void LastBossThroneState::_Start() {
	// のけぞり設定。
	_EnemyObject->ConfigDamageReaction(false);
}

void LastBossThroneState::_UpdateSubClass() {
	// 常にプレイヤーと距離判定し、バトル範囲外に出たら初期ステートに戻す。
	if (_EnemyObject->IsOutsideDiscovery()) {
		_EnemyObject->ChangeStateRequest(_EnemyObject->GetInitState());
	}

	// 側近が二体とも倒されればステート終了。
	bool isEnd = true;	
	int endWarpNum = 0;	// 攻撃開始の準備が完了している側近の数。
	for (auto itr = _entourageEnemys.begin(); itr != _entourageEnemys.end();) {
		if ((*itr)->GetNowState() && (*itr)->GetNowStateIndex() == EnemyCharacter::State::Death) {
			for (auto entourage : _entourageEnemys) {
				// お互いのペア関係を解除。
				entourage->SetPairGhost(nullptr);
			}
			itr = _entourageEnemys.erase(itr);
		}
		else {
			isEnd = false;
			itr++;
		}
	}

	_timeCounter += Time::DeltaTime();

	// 一定時間と確率で挑発とプレイヤーへのデバフ、側近へのバフと攻撃命令を行う。
	if (_timeCounter >= _interval) {
		//int rnd = rand() % 2;
		//if (rnd == 0) {
		_isCommand = true;
		//}
		_timeCounter = 0.0f;
	}

	if (_isCommand) {
		if (_entourageEnemys.size() == _entourageNum) {
			// 側近が一体も減っていない。
			for (auto enemy : _entourageEnemys) {
				enemy->SetIsCommand(true);
			}
			BossGhost::SelectPairAttack();
			_isCommand = false;
		}
	}

	if (isEnd) {
		_EndState();
	}
}

void LastBossThroneState::_ExitSubClass(EnemyCharacter::State next) {
	// 側近を削除。
	if (_entourageEnemys.size() > 0) {
		// プレイヤーがラスボスとの戦闘範囲外に出た。

		for (auto& enemy : _entourageEnemys) {
			INSTANCE(GameObjectManager)->AddRemoveList(enemy);
		}
	}
	_entourageEnemys.clear();

	// 玉座を消す。

}

void LastBossThroneState::_EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType) {
}
