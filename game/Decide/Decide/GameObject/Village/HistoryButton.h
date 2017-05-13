#pragma once
#include "fbEngine\_Object\_GameObject\GameObject.h"
#include "GameObject\Village\HistoryInfo.h"
#include "GameObject\Village\HistoryMenu.h"

class Sprite;
class CircleCollision;
class HistoryMenu;

//�f�o�b�O�p�̃{�^��
class HistoryMenuButton:public GameObject
{
public:
	HistoryMenuButton(const char* name);
	void Awake()override;
	void Update()override;
	
	void SetChipID(const ChipID& chipid);
	void SetInfo(const int& id,const int& idx)
	{
		_Id = id;
		_Idx = idx;
	}
	void SetMenuButtonFlag(bool flag)
	{
		_MenuButtonFlag = flag;
	}
private:
	void _NextChip();
	void _SetChip();	//�Z���N�g�����`�b�v���Z�b�g����
private:
	Sprite* _Sprite;			//�X�v���C�g
	CircleCollision* _Circle;	//�~�̂����蔻��
	HistoryMenu* _HistoryMenu;	//���j���j���[

	ChipID _ChipID;		//���j�`�b�v��ID
	TEXTURE* _Textures;	//�e�N�X�`��
	int _Id;
	int _Idx;
	HistoryInfo* _Info;
	
	//TRUE = ���j���[�{�^�� : FALSE = ���j���[�Z���N�g�{�^��
	bool _MenuButtonFlag;
};