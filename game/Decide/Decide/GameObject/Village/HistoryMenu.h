/**
* 歴史変更メニュー画面クラスの定義.
*/
#pragma once

#include "fbEngine\_Object\_GameObject\GameObject.h"
#include "GameObject\Village\HistoryInfo.h"

class HistoryBook;
class TextObject;

/**
* 歴史変更メニュー画面クラス.
* 歴史書を開いたときに表示される.
*/
class HistoryMenu:public GameObject
{
public:

	enum MenuNomber
	{
		One = 0,
		Two,
		Three,
	};

	/**
	* 場所コード.
	*/
	enum class LocationCodeE
	{
		Begin = 0,		//!< 始まりの集落.
		Hunting,		//!< 狩猟の村.
		Prosperity,		//!< 繁栄の町.
		LocationNum,	//!< 場所の数.
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

private:

	/**
	* 表示中の更新.
	*/
	void EnableUpdate();

private:

	/** 場所名表示オブジェクト. */
	TextObject* _LocationNameRender = nullptr;
	/** 現在選択中の場所. */
	int _NowSelectLocation = 0;

	/** 解放されている場所の個数. */
	int _ReleaseLocation = 0;

	/** 歴史書クラスのポインタ. */
	HistoryBook* _HistoryBook = nullptr;

};