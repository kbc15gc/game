#include"fbstdafx.h"
#include "Animation.h"

void Animation::Initialize(ID3DXAnimationController* anim)
{
	_AnimController = anim;
	_NumAnimSet = _AnimController->GetMaxNumAnimationSets();
	_NumMaxTracks = _AnimController->GetMaxNumTracks();
	_BlendRateTable.reset(new float[_NumMaxTracks]);
	_AnimationSets.reset(new ID3DXAnimationSet*[_NumAnimSet]);
	_EndTime.reset(new double[_NumAnimSet]);
	FOR(i, _NumMaxTracks)
	{
		_BlendRateTable[i] = 1.0f;
		//各トラックのローカルタイムを0に初期化。
		SetLocalAnimationTime(i, 0.0f);
	}
	//アニメーションセットを初期化。
	FOR(i, _NumAnimSet)
	{
		//アニメーションセットを取得して配列に格納
		_AnimController->GetAnimationSet(i, &_AnimationSets[i]);
		//終了時間取得
		_EndTime[i] = _AnimationSets[i]->GetPeriod();
	}
}

Animation::Animation(GameObject * g, Transform * t) :
	Component(g, t, typeid(this).name()),
	_AnimController(nullptr),
	_NumAnimSet(0),
	_IsBlending(false),
	_IsInterpolate(false),
	_NumMaxTracks(0),
	_InterpolateTimer(0.0f),
	_InterpolateEndTime(0.0f),
	_CurrentTrackNo(0),
	_PlaySpeed(1.0f)
{
#ifdef _DEBUG
	mbstowcs_s(nullptr, name, typeid(*this).name(), strlen(typeid(*this).name()));
#endif
}

Animation::~Animation()
{
	//ユニークポインタ破棄
	_AnimationSets.release();
	_BlendRateTable.release();
	_EndTime.release();

	//キューを解放
	unsigned int size = _AnimationQueue.size();
	FOR(idx,size)
	{
		PlayAnimInfo* info = _AnimationQueue.front();
		SAFE_DELETE(info);
		_AnimationQueue.pop();
	}
}

void Animation::Awake()
{
	
}

void Animation::PlayAnimation(const UINT animationSetIndex)
{
	if (animationSetIndex < _NumAnimSet) {
		if (_AnimController) {

			for (auto& call : _callback) {
				// 関数ポインタに設定された関数を実行。

				(call->object->*(call->callback))(animationSetIndex);
			}

			//再生開始
			_IsPlaying = true;
			_CurrentFrame = 0;
			_CurrentAnimationSetNo = animationSetIndex;
			_CurrentTrackNo = 0;

			//0番目以外のトラックは無効にする。
			FOR(i, _NumMaxTracks)
			{
				if (i == _CurrentTrackNo)
				{
					_AnimController->SetTrackEnable(i, TRUE);
				}
				else
				{
					_AnimController->SetTrackEnable(i, FALSE);
				}
			}
			//トラックにアニメーションセット
			LPD3DXANIMATIONSET aniset = _AnimationSets[(_NumAnimSet - 1) - _CurrentAnimationSetNo];
			_AnimController->SetTrackAnimationSet(_CurrentTrackNo, aniset);
			_EndTime[animationSetIndex] = _EndTime[animationSetIndex] > 0.0f ? _EndTime[animationSetIndex] : aniset->GetPeriod();
			SetLocalAnimationTime(0, 0.0f);
		}
	}
	else {
		//TK_LOG("warning!!! animationSetIndex is over range!!!!!");
	}
}

void Animation::PlayAnimation(const UINT animationSetIndex, const float interpolateTime, const int lnum)
{
	if (animationSetIndex < _NumAnimSet) {
		if (_AnimController) {
			for (auto& call : _callback) {
				// 関数ポインタに設定された関数を実行。

				(call->object->*(call->callback))(animationSetIndex);
			}

			//ループ数設定
			_LoopNum = lnum;
			_LoopCount = 0;
			//再生開始
			_IsPlaying = true;
			_CurrentFrame = 0;
			//現在再生中のアニメーションセットNo
			_CurrentAnimationSetNo = animationSetIndex;
			//補間開始の印。
			_IsInterpolate = true;
			//補完時間初期化
			_InterpolateTimer = 0.0f;
			//補完終了時間終了
			_InterpolateEndTime = interpolateTime;

			//トラックNo変更
			_CurrentTrackNo = (_CurrentTrackNo + 1) % _NumMaxTracks;
			//トラックを有効にする。
			_AnimController->SetTrackEnable(_CurrentTrackNo, TRUE);
			//トラックにアニメーションセット
			LPD3DXANIMATIONSET aniset = _AnimationSets[(_NumAnimSet - 1) - _CurrentAnimationSetNo];
			_AnimController->SetTrackAnimationSet(_CurrentTrackNo, aniset);
			_EndTime[animationSetIndex] = _EndTime[animationSetIndex] > 0.0f ? _EndTime[animationSetIndex] : aniset->GetPeriod();
			SetLocalAnimationTime(_CurrentTrackNo, 0.0f);
		}
	}
	else {
		//TK_LOG("warning!!! animationSetIndex is over range!!!!!");
	}
}

