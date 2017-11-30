#include"stdafx.h"
#include "HistoryManager.h"
#include "GameObject\Village\ContinentObject.h"
#include "fbEngine\_Object\_Component\_Physics\BoxCollider.h"

namespace
{
	//オブジェクトを識別するタイプ。
	const char* ObjectType[3] = { "Obj","NPC","Enemy" };
}


/** インスタンス. */
HistoryManager* HistoryManager::_Instance = nullptr;

/**
* コンストラクタ.
*/
HistoryManager::HistoryManager()
{
	for (int i = 0; i < (int)LocationCodeE::LocationNum; i++)
	{
		//グループ状態を0で初期化。
		_NowGroupIDList.push_back(0);

		//vectorを追加
		vector<GameObject*> list;
		_GameObjectList.push_back(list);
		vector<NPC*> npcs;
		_NPCList.push_back(npcs);

		_EnemyInfoList = vector<vector<unique_ptr<LoadEnemyInfo::EnemyInfo>>>(static_cast<int>(LocationCodeE::LocationNum));
	}
}

/**
* 初期化.
*/
void HistoryManager::Start()
{
	_LocationHistoryList.clear();

	if (IS_CONTINUE)
	{
		//CSVから歴史情報読み取り。
		Support::LoadCSVData<LocationHistoryInfo>("Asset/Data/LocationHistory.csv", HistoryInfoData, ARRAY_SIZE(HistoryInfoData), _LocationHistoryList);
	}
	else
	{
		//CSVから歴史情報読み取り。
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
	for (int type = static_cast<int>(LoadObjectType::Object); type < static_cast<int>(LoadObjectType::Max); type++) {
		//パス生成
		sprintf(path, "Asset/Data/GroupData/CommonGroup%s.csv", ObjectType[type]);
		_CreateObject(LocationCodeE::Common, path, static_cast<LoadObjectType>(type));
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
			HistoryPage* page = _HistoryBook->PutInChip(_LocationHistoryList.at(i)->_ChipSlot[j], _LocationHistoryList.at(i)->_LocationID, j);
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
bool HistoryManager::SetHistoryChip(LocationCodeE location, ChipID chip, int index)
{
	_HistoryBook->PutInChip(chip, location, index);
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

		for (int type = static_cast<int>(LoadObjectType::Object); type < static_cast<int>(LoadObjectType::Max); type++) {
			//パス生成
			sprintf(path, "Asset/Data/GroupData/Group%d%c%s.csv", (int)location, 'A' + group, ObjectType[type]);
			_CreateObject(location, path, static_cast<LoadObjectType>(type));
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
void HistoryManager::_CreateObject(LocationCodeE location, const char * path, HistoryManager::LoadObjectType type)
{
	if (type == HistoryManager::LoadObjectType::Object)
	{
		//前のオブジェクトを削除
		for (auto obj : _GameObjectList[static_cast<int>(location)])
		{
			INSTANCE(GameObjectManager)->AddRemoveList(obj);
		}
		_GameObjectList[static_cast<int>(location)].clear();

		//生成。
		CreateBuilding(path, _GameObjectList[static_cast<int>(location)]);

		//第3の村以降のみ。
		if (location == LocationCodeE::Prosperity || location == LocationCodeE::DevilKingdom)
		{
			for (auto obj : _GameObjectList[static_cast<int>(location)]) {
				//X軸に180ど回転させる。
				//Quaternion q = Quaternion::Identity;
				//q.SetRotation(Vector3::axisX, PI);
				//q.Multiply(obj->transform->GetRotation());
				//obj->transform->SetRotation(q);
			}
		}
	}
	else if (type == HistoryManager::LoadObjectType::NPC)
	{
		//前のNPCを削除
		for (auto npc : _NPCList[(int)location])
		{
			INSTANCE(GameObjectManager)->AddRemoveList(npc);
		}
		_NPCList[(int)location].clear();
		//生成。
		_CreateNPC(location, path);
	}
	else if (type == HistoryManager::LoadObjectType::Enemy) {
		// エネミーの情報を読み込み。
		_CreateEnemy(location, path);
	}
}

vector<GameObject*>& HistoryManager::CreateBuilding(const char* path, vector<GameObject*>& Builds)
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

			objInfo[i]->sca.y *= -1.0f;
			obj->transform->SetLocalScale(objInfo[i]->sca);
			obj->LoadModel(objInfo[i]->filename, objInfo[i]->coll);

			//配列に追加。
			Builds.push_back(obj);

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

				//コリジョンを自分で作成している場合は作成する。
				//名前チェック
				if (strcmp(info->filename, "coll") == 0 && objInfo[i]->coll)
				{
					//コリジョンを生成してゲームオブジェクトにアタッチ。
					BoxCollider* box = obj->AddComponent<BoxCollider>();
					RigidBody* coll = obj->AddComponent<RigidBody>();

					box->Create(Vector3(fabsf(info->sca.x), fabsf(info->sca.y), fabsf(info->sca.z)));
					RigidBodyInfo Rinfo;
					Rinfo.physicsType = Collision::PhysicsType::Static;
					Rinfo.mass = 0.0f;
					Rinfo.coll = box;
					//カメラと当たらないコリジョンかどうか？
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
	//CSVからオブジェクトの情報読み込み
	vector<unique_ptr<npc::NPCInfo>> npcInfo;
	Support::LoadCSVData<npc::NPCInfo>(path, npc::NPCInfoData, ARRAY_SIZE(npc::NPCInfoData), npcInfo);

	//情報からオブジェクト生成。
	FOR(i, npcInfo.size())
	{
		//生成
		NPC* npc = INSTANCE(GameObjectManager)->AddNew<NPC>(npcInfo[i]->filename, 2);
		npc->CreateNPC(npcInfo[i].get());
		
		auto model = npc->GetComponent<SkinModel>();
		model->GetModelData()->SetInstancing(false);
		model->SetCullMode(D3DCULL::D3DCULL_CCW);

		//管理用の配列に追加。
		_NPCList[static_cast<int>(location)].push_back(npc);
	}
	//いらんので破棄。
	npcInfo.clear();
}

void HistoryManager::_CreateEnemy(LocationCodeE location, const char * path) {
	// 前のエネミーを削除。
	if (_EnemyInfoList[(int)location].size() > 0) {
		_EnemyInfoList[(int)location].clear();
	}

	//CSVからエネミー情報読み取り。
	Support::LoadCSVData<LoadEnemyInfo::EnemyInfo>(path, LoadEnemyInfo::EnemyInfoDecl, ARRAY_SIZE(LoadEnemyInfo::EnemyInfoDecl), _EnemyInfoList[static_cast<int>(location)]);

	INSTANCE(EnemyManager)->CreateEnemys(location, _EnemyInfoList[static_cast<int>(location)]);
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