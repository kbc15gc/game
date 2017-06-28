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
			info->Object->SetActive(true);
			info->Object->SetIsDead(false);
		}
		else {
			// �������s�B
			// EnemyType�ɐV���ǉ������H�B
			abort();
		}
	}
}

void EnemyManager::ReSpawnEnemy() {
	for (auto enemy : _enemys) {
		if (enemy->Object->GetIsDead()) {
			// �G�l�~�[�����S���Ă���B

			// �G�l�~�[���A�N�e�B�u���B
			enemy->Object->SetActive(true);	
			enemy->Object->SetIsDead(false);
			// Transform���ݒ�B
			enemy->Object->transform->SetPosition(enemy->InfoData->position);
			enemy->Object->transform->SetRotation(enemy->InfoData->rotation);
			enemy->Object->transform->SetScale(enemy->InfoData->scale);
			// �p�����[�^�[�ݒ�B
			enemy->Object->GetComponent<CharacterParameter>()->ParamInit(enemy->InfoData->param);
		}
	}
}

void EnemyManager::DeathEnemy(EnemyCharacter* object) {
	object->SetActive(false);	// �X�V�ƕ`����X�g�b�v�B
	object->SetIsDead(true);
}