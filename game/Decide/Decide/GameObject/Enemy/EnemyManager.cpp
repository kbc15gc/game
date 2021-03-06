#include "stdafx.h"
#include "EnemyManager.h"
#include "fbEngine\_Support\Support.h"
#include "EnemyCharacter.h"
#include "Enemy.h"
#include "GameObject\SplitSpace.h"
#include "GameObject\Enemy\BossDrarian.h"
#include "GameObject\Component\ParticleEffect.h"
#include "EnemyGolem.h"
#include "EnemySoldier.h"
#include "BossGolem.h"
#include "CodeNameD.h"
#include "GameObject\Enemy\LastBoss.h"
#include "GameObject\Enemy\BossGhost.h"

EnemyManager* EnemyManager::_instance = nullptr;


EnemyManager::EnemyManager() {
}

EnemyManager::~EnemyManager() {
	for (auto& enemyArray : _enemys) {
		for (auto& enemy : enemyArray) {
			enemy->Object = nullptr;
		}
		enemyArray.clear();
	}
	_enemys.clear();
}

void EnemyManager::Start() {
	// ※消すな。
	//エネミーリストを初期化。
	// ※シーン切り替え時にいったん削除しないと不正なメモリをオブジェクトの削除リストに積む処理が走るため絶対必要。
	 if (_enemys.size() > 0) {
		 for (int idx = 0; idx < static_cast<int>(_enemys.size()); idx++) {
			 if (_enemys[idx].size() > 0) {
				 _enemys[idx].clear();
			 }
		 }
	 }
}


void EnemyManager::CreateEnemys(LocationCodeE location, vector<unique_ptr<LoadEnemyInfo::EnemyInfo>>& infos) {
	if (location != LocationCodeE::None) {
		// エネミーを作成。
		if (static_cast<int>(_enemys.size()) < static_cast<int>(location) + 1) {
			_enemys.resize(static_cast<int>(location) + 1);
		}
	}
	else {
		return;
	}

	if (_enemys[static_cast<int>(location)].size() > 0) {
		// すでにエネミーが生成されている。
		for (auto& obj : _enemys[static_cast<int>(location)]) {
			if (obj->Object) {
				INSTANCE(GameObjectManager)->AddRemoveList(obj->Object);
				INSTANCE(GameObjectManager)->AddRemoveList(obj->Respawner);
			}
		}
		_enemys[static_cast<int>(location)].clear();
	}

	for (int idx = 0; idx < static_cast<int>(infos.size()); idx++)
	{
		unique_ptr<ManagingData> newData(new ManagingData);
		newData->Object = nullptr;

		vector<BarColor> barColor;
		switch (static_cast<EnemyCharacter::EnemyType>(infos[idx]->type))
		{
		case EnemyCharacter::EnemyType::Born:
			// 骨エネミー生成。
			newData->Object = INSTANCE(GameObjectManager)->AddNew<Enemy>("EnemyProt", 1);
			barColor.push_back(BarColor::Red);
			
			break;
		case EnemyCharacter::EnemyType::BossDrarian:
			// ボスドラリアン生成。
			newData->Object = INSTANCE(GameObjectManager)->AddNew<BossDrarian>("EnemyDrarian", 1);
			barColor.push_back(BarColor::Yellow);
			barColor.push_back(BarColor::Red);
			
			break;
		case EnemyCharacter::EnemyType::Drarian:
			// ドラリアン生成。

			// ※まだ作成しない。
			barColor.push_back(BarColor::Red);
			break;
		case EnemyCharacter::EnemyType::Golem:
			//ゴーレム生成。
			newData->Object = INSTANCE(GameObjectManager)->AddNew<EnemyGolem>("EnemyGolem", 1);
			barColor.push_back(BarColor::Red);
			newData->Object->SetBarPos({ 0.0f,4.0f,0.0f });
			newData->Object->SetBigflag(true);
			break;
		case EnemyCharacter::EnemyType::BossGolem:
			//ボスゴーレム生成。
			newData->Object = INSTANCE(GameObjectManager)->AddNew<BossGolem>("BossGolem", 1);
			barColor.push_back(BarColor::Yellow);
			barColor.push_back(BarColor::Red);
			break;

		case EnemyCharacter::EnemyType::Soldier:
			//兵士を生成。
			newData->Object = INSTANCE(GameObjectManager)->AddNew<EnemySoldier>("EnemySoldier", 1);
			barColor.push_back(BarColor::Red);
			break;
		case EnemyCharacter::EnemyType::BossD:
			//ボスD生成。
			newData->Object = INSTANCE(GameObjectManager)->AddNew<BossD>("BossD", 1);
			barColor.push_back(BarColor::Green);
			barColor.push_back(BarColor::Yellow);
			barColor.push_back(BarColor::Red);
			newData->Object->SetBarPos({ 0.0f,2.7f,0.0f });
			newData->Object->SetBigflag(true);
			break;
		case EnemyCharacter::EnemyType::Ghost:
			//ゴースト生成。
			newData->Object = INSTANCE(GameObjectManager)->AddNew<BossGhost>("LastBoss", 1);
			barColor.push_back(BarColor::Red);
			break;
		case EnemyCharacter::EnemyType::BossLast:
			//ラスボス生成。
			newData->Object = INSTANCE(GameObjectManager)->AddNew<LastBoss>("LastBoss", 1);
			barColor.push_back(BarColor::Blue);
			barColor.push_back(BarColor::Green);
			barColor.push_back(BarColor::Yellow);
			barColor.push_back(BarColor::Red);
			break;
		}

		if (newData->Object) {
			newData->Object->SetLocationCode(location);
			// infoを保存。
			newData->InfoData = move(infos[idx]);
			// Transform情報設定。
			newData->Object->transform->SetPosition(newData->InfoData->position);
			newData->Object->transform->SetRotation(newData->InfoData->rotation);
			newData->Object->transform->SetScale(newData->InfoData->scale);
			// パラメーター設定。
			newData->Object->SetParamAll(barColor, newData->InfoData->param);
			// ドロップ設定。
			newData->Object->SetDropEXP(newData->InfoData->exp);
			newData->Object->SetDropMoney(newData->InfoData->money);
			newData->Object->SetItem(newData->InfoData->item, newData->InfoData->armor, newData->InfoData->weapon);
			newData->Object->SetProbability(newData->InfoData->probability);
			//カラーを設定するフラグの場合。
			if (newData->InfoData->colorflag)
			{
				//カラーの設定
				Color c;
				c.Set(newData->InfoData->color.x, newData->InfoData->color.y, newData->InfoData->color.z, newData->InfoData->color.w);
				newData->Object->SetColor(c);
			}
			// 視野の距離設定。
			newData->Object->SetViewRange(newData->InfoData->viewRange);
			newData->Object->SetViewAngle(newData->InfoData->viewAngle);
			// 徘徊範囲設定。
			newData->Object->SetWanderingRange(newData->InfoData->wanderingRange);
			// 追跡範囲設定。
			newData->Object->SetDiscoveryRange(newData->InfoData->discoveryRange);

			_enemys[static_cast<int>(location)].push_back(move(newData));
		}
		else {
			// 生成失敗。
			// EnemyTypeに新種を追加した？。
			abort();
		}
	}
}


