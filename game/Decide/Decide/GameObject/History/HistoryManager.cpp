#include"stdafx.h"
#include "HistoryManager.h"
#include "GameObject\Village\ContinentObject.h"
#include "fbEngine\_Object\_Component\_Physics\BoxCollider.h"

namespace
{
	//�I�u�W�F�N�g�����ʂ���^�C�v�B
	const char* ObjectType[3] = { "Obj","NPC","Enemy" };
}


/** �C���X�^���X. */
HistoryManager* HistoryManager::_Instance = nullptr;

/**
* �R���X�g���N�^.
*/
HistoryManager::HistoryManager()
{
	for (int i = 0; i < (int)LocationCodeE::LocationNum; i++)
	{
		//�O���[�v��Ԃ�0�ŏ������B
		_NowGroupIDList.push_back(0);

		//vector��ǉ�
		vector<GameObject*> list;
		_GameObjectList.push_back(list);
		vector<NPC*> npcs;
		_NPCList.push_back(npcs);

		_EnemyInfoList = vector<vector<unique_ptr<LoadEnemyInfo::EnemyInfo>>>(static_cast<int>(LocationCodeE::LocationNum));
	}
}

/**
* ������.
*/
void HistoryManager::Start()
{
	_LocationHistoryList.clear();

	if (IS_CONTINUE)
	{
		//CSV������j���ǂݎ��B
		Support::LoadCSVData<LocationHistoryInfo>("Asset/Data/LocationHistory.csv", HistoryInfoData, ARRAY_SIZE(HistoryInfoData), _LocationHistoryList);
	}
	else
	{
		//CSV������j���ǂݎ��B
		Support::LoadCSVData<LocationHistoryInfo>("Asset/Data/LocationHistory.csv", HistoryInfoData, ARRAY_SIZE(HistoryInfoData), _LocationHistoryList);
		FOR(i, _LocationHistoryList.size())
		{
			for (int j = 0; j < (int)ChipID::ChipNum; j++)
			{
				_LocationHistoryList.at(i)->_ChipSlot[j] = ChipID::None;
			}
		}
		Support::OutputCSV<LocationHistoryInfo>("Asset/Data/LocationHistory.csv", HistoryInfoData, ARRAY_SIZE(HistoryInfoData), _LocationHistoryList);
	}

	_HistoryMenu = (HistoryMenu*)INSTANCE(GameObjectManager)->FindObject("HistoryMenu");
	_HistoryBook = (HistoryBook*)INSTANCE(GameObjectManager)->FindObject("HistoryBook");

	_MysteryLight = INSTANCE(GameObjectManager)->AddNew<MysteryLight>("MysteryLight", 9);

	_Player = (Player*)INSTANCE(GameObjectManager)->FindObject("Player");

//�؂��ז��ȏꍇ������g���Ă��������B
//#define NPCONLY

#ifdef NPCONLY
	//���ʃI�u�W�F�N�g�����B
	char path[128];
		//�p�X����
	sprintf(path, "Asset/Data/GroupData/CommonGroup%s.csv", ObjectType[1]);
	_CreateObject((int)LocationCodeE::Common, path, 1);
#else // NPCONLY
	//���ʃI�u�W�F�N�g�����B
	char path[128];
	for (int type = static_cast<int>(LoadObjectType::Object); type < static_cast<int>(LoadObjectType::Max); type++) {
		//�p�X����
		sprintf(path, "Asset/Data/GroupData/CommonGroup%s.csv", ObjectType[type]);
		_CreateObject(LocationCodeE::Common, path, static_cast<LoadObjectType>(type));
	}

#endif
	
	//���j�I�u�W�F�N�g�����B
	FOR(i, _LocationHistoryList.size())
	{
		for (int j = 0; j < (int)ChipID::ChipNum; j++)
		{
			if (_LocationHistoryList.at(i)->_ChipSlot[j] == ChipID::None)
			{
				continue;
			}
			//�ǂݍ��񂾃f�[�^�����ɗ��j���Ƀy�[�W�����炩���ߒǉ��B
			HistoryPage* page = _HistoryBook->PutInChip(_LocationHistoryList.at(i)->_ChipSlot[j], _LocationHistoryList.at(i)->_LocationID, j);
			page->ChangeState(HistoryPage::StateCodeE::Close);
		}
		_ChangeLocation(_LocationHistoryList.at(i)->_LocationID);
	}
}

/**
* ���j��ς���.
*
* @param location	�ꏊID.
* @param slot		�X���b�g�ԍ�.
* @param chip		�`�b�vID.
*/
bool HistoryManager::SetHistoryChip(LocationCodeE location, ChipID chip, int index)
{
	_HistoryBook->PutInChip(chip, location, index);
	//�ЂƂ܂�����邾���ŏ㏑����Ă��܂�.
	_LocationHistoryList[(int)location]->SetData(_HistoryBook->GetLocationList(location));

	//�ύX�����̂ŗ��j�����ς�����.
	_ChangeLocation(location);

	//�f�[�^��ۑ�.
	Support::OutputCSV<LocationHistoryInfo>("Asset/Data/LocationHistory.csv", HistoryInfoData, ARRAY_SIZE(HistoryInfoData), _LocationHistoryList);

	return true;
}

