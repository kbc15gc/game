/**
* ヒストリーメニューで表示されるチップクラスの定義.
*/
#pragma once

#include"..\HistoryInfo.h"

/**
* ヒストリーメニューで表示されるチップクラス.
*/
class Chip2D : public GameObject
{
public:

	/**
	* サイズコード.
	*/
	enum class SizeCodeE
	{
		Select,		//!< 選択中.
		NoSelect,	//!< 選択外.
		SizeCodeNum,	//!< サイズ数.
	};



public:

	/**
	* コンストラクタ.
	*/
	Chip2D(const char* name) : 
		GameObject(name)
	{
	}

	/**
	* デストラクタ.
	*/
	~Chip2D()
	{
	}

	/**
	* 初期化.
	*/
	void Start(ChipID chipID);

	/**
	* サイズを設定.
	*/
	void SetSize(SizeCodeE size);

	ChipID GetChipID()
	{
		return _ChipID;
	}

private:

	/** チップ表示クラス. */
	Sprite* _ChipSprite = nullptr;

	/** チップID. */
	ChipID _ChipID = ChipID::None;

};