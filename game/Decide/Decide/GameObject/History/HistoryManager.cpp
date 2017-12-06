#include"stdafx.h"
#include "HistoryManager.h"
#include "GameObject\Village\ContinentObject.h"
#include "fbEngine\_Object\_Component\_Physics\BoxCollider.h"

namespace
{
	//オブジェクトを識別するタイプ。
	const char* ObjectType[3] = { "Obj","NPC","Enemy" };

	/**
	* チップIDからビット値を求める.
	*/
	int ChipIDToBit(ChipID id)
	{
		return BIT((int)id + 1);
	}
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
	}
}

/**
* 初期化.
*/
void HistoryManager::Start()
{
	//各オブジェクトリストを初期化。
	/*for (int i = 0; i < (int)LocationCodeE::LocationNum; i++)
	{
		_GameObjectList[i].clear();
		_NPCList[i].clear();
	}*/
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

	_MysteryLight = INSTANCE(GameObjectManager)->AddNew<MysteryLight>("MysteryLight", 10);
	_MysteryLight->SetActive(false, true);

	_Player = (Player*)INSTANCE(GameObjectManager)->FindObject("Player");

	_PlayerCamera = (PlayerCamera*)INSTANCE(GameObjectManager)->FindObject("PlayerCamera");
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

		LocationCodeE lCode = _LocationHistoryList.at(i)->_LocationID;
		//チップの状態からグループを計算。
		const int group = _CalcPattern(_LocationHistoryList[(int)lCode].get());
		//どれかのグループに該当するのなら。
		if (group >= 0)
		{
			if (_NowGroupIDList[(int)lCode] != group)
			{
				char path[128];
				for (int type = static_cast<int>(LoadObjectType::Object); type < static_cast<int>(LoadObjectType::Max); type++) {
					//パス生成
					sprintf(path, "Asset/Data/GroupData/Group%d%c%s.csv", (int)lCode, 'A' + _NowGroupIDList[(int)lCode], ObjectType[type]);
					_CreateObject(lCode, path, static_cast<LoadObjectType>(type));
					ZeroMemory(path, 128);
				}
			}
		}
	}
}

