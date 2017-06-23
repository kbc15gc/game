/**
* ���j���Ǘ�����N���X�̒�`.
*/
#pragma once

#include"HistoryInfo.h"
#include"HistoryMenu\HistoryMenu.h"

/** �e�ꏊ�̗��j�`�b�v�̏�. */
struct LocationHistoryInfo;

/**
* ���j���Ǘ�����N���X.
*/
class HistoryManager
{
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
	* ���j�I�u�W�F�N�g����.
	*/
	void CreateObject();

	/**
	* ���j��ς���.
	* 
	* @param location	�ꏊID.
	* @param slot		�X���b�g�ԍ�.
	* @param chip		�`�b�vID.
	*/
	bool SetHistoryChip(LocationCodeE location, UINT slot, ChipID chip);

	/**
	* ���j�����擾.
	*
	* @param location	�ꏊID.
	*/
	LocationHistoryInfo* GetHistory(LocationCodeE location)
	{
		return _LocationHistoryList.at((int)location);
	}

	/**
	* �V�����`�b�vID��ǉ�.
	*/
	void AddPossessionChip(ChipID chipID)
	{
		_PossessionChipList.push_back(chipID);
		_HistoryMenu->AddChip(chipID);
	}

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
	*/
	void _CreateObject(int location,const char* path);

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
	vector<LocationHistoryInfo*> _LocationHistoryList;

	/** �S�ꏊ�̗��j�ɂ���ďo���ꂽ�I�u�W�F�N�g�ւ̃|�C���^���X�g. */
	vector<vector<GameObject*>> _GameObjectList;

	/** �q�X�g���\���j���[�N���X�̃|�C���^. */
	HistoryMenu* _HistoryMenu = nullptr;

	/** �������Ă���`�b�v��ID. */
	vector<ChipID> _PossessionChipList;

};