#pragma once
#include "fbEngine\_Object\_GameObject\GameObject.h"
#include "GameObject\ItemManager\ItemManager.h"

class SkinModel;
class Player;
class ImageObject;
class HoldEquipment;

//ドロップアイテム。
class DropItem :public GameObject
{
public:

	//コンストラクタ。
	DropItem(const char* name);

	//デストラクタ。
	~DropItem();

	void Awake()override;

	//ドロップアイテムを作成。
	//落とすアイテムのInfo、座標、落とす数(武具の場合は設定しても1つになる)。
	void Create(Item::BaseInfo* info, const Vector3& pos, int dropNum = 1);

	//更新。
	void Update()override;

	//外部からInfoを設定する用。
	void SetInfo(Item::BaseInfo* info) {
		_DropItemInfo = info;
	}

	//寿命を外部から設定する用。
	void SetLife(float life) {
		_Life = life;
	}

	//落とすアイテムの数。
	void SetDropNum(int num) {
		_DropNum = num;
	}

#ifdef _DEBUG
	void Debug();
#endif // _DEBUG


private:
	SkinModel* _Model = nullptr;

	//落ちる場所。
	Vector3 _DropPos;

	//ドロップアイテムのInfo。
	Item::BaseInfo* _DropItemInfo = nullptr;

	//距離判定に使う。
	Player* _Player = nullptr;

	//ボタンのアイコン。
	ImageObject* _ButtonIconImage = nullptr;

	//寿命。
	float		 _Life = 0.0f;

	//フィールドに現れてから経った時間。
	float		 _TotalDeltaTime = 0.0f;
	
	//何個アイテムを落とすかの数。
	short int	 _DropNum = 1;

	//落とした装備品。
	HoldEquipment* _DropEquipment = nullptr;
};