/**
* �ꏊ�̕ω�.
*
* @param location	�ꏊID.
*/
void HistoryManager::_ChangeLocation(LocationCodeE location)
{
	//�`�b�v�̏�Ԃ���O���[�v���v�Z�B
	const int group = _CalcPattern(_LocationHistoryList[(int)location].get());
	//�ǂꂩ�̃O���[�v�ɊY������̂Ȃ�B
	if (group >= 0)
	{

		if (_NowGroupIDList[(int)location] != group)
		{
			_MysteryLight->SetActive(true, true);
			_NowGroupIDList[(int)location] = group;
		}

		char path[128];

		for (int type = static_cast<int>(LoadObjectType::Object); type < static_cast<int>(LoadObjectType::Max); type++) {
			//�p�X����
			sprintf(path, "Asset/Data/GroupData/Group%d%c%s.csv", (int)location, 'A' + group, ObjectType[type]);
			_CreateObject(location, path, static_cast<LoadObjectType>(type));
			ZeroMemory(path, 128);
		}
	}
}

/**
* �p�^�[���v�Z.
*/
int HistoryManager::_CalcPattern(const LocationHistoryInfo * info)
{
	//�嗤�ɍ������O���[�v�V�[�g�ǂݍ���
	char path[256];
	sprintf(path, "Asset/Data/Village%dGroup.csv", info->_LocationID);
	//CSV����O���[�v���ǂݍ���
	vector<unique_ptr<VillageGroup>> groupList;
	Support::LoadCSVData<VillageGroup>(path, VillageGroupData, ARRAY_SIZE(VillageGroupData), groupList);
	
	//��v������̂����邩���ׂ�B
	for(auto& group : groupList)
	{
		bool isMatch = true;
		//�e�X���b�g���r
		for (int i = 0; i < (int)ChipID::ChipNum; i++)
		{
			if (group->Slot[i] != info->_ChipSlot[i])
			{
				isMatch = false;
			}
		}

		if (!isMatch)
		{
			//�}�b�`���O���Ă��Ȃ��̂Ŏ���.
			continue;
		}

		//�p�^�[����v�����̂�ID�ݒ�B
		return group->GroupID;
	}

	return -1;
}

/**
* �I�u�W�F�N�g���쐬.
*
* @param location	�ꏊID.
* @param path		�t�H���_�p�X.
* @param type		��������I�u�W�F�N�g�̃^�C�v.
*/
void HistoryManager::_CreateObject(LocationCodeE location, const char * path, HistoryManager::LoadObjectType type)
{
	if (type == HistoryManager::LoadObjectType::Object)
	{
		//�O�̃I�u�W�F�N�g���폜
		for (auto obj : _GameObjectList[static_cast<int>(location)])
		{
			INSTANCE(GameObjectManager)->AddRemoveList(obj);
		}
		_GameObjectList[static_cast<int>(location)].clear();

		//�����B
		CreateBuilding(path, _GameObjectList[static_cast<int>(location)]);

		//��3�̑��ȍ~�̂݁B
		if (location == LocationCodeE::Prosperity || location == LocationCodeE::DevilKingdom)
		{
			for (auto obj : _GameObjectList[static_cast<int>(location)]) {
				//X����180�ǉ�]������B
				//Quaternion q = Quaternion::Identity;
				//q.SetRotation(Vector3::axisX, PI);
				//q.Multiply(obj->transform->GetRotation());
				//obj->transform->SetRotation(q);
			}
		}
	}
	else if (type == HistoryManager::LoadObjectType::NPC)
	{
		//�O��NPC���폜
		for (auto npc : _NPCList[(int)location])
		{
			INSTANCE(GameObjectManager)->AddRemoveList(npc);
		}
		_NPCList[(int)location].clear();
		//�����B
		_CreateNPC(location, path);
	}
	else if (type == HistoryManager::LoadObjectType::Enemy) {
		// �G�l�~�[�̏���ǂݍ��݁B
		_CreateEnemy(location, path);
	}
}

