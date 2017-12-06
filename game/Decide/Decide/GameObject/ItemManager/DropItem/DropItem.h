#pragma once
#include "fbEngine\_Object\_GameObject\GameObject.h"
#include "GameObject\ItemManager\ItemManager.h"
#include "fbEngine\_Object\_GameObject\SoundSource.h"
#include"fbEngine\_Object\_Component\_3D\Light.h"
#include "GameObject\TextImage\AttentionTextOnly.h"

class SkinModel;
class Player;
class ImageObject;
class HoldItemBase;
class ParticleEffect;
class CCharacterController;

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
	//落とすアイテムのIDとアイテムコード、座標、落とす数(武具の場合は設定しても1つになる)。
	void Create(int id,int typeId, const Vector3& pos, int dropNum = 1);

	//更新。
	void Update()override;

	//外部からInfoを設定する用。
	void SetInfo(Item::BaseInfo* info) {
		_DropItemInfo = info;
	}

	//フィールド上に存在できる最大時間を外部から設定する用。
	void SetAppearMaxTime(float life) {
		_AppearMaxTime = life;
	}

	//落とすアイテムの数。
	void SetDropNum(int num) {
		_DropNum = num;
	}

#ifdef _DEBUG
	void Debug()override;
#endif // _DEBUG

private:

	//生成した武具のランクをチェックし、ランクに適したSEとエフェクトを選択。
	void _EquipmentRankCheck_SelectSEAndEffect();

	//このクラスでGameobjectに登録したオブジェクトを全削除用関数。
	void _Release();

	//flagを見て取得成功か、失敗の適した文字列を決める。
	void _SelectText(bool flag);

	//flagを見て取得成功か失敗の文字列をAttentionTextに設定。
	void _SetText(const wchar_t* string, bool flag);

	//拾ったアイテムをインベントリのAdd関数に送る。
	bool _AddInventory(Item::ItemCodeE code);

private:
	SkinModel*			  _Model				= nullptr;	    //モデル。
	Vector3				  _DropPos			    = Vector3::zero;//落ちる場所。
	Item::BaseInfo*		  _DropItemInfo		    = nullptr;	    //ドロップアイテムのInfo。
	Player*				  _Player			    = nullptr;	    //距離判定に使う。
	ImageObject*		  _ButtonIconImage	    = nullptr;	    //ボタンのアイコン。
	float				  _AppearMaxTime		= 0.0f;		    //フィールド上に存在できる最大時間。
	float				  _TotalAppearTime	    = 0.0f;		    //フィールドに現れてから経った時間。
	short int			  _DropNum			    = 1;		    //何個アイテムを落とすかの数(デフォルトは1つ)。
	HoldItemBase*		  _DropEquipment		= nullptr;	    //落とした装備品。
	ParticleEffect*		  _RareDropPE		    = nullptr;	    //レアドロップアイテム用のエフェクト。
	float				  _GetLength			= 5.0f;		    //ドロップアイテムが拾える距離。
	float				  _ButtonIconPosOffSet  = 120.0f;	    //アイコンを出す位置を調整する用。
	SoundSource*		  _RareDropSE		    = nullptr;	    //レアドロップした時の音。
	SoundSource*		  _DropSE			    = nullptr;	    //ドロップした時の音。
	int					  _ButtonIconPriority   = 0;			//アイコンの優先度。
	Color				  _ModelColor		    = Color::zero;  //モデルを透明にしていくために使う。
	CCharacterController* _CCharacterController = nullptr;      //キャラクターコントローラー。															 
	float				  _Gravity				= 0.0f;			//重力。
	CharacterLight		  _TreasureChestLight;					//宝箱のライト。
	Vector3				  _TextPos				= Vector3::zero;//テキストを出す場所。
	float				  _TextFontSize			= 0.0f;			//テキストのサイズ。

	float _DitheRocalTime = 0.0f;	//!< ディザローカルタイム.
	float _DitheTime = 1.0f;		//!< ディザタイム.

	AttentionTextOnly* _AttentionText = nullptr;

};