/**
* 歴史書のページクラスの定義.
*/
#pragma once

/** 歴史書. */
class HistoryBook;

#include"..\..\HistoryInfo.h"

#include"HFSM\HistoryPageState.h"

/** 歴史書のページの状態の基底クラス. */
using HistoryPageStatePtr = unique_ptr<IHistoryPageState>;

/**
* 歴史書のページクラス.
*/
class HistoryPage : public GameObject
{
public:

	enum class StateCodeE
	{
		Invalid = -1,	//!< 無効.
		PutIn,			//!< 挟む.
		PutOut,			//!< 抜く.
		Turn,			//!< 捲る.
		Close,			//!< 閉じる.
	};

public:

	/**
	* コンストラクタ.
	*/
	HistoryPage(const char* name) : 
		GameObject(name)
	{
	}

	/**
	* デストラクタ.
	*/
	~HistoryPage()
	{
	}

	/**
	* コンストラクタ後の初期化.
	*/
	void Awake()override;

	/**
	* 初期化.
	*/
	void Start(ChipID chipID, LocationCodeE code);

	/**
	* 更新.
	*/
	void Update()override;

	/**
	* 遅めの更新.
	*/
	void LateUpdate()override;

	/**
	* 歴史書のポインタを設定.
	*/
	void SetHistoryBook(HistoryBook* book)
	{
		_HistoryBook = book;
	}

	/**
	* 回転行列を設定.
	*/
	void Rotation(float angle)
	{
		_Angle = angle;
		Quaternion rot;
		rot.SetEuler(Vector3(0.0f, _Angle, 0.0f));
		_RotationMatrix = rot.GetRotationMatrix();
	}

	/**
	* 角度を取得.
	*/
	float GetAngle() const
	{
		return _Angle;
	}

	/**
	* 状態の変化. 
	*/
	void ChangeState(StateCodeE state);

	//今どの場所にいるかを返す。
	LocationCodeE GetNowLocation()
	{
		return _NowLocatuion;
	}

private:

	/**
	* 状態リストの初期化.
	*/
	void InitState();

private:

	/** ページのモデル. */
	SkinModel* _Model = nullptr;

	/** 歴史書のポインタ. */
	HistoryBook* _HistoryBook = nullptr;
	/** 親の行列. */
	D3DXMATRIX* _ParentMatrix = nullptr;

	/** 角度. */
	float _Angle = 0.0f;
	/** 回転行列. */
	D3DXMATRIX _RotationMatrix;

	/** 状態リスト. */
	vector<HistoryPageStatePtr> _StateList;
	/** 現在の状態. */
	int _NowState = (int)StateCodeE::Invalid;

	/** マテリアル. */
	Material* _Material = nullptr;

	//今自分がどの場所にいるのか。
	LocationCodeE _NowLocatuion;
};