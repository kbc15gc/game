/**
* ���j���Ǘ�����N���X�̒�`.
*/
#pragma once

#include "HistoryInfo.h"
#include "HistoryMenu\HistoryMenu.h"
#include "HistoryBook\HistoryBook.h"
#include "GameObject\Village\NPC.h"
#include "Effect\MysteryLight.h"
#include "GameObject\Enemy\EnemyManager.h"

/** �e�ꏊ�̗��j�`�b�v�̏�. */
struct LocationHistoryInfo;
class Player;

/**
* ���j���Ǘ�����N���X.
*/
class HistoryManager
{
private:
	enum class LoadObjectType{Object = 0,NPC,Enemy,Max};
private:

	/**
	* �R���X�g���N�^.
	*/
	HistoryManager();

	/**
	* �f�X�g���N�^.
	*/
	~HistoryManager()
	{
	}

public:

	/**
	* �C���X�^���X�̎擾.
	*/
	static HistoryManager* Instance()
	{
		if (_Instance == nullptr)
		{
			_Instance = new HistoryManager();
		}
		return _Instance;
	}

	/**
	* ������.
	*/
	void Start();

	/**
	* ���j��ς���.
	* 
	* @param location	�ꏊID.
	* @param slot		�X���b�g�ԍ�.
	* @param chip		�`�b�vID.
	*/
	bool SetHistoryChip(LocationCodeE location, ChipID chip,int index);

	/**
	* ���j�����擾.
	*
	* @param location	�ꏊID.
	*/
	/*LocationHistoryInfo* GetHistory(LocationCodeE location)
	{
		return _LocationHistoryList.at((int)location).get();
	}*/

	/**
	* �V�����`�b�vID��ǉ�.
	*/
	void AddPossessionChip(ChipID chipID)
	{
		_HistoryMenu->AddChip(chipID);
	}

	//���j������w�肳�ꂽ�y�[�W���폜
	void PutOutPage(LocationCodeE location, vector<HistoryPage*>& list);

	//NPC�ւ̃|�C���^���Ǘ����Ă���x�N�^�[���擾�B
	vector<vector<NPC*>> GetNPCList()
	{
		return _NPCList;
	}

	/**
	* �����̃`�b�v���������܂�Ă��邩����.
	* �����񂱂ꂱ��A���ꂾ��A����B
	* true���Ԃ��Ă�����ǂ��炩�ɑ��݂��Ă��.
	*/
	//HistoryBook���쐬���ꂽ��Ɏg���Ă��������B
	bool IsSetChip(ChipID id)
	{
		if (_HistoryBook->IsSetChip(id) || _HistoryMenu->IsSetChip(id))
		{
			return true;
		}
		return false;
	}

	/**
	* �������쐬.
	*
	* @param path		�t�H���_�p�X.
	* @param Builds		�쐬�����������i�[����z��.
	*/
	vector<GameObject*>& CreateBuilding(const char* path, vector<GameObject*>& Builds);

private:
	
	/**
	* �ꏊ�̕ω�.
	*
	* @param location	�ꏊID.
	*/
	void _ChangeLocation(LocationCodeE location);

	/**
	* �p�^�[���v�Z.
	*
	* @param info	�ꏊ�̃A�h���X.
	*/
	int _CalcPattern(const LocationHistoryInfo* info);

	/**
	* �I�u�W�F�N�g���쐬.
	*
	* @param location	�ꏊID.
	* @param path		�t�H���_�p�X.
	* @param type		��������I�u�W�F�N�g�̃^�C�v.
	*/
	void _CreateObject(LocationCodeE location, const char* path, HistoryManager::LoadObjectType type);

	/**
	* NPC���쐬.
	*
	* @param location	�ꏊID.
	* @param path		�t�H���_�p�X.
	*/
	void _CreateNPC(LocationCodeE location, const char* path);

	/**
	* �G�l�~�[���쐬.
	*
	* @param location	�ꏊID.
	* @param path		�t�H���_�p�X.
	*/
	void _CreateEnemy(LocationCodeE location, const char * path);

	/**
	* �R���W�������쐬.
	*
	* @param location	�ꏊID.
	* @param path		�t�H���_�p�X.
	*/
	void _CreateCollision(int location, const char* path);

private:

	/** �C���X�^���X. */
	static HistoryManager* _Instance;
	
	/** �S�ꏊ�̗��j�̏�񃊃X�g. */
	vector<unique_ptr<LocationHistoryInfo>> _LocationHistoryList;

	/** �S�ꏊ�̗��j�ɂ���ďo���ꂽ�I�u�W�F�N�g�ւ̃|�C���^���X�g. */
	vector<vector<GameObject*>> _GameObjectList;
	//NPC�ւ̃|�C���^���܂Ƃ߂����́B
	vector<vector<NPC*>> _NPCList;
	//Enemy���\���̂ւ̃|�C���^���܂Ƃ߂����́B
	vector<vector<unique_ptr<LoadEnemyInfo::EnemyInfo>>> _EnemyInfoList;
	/** �q�X�g���\���j���[�N���X�̃|�C���^. */
	HistoryMenu* _HistoryMenu = nullptr;

	/** ���j���N���X�̃|�C���^. */
	HistoryBook* _HistoryBook = nullptr;

	MysteryLight* _MysteryLight = nullptr;
	vector<int> _NowGroupIDList;

Player* _Player = nullptr;

};