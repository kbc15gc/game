#include "stdafx.h"
#include "EnemyManager.h"
#include "fbEngine\_Support\Support.h"
#include "EnemyCharacter.h"
#include "Enemy.h"
#include "GameObject\SplitSpace.h"
#include "GameObject\Enemy\BossDrarian.h"
#include "GameObject\Component\ParticleEffect.h"

EnemyManager* EnemyManager::_instance = nullptr;


EnemyManager::EnemyManager() {
	LoadEnemyOrigin();
}

EnemyManager::~EnemyManager() {
	for (auto enemy : _enemys) {
		enemy->Object = nullptr;
		SAFE_DELETE(enemy);
	}
	_enemys.clear();
}

void EnemyManager::Start() {
	CreateEnemy();
}

void EnemyManager::LoadEnemyOrigin() {
	vector<unique_ptr<EnemyInfo>> infoDatas;

	//CSVからエネミー情報読み取り。
	Support::LoadCSVData<EnemyInfo>("Asset/Data/EnemyData/CommonGroupEnemy.csv", EnemyInfoDecl, ARRAY_SIZE(EnemyInfoDecl), infoDatas);

	for (int idx = 0; idx < infoDatas.size(); idx++) {
		// 読み取ったデータを管理用構造体で登録。
		ManagingData* data(new ManagingData());
		data->Object = nullptr;
		data->InfoData = move(const_cast<unique_ptr<EnemyInfo>&>(infoDatas[idx]));
		
		// 生成した管理用構造体をリストに追加。
		_enemys.push_back(data);
	}
}

void EnemyManager::CreateEnemy() {
	for (auto info : _enemys)
	{
		EnemyCharacter* enemy = nullptr;
		vector<BarColor> Color;
		switch (static_cast<EnemyCharacter::EnemyType>(info->InfoData->type))
		{
		case EnemyCharacter::EnemyType::Born:
			// 骨エネミー生成。
			enemy = INSTANCE(GameObjectManager)->AddNew<Enemy>("EnemyProt", 1);
			Color.push_back(BarColor::Red);
			break;
		case EnemyCharacter::EnemyType::BossDrarian:
			// ボスドラリアン生成。
			enemy = INSTANCE(GameObjectManager)->AddNew<BossDrarian>("EnemyDrarian", 1);
			Color.push_back(BarColor::Yellow);
			Color.push_back(BarColor::Red);
			break;
		case EnemyCharacter::EnemyType::Drarian:
			// ドラリアン生成。

			// ※まだ作成しない。
			Color.push_back(BarColor::Red);
			break;
		}

		if (enemy) {
			info->Object = enemy;	// 生成したエネミーオブジェクトを保存。
			// Transform情報設定。
			enemy->transform->SetPosition(info->InfoData->position);
			enemy->transform->SetRotation(info->InfoData->rotation);
			enemy->transform->SetScale(info->InfoData->scale);
			// パラメーター設定。
			enemy->SetParamAll(Color, info->InfoData->param);
			// ドロップ設定。
			enemy->SetDropEXP(info->InfoData->exp);
			enemy->SetDropMoney(info->InfoData->money);
		}
		else {
			// 生成失敗。
			// EnemyTypeに新種を追加した？。
			abort();
		}
	}
}

void EnemyManager::DeathEnemy(EnemyCharacter* object) {
	INSTANCE(GameObjectManager)->AddRemoveList(object);
	for (auto enemy : _enemys) {
		if (object == enemy->Object) {
			// このクラスで生成したエネミーが死亡している。

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
			
			// エネミーをリスポーン。
			vector<BarColor> Color;
			ObjectSpawn* Spawner = enemy->Object->GetSpawner();
			if (Spawner) {
				switch (static_cast<EnemyCharacter::EnemyType>(enemy->InfoData->type)) {
				case EnemyCharacter::EnemyType::Born:
					enemy->Object = Spawner->DeathAndRespawnObject<Enemy>(nullptr, 0.0f, enemy->InfoData->position, enemy->InfoData->rotation, enemy->InfoData->scale, nullptr);
					Color.push_back(BarColor::Red);
					break;
				case EnemyCharacter::EnemyType::BossDrarian:
					enemy->Object = Spawner->DeathAndRespawnObject<BossDrarian>(nullptr, 60.0f, enemy->InfoData->position, enemy->InfoData->rotation, enemy->InfoData->scale, nullptr);
					Color.push_back(BarColor::Yellow);
					Color.push_back(BarColor::Red);
					break;
				case EnemyCharacter::EnemyType::Drarian:
					// まだ何もしない。
					break;
				}
			}
			// パラメーター設定。
			enemy->Object->SetParamAll(Color,enemy->InfoData->param);
			enemy->Object->SetDropEXP(enemy->InfoData->exp);
			enemy->Object->SetDropMoney(enemy->InfoData->money);
			return;
		}
	}
}