void EnemyManager::DeathEnemy(EnemyCharacter* object) {
	// エネミー削除予約。
	INSTANCE(GameObjectManager)->AddRemoveList(object);

	//バフデバフエフェクトを消す処理。
	ParticleEffect* pe = object->GetComponent<ParticleEffect>();
	if (pe) {
		pe->SetBuffEffectFlag(false);
		pe->SetDebuffEffectFlag(false);
		pe->SetBigMonsterDebuffEffectFlag(false);
	}

	//死んだのですべてのバフデバフアイコンを削除。
	BuffDebuffICon* icon = object->GetComponent<BuffDebuffICon>();
	if (icon) {
		icon->DeleteAllBuffDebuffIcon();
	}

	if (object->GetLocationCode() == LocationCodeE::None) {
		// 外部から読み込まれたデータではないので終了。
		return;
	}

	LocationCodeE code = object->GetLocationCode();
	for (auto& enemy : _enemys[static_cast<int>(code)]) {
		if (object == enemy->Object) {
			// このクラスで生成したエネミーが死亡している。
	
			// エネミーをリスポーン。
			vector<BarColor> barColor;
			ObjectSpawn* Spawner = enemy->Object->GetSpawner();
			if (Spawner) {
				switch (static_cast<EnemyCharacter::EnemyType>(enemy->InfoData->type)) {
				case EnemyCharacter::EnemyType::Born:
					enemy->Object = Spawner->DeathAndRespawnObject<Enemy>(nullptr, enemy->InfoData->respawnTime, enemy->InfoData->position, enemy->InfoData->rotation, enemy->InfoData->scale, nullptr);
					barColor.push_back(BarColor::Red);
					break;
				case EnemyCharacter::EnemyType::BossDrarian:
					enemy->Object = Spawner->DeathAndRespawnObject<BossDrarian>(nullptr, enemy->InfoData->respawnTime, enemy->InfoData->position, enemy->InfoData->rotation, enemy->InfoData->scale, nullptr);
					barColor.push_back(BarColor::Yellow);
					barColor.push_back(BarColor::Red);
					break;
				case EnemyCharacter::EnemyType::Drarian:
					// まだ何もしない。
					break;
				case EnemyCharacter::EnemyType::Golem:
					enemy->Object = Spawner->DeathAndRespawnObject<EnemyGolem>(nullptr, enemy->InfoData->respawnTime, enemy->InfoData->position, enemy->InfoData->rotation, enemy->InfoData->scale, nullptr);
					barColor.push_back(BarColor::Red);
					enemy->Object->SetBarPos({ 0.0f,4.0f,0.0f });
					enemy->Object->SetBigflag(true);
					break;
				case EnemyCharacter::EnemyType::BossGolem:
					enemy->Object = Spawner->DeathAndRespawnObject<BossGolem>(nullptr, enemy->InfoData->respawnTime, enemy->InfoData->position, enemy->InfoData->rotation, enemy->InfoData->scale, nullptr);
					barColor.push_back(BarColor::Yellow);
					barColor.push_back(BarColor::Red);
					break;
				case EnemyCharacter::EnemyType::Soldier:
					enemy->Object = Spawner->DeathAndRespawnObject<EnemySoldier>(nullptr, enemy->InfoData->respawnTime, enemy->InfoData->position, enemy->InfoData->rotation, enemy->InfoData->scale, nullptr);
					barColor.push_back(BarColor::Red);
					break;
				case EnemyCharacter::EnemyType::BossD:
					enemy->Object = Spawner->DeathAndRespawnObject<BossD>(nullptr, enemy->InfoData->respawnTime, enemy->InfoData->position, enemy->InfoData->rotation, enemy->InfoData->scale, nullptr);
					barColor.push_back(BarColor::Green);
					barColor.push_back(BarColor::Yellow);
					barColor.push_back(BarColor::Red);
					enemy->Object->SetBarPos({ 0.0f,2.7f,0.0f });
					enemy->Object->SetBigflag(true);
					break;
				case EnemyCharacter::EnemyType::Ghost:
					//ゴースト生成。
					enemy->Object = Spawner->DeathAndRespawnObject<BossGhost>(nullptr, enemy->InfoData->respawnTime, enemy->InfoData->position, enemy->InfoData->rotation, enemy->InfoData->scale, nullptr);
					barColor.push_back(BarColor::Red);
					break;
				case EnemyCharacter::EnemyType::BossLast:
					//ラスボス生成。
					enemy->Object = Spawner->DeathAndRespawnObject<LastBoss>(nullptr, enemy->InfoData->respawnTime, enemy->InfoData->position, enemy->InfoData->rotation, enemy->InfoData->scale, nullptr);
					barColor.push_back(BarColor::Blue);
					barColor.push_back(BarColor::Green);
					barColor.push_back(BarColor::Yellow);
					barColor.push_back(BarColor::Red);
					break;
				}

				// リスポナーを確保。
				// ※歴史切り替えで復活予告していたエネミーが消えた場合にリスポナーを削除するため。
				enemy->Respawner = Spawner->GetReSpawner();
			}
			// パラメーター設定。
			enemy->Object->SetParamAll(barColor,enemy->InfoData->param);
			enemy->Object->SetDropEXP(enemy->InfoData->exp);
			enemy->Object->SetDropMoney(enemy->InfoData->money);
			enemy->Object->SetItem(enemy->InfoData->item, enemy->InfoData->armor, enemy->InfoData->weapon);
			enemy->Object->SetProbability(enemy->InfoData->probability);
			enemy->Object->SetLocationCode(code);
			// 視野の距離設定。
			enemy->Object->SetViewRange(enemy->InfoData->viewRange);
			enemy->Object->SetViewAngle(enemy->InfoData->viewAngle);
			// 徘徊範囲設定。
			enemy->Object->SetWanderingRange(enemy->InfoData->wanderingRange);
			// 追跡範囲設定。
			enemy->Object->SetDiscoveryRange(enemy->InfoData->discoveryRange);

			//カラーを設定。
			Color c;
			c.Set(enemy->InfoData->color.x, enemy->InfoData->color.y, enemy->InfoData->color.z, enemy->InfoData->color.w);
			//カラーを設定するフラグの場合。
			//@todo for debug
			//いったん消します。
			if (enemy->InfoData->colorflag == true)
			{
				enemy->Object->SetColor(c);
			}
			return;
		}
	}
}