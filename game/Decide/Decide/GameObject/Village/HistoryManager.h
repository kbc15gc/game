/**
* ���j���Ǘ�����N���X�̒�`.
*/
#pragma once

struct HistoryInfo;

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
	* ���j�I�u�W�F�N�g����.
	*/
	void CreateObject();
	//���j��ς���B
	//[in] unsigned int �ύX�������嗤��ID
	//[in] unsigned int �Z�b�g����X���b�g
	//[in] int �Z�b�g�������`�b�v��ID
	const bool SetHistoryChip(const unsigned int& continent,const unsigned int& idx, const int& chip);
	//���j�����擾
	//[in] unsigned int ���j���擾�������嗤�̓Y����
	//[out] HistoryInfo* ���j���
	HistoryInfo* GetHistory(const unsigned int idx)
	{
		return _HistoryList.at(idx);
	}
	static HistoryManager* Instance()
	{
		if (_Instance == nullptr)
		{
			_Instance = new HistoryManager();
		}
		return _Instance;
	}
private:
	//�嗤�̕ω�
	void _ChangeContinent(const unsigned int& continent);
	//�p�^�[���v�Z
	const int _CalcPattern(const HistoryInfo* info);

	void _CreateObject(const int& continent,const char* path);
	void _CreateCollision(const int& continent, const char* path);
private:

	/** �C���X�^���X. */
	static HistoryManager* _Instance;
	
	/** �S�嗤�̗��j�̏�񃊃X�g. */
	vector<HistoryInfo*> _HistoryList;
	/** �S�嗤�̗��j�ɂ���ďo���ꂽ�I�u�W�F�N�g�ւ̃|�C���^���X�g. */
	vector<vector<GameObject*>> _GameObjectList;

};