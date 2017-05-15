#include"stdafx.h"
#include "HistoryManager.h"

#include "GameObject\Village\HistoryInfo.h"
#include "GameObject\Village\ContinentObject.h"
#include "GameObject\Village\NPC.h"


HistoryManager* HistoryManager::_Instance = nullptr;

HistoryManager::HistoryManager()
{
	Support::LoadCSVData<HistoryInfo>("Asset/Data/VillageHistory.csv", HistoryInfoData, ARRAY_SIZE(HistoryInfoData), _HistoryList);

	FOR(i, _HistoryList.size())
	{
		//vector��ǉ�
		vector<GameObject*> list;
		_GameObjects.push_back(list);
	}
}

void HistoryManager::CreateObject()
{
	FOR(i, _HistoryList.size())
	{
		_ChangeContinent(i);
	}
}

const bool HistoryManager::SetHistoryChip(const unsigned int & continent, const unsigned int & idx, const int & chip)
{
	//�����������ǂ����H
	bool success = false;
	//���j�̃`�b�v�Z�b�g
	if(success =_HistoryList[continent]->SetChip(idx, (ChipID)chip))
	{
		//�����ɃZ�b�g���ꂽ�Ȃ�嗤��ω�������B
		_ChangeContinent(continent);
		//csv�ۑ�����
		Support::OutputCSV<HistoryInfo>("Asset/Data/VillageHistory.csv", HistoryInfoData, ARRAY_SIZE(HistoryInfoData), _HistoryList);
	}
	return success;
}

void HistoryManager::_ChangeContinent(const unsigned int& continent)
{
	//�O�̂�폜���ăN���A
	for each (GameObject* obj in _GameObjects[continent])
	{
		INSTANCE(GameObjectManager)->AddRemoveList(obj);
	}
	_GameObjects[continent].clear();

	//�嗤�̕ω��̃p�^�[���v�Z
	int pattern = _CalcPattern(_HistoryList[continent]);

	char dirpath[] = "Asset/Data/Group";
	char group[] = { 'A' + pattern,0 };
	char* type[2] = { "Obj","NPC" };
	char path[128];
	
	//OBJ
	{
		//�p�X����
		strcpy(path, dirpath);
		strcat(path, group);
		strcat(path, type[0]);
		strcat(path, ".csv");
		//CSV����I�u�W�F�N�g�̏��ǂݍ���
		vector<ObjectInfo*> objInfo;
		Support::LoadCSVData<ObjectInfo>(path, ObjectInfoData, ARRAY_SIZE(ObjectInfoData), objInfo);

		//��񂩂�I�u�W�F�N�g�����B
		FOR(i, objInfo.size())
		{
			//����
			ContinentObject* obj = INSTANCE(GameObjectManager)->AddNew<ContinentObject>("ContinentObject", 2);
			obj->LoadModel(objInfo[i]->filename);
			obj->transform->SetLocalPosition(objInfo[i]->pos + Vector3(0, 6, 0));
			obj->transform->SetLocalAngle(objInfo[i]->ang);
			obj->transform->SetLocalScale(objInfo[i]->sca);
			_GameObjects[continent].push_back(obj);

			//��������Ȃ��̂ŉ��
			SAFE_DELETE(objInfo[i]);
		}

		objInfo.clear();
	}
	ZeroMemory(path, 128);
	//NPC
	{
		//�p�X����
		strcpy(path, dirpath);
		strcat(path, group);
		strcat(path, type[1]);
		strcat(path, ".csv");
		//CSV����I�u�W�F�N�g�̏��ǂݍ���
		vector<NPCInfo*> npcInfo;
		Support::LoadCSVData<NPCInfo>(path, NPCInfoData, ARRAY_SIZE(NPCInfoData), npcInfo);

		//��񂩂�I�u�W�F�N�g�����B
		FOR(i, npcInfo.size())
		{
			//����
			NPC* npc = INSTANCE(GameObjectManager)->AddNew<NPC>("NPC", 2);
			npc->LoadModel(npcInfo[i]->filename);
			npc->SetMesseage(npcInfo[i]->MesseageID, npcInfo[i]->ShowTitle);
			npc->transform->SetLocalPosition(npcInfo[i]->pos + Vector3(0, 6, 0));
			npc->transform->SetLocalAngle(npcInfo[i]->ang);
			npc->transform->SetLocalScale(npcInfo[i]->sca);
			_GameObjects[continent].push_back(npc);

			//��������Ȃ��̂ŉ��
			SAFE_DELETE(npcInfo[i]);
		}

		npcInfo.clear();
	}
}

const int HistoryManager::_CalcPattern(const HistoryInfo * info)
{
	int pattern = 0;
	//�嗤�ɍ������O���[�v�V�[�g�ǂݍ���
	char path[256] = { "Asset/Data/Village" }; 
	char idx[] = { '0' /*+ info->ContinentID*/,0 };
	char Group[] = { "Group.csv" };
	strcat(path, idx);
	strcat(path, Group);
	vector<VillageGroup*> groupList;
	Support::LoadCSVData<VillageGroup>(path, VillageGroupData, ARRAY_SIZE(VillageGroupData), groupList);
	
	for each (VillageGroup* group in groupList)
	{
		//�e�X���b�g���r
		if (group->Slot[0] != info->Chips[0])
			continue;
		if (group->Slot[1] != info->Chips[1])
			continue;
		if (group->Slot[2] != info->Chips[2])
			continue;

		//�S�Ĉ�v
		pattern = group->GroupID;
		break;
	}

	//�s�v�ɂȂ����̂ŉ���B
	FOR(i, groupList.size())
	{
		SAFE_DELETE(groupList[i]);
	}
	return pattern;
}
