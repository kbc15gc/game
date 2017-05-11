#pragma once
#include "fbEngine\_Object\_GameObject\GameObject.h"
#include "GameObject\Village\HistoryInfo.h"

class Sprite;
class CircleCollision;

//デバッグ用のボタン
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
	Sprite* _Sprite;			//スプライト
	CircleCollision* _Circle;	//円のあたり判定

	ChipID _ChipID;		//歴史チップのID
	TEXTURE* _Textures;	//テクスチャ
	int _Id;
	int _Idx;
	HistoryInfo* _Info;
};