/**
* 更新.
*/
void HistoryManager::Update()
{
	if (_IsEvolution)
	{
		static float LocalTime = 0.0f;
		const float EvolutionTime = 1.0f;
		if (LocalTime >= EvolutionTime)
		{
			char path[128];
			for (int type = static_cast<int>(LoadObjectType::Object); type < static_cast<int>(LoadObjectType::Max); type++) {
				//パス生成
				sprintf(path, "Asset/Data/GroupData/Group%d%c%s.csv", (int)_EvolutionLocation, 'A' + _NowGroupIDList[(int)_EvolutionLocation], ObjectType[type]);
				_CreateObject(_EvolutionLocation, path, static_cast<LoadObjectType>(type));
				ZeroMemory(path, 128);
			}
			if (_NowLocationCode == (int)_EvolutionLocation)
			{
				//_PlayerCamera->transform->SetParent(_Player->transform);
				_Player->transform->SetLocalPosition(LocationPosition[(int)_EvolutionLocation]);
				_PlayerCamera->LookAtTarget();
				//_PlayerCamera->transform->SetParent(nullptr);

				Camera* camera = _PlayerCamera->GetComponent<Camera>();
				Vector3 cameraFoward = camera->GetTarget() - _PlayerCamera->transform->GetPosition();
				cameraFoward.Normalize();
				cameraFoward.y -= 0.5f;
				cameraFoward.Scale(0.8f);
				_HistoryBook->transform->SetLocalPosition(_PlayerCamera->transform->GetPosition() + cameraFoward);
				_HistoryBook->transform->SetRotation(_PlayerCamera->transform->GetRotation());
			}
			_IsEvolution = false;
			LocalTime = 0.0f;
		}
		LocalTime += Time::DeltaTime();
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
	//_ChangeLocation(location);

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
			_IsEvolution = true;
			_EvolutionLocation = location;
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

	int infoBit = 0;
	for (int i = 0; i < (int)ChipID::ChipNum; i++)
	{
		infoBit += ChipIDToBit(info->_ChipSlot[i]);
	}
	
	//一致するものがあるか調べる。
	for(auto& group : groupList)
	{
		int groupBit = 0;
		for (int i = 0; i < (int)ChipID::ChipNum; i++)
		{
			groupBit += ChipIDToBit(group->Slot[i]);
		}

		//bool isMatch = true;
		//各スロットを比較
		/*for (int i = 0; i < (int)ChipID::ChipNum; i++)
		{
			if (group->Slot[i] != info->_ChipSlot[i])
			{
				isMatch = false;
			}
		}*/

		if (groupBit == infoBit)
		{
			return group->GroupID;
		}

		//if (!isMatch)
		//{
		//	//マッチングしていないので次へ.
		//	continue;
		//}

		////パターン一致したのでID設定。
		//return group->GroupID;
	}

	return 0;
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
		if (location == LocationCodeE::Prosperity)
		{
			for (auto obj : _GameObjectList[static_cast<int>(location)]) {
				//X軸に180ど回転させる。
				Quaternion q = Quaternion::Identity;
				q.SetRotation(Vector3::axisX, PI);
				q.Multiply(obj->transform->GetRotation());
				obj->transform->SetRotation(q);
			}
		}
	}
	else if (type == HistoryManager::LoadObjectType::NPC)
	{
		for (auto& npc : _NPCList[(int)location])
		{
			if (npc != nullptr)
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
		if (strcmp(objInfo[i]->filename, "coll") != 0)
		{
			//コリジョンではない。

			//オブジェクト生成
			ContinentObject* obj = INSTANCE(GameObjectManager)->AddNew<ContinentObject>(objInfo[i]->filename, 2);

			obj->transform->SetLocalPosition(objInfo[i]->pos);

			obj->transform->SetRotation(objInfo[i]->ang);

			objInfo[i]->sca.y *= -1.0f;
			obj->transform->SetLocalScale(objInfo[i]->sca);
			obj->LoadModel(objInfo[i]->filename, objInfo[i]->coll);

			//配列に追加。
			Builds.push_back(obj);

			if (objInfo[i]->coll)
			{
				// ユニティーで設定したコライダーを使用してコリジョンを作成する。
				while (++i < static_cast<int>(objInfo.size()) && strcmp(objInfo[i]->filename, "coll") == 0)
				{
					// 次の要素がコリジョン。

					//コリジョンを生成してゲームオブジェクトにアタッチ。
					BoxCollider* box = obj->AddComponent<BoxCollider>();
					RigidBody* coll = obj->AddComponent<RigidBody>();

					box->Create(Vector3(fabsf(objInfo[i]->sca.x), fabsf(objInfo[i]->sca.y), fabsf(objInfo[i]->sca.z)));
					RigidBodyInfo Rinfo;
					Rinfo.physicsType = Collision::PhysicsType::Static;
					Rinfo.mass = 0.0f;
					Rinfo.coll = box;
					//カメラと当たらないコリジョンかどうか？
					Rinfo.id = ((bool)objInfo[i]->hitcamera) ? Collision_ID::BUILDING : (Collision_ID::BUILDING | Collision_ID::NOTHITCAMERA);
					Rinfo.offset = objInfo[i]->pos;
					//Quaternion q;
					//q.SetRotation(Vector3::up, PI / 2);
					//q.Multiply(info->ang);
					Rinfo.rotation = objInfo[i]->ang;
					coll->Create(Rinfo, true);
				}
			}
			else {
				i++;
			}
		}
		else {
			// 裸のコリジョンはとりあえず無視。
			i++;
		}
	}
	objInfo.clear();

	return Builds;
}

void HistoryManager::ChangeLocation(LocationCodeE value)
{
	_ChangeLocation(value);
	if (!_IsEvolution)
	{
		_HistoryMenu->SetIsOperation(true);
	}
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
	//Enemy情報構造体へのポインタをまとめたもの。
	// ※読み込み用。
	vector<unique_ptr<LoadEnemyInfo::EnemyInfo>> _EnemyInfoList;

	//CSVからエネミー情報読み取り。
	Support::LoadCSVData<LoadEnemyInfo::EnemyInfo>(path, LoadEnemyInfo::EnemyInfoDecl, ARRAY_SIZE(LoadEnemyInfo::EnemyInfoDecl), _EnemyInfoList);

	INSTANCE(EnemyManager)->CreateEnemys(location, _EnemyInfoList);
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
	//_ChangeLocation(location);

	//データを保存.
	Support::OutputCSV<LocationHistoryInfo>("Asset/Data/LocationHistory.csv", HistoryInfoData, ARRAY_SIZE(HistoryInfoData), _LocationHistoryList);
}