#include "stdafx.h"
#include "EnemyManager.h"
#include "fbEngine\_Support\Support.h"
#include "EnemyCharacter.h"
#include "Enemy.h"
#include "GameObject\SplitSpace.h"

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
	//CreateEnemy();
}

void EnemyManager::LoadEnemyOrigin() {
	vector<unique_ptr<EnemyInfo>> infoDatas;

	//CSV����G�l�~�[���ǂݎ��B
	Support::LoadCSVData<EnemyInfo>("Asset/Data/EnemyData/CommonGroupEnemy.csv", EnemyInfoDecl, ARRAY_SIZE(EnemyInfoDecl), infoDatas);

	for (int idx = 0; idx < infoDatas.size(); idx++) {
		// �ǂݎ�����f�[�^���Ǘ��p�\���̂œo�^�B
		ManagingData* data(new ManagingData());
		data->Object = nullptr;
		data->InfoData = move(const_cast<unique_ptr<EnemyInfo>&>(infoDatas[idx]));
		
		// ���������Ǘ��p�\���̂����X�g�ɒǉ��B
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
			// �v���g�G�l�~�[�����B
			enemy = INSTANCE(GameObjectManager)->AddNew<Enemy>("EnemyProt", 1);
			break;
		}

		if (enemy) {
			info->Object = enemy;	// ���������G�l�~�[�I�u�W�F�N�g��ۑ��B
			// Transform���ݒ�B
			enemy->transform->SetPosition(info->InfoData->position);
			enemy->transform->SetRotation(info->InfoData->rotation);
			enemy->transform->SetScale(info->InfoData->scale);		
			// �p�����[�^�[�ݒ�B
			vector<BarColor> Color;
			Color.push_back(BarColor::Red);
			enemy->SetParamAll(Color,info->InfoData->param);
		}
		else {
			// �������s�B
			// EnemyType�ɐV���ǉ������H�B
			abort();
		}
	}
}

void EnemyManager::DeathEnemy(EnemyCharacter* object) {
	INSTANCE(GameObjectManager)->AddRemoveList(object);
	for (auto enemy : _enemys) {
		if (object == enemy->Object) {
			// ���̃N���X�Ő��������G�l�~�[�����S���Ă���B

			// �G�l�~�[�����X�|�[���B
			ObjectSpawn* Spawner = enemy->Object->GetSpawner();
			if (Spawner) {
				switch (static_cast<EnemyCharacter::EnemyType>(enemy->InfoData->type)) {
				case EnemyCharacter::EnemyType::Prot:
					enemy->Object = Spawner->DeathAndRespawnObject<Enemy>(nullptr, 0.0f, enemy->InfoData->position, enemy->InfoData->rotation, enemy->InfoData->scale, nullptr);
					break;
				}
			}
			// �p�����[�^�[�ݒ�B
			vector<BarColor> Color;
			Color.push_back(BarColor::Red);
			enemy->Object->SetParamAll(Color,enemy->InfoData->param);
			return;
		}
	}
}