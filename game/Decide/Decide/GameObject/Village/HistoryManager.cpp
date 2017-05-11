#include "HistoryManager.h"
#include "GameObject\Village\ContinentObject.h"
#include "GameObject\Village\HistoryInfo.h"

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
	//�嗤�̕ω��̃p�^�[���v�Z
	int pattern = _CalcPattern(_HistoryList[continent]);
	
	//�p�X����
	char path[128] = "Asset/Data/HistoryGroup";
	char group[2] = { 'A' + pattern,0 };
	strcat(path, group);
	strcat(path, ".csv");

	//CSV����I�u�W�F�N�g�̏��ǂݍ���
	vector<ObjectInfo*> _InfoList;
	Support::LoadCSVData<ObjectInfo>(path, ObjectInfoData, ARRAY_SIZE(ObjectInfoData), _InfoList);

	//�O�̂�폜���ăN���A
	for each (GameObject* obj in _GameObjects[continent])
	{
		INSTANCE(GameObjectManager)->AddRemoveList(obj);
	}
	_GameObjects[continent].clear();

	//��񂩂�I�u�W�F�N�g�����B
	FOR(i,_InfoList.size())
	{
		//����
		ContinentObject* obj = INSTANCE(GameObjectManager)->AddNew<ContinentObject>("ContinentObject", 2);
		obj->LoadModel(_InfoList[i]->filename);
		obj->transform->SetLocalPosition(_InfoList[i]->pos + Vector3(0, 10, 0));
		obj->transform->SetLocalAngle(_InfoList[i]->ang);
		obj->transform->SetLocalScale(_InfoList[i]->sca);
		_GameObjects[continent].push_back(obj);

		//��������Ȃ��̂ŉ��
		SAFE_DELETE(_InfoList[i]);
	}

	_InfoList.clear();
}

const int HistoryManager::_CalcPattern(const HistoryInfo * info)
{
	int pattern = 0;
	//�Ƃ肠�����K��
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
