/**
* 歴史変更メニュー画面クラスの定義.
*/
#pragma once

#include "fbEngine\_Object\_GameObject\GameObject.h"

#include"..\HistoryInfo.h"

#include"Chip2D.h"

class HistoryBook;
class TextObject;

/**
* 歴史変更メニュー画面クラス.
* 歴史書を開いたときに表示される.
*/
class HistoryMenu : public GameObject
{
public:

	/**
	* 選択コード.
	*/
	enum class SelectCodeE
	{
		Location,	//!< 場所.
		Page,		//!< ページ.
		Chip,		//!< チップ.
	};

public:

	/**
	* コンストラクタ.
	*/
	HistoryMenu(const char* name) :
		GameObject(name)
	{
	}

	/**
	* デストラクタ.
	*/
	~HistoryMenu()
	{
	}

	/**
	* 初期化.
	*/
	void Start()override;

	/**
	* 更新.
	*/
	void Update()override;

	/**
	* チップを追加.
	*/
	void AddChip(ChipID chipID);

private:

	/**
	* 表示中の更新.
	*/
	void EnableUpdate();

private:

	/** 選択されているもの. */
	int _SelectCode = (int)SelectCodeE::Location;

	/** 場所名表示オブジェクト. */
	TextObject* _LocationNameRender = nullptr;
	/** 現在選択中の場所. */
	int _NowSelectLocation = 0;

	/** 解放されている場所の個数. */
	int _ReleaseLocation = 0;

	/** 歴史書クラスのポインタ. */
	HistoryBook* _HistoryBook = nullptr;

	/** 所持チップを表示するクラスリスト. */
	vector<Chip2D*> _Chip2DList;

	/** 現在選択している所持チップ. */
	int _NowSelectChip = 0;
	/** 現在みているページ. */	
	int _NowLookPage = 0;

	/** 操作可能フラグ. */
	bool _IsOperation = true;

};