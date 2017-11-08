#include"stdafx.h"
#include "HistoryManager.h"
#include "GameObject\Village\ContinentObject.h"
#include "fbEngine\_Object\_Component\_Physics\BoxCollider.h"

namespace
{
	//オブジェクトを識別するタイプ。
	const char* ObjectType[2] = { "Obj","NPC" };
}


/** インスタンス. */
HistoryManager* HistoryManager::_Instance = nullptr;

/**
* コンストラクタ.
*/
HistoryManager::HistoryManager()
{
	//CSVから歴史情報読み取り。
	Support::LoadCSVData<LocationHistoryInfo>("Asset/Data/LocationHistory.csv", HistoryInfoData, ARRAY_SIZE(HistoryInfoData), _LocationHistoryList);

	for (int i = 0; i < (int)LocationCodeE::LocationNum; i++)
	{
		//グループ状態を0で初期化。
		_NowGroupIDList.push_back(0);

		//vectorを追加
		vector<GameObject*> list;
		_GameObjectList.push_back(list);
		vector<NPC*> npcs;
		_NPCList.push_back(npcs);
	}
}

/**
* 初期化.
*/
void HistoryManager::Start()
{
	_HistoryMenu = (HistoryMenu*)INSTANCE(GameObjectManager)->FindObject("HistoryMenu");
	_HistoryBook = (HistoryBook*)INSTANCE(GameObjectManager)->FindObject("HistoryBook");

	_MysteryLight = INSTANCE(GameObjectManager)->AddNew<MysteryLight>("MysteryLight", 9);

//木が邪魔な場合これを使ってください。
//#define NPCONLY

#ifdef NPCONLY
	//共通オブジェクト生成。
	char path[128];
		//パス生成
	sprintf(path, "Asset/Data/GroupData/CommonGroup%s.csv", ObjectType[1]);
	_CreateObject((int)LocationCodeE::Common, path, 1);
#else // NPCONLY
	//共通オブジェクト生成。
	char path[128];
	FOR(type, 2)
	{
		//パス生成
		sprintf(path, "Asset/Data/GroupData/CommonGroup%s.csv", ObjectType[type]);
		_CreateObject((int)LocationCodeE::Common, path, type);
	}

#endif
	
	//歴史オブジェクト生成。
	FOR(i, _LocationHistoryList.size())
	{
		for (int j = 0; j < (int)ChipID::ChipNum; j++)
		{
			if (_LocationHistoryList.at(i)->_ChipSlot[j] == ChipID::None)
			{
				continue;
			}
			//読み込んだデータを元に歴史書にページをあらかじめ追加。
			HistoryPage* page = _HistoryBook->PutInChip(_LocationHistoryList.at(i)->_ChipSlot[j], _LocationHistoryList.at(i)->_LocationID);
			page->transform->SetPosition(Vector3(0.0f, 0.0f, 0.2f));
			page->ChangeState(HistoryPage::StateCodeE::Close);
		}
		_ChangeLocation(_LocationHistoryList.at(i)->_LocationID);
	}
}

/**
* 歴史を変える.
*
* @param location	場所ID.
* @param slot		スロット番号.
* @param chip		チップID.
*/
bool HistoryManager::SetHistoryChip(LocationCodeE location, ChipID chip)
{
	_HistoryBook->PutInChip(chip, location);
	//ひとまず入れるだけで上書されてしまう.
	_LocationHistoryList[(int)location]->SetData(_HistoryBook->GetLocationList(location));

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
	//チップの状態からグループを計算。
	const int group = _CalcPattern(_LocationHistoryList[(int)location].get());
	//どれかのグループに該当するのなら。
	if (group >= 0)
	{

		if (_NowGroupIDList[(int)location] != group)
		{
			_MysteryLight->SetActive(true, true);
			_NowGroupIDList[(int)location] = group;
		}

		char path[128];

		FOR(type,2)		
		{
			//パス生成
			sprintf(path, "Asset/Data/GroupData/Group%d%c%s.csv", (int)location, 'A' + group, ObjectType[type]);
			_CreateObject((int)location, path, type);
			ZeroMemory(path, 128);
		}
	}
}

/**
* パターン計算.
*/
int HistoryManager::_CalcPattern(const LocationHistoryInfo * info)
{
	//大陸に合ったグループシート読み込み
	char path[256];
	sprintf(path, "Asset/Data/Village%dGroup.csv", info->_LocationID);
	//CSVからグループ情報読み込み
	vector<unique_ptr<VillageGroup>> groupList;
	Support::LoadCSVData<VillageGroup>(path, VillageGroupData, ARRAY_SIZE(VillageGroupData), groupList);
	
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
		return group->GroupID;
	}

	return -1;
}

/**
* オブジェクトを作成.
*
* @param location	場所ID.
* @param path		フォルダパス.
* @param type		生成するオブジェクトのタイプ.
*/
void HistoryManager::_CreateObject(int location, const char * path, int type)
{
	if (type == 0)
	{
		//前のオブジェクトを削除
		for (auto& it : _GameObjectList[(int)location])
		{
			INSTANCE(GameObjectManager)->AddRemoveList(it);
		}
		_GameObjectList[(int)location].clear();
		//生成。
		_CreateBuilding(location, path);
	}
	else if (type == 1)
	{
		//前のNPCを削除
		for (auto& it : _NPCList[(int)location])
		{
			INSTANCE(GameObjectManager)->AddRemoveList(it);
		}
		_NPCList[(int)location].clear();
		//生成。
		_CreateNPC(location, path);
	}
}

