#pragma once
#include "fbEngine\_Object\_GameObject\GameObject.h"
#include "GameObject\Village\HistoryInfo.h"

class Button;
class HistoryMenuButton;

//���j�ύX�̃f�o�b�O���j���[
class HistoryMenuSelect :public GameObject
{
	
public:
	HistoryMenuSelect(const char* name);
	~HistoryMenuSelect();
	void Awake()override;
	void Start()override;
	void Update()override;
	//�ǂ̃`�b�v�����j���[�ɃZ���N�g���邩�B
	void ChipSelect(ChipID id);
private:
	//���j���[�̗L���t���O�ݒ�
	void _SetMenuEnabel(const bool& enabel);
private:
	//���j���[�̗L���t���O
	bool _MenuEnabel;		
	//�{�^������
	HistoryMenuButton* _Buttons[CONTINENT_NUM][HISTORY_CHIP_NUM];
	//OK�{�^��
	Button* _OK;
	//���j���[�Ɏg���Ă�I�u�W�F�N�g����
	vector<GameObject*> _MenuSelectObjects;
	GameObject* _MenuSelectFire;
	GameObject* _MenuSelectTetu;
	GameObject* _MenuSelectAbura;
};