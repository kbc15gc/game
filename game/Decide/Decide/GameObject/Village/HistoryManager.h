#pragma once
#include "fbEngine\_Object\_GameObject\GameObject.h"

class Button;

namespace
{
	//�e�嗤�̗��j���_���̏�
	struct History:Noncopyable
	{
		History()
		{
			ContinentID = -1;
			FOR(i, HISTORY_CHIP_NUM)
				IsSetting[i] = -1;
		}
		//�嗤��ID
		int ContinentID;
		//�`�b�v���Z�b�g����Ă��邩�H
		bool IsSetting[HISTORY_CHIP_NUM];
	};
}

//�嗤�̗��j���Ǘ�����
class HistoryManager:public GameObject
{
public:
	HistoryManager(const char* name);
	~HistoryManager();

	void Start()override;
	void Update()override;
private:
	//���j���[�̗L���t���O�ݒ�
	void _SetMenuEnabel(const bool& enabel);
	//���j���[���J������
	void _OpenMenu();
	//���j���[���J���Ă��鎞�̏���
	void _MenuUpdate();
	//���j�̕ύX���X�V
	void _UpdateHistory();
private:
	//�e�嗤�̗��j���̃��X�g
	vector<History*> _HistoryList;
	//���j���[�̗L���t���O
	bool _MenuEnabel;

	//���j�{�^������
	Button* _HistoryButtons[CONTINENT_NUM][HISTORY_CHIP_NUM];
	//OK�{�^��
	Button* _OK;
	//���j���[�Ɏg���Ă�I�u�W�F�N�g����
	vector<GameObject*> _MenuObjects;
};