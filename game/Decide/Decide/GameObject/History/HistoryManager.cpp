#include"stdafx.h"
#include "HistoryManager.h"

#include "GameObject\Village\ContinentObject.h"
#include "GameObject\Village\NPC.h"

/** インスタンス. */
HistoryManager* HistoryManager::_Instance = nullptr;

/**
* コンストラクタ.
*/
HistoryManager::HistoryManager()
{
	//CSVから歴史情報読み取り。
	Support::LoadCSVData<LocationHistoryInfo>("Asset/Data/LocationHistory.csv", HistoryInfoData, ARRAY_SIZE(HistoryInfoData), _LocationHistoryList);

	FOR(i, _LocationHistoryList.size())
	{
		//vectorを追加
		vector<GameObject*> list;
		_GameObjectList.push_back(list);
	}

}

/**
* 初期化.
*/
void HistoryManager::Start()
{
	_HistoryMenu = (HistoryMenu*)INSTANCE(GameObjectManager)->FindObject("HistoryMenu");
}

/**
* 歴史オブジェクト生成.
*/
void HistoryManager::CreateObject()
{
	FOR(i, _LocationHistoryList.size())
	{
		//歴史のオブジェクト切り替え・生成
		_ChangeLocation((LocationCodeE)i);
	}

	//共通オブジェクト生成
	char* type[2] = { "Obj","NPC" };
	char path[128];
	{		
		//パス生成
#ifdef DEBUG_SPLITSPACE
		sprintf(path, "Asset/Data/GroupData/_CommonGroup%s.csv", type[0]);
#else
		sprintf(path, "Asset/Data/GroupData/CommonGroup%s.csv", type[0]);
#endif
		_CreateObject(-1, path);
	}
}

/**
* 歴史を変える.
*
* @param location	場所ID.
* @param slot		スロット番号.
* @param chip		チップID.
*/
bool HistoryManager::SetHistoryChip(LocationCodeE location, UINT slot, ChipID chip)
{
	//ひとまず入れるだけで上書されてしまう.
	_LocationHistoryList[(int)location]->_ChipSlot[slot] = chip;

	//変更したので歴史を改変させる.
	_ChangeLocation(location);

	//データを保存.
	Support::OutputCSV<LocationHistoryInfo>("Asset/Data/LocationHistory.csv", HistoryInfoData, ARRAY_SIZE(HistoryInfoData), _LocationHistoryList);

	return true;
}

/**
* 場所の変化.
*
* @param location	場所ID.
*/
void HistoryManager::_ChangeLocation(LocationCodeE location)
{
	//前のオブジェクトを削除
	for(auto& it : _GameObjectList[(int)location])
	{
		INSTANCE(GameObjectManager)->AddRemoveList(it);
	}
	_GameObjectList[(int)location].clear();

	//チップの状態からグループを計算。
	const int group = _CalcPattern(_LocationHistoryList[(int)location]);

	char* type[2] = { "Obj","NPC" };
	char path[128];
	
	//OBJ
	{
		//パス生成
		sprintf(path, "Asset/Data/GroupData/Group%c%s.csv", 'A' + group, type[0]);
		_CreateObject((int)location, path);
		
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
			_GameObjectList[(int)location].push_back(npc);

			//もういらないので解放
			SAFE_DELETE(npcInfo[i]);
		}

		npcInfo.clear();
	}
}

/**
* パターン計算.
*/
int HistoryManager::_CalcPattern(const LocationHistoryInfo * info)
{
	//大陸に合ったグループシート読み込み
	char path[256];
	sprintf(path, "Asset/Data/Village%dGroup.csv", 0/*+ info->ContinentID*/);
	//CSVからグループ情報読み込み
	vector<VillageGroup*> groupList;
	Support::LoadCSVData<VillageGroup>(path, VillageGroupData, ARRAY_SIZE(VillageGroupData), groupList);
	
	int pattern = 0;
	//一致するものがあるか調べる。
	for(auto& group : groupList)
	{
		bool isMatch = true;
		//各スロットを比較
		for (int i = 0; i < (int)ChipID::ChipNum; i++)
		{
			if (group->Slot[i] != info->_ChipSlot[i])
			{
				isMatch = false;
			}
		}

		if (!isMatch)
		{
			//マッチングしていないので次へ.
			continue;
		}

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

/**
* オブジェクトを作成.
*
* @param location	場所ID.
* @param path		フォルダパス.
*/
void HistoryManager::_CreateObject(int location,const char * path)
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
		if (location >= 0)
		{
			_GameObjectList[location].push_back(obj);
		}

		//もういらないので解放
		SAFE_DELETE(objInfo[i]);
	}

	objInfo.clear();
}

/**
* コリジョンを作成.
*
* @param location	場所ID.
* @param path		フォルダパス.
*/
void HistoryManager::_CreateCollision(int location, const char * path)
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
		if (location >= 0)
		{
			_GameObjectList[location].push_back(coll);
		}

		//もういらないので解放
		SAFE_DELETE(colls[i]);
	}

	colls.clear();
}