bool Animation::PlayAnimation(const UINT animationSetIndex, const float interpolateTime, const float transitionTime, const int loopnum)
{
	bool play;
	if(play = (transitionTime <= _TimeRatio))
	{
		PlayAnimation(animationSetIndex, interpolateTime, loopnum);
	}
	return play;
}

void Animation::AddAnimationQueue(PlayAnimInfo* info)
{
	if (_IsPlaying)
	{
		//キューに追加。
		_AnimationQueue.push(info);
	}
	else
	{
		//普通に再生。
		PlayAnimation(info->Index, info->InterpolateTime, info->TransitionTime, info->LoopNum);
		//解放
		SAFE_DELETE(info);
	}
}

void Animation::_NextQueue()
{
	//次のアニメーション添え字をキューから取り出す。
	PlayAnimInfo* info = _AnimationQueue.front();
	//アニメーション再生。
	if (PlayAnimation(info->Index, info->InterpolateTime, info->TransitionTime, info->LoopNum))
	{
		_AnimationQueue.pop();
		//解放
		SAFE_DELETE(info);
	}
}

void Animation::_EndAnimation(const float endtime)
{
	if (_LoopNum != -1 &&		//無限ループではない
		_LoopCount >= _LoopNum)	//カウントが指定した数以上になった
	{
		//アニメーション終了
		_IsPlaying = false;
		//最後の方で止める。
		SetLocalAnimationTime(_CurrentTrackNo, endtime - 0.001f);
	}
	else
	{
		//アニメーション時間をリセット
		SetLocalAnimationTime(_CurrentTrackNo, _LocalAnimationTime - endtime);
	}
}

void Animation::_InterpolateAnimation(const float delta)
{
	//補完するよ。
	if (_IsInterpolate) {
		//補間時間加算。
		_InterpolateTimer += delta;
		//重み。
		float weight = 0.0f;
		if (_InterpolateTimer > _InterpolateEndTime) {
			//補間終了。
			_IsInterpolate = false;
			weight = 1.0f;
			_AnimController->SetTrackWeight(_CurrentTrackNo, weight);
			//現在のトラック以外を無効にする。
			FOR(i, _NumMaxTracks) {
				if (i != _CurrentTrackNo) {
					_AnimController->SetTrackEnable(i, FALSE);
				}
			}
		}
		//補完中
		else {
			//割合を設定
			weight = _InterpolateTimer / _InterpolateEndTime;
			float invWeight = 1.0f - weight;
			//ウェイトを設定していく。
			for (int i = 0; i < _NumMaxTracks; i++) {
				if (i != _CurrentTrackNo) {
					_AnimController->SetTrackWeight(i, _BlendRateTable[i] * invWeight);
				}
				else {
					_AnimController->SetTrackWeight(i, weight);
				}
			}
		}
	}
}

void Animation::Update()
{
	//アニメーション再生中なら。
	if (_IsPlaying)
	{
		//nullチェック
		if (_AnimController == nullptr)
			return;

		//デルタタイム
		double delta = Time::DeltaTime() * (double)_PlaySpeed;		
		//アニメーションの補完
		_InterpolateAnimation(delta);
				
		//フレームを増加させる
		_CurrentFrame += 1.0f;
		//ローカルタイムに加算
		_LocalAnimationTime += delta;		
		//グローバルタイムに加算
		_AnimController->AdvanceTime(delta, NULL);

		//エンドタイムを取得。
		double endtime = _EndTime[_CurrentAnimationSetNo];
		//アニメーションの再生割合を計算。( 経過時間 / 終了時間 )
		_TimeRatio = min(1.0f, _LocalAnimationTime / endtime);

		//キューがあるかどうか？
		if (_AnimationQueue.size() > 0)
		{
			//次の要素に
			_NextQueue();
		}

		//アニメーション終了時間を超えた。(1Loop終了した。)
		if (endtime <= _LocalAnimationTime) 
		{
			//経過したフレーム初期化
			_CurrentFrame = 0.0f;
			//ループ数増加
			_LoopCount++;

			//アニメーションが終了した時の処理。				
			_EndAnimation(endtime);
		}
		else
		{
			//普通に加算
			SetLocalAnimationTime(_CurrentTrackNo, _LocalAnimationTime);
		}
	}
}