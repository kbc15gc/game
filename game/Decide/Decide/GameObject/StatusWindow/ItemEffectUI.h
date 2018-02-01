/**
* アイテム効果UIクラスの定義.
*/
#pragma once

#include"fbEngine\_Object\_GameObject\ImageObject.h"
#include"fbEngine\_Object\_GameObject\TextObject.h"
#include"GameObject\ItemManager\HoldItem\ConsumptionItem.h"

/**
* アイテム効果UIクラス.
*/
class ItemEffectUI : public GameObject
{
public:

	/**
	* アイテムの効果パラメータコード.
	*/
	enum class IEParamCode
	{
		Hp,		//!< 体力.
		Atk,	//!< 攻撃力.
		Mat,	//!< 魔法攻撃力.
		Def,	//!< 防御力.
		Mde,	//!< 魔法防御力.
		Dex,	//!< クリティカル率.
		Crt,	//!< クリティカル倍率.
		Count,	//!< 数.
	};

public:

	/**
	* コンストラクタ.
	*/
	ItemEffectUI(const char* name) :
		GameObject(name)
	{
	}

	/**
	* デストラクタ.
	*/
	~ItemEffectUI()
	{
	}

	/**
	* 初期化.
	*/
	void Awake()override;

	/**
	* アイテムデータを設定.
	*/
	void SetItemData(ConsumptionItem* itemData);

private:

	/** パラメータ名. */
	TextObject* _Param[(int)IEParamCode::Count] = { nullptr };

};