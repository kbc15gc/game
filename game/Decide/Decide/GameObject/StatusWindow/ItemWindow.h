/**
* アイテム表示画面クラスの定義.
*/
#pragma once

#include"fbEngine\_Object\_GameObject\ImageObject.h"
#include"fbEngine\_Object\_GameObject\TextObject.h"
#include"GameObject\ItemManager\ItemManager.h"
#include"Item2D.h"
#include"GameObject\Player\Player.h"
#include"ParameterRender.h"
#include"HoldItem2D.h"
#include "GameObject\StatusWindow\Cursor.h"

#include"Dialog.h"

/**
* アイテム表示画面クラス.
*/
class ItemWindow : public GameObject
{
public:

	enum class IconIndex {
		LV = 0,		//!< レベル.
		HP,			//!< 体力.
		//MP,			//!< 魔力.
		ATK,		//!< 物理攻撃.
		//MAT,		//!< 魔法攻撃.
		DEF,		//!< 物理防御.
		MDE,		//!< 魔法防御.
		DEX,		//!< 器用度.
		CRT,		// クリティカル威力。
		RANK,		// ランク。
		MAX			//!< 数.
	};

	/**
	* 消費アイテム表示ステータス.
	*/
	enum class CIShowStatus
	{ 
		LV = 0,		//!< レベル.
		HP,			//!< 体力.
		//MP,			//!< 魔力.
		ATK,		//!< 物理攻撃.
		//MAT,		//!< 魔法攻撃.
		DEF,		//!< 物理防御.
		MDE,		//!< 魔法防御.
		DEX,		//!< 器用度.
		MAX			//!< 数.
	};

	/**
	* 武器アイテム表示ステータス.
	*/
	enum class WIShowStatus
	{
		RANK,		//!< ランク.
		ATK,		//!< 物理攻撃.
		//MAT,		//!< 魔法攻撃.
		CRT,		//!< クリティカル率.
		DEX,		//!< 器用度.
		MAX			//!< 数.
	};

	/**
	* 防具アイテム表示ステータス.
	*/
	enum class AIShowStatus
	{
		RANK,		//!< ランク.
		DEF,		//!< 物理防御.
		MDE,		//!< 魔法防御.
		CRT,		//!< クリティカル率.
		DEX,		//!< 器用度.
		MAX			//!< 数.
	};

public:

	/**
	* コンストラクタ.
	*/
	ItemWindow(char* name): 
		GameObject(name)
	{
	}

	/**
	* デストラクタ.
	*/
	~ItemWindow()
	{
	}

	/**
	* 初期化.
	*/
	void Awake()override;

	/**
	* 自分で呼ぶ初期化.
	*/
	void Init(Item::ItemCodeE code);

	/**
	* 更新.
	*/
	void LateUpdate()override;

	///**
	//* アクティブ.
	//*/
	//void OnEnable()override
	//{
	//	if (INSTANCE(Inventory)->GetInventoryListSize() > 0)
	//	{
	//		Update();
	//	}
	//}

	/**
	* 非アクティブ.
	*/
	void OnDisable()override;

	/**
	* アクティブ.
	*/
	void OnEnable()override;

private:

	/**
	* 消費アイテムの初期化.
	*/
	void ItemInit();

	/**
	* 武器の初期化.
	*/
	void WeaponInit();
	
	/**
	* 防具の初期化.
	*/
	void ArmorInit();

	/**
	* 入力.
	*/
	void Input();

	void ArrowUpdate();

private:

	/**
	* 消費アイテムの表示ステータスの作成.
	*/
	void _CreateCIShowStatus();

	/**
	* 武器アイテムの表示ステータスの作成.
	*/
	void _CreateWIShowStatus();

	/**
	* 防具アイテムの表示ステータスの作成.
	*/
	void _CreateAIShowStatus();

	/**
	* パラメータ表示クラスのインスタンスに値を設定.
	*/
	void _ConfigParamRender();
private:

	Player* _Player = nullptr;
	// プレイヤーのレベル。
	int _playerLevel = 0;

	/** セルサイズ. */
	static const int ItemCellSize = 8;

	ImageObject* _ItemBackWindow = nullptr;

	/** ウィンドウ名表示. */
	TextObject* _WindowName = nullptr;

	/** アイテム描画リスト. */
	vector<Item2D*> _Item2DList;

	/** アイテムコード. */
	Item::ItemCodeE _ItemCode;

	/** 前回のアイテム数. */
	int _BefItemCount = 0;
	/** 現在選択中のアイテム. */
	int _NowSelectItem = 0;
	/** リストの初めの添え字. */
	int _StartLoadCount = 0;
	/** セレクトカーソル. */
	//ImageObject* _SelectCursor = nullptr;
	/** セレクトカーソル. */
	Cursor* _Cursor = nullptr;

	// 見えているアイテム枠を超えてアイテムがまだ存在するかを示すアイコン。
	ImageObject* _UpArrow = nullptr;
	ImageObject* _DownArrow = nullptr;

	/** Eアイコン. */
	ImageObject* _EIconImage = nullptr;

	// ゲージ。
	ParameterBar* _ExpBar = nullptr;
	ParameterBar* _HpBar = nullptr;
	//ParameterBar* _MpBar = nullptr;

	/** パラメーターリスト. */
	vector<ParameterRender*> _ParameterRenderList;

	/** 装備アイテムリスト. */
	//vector<HoldItem2D*> _HoldItem2DList;

	/** ダイアログ. */
	Dialog* _Dialog = nullptr;

	static const char* IconTextureNameList[static_cast<int>(IconIndex::MAX)];
};