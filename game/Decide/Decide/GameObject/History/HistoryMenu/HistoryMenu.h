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
		Min = 0,
		Location = Min,	//!< 場所.
		Page,		//!< ページ.
		Max = Page,
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
	void Awake()override;

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
	void AddChip(ChipID chipID, bool isSave = true);

	/**
	* 引数のチップが差し込まれているか判定.
	*/
	bool IsSetChip(ChipID id)
	{
		for (auto it : _Chip2DList)
		{
			if (it->GetChipID() == id)
			{
				return true;
			}
		}
		return false;
	}



private:

	/**
	* 表示中の更新.
	*/
	void EnableUpdate();

	/**
	* 場所選択中の更新.
	*/
	void SelectLocationUpdate();

	/**
	* ページ選択中の更新.
	*/
	void SelectPageUpdate();

	/**
	* Chip選択中の更新
	*/
	void SelectChipUpdate();

	/**
	* チップの座標移動.
	*/
	void ChipMove();

	/**
	* ページの座標移動.
	*/
	void PageMove();

	void SaveChip()
	{
		picojson::array chipList;
		for (auto it : _Chip2DList)
		{
			chipList.push_back((picojson::value)(double)it->GetChipID());
		}
		JsonData ChipData;
		ChipData.SetDataObject("ChipID", chipList);
		ChipData.Save("HaveChip");
	}

	void LoadChip()
	{
		JsonData ChipData;
		if (ChipData.Load("HaveChip"))
		{
			picojson::array chipList = ChipData.GetDataArray("ChipID");

			for (auto it : chipList)
			{
				AddChip((ChipID)(int)it.get<double>(), false);
			}
		}
	}

private:

	/** カーソル表示クラス. */
	ImageObject* _CursorSpriteL = nullptr;
	ImageObject* _CursorSpriteR = nullptr;

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