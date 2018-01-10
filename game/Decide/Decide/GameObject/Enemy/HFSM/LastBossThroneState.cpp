#include"stdafx.h"
#include "../EnemyCharacter.h"
#include "LastBossThroneState.h"
#include "EnemyTranslationState.h"
#include "EnemyWaitState.h"
#include "GameObject\Enemy\BossGhost.h"
#include "GameObject\Enemy\HFSM\GhostPairAttackState.h"
#include "GameObject\Enemy\HFSM\EnemyChaceState.h"
#include "GameObject\Player\Player.h"
#include "GameObject\Enemy\HFSM\EnemyBackStepState.h"
#include "GameObject\Enemy\LastBoss.h"

//const int LastBossThroneState::_entourageNum = 2;

// テスト。
const int LastBossThroneState::_entourageNum = 2;

LastBossThroneState::LastBossThroneState(EnemyCharacter* Object) : EnemyState(Object)
{
	_player = static_cast<Player*>(INSTANCE(GameObjectManager)->FindObject("Player"));
	_playAnimation = EnemyCharacter::AnimationType::Idle;
	_interpolate = 0.3f;
	_loopNum = -1;
}


LastBossThroneState::~LastBossThroneState()
{
}

bool LastBossThroneState::EntourageCommand() {
	if (_entourageEnemys.size() == _entourageNum) {
		// 側近が一体も減っていない。
		for (auto enemy : _entourageEnemys) {
			enemy->SetIsCommand(true);
		}
		BossGhost::SelectPairAttack();
		return true;
	}
	return false;
}

void LastBossThroneState::EncourageBuff() {
	int value[CharacterParameter::Param::MAX];
	for (int idx = 0; idx < static_cast<int>(CharacterParameter::Param::MAX); idx++) {
		value[idx] = 0;
	}
	if (_entourageEnemys.size() == _entourageNum) {
		// 側近が一体も減っていない。

		value[static_cast<int>(CharacterParameter::Param::ATK)] = 10;
		value[static_cast<int>(CharacterParameter::Param::MAT)] = 10;
		value[static_cast<int>(CharacterParameter::Param::DEF)] = 30;
		value[static_cast<int>(CharacterParameter::Param::MDE)] = 30;
	}
	else {
		value[static_cast<int>(CharacterParameter::Param::ATK)] = 50;
		value[static_cast<int>(CharacterParameter::Param::MAT)] = 50;
		value[static_cast<int>(CharacterParameter::Param::DEF)] = 60;
		value[static_cast<int>(CharacterParameter::Param::MDE)] = 60;
	}

	for (auto enemy : _entourageEnemys) {
		enemy->BuffAndDebuff(value,30.0f);
	}
}

void LastBossThroneState::_EntrySubClass() {

	// 側近召喚。
	float dir = 1.0f;
	float startAttackOffset = 1.0f;	// 共同攻撃開始タイミングをずらす処理。
	for (int idx = 0; idx < _entourageNum; idx++) {
		_entourageEnemys.push_back(INSTANCE(GameObjectManager)->AddNew<BossGhost>("_entourageEnemy", 1));
		vector<BarColor> color;
		color.push_back(BarColor::Red);
		vector<int> param = vector<int>(CharacterParameter::Param::MAX,0);

		param[CharacterParameter::Param::HP] = 8500;
		param[CharacterParameter::Param::ATK] = 500;
		param[CharacterParameter::Param::MAT] = 500;
		param[CharacterParameter::Param::DEF] = 50;
		param[CharacterParameter::Param::MDE] = 50;
		param[CharacterParameter::Param::DEX] = 10;
		param[CharacterParameter::Param::CRT] = 10;

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

	_isDeathEntourage = false;
	_timeCounter = 0.0f;
}

void LastBossThroneState::_StartSubClass() {
	// のけぞり設定。
	_EnemyObject->ConfigDamageReaction(false);
}

void LastBossThroneState::_UpdateSubClass() {

	// 常にプレイヤーと距離判定し、バトル範囲外に出たら初期ステートに戻す。
	if (_EnemyObject->IsOutsideDiscovery()) {
		_EnemyObject->ChangeStateRequest(_EnemyObject->GetInitState());
	}

	if (_isDeathEntourage) {
		if (!_EnemyObject->GetIsPlaying()) {
			// 玉座を消したらステート終了。
			_EndState();
		}
	}
	else {
		_isDeathEntourage = true;
		// 側近が二体とも倒されればステート終了。
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
				_isDeathEntourage = false;
				itr++;
			}
		}

		if (_isDeathEntourage) {
			_EnemyObject->PlayAnimation_OriginIndex(static_cast<int>(LastBoss::AnimationLastBoss::ThroneEnd), 0.3f, 1, 0);
			return;
		}

		_timeCounter += Time::DeltaTime();

		_EnemyObject->LookAtObject(_player);


		// 一定時間と確率で挑発とプレイヤーへのデバフ、側近へのバフと攻撃命令を行う。
		if (_timeCounter >= _interval) {
			_ChangeLocalState(EnemyCharacter::State::StartAttack);
			_timeCounter = 0.0f;
		}

		if (_NowLocalStateIdx != EnemyCharacter::State::StartAttack) {
			// 何もアクションをしていない。

			// 常にプレイヤーと一定の距離を取る。
			float minRange = 6.0f;	// 6メートルは最低でも離れる。
			float maxRange = 7.0f;	// 7メートルより遠ければ近寄る。

			Vector3 enemyToPlayer = _player->transform->GetPosition() - _EnemyObject->transform->GetPosition();
			float length = enemyToPlayer.Length();
			if (length >= minRange && length <= maxRange) {
				// プレイヤーとの距離が一定範囲内。


				if (_NowLocalStateIdx == EnemyCharacter::State::BackStep) {
					_ChangeLocalState(EnemyCharacter::State::None);
					_PlayAnimation();
				}

			}
			else {
				// プレイヤーとの距離が許容範囲外になった。

				if (_NowLocalStateIdx != EnemyCharacter::State::BackStep) {
					_ChangeLocalState(EnemyCharacter::State::BackStep);
					static_cast<EnemyBackStepState*>(_NowLocalState)->SetAddPower(10.0f);
					static_cast<EnemyBackStepState*>(_NowLocalState)->SetMaxPower(5.0f);
				}

				if (_NowLocalStateIdx == EnemyCharacter::State::BackStep) {
					if (length >= maxRange) {
						// 離れすぎている。

						// 許容範囲まで追跡。
						static_cast<EnemyBackStepState*>(_NowLocalState)->SetDirection(_EnemyObject->transform->GetForward());
					}
					else {
						// 距離が近すぎる。

						// 許容範囲まで後退。
						static_cast<EnemyBackStepState*>(_NowLocalState)->SetDirection(_EnemyObject->transform->GetForward() * -1.0f);
					}
				}
			}
		}
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
