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

EnemyManager* EnemyManager::_instance = nullptr;


EnemyManager::EnemyManager() {
}

EnemyManager::~EnemyManager() {
	for (auto enemy : _commonEnemys) {
		enemy->Object = nullptr;
		SAFE_DELETE(enemy);
	}
	_commonEnemys.clear();
	for (auto& enemyArray : _historyEnemys) {
		for (auto enemy : enemyArray) {
			enemy->Object = nullptr;
			SAFE_DELETE(enemy);
		}
		enemyArray.clear();
	}
	_historyEnemys.clear();
}

void EnemyManager::Start() {
}


void EnemyManager::CreateEnemys(LocationCodeE location, const vector<unique_ptr<LoadEnemyInfo::EnemyInfo>>& infos) {
	vector<ManagingData*>* l_infos = nullptr;
	if (location != LocationCodeE::None) {
		if (location == LocationCodeE::Common) {
			// 共通エネミーを作成。
			l_infos = &_commonEnemys;
		}
		else {
			// 歴史に影響されるエネミーを作成。
			if (static_cast<int>(_historyEnemys.size()) < static_cast<int>(location) + 1) {
				_historyEnemys.resize(static_cast<int>(location) + 1);
			}
			l_infos = &(_historyEnemys[static_cast<int>(location)]);
		}
	}

	if (l_infos == nullptr) {
		return;
	}
	else {
		if (l_infos->size() > 0) {
			for (auto obj : *l_infos) {
				if (obj->Object) {
					INSTANCE(GameObjectManager)->AddRemoveList(obj->Object);
				}
			}
			l_infos->clear();
		}

		for (int idx = 0; idx < static_cast<int>(infos.size()); idx++)
		{
			ManagingData* newData = new ManagingData;
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
				barColor.push_back(BarColor::Yellow);
				barColor.push_back(BarColor::Red);
				break;
			}

			if (newData->Object) {
				newData->Object->SetLocationCode(location);
				// infoを保存。
				newData->InfoData = infos[idx].get();
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
				//カラーを設定するフラグの場合。
				if (newData->InfoData->colorflag)
				{
					//カラーの設定
					float* color = newData->InfoData->color;
					Color c;
					c.Set(color[0], color[1], color[2], color[3]);
					newData->Object->SetColor(c);
				}

				l_infos->push_back(newData);
			}
			else {
				// 生成失敗。
				// EnemyTypeに新種を追加した？。
				abort();
			}
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
	}

	//死んだのですべてのバフデバフアイコンを削除。
	BuffDebuffICon* icon = object->GetComponent<BuffDebuffICon>();
	if (icon) {
		icon->DeleteAllBuffDebuffIcon();
	}

	vector<ManagingData*>* datas = nullptr;

	if (object->GetLocationCode() == LocationCodeE::Common) {
		datas = &_commonEnemys;
	}
	else {
		if (object->GetLocationCode() == LocationCodeE::None) {
			// 外部から読み込まれたデータではないので終了。
			return;
		}
		else {
			datas = &_historyEnemys[static_cast<int>(object->GetLocationCode())];
		}
	}

	for (auto enemy : *datas) {
		if (object == enemy->Object) {
			// このクラスで生成したエネミーが死亡している。
	
			// エネミーをリスポーン。
			vector<BarColor> barColor;
			ObjectSpawn* Spawner = enemy->Object->GetSpawner();
			if (Spawner) {
				switch (static_cast<EnemyCharacter::EnemyType>(enemy->InfoData->type)) {
				case EnemyCharacter::EnemyType::Born:
					enemy->Object = Spawner->DeathAndRespawnObject<Enemy>(nullptr, 0.0f, enemy->InfoData->position, enemy->InfoData->rotation, enemy->InfoData->scale, nullptr);
					barColor.push_back(BarColor::Red);
					break;
				case EnemyCharacter::EnemyType::BossDrarian:
					enemy->Object = Spawner->DeathAndRespawnObject<BossDrarian>(nullptr, 60.0f, enemy->InfoData->position, enemy->InfoData->rotation, enemy->InfoData->scale, nullptr);
					barColor.push_back(BarColor::Yellow);
					barColor.push_back(BarColor::Red);
					break;
				case EnemyCharacter::EnemyType::Drarian:
					// まだ何もしない。
					break;
				case EnemyCharacter::EnemyType::Golem:
					enemy->Object = Spawner->DeathAndRespawnObject<EnemyGolem>(nullptr, 60.0f, enemy->InfoData->position, enemy->InfoData->rotation, enemy->InfoData->scale, nullptr);
					barColor.push_back(BarColor::Red);
					break;
				case EnemyCharacter::EnemyType::BossGolem:
					enemy->Object = Spawner->DeathAndRespawnObject<BossGolem>(nullptr, 60.0f, enemy->InfoData->position, enemy->InfoData->rotation, enemy->InfoData->scale, nullptr);
					barColor.push_back(BarColor::Yellow);
					barColor.push_back(BarColor::Red);
					break;
				case EnemyCharacter::EnemyType::Soldier:
					enemy->Object = Spawner->DeathAndRespawnObject<EnemySoldier>(nullptr, 60.0f, enemy->InfoData->position, enemy->InfoData->rotation, enemy->InfoData->scale, nullptr);
					barColor.push_back(BarColor::Red);
					break;
				case EnemyCharacter::EnemyType::BossD:
					enemy->Object = Spawner->DeathAndRespawnObject<BossD>(nullptr, 60.0f, enemy->InfoData->position, enemy->InfoData->rotation, enemy->InfoData->scale, nullptr);
					barColor.push_back(BarColor::Yellow);
					barColor.push_back(BarColor::Red);
					break;
				}
			}
			// パラメーター設定。
			enemy->Object->SetParamAll(barColor,enemy->InfoData->param);
			enemy->Object->SetDropEXP(enemy->InfoData->exp);
			enemy->Object->SetDropMoney(enemy->InfoData->money);
			enemy->Object->SetItem(enemy->InfoData->item, enemy->InfoData->armor, enemy->InfoData->weapon);
			//カラーを設定。
			float* color = enemy->InfoData->color;
			Color c;
			c.Set(color[0], color[1], color[2], color[3]);
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