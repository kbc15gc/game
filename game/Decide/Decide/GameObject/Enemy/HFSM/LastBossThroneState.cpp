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
		dir *= -1.0f;
	}
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

	// 一定時間と確率で挑発とプレイヤーへのデバフ、側近へのバフと攻撃命令を行う。

	// 側近が二体とも倒されればステート終了。
	bool isEnd = true;	
	int endWarpNum = 0;	// 攻撃開始の準備が完了している側近の数。
	for (auto itr = _entourageEnemys.begin(); itr != _entourageEnemys.end();) {
		if ((*itr)->GetNowState() && (*itr)->GetNowStateIndex() == EnemyCharacter::State::Death) {
			itr = _entourageEnemys.erase(itr);
		}
		else {

			if (_entourageEnemys.size() == _entourageNum) {
				// 側近が一体も減っていない。

				if (static_cast<int>((*itr)->GetNowStateIndex()) == static_cast<int>(BossGhost::BossGhostState::BossGhostPairAttack)) {
					// 共同攻撃状態。
					if (static_cast<GhostPairAttackState*>((*itr)->GetNowState())->GetIsEndWarp()) {
						// 今参照している側近の攻撃開始準備が完了している。
						endWarpNum++;

						if (endWarpNum >= _entourageNum) {
							// すべての側近の攻撃準備が完了した。

							float startAttackOffset = 1.0f;	// 攻撃開始タイミングをずらす処理。
							for (auto enemy : _entourageEnemys) {
								// 側近に攻撃を開始させる。

								static_cast<GhostPairAttackState*>(enemy->GetNowState())->SetIsStartAttack(true);
								static_cast<GhostPairAttackState*>(enemy->GetNowState())->SetStartAttackInterval(startAttackOffset);
								startAttackOffset += 1.0f;
							}
						}
					}
				}

				// 暫定処理。
				// ※そのうち確率か経過時間制にする。
				{
					static_cast<BossGhost*>(*itr)->SetIsCommand(true); 
				}
			}

			isEnd = false;
			itr++;
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
