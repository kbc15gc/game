#include"stdafx.h"
#include "HistoryManager.h"

#include "GameObject\Village\ContinentObject.h"
#include "GameObject\Village\NPC.h"

/** �C���X�^���X. */
HistoryManager* HistoryManager::_Instance = nullptr;

/**
* �R���X�g���N�^.
*/
HistoryManager::HistoryManager()
{
	//CSV������j���ǂݎ��B
	Support::LoadCSVData<LocationHistoryInfo>("Asset/Data/LocationHistory.csv", HistoryInfoData, ARRAY_SIZE(HistoryInfoData), _LocationHistoryList);

	FOR(i, _LocationHistoryList.size())
	{
		//vector��ǉ�
		vector<GameObject*> list;
		_GameObjectList.push_back(list);
	}

}

/**
* ������.
*/
void HistoryManager::Start()
{
	_HistoryMenu = (HistoryMenu*)INSTANCE(GameObjectManager)->FindObject("HistoryMenu");
}

/**
* ���j�I�u�W�F�N�g����.
*/
void HistoryManager::CreateObject()
{
	FOR(i, _LocationHistoryList.size())
	{
		//���j�̃I�u�W�F�N�g�؂�ւ��E����
		_ChangeLocation((LocationCodeE)i);
	}

	//���ʃI�u�W�F�N�g����
	char* type[2] = { "Obj","NPC" };
	char path[128];
	{		
		//�p�X����
#ifdef DEBUG_SPLITSPACE
		sprintf(path, "Asset/Data/GroupData/_CommonGroup%s.csv", type[0]);
#else
		sprintf(path, "Asset/Data/GroupData/CommonGroup%s.csv", type[0]);
#endif
		_CreateObject(-1, path);
	}
}

/**
* ���j��ς���.
*
* @param location	�ꏊID.
* @param slot		�X���b�g�ԍ�.
* @param chip		�`�b�vID.
*/
bool HistoryManager::SetHistoryChip(LocationCodeE location, UINT slot, ChipID chip)
{
	//�ЂƂ܂�����邾���ŏ㏑����Ă��܂�.
	_LocationHistoryList[(int)location]->_ChipSlot[slot] = chip;

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
	//�O�̃I�u�W�F�N�g���폜
	for(auto& it : _GameObjectList[(int)location])
	{
		INSTANCE(GameObjectManager)->AddRemoveList(it);
	}
	_GameObjectList[(int)location].clear();

	//�`�b�v�̏�Ԃ���O���[�v���v�Z�B
	const int group = _CalcPattern(_LocationHistoryList[(int)location]);

	char* type[2] = { "Obj","NPC" };
	char path[128];
	
	//OBJ
	{
		//�p�X����
		sprintf(path, "Asset/Data/GroupData/Group%c%s.csv", 'A' + group, type[0]);
		_CreateObject((int)location, path);
		
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
			_GameObjectList[(int)location].push_back(npc);

			//��������Ȃ��̂ŉ��
			SAFE_DELETE(npcInfo[i]);
		}

		npcInfo.clear();
	}
}

/**
* �p�^�[���v�Z.
*/
int HistoryManager::_CalcPattern(const LocationHistoryInfo * info)
{
	//�嗤�ɍ������O���[�v�V�[�g�ǂݍ���
	char path[256];
	sprintf(path, "Asset/Data/Village%dGroup.csv", 0/*+ info->ContinentID*/);
	//CSV����O���[�v���ǂݍ���
	vector<VillageGroup*> groupList;
	Support::LoadCSVData<VillageGroup>(path, VillageGroupData, ARRAY_SIZE(VillageGroupData), groupList);
	
	int pattern = 0;
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

/**
* �I�u�W�F�N�g���쐬.
*
* @param location	�ꏊID.
* @param path		�t�H���_�p�X.
*/
void HistoryManager::_CreateObject(int location,const char * path)
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
		if (location >= 0)
		{
			_GameObjectList[location].push_back(obj);
		}

		//��������Ȃ��̂ŉ��
		SAFE_DELETE(objInfo[i]);
	}

	objInfo.clear();
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
		if (location >= 0)
		{
			_GameObjectList[location].push_back(coll);
		}

		//��������Ȃ��̂ŉ��
		SAFE_DELETE(colls[i]);
	}

	colls.clear();
}
