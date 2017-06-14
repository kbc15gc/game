/**
* 歴史書クラスの定義.
*/
#pragma once
#include "fbEngine\_Object\_GameObject\GameObject.h"

//ステート.
#include "HFSM\HistoryBookStateClose.h"
#include "HFSM\HistoryBookStateOpening.h"
#include "HFSM\HistoryBookStateOpen.h"
#include "HFSM\HistoryBookStateCloseing.h"

class Player;

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
		Invalid = -1,	//!< 設定なし.
		Close,			//!< 閉じた状態。
		Opening,		//!< 本が開いている。
		Open,			//!< 開いた状態。
		Closeing,		//!< 本が閉じている。
	};

	/**
	* 歴史書のアニメーションコード.
	*/
	enum class AnimationNo
	{
		AnimationInvalid = -1,	//!< 無効。
		AnimationClose,			//!< 本が閉じた状態。
		AnimationOpening,		//!< 本が開いている状態。
		AnimationOpen,			//!< 本が開いたままの状態。
		AnimationCloseing,		//!< 本が閉じている状態。
		AnimationNum,			//!< アニメーション数.
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
	* 歴史書を開いている判定フラグを取得.
	*/
	bool GetIsLookAtHistoryFlag() const
	{
		return _IsLookAtHistoryFlag;
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
	* @param animno				アニメーションのナンバー.
	* @param interpolatetime	補間時間.
	* @param loopnum			ループ回数 (デフォルトは-1).
	*/
	void PlayAnimation(AnimationNo animno, float interpolatetime, int loopnum = -1);
	
	/**
	* 状態の変更.
	*
	* @param nextState	次の状態.
	*/
	void ChangeState(StateCodeE nextState);

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
	/** アニメーションの終了時間. */
	double _AnimationEndTime[(int)AnimationNo::AnimationNum] = { 0.0f,0.0f,0.0f,0.0f };
	
	/** 歴史書の現在の状態. */
	int _NowState = (int)StateCodeE::Invalid;
	/** 歴史書の状態リスト, */
	vector<HistoryBookState*> _StateList;
	
	/** 何の角度? */
	float _AngleY = 0.0f;

	/** 歴史書を見ているかどうかのフラグ(最初は見ていないのでfalse). */
	bool _IsLookAtHistoryFlag = false;
	/** プレイヤーのポインタ. */
	Player*	_Player = nullptr;
	/** プレイヤーの前方向. */
	Vector3	_PlayerFoward = Vector3::zero;

};