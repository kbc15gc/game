#pragma once
#include "fbEngine\_Object\_GameObject\GameObject.h"
#include "GameObject\Village\HistoryInfo.h"

class Sprite;
class CircleCollision;

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
private:
	void _NextChip();
private:
	Sprite* _Sprite;			//�X�v���C�g
	CircleCollision* _Circle;	//�~�̂����蔻��

	ChipID _ChipID;		//���j�`�b�v��ID
	TEXTURE* _Textures;	//�e�N�X�`��
	int _Id;
	int _Idx;
	HistoryInfo* _Info;
};