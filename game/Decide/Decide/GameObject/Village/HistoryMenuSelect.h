#pragma once
#include "fbEngine\_Object\_GameObject\GameObject.h"

class Button;
class HistoryMenuButton;

//���j�ύX�̃f�o�b�O���j���[
class HistoryMenuSelect :public GameObject
{
public:
	HistoryMenuSelect(const char* name);
	~HistoryMenuSelect();

	void Start()override;
	void Update()override;
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
	vector<GameObject*> _MenuSelectObjects;
};