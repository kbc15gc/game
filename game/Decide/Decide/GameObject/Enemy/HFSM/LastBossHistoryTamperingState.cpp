#include"stdafx.h"
#include "../EnemyCharacter.h"
#include "LastBossHistoryTamperingState.h"
#include "EnemyTranslationState.h"
#include "EnemyWaitState.h"
#include "GameObject\Enemy\LastBoss.h"
#include "GameObject\History\HistoryManager.h"
#include "GameObject\Village\ContinentObject.h"

LastBossHistoryTamperingState::LastBossHistoryTamperingState(EnemyCharacter* Object) : EnemyState(Object)
{
	//_buildingsParent.reset(new Transform(nullptr,nullptr));
	//_buildingsParent->SetPosition(_EnemyObject->transform->GetPosition());
	//_buildingsParent->SetRotation(Quaternion::Identity);
	// INSTANCE(HistoryManager)->CreateBuilding("Asset/Data/LastBossStageEquipment/Test.csv", _historyBuildings);
	// for (auto obj : _historyBuildings) {
	//	 // 吐き出した座標をバトルフィールドの座標系に変換。
	//	 obj->transform->SetParent(_buildingsParent.get());
	//	 obj->transform->SetLocalPosition(obj->transform->GetPosition());
	//	 obj->transform->SetLocalRotation(obj->transform->GetRotation());
	// }
}


LastBossHistoryTamperingState::~LastBossHistoryTamperingState()
{
}

void LastBossHistoryTamperingState::_EntrySubClass() {
	// 無敵発動。
	
	// 歴史書召喚。
}

void LastBossHistoryTamperingState::_StartSubClass() {
	// のけぞり設定。
	_EnemyObject->ConfigDamageReaction(false);
}

void LastBossHistoryTamperingState::_UpdateSubClass() {
	// 常にプレイヤーと距離判定し、バトル範囲外に出たら初期ステートに戻す。

	// 舞台装置を発動させたのち、極大攻撃発動。

	// 大魔法が撃たれるとステート終了。

	// 大魔法がキャンセルされるとダウンに移行。
	_EnemyObject->ChangeStateRequest(static_cast<EnemyCharacter::State>(LastBoss::LastBossState::LastBossDown));
	
	for (auto obj : _historyBuildings) {
		// 吐き出した座標をバトルフィールドの座標系に変換。
		if (static_cast<ContinentObject*>(obj)->GetIsAddPhysicsWorld()) {
			OutputDebugString("LastBossBuild!");
		}
	}

	// ※暫定処理。
	//_EndState();
}

void LastBossHistoryTamperingState::_ExitSubClass(EnemyCharacter::State next) {
	// 歴史書を消す。

}

void LastBossHistoryTamperingState::_EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType) {
}