vector<GameObject*>& HistoryManager::CreateBuilding(const char* path, vector<GameObject*>& Builds)
{
	//CSV����I�u�W�F�N�g�̏��ǂݍ���
	vector<unique_ptr<ObjectInfo>> objInfo;
	Support::LoadCSVData<ObjectInfo>(path, ObjectInfoData, ARRAY_SIZE(ObjectInfoData), objInfo);

	//��񂩂�I�u�W�F�N�g�����B
	for (short i = 0; i < static_cast<int>(objInfo.size());)
	{
		//�R���W�������ǂ����H
		if (strcmp(objInfo[i]->filename, "coll") != 0)
		{
			//�I�u�W�F�N�g����
			ContinentObject* obj = INSTANCE(GameObjectManager)->AddNew<ContinentObject>(objInfo[i]->filename, 2);

			obj->transform->SetLocalPosition(objInfo[i]->pos);

			obj->transform->SetRotation(objInfo[i]->ang);

			objInfo[i]->sca.y *= -1.0f;
			obj->transform->SetLocalScale(objInfo[i]->sca);
			obj->LoadModel(objInfo[i]->filename, objInfo[i]->coll);

			//�z��ɒǉ��B
			Builds.push_back(obj);

			//�����R���W�������ǂ����H
			while (true)
			{
				ObjectInfo* info;
				//�͈͊O�`�F�b�N�B
				try {
					info = objInfo.at(++i).get();
				}
				catch (std::out_of_range& ex) {
					break;
				}

				//�R���W�����������ō쐬���Ă���ꍇ�͍쐬����B
				//���O�`�F�b�N
				if (strcmp(info->filename, "coll") == 0 && objInfo[i]->coll)
				{
					//�R���W�����𐶐����ăQ�[���I�u�W�F�N�g�ɃA�^�b�`�B
					BoxCollider* box = obj->AddComponent<BoxCollider>();
					RigidBody* coll = obj->AddComponent<RigidBody>();

					box->Create(Vector3(fabsf(info->sca.x), fabsf(info->sca.y), fabsf(info->sca.z)));
					RigidBodyInfo Rinfo;
					Rinfo.physicsType = Collision::PhysicsType::Static;
					Rinfo.mass = 0.0f;
					Rinfo.coll = box;
					//�J�����Ɠ�����Ȃ��R���W�������ǂ����H
					Rinfo.id = ((bool)info->hitcamera) ? Collision_ID::BUILDING : (Collision_ID::BUILDING | Collision_ID::NOTHITCAMERA);
					Rinfo.offset = info->pos;
					//Quaternion q;
					//q.SetRotation(Vector3::up, PI / 2);
					//q.Multiply(info->ang);
					Rinfo.rotation = info->ang;
					coll->Create(Rinfo, true);
				}
				else
				{
					break;
				}
			}
		}

	}
	objInfo.clear();

	return Builds;
}

void HistoryManager::_CreateNPC(LocationCodeE location, const char * path)
{
	//CSV����I�u�W�F�N�g�̏��ǂݍ���
	vector<unique_ptr<npc::NPCInfo>> npcInfo;
	Support::LoadCSVData<npc::NPCInfo>(path, npc::NPCInfoData, ARRAY_SIZE(npc::NPCInfoData), npcInfo);

	//��񂩂�I�u�W�F�N�g�����B
	FOR(i, npcInfo.size())
	{
		//����
		NPC* npc = INSTANCE(GameObjectManager)->AddNew<NPC>(npcInfo[i]->filename, 2);
		npc->CreateNPC(npcInfo[i].get());
		
		auto model = npc->GetComponent<SkinModel>();
		model->GetModelData()->SetInstancing(false);
		model->SetCullMode(D3DCULL::D3DCULL_CCW);

		//�Ǘ��p�̔z��ɒǉ��B
		_NPCList[static_cast<int>(location)].push_back(npc);
	}
	//�����̂Ŕj���B
	npcInfo.clear();
}

void HistoryManager::_CreateEnemy(LocationCodeE location, const char * path) {
	// �O�̃G�l�~�[���폜�B
	if (_EnemyInfoList[(int)location].size() > 0) {
		_EnemyInfoList[(int)location].clear();
	}

	//CSV����G�l�~�[���ǂݎ��B
	Support::LoadCSVData<LoadEnemyInfo::EnemyInfo>(path, LoadEnemyInfo::EnemyInfoDecl, ARRAY_SIZE(LoadEnemyInfo::EnemyInfoDecl), _EnemyInfoList[static_cast<int>(location)]);

	INSTANCE(EnemyManager)->CreateEnemys(location, _EnemyInfoList[static_cast<int>(location)]);
}

/**
* �R���W�������쐬.
*
* @param location	�ꏊID.
* @param path		�t�H���_�p�X.
*/
void HistoryManager::_CreateCollision(int location, const char * path)
{
	//CSV���瓖���蔻��̏��ǂݍ���
	vector<unique_ptr<CollisionInfo>> colls;
	Support::LoadCSVData<CollisionInfo>(path, CollisionInfoData, ARRAY_SIZE(CollisionInfoData), colls);

	//��񂩂瓖���蔻�萶���B
	FOR(i, colls.size())
	{
		//����
		ContinentObject* coll = INSTANCE(GameObjectManager)->AddNew<ContinentObject>("StageCollision", 2);
		
		coll->transform->SetLocalPosition(colls[i]->pos);
		coll->transform->SetRotation(colls[i]->ang);
		coll->transform->SetLocalScale(colls[i]->sca);
		//�Ǘ��p�̔z��ɒǉ��B
		_GameObjectList[location].push_back(coll);
	}

	colls.clear();
}

void HistoryManager::PutOutPage(LocationCodeE location,vector<HistoryPage*>& list)
{
	_LocationHistoryList[(int)location]->SetData(list);

	//�ύX�����̂ŗ��j�����ς�����.
	_ChangeLocation(location);

	//�f�[�^��ۑ�.
	Support::OutputCSV<LocationHistoryInfo>("Asset/Data/LocationHistory.csv", HistoryInfoData, ARRAY_SIZE(HistoryInfoData), _LocationHistoryList);
}