void HistoryManager::_CreateBuilding(int location, const char * path)
{
	//CSVからオブジェクトの情報読み込み
	vector<unique_ptr<ObjectInfo>> objInfo;
	Support::LoadCSVData<ObjectInfo>(path, ObjectInfoData, ARRAY_SIZE(ObjectInfoData), objInfo);

	//情報からオブジェクト生成。
	for (short i = 0; i < static_cast<int>(objInfo.size());)
	{
		//コリジョンかどうか？
		if (strcmp(objInfo[i]->filename, "coll") != 0)
		{
			//オブジェクト生成
			ContinentObject* obj = INSTANCE(GameObjectManager)->AddNew<ContinentObject>(objInfo[i]->filename, 2);

			obj->transform->SetLocalPosition(objInfo[i]->pos);
			
			obj->transform->SetRotation(objInfo[i]->ang);
			//第3の村だけ
			if(location == (int)LocationCodeE::Prosperity)
			{
				//X軸に180ど回転させる。
				Quaternion q = Quaternion::Identity;
				q.SetRotation(Vector3::axisX , PI);
				q.Multiply(objInfo[i]->ang);
				obj->transform->SetRotation(q);

			}

			objInfo[i]->sca.y *= -1.0f;
			obj->transform->SetLocalScale(objInfo[i]->sca);
			obj->LoadModel(objInfo[i]->filename);

			//管理用の配列に追加。
			_GameObjectList[location].push_back(obj);

			//次がコリジョンかどうか？
			while (true)
			{
				ObjectInfo* info;
				//範囲外チェック。
				try {
					info = objInfo.at(++i).get();
				}
				catch (std::out_of_range& ex) {
					break;
				}

				//名前チェック
				if (strcmp(info->filename, "coll") == 0)
				{
					//コリジョンを生成してゲームオブジェクトにアタッチ。
					//BoxCollider* box = obj->AddComponent<BoxCollider>();
					//RigidBody* coll = obj->AddComponent<RigidBody>();

					//box->Create(Vector3(fabsf(info->sca.x), fabsf(info->sca.y), fabsf(info->sca.z)));
					//RigidBodyInfo Rinfo;
					//Rinfo.physicsType = Collision::PhysicsType::Static;
					//Rinfo.mass = 0.0f;
					//Rinfo.coll = box;
					////カメラと当たらないコリジョンかどうか？
					//Rinfo.id = ((bool)info->hitcamera) ? Collision_ID::BUILDING : (Collision_ID::BUILDING | Collision_ID::NOTHITCAMERA);
					//Rinfo.offset = info->pos;
					//Quaternion q;
					//q.Multiply(info->ang);
					//Rinfo.rotation = q;
					//coll->Create(Rinfo, true);
				}
				else
				{
					break;
				}
			}
		}
	}

	objInfo.clear();
}

void HistoryManager::_CreateNPC(int location, const char * path)
{
	//CSVからオブジェクトの情報読み込み
	vector<unique_ptr<NPCInfo>> npcInfo;
	Support::LoadCSVData<NPCInfo>(path, NPCInfoData, ARRAY_SIZE(NPCInfoData), npcInfo);

	//情報からオブジェクト生成。
	FOR(i, npcInfo.size())
	{
		//生成
		NPC* npc = INSTANCE(GameObjectManager)->AddNew<NPC>(npcInfo[i]->filename, 2);
		
		npc->SetMesseage(npcInfo[i]->MesseageID, npcInfo[i]->ShowTitle);
		npc->transform->SetLocalPosition(npcInfo[i]->pos);
		npc->transform->SetRotation(npcInfo[i]->ang);
		npc->transform->SetLocalScale(npcInfo[i]->sca);

		npc->LoadModel(npcInfo[i]->filename);
		auto model = npc->GetComponent<SkinModel>();
		model->GetModelData()->SetInstancing(false);

		//管理用の配列に追加。
		_NPCList[location].push_back(npc);
	}
	//いらんので破棄。
	npcInfo.clear();
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
	vector<unique_ptr<CollisionInfo>> colls;
	Support::LoadCSVData<CollisionInfo>(path, CollisionInfoData, ARRAY_SIZE(CollisionInfoData), colls);

	//情報から当たり判定生成。
	FOR(i, colls.size())
	{
		//生成
		ContinentObject* coll = INSTANCE(GameObjectManager)->AddNew<ContinentObject>("StageCollision", 2);
		
		coll->transform->SetLocalPosition(colls[i]->pos);
		coll->transform->SetRotation(colls[i]->ang);
		coll->transform->SetLocalScale(colls[i]->sca);
		//管理用の配列に追加。
		_GameObjectList[location].push_back(coll);
	}

	colls.clear();
}

void HistoryManager::PutOutPage(LocationCodeE location,vector<HistoryPage*>& list)
{
	_LocationHistoryList[(int)location]->SetData(list);

	//変更したので歴史を改変させる.
	_ChangeLocation(location);

	//データを保存.
	Support::OutputCSV<LocationHistoryInfo>("Asset/Data/LocationHistory.csv", HistoryInfoData, ARRAY_SIZE(HistoryInfoData), _LocationHistoryList);
}