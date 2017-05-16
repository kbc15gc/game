#include"stdafx.h"
#include "HistoryManager.h"

#include "GameObject\Village\HistoryInfo.h"
#include "GameObject\Village\ContinentObject.h"
//#include "GameObject\Village\NPC.h"


HistoryManager* HistoryManager::_Instance = nullptr;

HistoryManager::HistoryManager()
{
	Support::LoadCSVData<HistoryInfo>("Asset/Data/VillageHistory.csv", HistoryInfoData, ARRAY_SIZE(HistoryInfoData), _HistoryList);

	FOR(i, _HistoryList.size())
	{
		//vectorを追加
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
	//成功したかどうか？
	bool success = false;
	//歴史のチップセット
	if(success =_HistoryList[continent]->SetChip(idx, (ChipID)chip))
	{
		//無事にセットされたなら大陸を変化させる。
		_ChangeContinent(continent);
		//csv保存処理
		Support::OutputCSV<HistoryInfo>("Asset/Data/VillageHistory.csv", HistoryInfoData, ARRAY_SIZE(HistoryInfoData), _HistoryList);
	}
	return success;
}

void HistoryManager::_ChangeContinent(const unsigned int& continent)
{
	//前のやつ削除してクリア
	for each (GameObject* obj in _GameObjects[continent])
	{
		INSTANCE(GameObjectManager)->AddRemoveList(obj);
	}
	_GameObjects[continent].clear();

	//大陸の変化のパターン計算
	int pattern = _CalcPattern(_HistoryList[continent]);

	char dirpath[] = "Asset/Data/Group";
	char group[] = { 'A' + pattern,0 };
	char* type[2] = { "Obj","NPC" };
	char path[128];
	
	//OBJ
	{
		//パス生成
		strcpy(path, dirpath);
		strcat(path, group);
		strcat(path, type[0]);
		strcat(path, ".csv");
		//CSVからオブジェクトの情報読み込み
		vector<ObjectInfo*> objInfo;
		Support::LoadCSVData<ObjectInfo>(path, ObjectInfoData, ARRAY_SIZE(ObjectInfoData), objInfo);

		//情報からオブジェクト生成。
		FOR(i, objInfo.size())
		{
			//生成
			ContinentObject* obj = INSTANCE(GameObjectManager)->AddNew<ContinentObject>("ContinentObject", 2);
			obj->LoadModel(objInfo[i]->filename);
			obj->transform->SetLocalPosition(objInfo[i]->pos + Vector3(0, 6, 0));
			obj->transform->SetLocalAngle(objInfo[i]->ang);
			obj->transform->SetLocalScale(objInfo[i]->sca);
			_GameObjects[continent].push_back(obj);

			//もういらないので解放
			SAFE_DELETE(objInfo[i]);
		}

		objInfo.clear();
	}
	ZeroMemory(path, 128);
	//NPC
	{
		//パス生成
		strcpy(path, dirpath);
		strcat(path, group);
		strcat(path, type[1]);
		strcat(path, ".csv");
		//CSVからオブジェクトの情報読み込み
		//vector<NPCInfo*> npcInfo;
		//Support::LoadCSVData<NPCInfo>(path, NPCInfoData, ARRAY_SIZE(NPCInfoData), npcInfo);

		//情報からオブジェクト生成。
		//FOR(i, npcInfo.size())
		//{
		//	//生成
		//	NPC* npc = INSTANCE(GameObjectManager)->AddNew<NPC>("NPC", 2);
		//	npc->LoadModel(npcInfo[i]->filename);
		//	npc->SetMesseage(npcInfo[i]->MesseageID, npcInfo[i]->ShowTitle);
		//	npc->transform->SetLocalPosition(npcInfo[i]->pos + Vector3(0, 6, 0));
		//	npc->transform->SetLocalAngle(npcInfo[i]->ang);
		//	npc->transform->SetLocalScale(npcInfo[i]->sca);
		//	_GameObjects[continent].push_back(npc);

		//	//もういらないので解放
		//	SAFE_DELETE(npcInfo[i]);
		//}

		//npcInfo.clear();
	}
}

const int HistoryManager::_CalcPattern(const HistoryInfo * info)
{
	int pattern = 0;
	//大陸に合ったグループシート読み込み
	char path[256] = { "Asset/Data/Village" }; 
	char idx[] = { '0' /*+ info->ContinentID*/,0 };
	char Group[] = { "Group.csv" };
	strcat(path, idx);
	strcat(path, Group);
	vector<VillageGroup*> groupList;
	Support::LoadCSVData<VillageGroup>(path, VillageGroupData, ARRAY_SIZE(VillageGroupData), groupList);
	
	for each (VillageGroup* group in groupList)
	{
		//各スロットを比較
		if (group->Slot[0] != info->Chips[0])
			continue;
		if (group->Slot[1] != info->Chips[1])
			continue;
		if (group->Slot[2] != info->Chips[2])
			continue;

		//全て一致
		pattern = group->GroupID;
		break;
	}

	//不要になったので解放。
	FOR(i, groupList.size())
	{
		SAFE_DELETE(groupList[i]);
	}
	return pattern;
}
