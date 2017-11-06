#include"stdafx.h"
#include "../EnemyCharacter.h"
#include "LastBossThroneState.h"
#include "EnemyTranslationState.h"
#include "EnemyWaitState.h"
#include "GameObject\Enemy\BossGhost.h"

//const int LastBossThroneState::_entourageNum = 2;

// テスト。
const int LastBossThroneState::_entourageNum = 1;

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

	// 一定時間と確率で挑発とプレイヤーへのデバフを行う。

	// 側近が二体とも倒されればステート終了。
	bool isEnd = true;

	for (auto itr = _entourageEnemys.begin(); itr != _entourageEnemys.end();) {
		if ((*itr)->GetNowState() && (*itr)->GetNowStateIndex() == EnemyCharacter::State::Death) {
			itr = _entourageEnemys.erase(itr);
		}
		else {
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
	_entourageEnemys.clear();

	// 玉座を消す。

}

void LastBossThroneState::_EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType) {
}
