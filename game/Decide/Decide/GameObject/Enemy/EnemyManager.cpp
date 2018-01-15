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
	// �������ȁB
	//�G�l�~�[���X�g���������B
	// ���V�[���؂�ւ����ɂ�������폜���Ȃ��ƕs���ȃ��������I�u�W�F�N�g�̍폜���X�g�ɐςޏ��������邽�ߐ�ΕK�v�B
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
		// �G�l�~�[���쐬�B
		if (static_cast<int>(_enemys.size()) < static_cast<int>(location) + 1) {
			_enemys.resize(static_cast<int>(location) + 1);
		}
	}
	else {
		return;
	}

	if (_enemys[static_cast<int>(location)].size() > 0) {
		// ���łɃG�l�~�[����������Ă���B
		for (auto& obj : _enemys[static_cast<int>(location)]) {
			if (obj->Object) {
				INSTANCE(GameObjectManager)->AddRemoveList(obj->Object);
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
			// ���G�l�~�[�����B
			newData->Object = INSTANCE(GameObjectManager)->AddNew<Enemy>("EnemyProt", 1);
			barColor.push_back(BarColor::Red);
			
			break;
		case EnemyCharacter::EnemyType::BossDrarian:
			// �{�X�h�����A�������B
			newData->Object = INSTANCE(GameObjectManager)->AddNew<BossDrarian>("EnemyDrarian", 1);
			barColor.push_back(BarColor::Yellow);
			barColor.push_back(BarColor::Red);
			
			break;
		case EnemyCharacter::EnemyType::Drarian:
			// �h�����A�������B

			// ���܂��쐬���Ȃ��B
			barColor.push_back(BarColor::Red);
			break;
		case EnemyCharacter::EnemyType::Golem:
			//�S�[���������B
			newData->Object = INSTANCE(GameObjectManager)->AddNew<EnemyGolem>("EnemyGolem", 1);
			barColor.push_back(BarColor::Red);
			newData->Object->SetBarPos({ 0.0f,4.0f,0.0f });
			newData->Object->SetBigflag(true);
			break;
		case EnemyCharacter::EnemyType::BossGolem:
			//�{�X�S�[���������B
			newData->Object = INSTANCE(GameObjectManager)->AddNew<BossGolem>("BossGolem", 1);
			barColor.push_back(BarColor::Yellow);
			barColor.push_back(BarColor::Red);
			break;

		case EnemyCharacter::EnemyType::Soldier:
			//���m�𐶐��B
			newData->Object = INSTANCE(GameObjectManager)->AddNew<EnemySoldier>("EnemySoldier", 1);
			barColor.push_back(BarColor::Red);
			break;
		case EnemyCharacter::EnemyType::BossD:
			//�{�XD�����B
			newData->Object = INSTANCE(GameObjectManager)->AddNew<BossD>("BossD", 1);
			barColor.push_back(BarColor::Green);
			barColor.push_back(BarColor::Yellow);
			barColor.push_back(BarColor::Red);
			break;
		case EnemyCharacter::EnemyType::Ghost:
			//�S�[�X�g�����B
			newData->Object = INSTANCE(GameObjectManager)->AddNew<BossGhost>("LastBoss", 1);
			barColor.push_back(BarColor::Red);
			break;
		case EnemyCharacter::EnemyType::BossLast:
			//���X�{�X�����B
			newData->Object = INSTANCE(GameObjectManager)->AddNew<LastBoss>("LastBoss", 1);
			barColor.push_back(BarColor::Blue);
			barColor.push_back(BarColor::Green);
			barColor.push_back(BarColor::Yellow);
			barColor.push_back(BarColor::Red);
			break;
		}

		if (newData->Object) {
			newData->Object->SetLocationCode(location);
			// info��ۑ��B
			newData->InfoData = move(infos[idx]);
			// Transform���ݒ�B
			newData->Object->transform->SetPosition(newData->InfoData->position);
			newData->Object->transform->SetRotation(newData->InfoData->rotation);
			newData->Object->transform->SetScale(newData->InfoData->scale);
			// �p�����[�^�[�ݒ�B
			newData->Object->SetParamAll(barColor, newData->InfoData->param);
			// �h���b�v�ݒ�B
			newData->Object->SetDropEXP(newData->InfoData->exp);
			newData->Object->SetDropMoney(newData->InfoData->money);
			newData->Object->SetItem(newData->InfoData->item, newData->InfoData->armor, newData->InfoData->weapon);
			newData->Object->SetProbability(newData->InfoData->probability);
			//�J���[��ݒ肷��t���O�̏ꍇ�B
			if (newData->InfoData->colorflag)
			{
				//�J���[�̐ݒ�
				Color c;
				c.Set(newData->InfoData->color.x, newData->InfoData->color.y, newData->InfoData->color.z, newData->InfoData->color.w);
				newData->Object->SetColor(c);
			}
			// ����̋����ݒ�B
			newData->Object->SetViewRange(newData->InfoData->viewRange);
			// �p�j�͈͐ݒ�B
			newData->Object->SetWanderingRange(newData->InfoData->wanderingRange);
			// �ǐՔ͈͐ݒ�B
			newData->Object->SetDiscoveryRange(newData->InfoData->discoveryRange);

			_enemys[static_cast<int>(location)].push_back(move(newData));
		}
		else {
			// �������s�B
			// EnemyType�ɐV���ǉ������H�B
			abort();
		}
	}
}


