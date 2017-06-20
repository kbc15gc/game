/**
* 歴史書クラスの定義.
*/
#pragma once

//状態.
#include"HFSM\HistoryBookStateUnused.h"
#include"HFSM\HistoryBookStateIdol.h"
#include"HFSM\HistoryBookStateMove.h"
#include"HFSM\HistoryBookStateOpen.h"
#include"HFSM\HistoryBookStateClose.h"

class Player;

using HistoryBookStatePtr = unique_ptr<IHistoryBookState>;

/**
* 歴史書クラス.
*/
class HistoryBook : public GameObject
{
public:
	
	/**
	* 歴史書の状態コード.
	*/
	enum class StateCodeE
	{
		Invalid = -1,	//!< 無効.
		Unused,			//!< 未使用.
		Idol,			//!< 閉じた状態.
		Move,			//!< 移動.
		Open,			//!< 開く.
		Close,			//!< 閉じる.
		StateNum,		//!< 状態の数.
	};

	/**
	* 歴史書のアニメーションコード.
	*/
	enum class AnimationCodeE
	{
		Invalid = -1,	//!< 無効.
		CloseIdol,		//!< 待機_閉.
		OpenIdol,		//!< 待機_開.
		Open,			//!< 開くアニメーション.
		Close,			//!< 閉じるアニメーション.
		AnimationNum,	//!< アニメーション数.
	};

	/**
	* コンストラクタ.
	*/
	HistoryBook(const char* name) :
		GameObject(name)
	{
	}

	/**
	* デストラクタ.
	*/
	~HistoryBook()
	{
	}

	/**
	* コンストラクタ後の初期化.
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
	* 現在の状態を取得.
	*/
	int GetNowState() const
	{
		return _NowState;
	}

	/**
	* 歴史書を表示フラグを設定.
	*/
	void SetEnable(bool flag)
	{
		_Model->enable = flag;
	}

	/**
	* アニメーションの再生判定を取得.
	*/
	bool GetIsPlay()
	{
		if (_Anim != nullptr)
		{
			return _Anim->GetPlaying();
		}
	}

	/**
	* アニメーションの再生.
	*
	* @param animNo				アニメーションのナンバー.
	* @param interpolatetime	補間時間.
	* @param loopnum			ループ回数 (デフォルトは-1).
	*/
	void PlayAnimation(AnimationCodeE animNo, float interpolatetime, int loopnum = -1);
	
	/**
	* 状態の変更.
	*
	* @param nextState	次の状態.
	*/
	void ChangeState(StateCodeE nextState);

	/**
	* 目的座標を設定.
	* HistoryBookStateMoveクラスがこの地点へ移動する.
	*/
	void SetDestPos(const Vector3& dest)
	{
		_DestPos = dest;
	}

	/**
	* 目的座標を取得.
	*/
	const Vector3& GetDestPos() const
	{
		return _DestPos;
	}
	
	/**
	* 目的回転を設定.
	*/
	void SetDestRot(const Quaternion& dest)
	{
		_DestRot = dest;
	}

	/**
	* 目的回転を取得.
	*/
	const Quaternion& GetDestRot() const
	{
		return _DestRot;
	}


	/**
	* 開閉判定を取得.
	*/
	bool GetIsOpenOrClose() const
	{
		return _IsOpenOrClose;
	}



private:

	/**
	* 状態リストの初期化.
	*/
	void _InitState();

	/**
	* 歴史書を開いている判定フラグを変更.
	*/
	void _ChangeIsLookAtHistoryFlag();

private:

	/** 歴史書のモデル. */
	SkinModel* _Model = nullptr;
	/** 歴史書のアニメーション. */
	Animation* _Anim = nullptr;
	
	/** 歴史書の現在の状態. */
	int _NowState = (int)StateCodeE::Invalid;
	/** 歴史書の状態リスト, */
	vector<HistoryBookStatePtr> _StateList;
	
	/** プレイヤーのポインタ. */
	Player*	_Player = nullptr;
	/** プレイヤーの前方向. */
	Vector3	_PlayerFoward = Vector3::zero;

	/** 目的座標. */
	Vector3 _DestPos = Vector3::zero;
	/** 目的回転. */
	Quaternion _DestRot = Quaternion::Identity;

	bool _IsOpenOrClose = true;

};