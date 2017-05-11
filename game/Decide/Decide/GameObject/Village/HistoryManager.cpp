#include "HistoryManager.h"
#include "GameObject\Village\ContinentObject.h"
#include "GameObject\Village\HistoryInfo.h"

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
	//大陸の変化のパターン計算
	int pattern = _CalcPattern(_HistoryList[continent]);
	
	//パス生成
	char path[128] = "Asset/Data/HistoryGroup";
	char group[2] = { 'A' + pattern,0 };
	strcat(path, group);
	strcat(path, ".csv");

	//CSVからオブジェクトの情報読み込み
	vector<ObjectInfo*> _InfoList;
	Support::LoadCSVData<ObjectInfo>(path, ObjectInfoData, ARRAY_SIZE(ObjectInfoData), _InfoList);

	//前のやつ削除してクリア
	for each (GameObject* obj in _GameObjects[continent])
	{
		INSTANCE(GameObjectManager)->AddRemoveList(obj);
	}
	_GameObjects[continent].clear();

	//情報からオブジェクト生成。
	FOR(i,_InfoList.size())
	{
		//生成
		ContinentObject* obj = INSTANCE(GameObjectManager)->AddNew<ContinentObject>("ContinentObject", 2);
		obj->LoadModel(_InfoList[i]->filename);
		obj->transform->SetLocalPosition(_InfoList[i]->pos + Vector3(0, 10, 0));
		obj->transform->SetLocalAngle(_InfoList[i]->ang);
		obj->transform->SetLocalScale(_InfoList[i]->sca);
		_GameObjects[continent].push_back(obj);

		//もういらないので解放
		SAFE_DELETE(_InfoList[i]);
	}

	_InfoList.clear();
}

const int HistoryManager::_CalcPattern(const HistoryInfo * info)
{
	int pattern = 0;
	//とりあえず適当
	switch (info->Chips[0])
	{
	case ChipID::NONE:
		pattern = 0;
		break;
	case ChipID::FIRE:
		pattern = 1;
		break;
	case ChipID::IRON:
		pattern = 2;
		break;
	case ChipID::OIL:
		pattern = 3;
		break;
	default:
		break;
	}
	return pattern;
}