void EnemyManager::DeathEnemy(EnemyCharacter* object) {
	// �G�l�~�[�폜�\��B
	INSTANCE(GameObjectManager)->AddRemoveList(object);

	//�o�t�f�o�t�G�t�F�N�g�����������B
	ParticleEffect* pe = object->GetComponent<ParticleEffect>();
	if (pe) {
		pe->SetBuffEffectFlag(false);
		pe->SetDebuffEffectFlag(false);
		pe->SetBigMonsterDebuffEffectFlag(false);
	}

	//���񂾂̂ł��ׂẴo�t�f�o�t�A�C�R�����폜�B
	BuffDebuffICon* icon = object->GetComponent<BuffDebuffICon>();
	if (icon) {
		icon->DeleteAllBuffDebuffIcon();
	}

	if (object->GetLocationCode() == LocationCodeE::None) {
		// �O������ǂݍ��܂ꂽ�f�[�^�ł͂Ȃ��̂ŏI���B
		return;
	}

	LocationCodeE code = object->GetLocationCode();
	for (auto& enemy : _enemys[static_cast<int>(code)]) {
		if (object == enemy->Object) {
			// ���̃N���X�Ő��������G�l�~�[�����S���Ă���B
	
			// �G�l�~�[�����X�|�[���B
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
					// �܂��������Ȃ��B
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
					barColor.push_back(BarColor::Yellow);
					barColor.push_back(BarColor::Red);
					break;
				case EnemyCharacter::EnemyType::Ghost:
					//�S�[�X�g�����B
					enemy->Object = Spawner->DeathAndRespawnObject<BossGhost>(nullptr, enemy->InfoData->respawnTime, enemy->InfoData->position, enemy->InfoData->rotation, enemy->InfoData->scale, nullptr);
					barColor.push_back(BarColor::Red);
					break;
				case EnemyCharacter::EnemyType::BossLast:
					//���X�{�X�����B
					enemy->Object = Spawner->DeathAndRespawnObject<LastBoss>(nullptr, enemy->InfoData->respawnTime, enemy->InfoData->position, enemy->InfoData->rotation, enemy->InfoData->scale, nullptr);
					barColor.push_back(BarColor::Blue);
					barColor.push_back(BarColor::Green);
					barColor.push_back(BarColor::Yellow);
					barColor.push_back(BarColor::Red);
					break;
				}
			}
			// �p�����[�^�[�ݒ�B
			enemy->Object->SetParamAll(barColor,enemy->InfoData->param);
			enemy->Object->SetDropEXP(enemy->InfoData->exp);
			enemy->Object->SetDropMoney(enemy->InfoData->money);
			enemy->Object->SetItem(enemy->InfoData->item, enemy->InfoData->armor, enemy->InfoData->weapon);
			enemy->Object->SetProbability(enemy->InfoData->probability);
			enemy->Object->SetLocationCode(code);
			// ����̋����ݒ�B
			enemy->Object->SetViewRange(enemy->InfoData->viewRange);
			// �p�j�͈͐ݒ�B
			enemy->Object->SetWanderingRange(enemy->InfoData->wanderingRange);
			// �ǐՔ͈͐ݒ�B
			enemy->Object->SetDiscoveryRange(enemy->InfoData->discoveryRange);

			//�J���[��ݒ�B
			Color c;
			c.Set(enemy->InfoData->color.x, enemy->InfoData->color.y, enemy->InfoData->color.z, enemy->InfoData->color.w);
			//�J���[��ݒ肷��t���O�̏ꍇ�B
			//@todo for debug
			//������������܂��B
			if (enemy->InfoData->colorflag == true)
			{
				enemy->Object->SetColor(c);
			}
			return;
		}
	}
}