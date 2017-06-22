#include"stdafx.h"
#include "HistoryManager.h"

#include "GameObject\Village\HistoryInfo.h"
#include "GameObject\Village\ContinentObject.h"
#include "GameObject\Village\NPC.h"


HistoryManager* HistoryManager::_Instance = nullptr;

HistoryManager::HistoryManager()
{
	//CSV������j���ǂݎ��B
	Support::LoadCSVData<HistoryInfo>("Asset/Data/VillageHistory.csv", HistoryInfoData, ARRAY_SIZE(HistoryInfoData), _HistoryList);

	FOR(i, _HistoryList.size())
	{
		//vector��ǉ�
		vector<GameObject*> list;
		_GameObjectList.push_back(list);
	}
}

void HistoryManager::CreateObject()
{
	FOR(i, _HistoryList.size())
	{
		//���j�̃I�u�W�F�N�g�؂�ւ��E����
		_ChangeContinent(i);
	}

	//���ʃI�u�W�F�N�g����
	char* type[2] = { "Obj","NPC" };
	char path[128];
	{		
		//�p�X����
		sprintf(path, "Asset/Data/GroupData/CommonGroup%s.csv", type[0]);
		_CreateObject(-1, path);
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
	//�O�̃I�u�W�F�N�g���폜
	{
		for each (GameObject* obj in _GameObjectList[continent])
		{
			INSTANCE(GameObjectManager)->AddRemoveList(obj);
		}
		_GameObjectList[continent].clear();
	}

	//�`�b�v�̏�Ԃ���O���[�v���v�Z�B
	const int group = _CalcPattern(_HistoryList[continent]);

	char* type[2] = { "Obj","NPC" };
	char path[128];
	
	//OBJ
	{
		//�p�X����
		sprintf(path, "Asset/Data/GroupData/Group%c%s.csv", 'A' + group, type[0]);
		_CreateObject(continent, path);
		
	}
	ZeroMemory(path, 128);
	//NPC
	{
		//�p�X����
		sprintf(path, "Asset/Data/GroupData/Group%c%s.csv", 'A' + group, type[1]);
		
		
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
			npc->transform->SetLocalPosition(npcInfo[i]->pos);
			npc->transform->SetLocalAngle(npcInfo[i]->ang);
			npc->transform->SetLocalScale(npcInfo[i]->sca);
			//�Ǘ��p�̔z��ɒǉ��B
			_GameObjectList[continent].push_back(npc);

			//��������Ȃ��̂ŉ��
			SAFE_DELETE(npcInfo[i]);
		}

		npcInfo.clear();
	}
}

const int HistoryManager::_CalcPattern(const HistoryInfo * info)
{
	//�嗤�ɍ������O���[�v�V�[�g�ǂݍ���
	char path[256];
	sprintf(path, "Asset/Data/Village%dGroup.csv", 0/*+ info->ContinentID*/);
	//CSV����O���[�v���ǂݍ���
	vector<VillageGroup*> groupList;
	Support::LoadCSVData<VillageGroup>(path, VillageGroupData, ARRAY_SIZE(VillageGroupData), groupList);
	
	int pattern = 0;
	//��v������̂����邩���ׂ�B
	for each (VillageGroup* group in groupList)
	{
		//�e�X���b�g���r
		if (group->Slot[0] != info->Slot[0])
			continue;
		if (group->Slot[1] != info->Slot[1])
			continue;
		if (group->Slot[2] != info->Slot[2])
			continue;

		//�p�^�[����v�����̂�ID�ݒ�B
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

void HistoryManager::_CreateObject(const int& continent,const char * path)
{
	//CSV����I�u�W�F�N�g�̏��ǂݍ���
	vector<ObjectInfo*> objInfo;
	Support::LoadCSVData<ObjectInfo>(path, ObjectInfoData, ARRAY_SIZE(ObjectInfoData), objInfo);

	//��񂩂�I�u�W�F�N�g�����B
	FOR(i, objInfo.size())
	{
		//����
		ContinentObject* obj = INSTANCE(GameObjectManager)->AddNew<ContinentObject>("ContinentObject", 2);
		obj->LoadModel(objInfo[i]->filename);
		obj->transform->SetLocalPosition(objInfo[i]->pos);
		obj->transform->SetLocalAngle(objInfo[i]->ang);
		obj->transform->SetLocalScale(objInfo[i]->sca);
		//�Ǘ��p�̔z��ɒǉ��B
		if (continent >= 0)
			_GameObjectList[continent].push_back(obj);

		//��������Ȃ��̂ŉ��
		SAFE_DELETE(objInfo[i]);
	}

	objInfo.clear();
}

void HistoryManager::_CreateCollision(const int & continent, const char * path)
{
	//CSV���瓖���蔻��̏��ǂݍ���
	vector<CollisionInfo*> colls;
	Support::LoadCSVData(path, CollisionInfoData, ARRAY_SIZE(CollisionInfoData), colls);

	//��񂩂瓖���蔻�萶���B
	FOR(i, colls.size())
	{
		//����
		ContinentObject* coll = INSTANCE(GameObjectManager)->AddNew<ContinentObject>("StageCollision", 2);
		
		coll->transform->SetLocalPosition(colls[i]->pos);
		coll->transform->SetLocalAngle(colls[i]->ang);
		coll->transform->SetLocalScale(colls[i]->sca);
		//�Ǘ��p�̔z��ɒǉ��B
		if (continent >= 0)
			_GameObjectList[continent].push_back(coll);

		//��������Ȃ��̂ŉ��
		SAFE_DELETE(colls[i]);
	}

	colls.clear();
}
