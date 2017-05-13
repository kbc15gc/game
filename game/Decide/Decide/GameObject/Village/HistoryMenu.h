#pragma once
#include "fbEngine\_Object\_GameObject\GameObject.h"
#include "GameObject\Village\HistoryInfo.h"

class Button;
class HistoryMenuButton;

//���j�ύX�̃f�o�b�O���j���[
class HistoryMenu:public GameObject
{
public:
	HistoryMenu(const char* name);
	~HistoryMenu();

	void Start()override;
	void Update()override;

	//�I�������`�b�v�����j���[�ɐݒ肷��B
	void SetMenuSelectChip(ChipID chipid);
private:
	//���j���[�̗L���t���O�ݒ�
	void _SetMenuEnabel(const bool& enabel);
	//���j���[���J������
	void _OpenMenu();
private:
	//���j���[�̗L���t���O
	bool _MenuEnabel;

	//�{�^������
	HistoryMenuButton* _Buttons[CONTINENT_NUM][HISTORY_CHIP_NUM];
	//OK�{�^��
	Button* _OK;
	//���j���[�Ɏg���Ă�I�u�W�F�N�g����
	vector<GameObject*> _MenuObjects;
};