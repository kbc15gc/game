#include"stdafx.h"
#include "HistoryManager.h"

#include "GameObject\Village\HistoryInfo.h"
#include "GameObject\Village\ContinentObject.h"
#include "GameObject\Village\NPC.h"


HistoryManager* HistoryManager::_Instance = nullptr;

HistoryManager::HistoryManager()
{
	//CSVから歴史情報読み取り。
	Support::LoadCSVData<HistoryInfo>("Asset/Data/VillageHistory.csv", HistoryInfoData, ARRAY_SIZE(HistoryInfoData), _HistoryList);

	FOR(i, _HistoryList.size())
	{
		//vectorを追加
		vector<GameObject*> list;
		_GameObjectList.push_back(list);
	}
}

void HistoryManager::CreateObject()
{
	FOR(i, _HistoryList.size())
	{
		//歴史のオブジェクト切り替え・生成
		_ChangeContinent(i);
	}

	//共通オブジェクト生成
	char* type[2] = { "Obj","NPC" };
	char path[128];
	{		
		//パス生成
		sprintf(path, "Asset/Data/GroupData/CommonGroup%s.csv", type[0]);
		_CreateObject(-1, path);
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
	//前のオブジェクトを削除
	{
		for each (GameObject* obj in _GameObjectList[continent])
		{
			INSTANCE(GameObjectManager)->AddRemoveList(obj);
		}
		_GameObjectList[continent].clear();
	}

	//チップの状態からグループを計算。
	const int group = _CalcPattern(_HistoryList[continent]);

	char* type[2] = { "Obj","NPC" };
	char path[128];
	
	//OBJ
	{
		//パス生成
		sprintf(path, "Asset/Data/GroupData/Group%c%s.csv", 'A' + group, type[0]);
		_CreateObject(continent, path);
		
	}
	ZeroMemory(path, 128);
	//NPC
	{
		//パス生成
		sprintf(path, "Asset/Data/GroupData/Group%c%s.csv", 'A' + group, type[1]);
		
		
		//CSVからオブジェクトの情報読み込み
		vector<NPCInfo*> npcInfo;
		Support::LoadCSVData<NPCInfo>(path, NPCInfoData, ARRAY_SIZE(NPCInfoData), npcInfo);

		//情報からオブジェクト生成。
		FOR(i, npcInfo.size())
		{
			//生成
			NPC* npc = INSTANCE(GameObjectManager)->AddNew<NPC>("NPC", 2);
			npc->LoadModel(npcInfo[i]->filename);
			npc->SetMesseage(npcInfo[i]->MesseageID, npcInfo[i]->ShowTitle);
			npc->transform->SetLocalPosition(npcInfo[i]->pos);
			npc->transform->SetLocalAngle(npcInfo[i]->ang);
			npc->transform->SetLocalScale(npcInfo[i]->sca);
			//管理用の配列に追加。
			_GameObjectList[continent].push_back(npc);

			//もういらないので解放
			SAFE_DELETE(npcInfo[i]);
		}

		npcInfo.clear();
	}
}

const int HistoryManager::_CalcPattern(const HistoryInfo * info)
{
	//大陸に合ったグループシート読み込み
	char path[256];
	sprintf(path, "Asset/Data/Village%dGroup.csv", 0/*+ info->ContinentID*/);
	//CSVからグループ情報読み込み
	vector<VillageGroup*> groupList;
	Support::LoadCSVData<VillageGroup>(path, VillageGroupData, ARRAY_SIZE(VillageGroupData), groupList);
	
	int pattern = 0;
	//一致するものがあるか調べる。
	for each (VillageGroup* group in groupList)
	{
		//各スロットを比較
		if (group->Slot[0] != info->Slot[0])
			continue;
		if (group->Slot[1] != info->Slot[1])
			continue;
		if (group->Slot[2] != info->Slot[2])
			continue;

		//パターン一致したのでID設定。
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

void HistoryManager::_CreateObject(const int& continent,const char * path)
{
	//CSVからオブジェクトの情報読み込み
	vector<ObjectInfo*> objInfo;
	Support::LoadCSVData<ObjectInfo>(path, ObjectInfoData, ARRAY_SIZE(ObjectInfoData), objInfo);

	//情報からオブジェクト生成。
	FOR(i, objInfo.size())
	{
		//生成
		ContinentObject* obj = INSTANCE(GameObjectManager)->AddNew<ContinentObject>("ContinentObject", 2);
		obj->LoadModel(objInfo[i]->filename);
		obj->transform->SetLocalPosition(objInfo[i]->pos);
		obj->transform->SetLocalAngle(objInfo[i]->ang);
		obj->transform->SetLocalScale(objInfo[i]->sca);
		//管理用の配列に追加。
		if (continent >= 0)
			_GameObjectList[continent].push_back(obj);

		//もういらないので解放
		SAFE_DELETE(objInfo[i]);
	}

	objInfo.clear();
}

void HistoryManager::_CreateCollision(const int & continent, const char * path)
{
	//CSVから当たり判定の情報読み込み
	vector<CollisionInfo*> colls;
	Support::LoadCSVData(path, CollisionInfoData, ARRAY_SIZE(CollisionInfoData), colls);

	//情報から当たり判定生成。
	FOR(i, colls.size())
	{
		//生成
		ContinentObject* coll = INSTANCE(GameObjectManager)->AddNew<ContinentObject>("StageCollision", 2);
		
		coll->transform->SetLocalPosition(colls[i]->pos);
		coll->transform->SetLocalAngle(colls[i]->ang);
		coll->transform->SetLocalScale(colls[i]->sca);
		//管理用の配列に追加。
		if (continent >= 0)
			_GameObjectList[continent].push_back(coll);

		//もういらないので解放
		SAFE_DELETE(colls[i]);
	}

	colls.clear();
}
