#pragma once
#include "fbEngine\_Object\_GameObject\GameObject.h"
#include "GameObject\ItemManager\ItemManager.h"
#include "fbEngine\_Object\_GameObject\SoundSource.h"

class SkinModel;
class Player;
class ImageObject;
class HoldEquipment;
class ParticleEffect;

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
	void Debug()override;
#endif // _DEBUG


private:
	SkinModel*			_Model				 = nullptr;		 //モデル。
	Vector3				_DropPos			 = Vector3::zero;//落ちる場所。
	Item::BaseInfo*		_DropItemInfo		 = nullptr;		 //ドロップアイテムのInfo。
	Player*				_Player				 = nullptr;		 //距離判定に使う。
	ImageObject*		_ButtonIconImage	 = nullptr;		 //ボタンのアイコン。
	float				_Life				 = 0.0f;		 //寿命。
	float				_TotalDeltaTime		 = 0.0f;		 //フィールドに現れてから経った時間。
	short int			_DropNum			 = 1;			 //何個アイテムを落とすかの数(デフォルトは1つ)。
	HoldEquipment*		_DropEquipment		 = nullptr;		 //落とした装備品。
	ParticleEffect*		_RareDropPE			 = nullptr;		 //レアドロップアイテム用のエフェクト。
	float				_GetLength			 = 5.0f;		 //ドロップアイテムが拾える距離。
	float				_ButtonIconPosOffSet = 100.0f;		 //アイコンを出す位置を調整する用。
	SoundSource*		_RareDropSE			 = nullptr;		 //レアドロップした時の音。
	SoundSource*		_DropSE				 = nullptr;		 //ドロップした時の音。
};