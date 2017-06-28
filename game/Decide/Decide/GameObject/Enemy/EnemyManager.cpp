#include "stdafx.h"
#include "EnemyManager.h"
#include "fbEngine\_Support\Support.h"
#include "Enemy.h"

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
		switch (static_cast<EnemyCharacter::EnemyType>(info->InfoData->type))
		{
		case EnemyCharacter::EnemyType::Prot:
			// プロトエネミー生成。
			enemy = INSTANCE(GameObjectManager)->AddNew<Enemy>("EnemyProt", 1);
			break;
		}

		if (enemy) {
			info->Object = enemy;	// 生成したエネミーオブジェクトを保存。
			info->Object->SetActive(true);
			info->Object->SetIsDead(false);
		}
		else {
			// 生成失敗。
			// EnemyTypeに新種を追加した？。
			abort();
		}
	}
}

void EnemyManager::ReSpawnEnemy() {
	for (auto enemy : _enemys) {
		if (enemy->Object->GetIsDead()) {
			// エネミーが死亡している。

			// エネミーをアクティブ化。
			enemy->Object->SetActive(true);	
			enemy->Object->SetIsDead(false);
			// Transform情報設定。
			enemy->Object->transform->SetPosition(enemy->InfoData->position);
			enemy->Object->transform->SetRotation(enemy->InfoData->rotation);
			enemy->Object->transform->SetScale(enemy->InfoData->scale);
			// パラメーター設定。
			enemy->Object->GetComponent<CharacterParameter>()->ParamInit(enemy->InfoData->param);
		}
	}
}

void EnemyManager::DeathEnemy(EnemyCharacter* object) {
	object->SetActive(false);	// 更新と描画をストップ。
	object->SetIsDead(true);
}