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
			// ���ʃG�l�~�[���쐬�B
			l_infos = &_commonEnemys;
		}
		else {
			// ���j�ɉe�������G�l�~�[���쐬�B
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
				barColor.push_back(BarColor::Yellow);
				barColor.push_back(BarColor::Red);
				break;
			}

			if (newData->Object) {
				newData->Object->SetLocationCode(location);
				// info��ۑ��B
				newData->InfoData = infos[idx].get();
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
				//�J���[��ݒ肷��t���O�̏ꍇ�B
				if (newData->InfoData->colorflag)
				{
					//�J���[�̐ݒ�
					float* color = newData->InfoData->color;
					Color c;
					c.Set(color[0], color[1], color[2], color[3]);
					newData->Object->SetColor(c);
				}

				l_infos->push_back(newData);
			}
			else {
				// �������s�B
				// EnemyType�ɐV���ǉ������H�B
				abort();
			}
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
	}

	//���񂾂̂ł��ׂẴo�t�f�o�t�A�C�R�����폜�B
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
			// �O������ǂݍ��܂ꂽ�f�[�^�ł͂Ȃ��̂ŏI���B
			return;
		}
		else {
			datas = &_historyEnemys[static_cast<int>(object->GetLocationCode())];
		}
	}

	for (auto enemy : *datas) {
		if (object == enemy->Object) {
			// ���̃N���X�Ő��������G�l�~�[�����S���Ă���B
	
			// �G�l�~�[�����X�|�[���B
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
					// �܂��������Ȃ��B
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
			// �p�����[�^�[�ݒ�B
			enemy->Object->SetParamAll(barColor,enemy->InfoData->param);
			enemy->Object->SetDropEXP(enemy->InfoData->exp);
			enemy->Object->SetDropMoney(enemy->InfoData->money);
			enemy->Object->SetItem(enemy->InfoData->item, enemy->InfoData->armor, enemy->InfoData->weapon);
			//�J���[��ݒ�B
			float* color = enemy->InfoData->color;
			Color c;
			c.Set(color[0], color[1], color[2], color[3]);
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