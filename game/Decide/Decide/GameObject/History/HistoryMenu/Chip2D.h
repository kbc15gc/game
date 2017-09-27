/**
* ヒストリーメニューで表示されるチップクラスの定義.
*/
#pragma once

#include"..\HistoryInfo.h"
#include"fbEngine\_Object\_Component\_2D\Sprite.h"


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
	* 更新.
	*/
	void Update()override;

	/**
	* サイズを設定.
	*/
	void SetMove(SizeCodeE size, Vector3 pos)
	{
		if (!isMove)
		{
			_DestSize = SizeData[(int)size];
			_Size = _ChipSprite->GetSize();
			_DestPos = pos;
			_Pos = transform->GetPosition();
			_LerpRate = 0.0f;
			isMove = true;
		}
	}

	ChipID GetChipID()
	{
		return _ChipID;
	}

	void SetSize(SizeCodeE size)
	{
		_ChipSprite->SetSize(SizeData[(int)size]);
	}

private:

	/**
	* サイズ.
	*/
	const Vector2 SizeData[(int)Chip2D::SizeCodeE::SizeCodeNum] =
	{
		Vector2(117.0f, 192.0f),
		Vector2(78.0f, 128.0f),
	};

	/** チップ表示クラス. */
	Sprite* _ChipSprite = nullptr;

	/** チップID. */
	ChipID _ChipID = ChipID::None;

	Vector3 _Pos = Vector3::zero;
	/** 目的座標. */
	Vector3 _DestPos = Vector3::zero;
	Vector2 _Size = Vector2(0.0f, 0.0f);
	/** 目的サイズ. */
	Vector2 _DestSize = Vector2(0.0f, 0.0f);
	/** 補間レート. */
	float _LerpRate = 0.0f;

	bool isMove = true;

};