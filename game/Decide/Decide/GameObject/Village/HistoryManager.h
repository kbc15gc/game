#pragma once

struct HistoryInfo;

//�嗤�̗��j���Ǘ�����N���X
class HistoryManager
{
public:
	HistoryManager();
	//���j��ς���B
	//[in] unsigned int �ύX�������嗤��ID
	//[in] unsigned int �Z�b�g����ꏊ
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
private:
	//�S�嗤�̗��j�̏��
	vector<HistoryInfo*> _HistoryList;
	//�S�嗤�̗��j�ɂ���ďo���ꂽ�I�u�W�F�N�g�ւ̃|�C���^
	vector<vector<GameObject*>> _GameObjects;
	static HistoryManager* _Instance;
};