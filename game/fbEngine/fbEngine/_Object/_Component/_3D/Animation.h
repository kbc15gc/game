/*!
 * @brief	アニメーションクラス。
 */
#pragma once
#include "_Object\_Component\Component.h"
#include <queue>

/*!
 * @brief	アニメーションクラス。
 */
class Animation : public Component{
public:
	typedef void (Object::*CallBack)(int);	// 関数ポインタ(引数は再生されたアニメーション番号)。

	struct StartAnimationCallBack {
		Object* object = nullptr;	// コールバック関数の持ち主。
		CallBack callback = nullptr;	// アニメーション終了時に呼び出したい関数のポインタ。

		StartAnimationCallBack(Object* obj, CallBack call) {
			object = obj;
			callback = call;
		}
	};

	//アニメーション再生に必要な情報
	struct PlayAnimInfo
	{
	public:
		//アニメーションの添え字
		//アニメーション補完時間
		//アニメーション遷移開始時間
		//ループ数。
		PlayAnimInfo(const UINT index, const float interpolateTime, const float transitionTime, const int loopnum = -1)
		{
			Index = index;
			InterpolateTime = interpolateTime;
			TransitionTime = transitionTime;
			LoopNum = loopnum;
		}
	
		//アニメーションセットの添え字。
		unsigned int Index;
		//アニメーション補完時間
		float InterpolateTime;
		//アニメーションの遷移開始時間。(割合)
		float TransitionTime;
		//ループする数
		int LoopNum;
	};
	/*!
	 * @brief	コンストラクタ
	 */
	Animation(GameObject* g, Transform* t);
	/*!
	 * @brief	デストラクタ。
	 */
	~Animation();
	
	
	void Initialize(ID3DXAnimationController* anim);
	void Awake();
	void Update();
	//アニメーション再生(補完なし)
	void PlayAnimation(const UINT animationSetIndex);
	//アニメーション再生(補完あり)
	//第1引数　再生したいアニメーションのインデックス
	//第2引数　補間時間
	//第3引数　ループ数
	void PlayAnimation(const UINT animationSetIndex, const float interpolateTime, const int loopnum = -1);
	//アニメーション再生(補完あり)
	//第1引数　再生したいアニメーションのインデックス
	//第2引数　補間時間
	//第3引数　アニメーションを開始する時間。
	//第4引数　ループ数
	//戻り値　再生できたかどうか？
	bool PlayAnimation(const UINT animationSetIndex, const float interpolateTime, const float transitionTime, const int loopnum = -1);

	//キューにアニメーションの再生情報を追加。
	//確保したアドレスはアニメーションないで解放される。
	void AddAnimationQueue(PlayAnimInfo* info);

	inline void PopAnimationQueue()
	{
		while (!_AnimationQueue.empty()) _AnimationQueue.pop();
	}

	/*!
	*@brief	アニメーションセットの取得。
	*/
	int GetNumAnimationSet() const
	{
		return _NumAnimSet;
	}
	//現在再生中のアニメーション番号取得
	const UINT GetPlayAnimNo() const
	{
		return _CurrentAnimationSetNo;
	}
	//アニメーションが再生中か
	const bool GetPlaying()
	{
		return _IsPlaying;
	}

	const double GetLocalAnimationTime()
	{
		return _LocalAnimationTime;
	}

	const double NowFrame()
	{
		return _CurrentFrame;
	}
	//割合取得
	const double GetTimeRatio()
	{
		return _TimeRatio;
	}

	void SetAnimeSpeed(float sp)
	{
		_PlaySpeed = sp;
	}
	/*!
	*@brief	アニメーション終了時間設定
	*@param[in]		idx	アニメーションセットの番号。
	*@param[in]		endtime		終了時間。
	*/
	void SetAnimationEndTime(const UINT idx, const double endtime)
	{
		//範囲内か？
		if (idx < _NumAnimSet)
			_EndTime[idx] = endtime;
	}
	const double GetAnimationEndTime(const UINT idx)
	{
		//範囲内か？
		if (idx < _NumAnimSet)
			return _EndTime[idx];
	}
	//アニメーションのローカルタイム設定
	void SetLocalAnimationTime(const UINT track, const double t)
	{
		_LocalAnimationTime = t;
		//時間設定。
		_AnimController->SetTrackPosition(track, _LocalAnimationTime);
		//ポジションを更新するために呼ばなければいけない
		_AnimController->AdvanceTime(0.0, NULL);
	}

	inline void AddCallBack(unique_ptr<StartAnimationCallBack> callback) {
		_callback.push_back(move(callback));
	}
private:
	//アニメーションの補完をする関数。
	void _InterpolateAnimation(const float delta);
	//次の要素をキューから取り出す。
	void _NextQueue();
	//アニメーションが終了した時の処理。
	void _EndAnimation(const float endtime);
private:
	ID3DXAnimationController*				_AnimController = nullptr;		//!<アニメーションコントローラ。
	UINT									_NumAnimSet = 0;				//!<アニメーションセットの数。
	std::unique_ptr<ID3DXAnimationSet*[]>	_AnimationSets = nullptr;			//!<アニメーションセットの配列。
	vector<float>							_BlendRateTable;			//!<ブレンディングレートのテーブル。
	UINT									_CurrentAnimationSetNo = 0;	//!<現在再生中のアニメーショントラックの番号。
	UINT									_CurrentTrackNo = 0;			//!<現在のトラックの番号。
	UINT									_NumMaxTracks = 0;			//!<アニメーショントラックの最大数。
	bool									_IsBlending = true;				//!<アニメーションブレンディング中？
	bool									_IsInterpolate = true;			//!<補間中？
	float									_InterpolateEndTime = 0.0f;		//!<補間終了時間。
	float									_InterpolateTimer = 0.0f;		//!<補間時間。

	vector<double> _EndTime;			//各アニメーションの終了時間を格納した配列
	double _TimeRatio = 0.0;					//正規化された時間の割合。
	double _LocalAnimationTime = 0.0;			//ローカルなアニメーションの経過時間(ややこしかったので自分で管理することにした)
	double _CurrentFrame = 0.0;				//アニメーションが再生されて現在何フレーム目か。
	float _PlaySpeed = 0.0f;					//再生速度
	int _LoopNum = 0;						//アニメーションをループさせる数。
	int _LoopCount = 0;						//ループ数をカウントする。
	bool _IsPlaying = true;					//アニメーション再生中であることを示す。

	std::queue<PlayAnimInfo*> _AnimationQueue;	//アニメーションを保持するキュー。

	vector<unique_ptr<StartAnimationCallBack>> _callback;	// アニメーションがスタートする際に呼ばれるコールバック(ループ再生なら1ループごとに呼ばれる)。
};