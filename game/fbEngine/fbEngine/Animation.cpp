#include "Animation.h"

void Animation::Initialize(ID3DXAnimationController* anim)
{
	_AnimController = anim;
	_NumAnimSet = _AnimController->GetMaxNumAnimationSets();
	_NumMaxTracks = _AnimController->GetMaxNumTracks();
	_BlendRateTable.reset(new float[_NumMaxTracks]);
	_AnimationSets.reset(new ID3DXAnimationSet*[_NumAnimSet]);
	_EndTime.reset(new double[_NumAnimSet]);
	for(int i=0; i<_NumAnimSet;i++)
	{
		_EndTime[i] = -1.0f;
	}
	for (int i = 0; i < _NumMaxTracks; i++){
		_BlendRateTable[i] = 1.0f;
	}
	//アニメーションセットを初期化。
	for (int i = 0; i < _NumAnimSet; i++) {
		_AnimController->GetAnimationSet(i, &_AnimationSets[i]);
		_AnimController->SetTrackPosition(i, 0.0f);
		_AnimController->AdvanceTime(0, NULL);
	}
}

void Animation::Awake()
{
	_AnimController = nullptr;
	_NumAnimSet = 0;
	_IsBlending = false;
	_IsInterpolate = false;
	_NumMaxTracks = 0;
	_InterpolateTime = 0.0f;
	_InterpolateEndTime = 0.0f;
	_CurrentTrackNo = 0;
}

void Animation::PlayAnimation(const int& animationSetIndex)
{
	if (animationSetIndex < _NumAnimSet) {
		if (_AnimController) {
			//再生開始
			_IsPlaying = true;
			_CurrentFrame = 0;
			//アニメーションのグローバルタイムをリセット
			_AnimController->ResetTime();
			_CurrentAnimationSetNo = animationSetIndex;
			_CurrentTrackNo = 0;
			//0番目以外のトラックは無効にする。
			for (int i = 1; i < _NumMaxTracks; i++) {
				_AnimController->SetTrackEnable(i, FALSE);
			}
			_AnimController->SetTrackWeight(0, 1.0f);
			_AnimController->SetTrackAnimationSet(_CurrentTrackNo, _AnimationSets[(_NumAnimSet - 1) - _CurrentAnimationSetNo]);
			_AnimController->SetTrackEnable(0, TRUE);
			_AnimController->SetTrackPosition(0, 0.0f);
			_AnimController->AdvanceTime(0, NULL);
			SetAnimeSpeed(1.0f);
		}
	}
	else {
		//TK_LOG("warning!!! animationSetIndex is over range!!!!!");
	}
}

void Animation::PlayAnimation(int animationSetIndex, float _InterpolateTime, int lnum)
{
	if (animationSetIndex < _NumAnimSet) {
		if (_AnimController) {
			//ループ数設定
			_LoopNum = lnum;
			_LoopCount = 0;
			//再生開始
			_IsPlaying = true;
			_CurrentFrame = 0;
			//アニメーションのグローバルタイムをリセット
			_AnimController->ResetTime();
			_CurrentAnimationSetNo = animationSetIndex;
			//補間開始の印。
			_IsInterpolate = true;
			this->_InterpolateTime = 0.0f;
			_InterpolateEndTime = _InterpolateTime;
			_CurrentTrackNo = (_CurrentTrackNo + 1) % _NumMaxTracks;
			//トラックにアニメーションセット
			_AnimController->SetTrackAnimationSet(_CurrentTrackNo, _AnimationSets[(_NumAnimSet - 1) - _CurrentAnimationSetNo]);
			_AnimController->SetTrackEnable(_CurrentTrackNo, TRUE);
			_AnimController->SetTrackPosition(_CurrentTrackNo, 0.0f);
			SetAnimeSpeed(1.0f);
			_AnimController->AdvanceTime(0, NULL);
		}
	}
	else {
		//TK_LOG("warning!!! animationSetIndex is over range!!!!!");
	}
}

void Animation::Update()
{
	//指定されたループ数ないかどうか
	if (_IsPlaying)
	{
		//nullチェック
		if (_AnimController == nullptr)
			return;

		//フレームを増加させる
		_CurrentFrame++;
		//現在のトラックのアニメーションセット取得
		LPD3DXANIMATIONSET aniset;
		//設定されているトラックからアニメーションセット取得
		_AnimController->GetTrackAnimationSet(_CurrentTrackNo, &aniset);

		//アニメーションの終了時間設定
		//エンドタイムが指定されているのならそちらを優先
		double endtime = _EndTime.get()[_CurrentAnimationSetNo] > 0.0f ? _EndTime[_CurrentAnimationSetNo] : aniset->GetPeriod();
		//時間を進める前に現在のアニメーションの時間を取得
		double nowtime = aniset->GetPeriodicPosition(_AnimController->GetTime());

		//前フレームからの差分を計算
		double delta = Time::DeltaTime() * (double)_PlaySpeed;
		//直接取得すると最大値を超えていた場合0に戻されるので自前で計算。
		_NowTime = nowtime + delta;
		//アニメーションの時間加算
		_AnimController->AdvanceTime(delta, NULL);
		//割合を計算
		_TimeRatio = min(1.0f, _NowTime / endtime);

		//補完するよ。
		if (_IsInterpolate) {
			//補間中。
			_InterpolateTime += delta;
			float weight = 0.0f;
			if (_InterpolateTime > _InterpolateEndTime) {
				//補間終了。
				_IsInterpolate = false;
				weight = 1.0f;
				_AnimController->SetTrackWeight(_CurrentTrackNo, weight);
				//現在のトラック以外を無効にする。
				for (int i = 0; i < _NumMaxTracks; i++) {
					if (i != _CurrentTrackNo) {

						_AnimController->SetTrackEnable(i, FALSE);
					}
				}
			}
			else {
				weight = _InterpolateTime / _InterpolateEndTime;
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

		//アニメーション終了時間を超えた。
		if (endtime <= _NowTime) {
			//経過したフレーム初期化
			_CurrentFrame = 0;
			//ループ数増加
			_LoopCount++;

			//グローバルタイムリセット
			_AnimController->ResetTime();
			//アニメーション時間をリセット
			_AnimController->SetTrackPosition(_CurrentTrackNo, 0);
			//超過分を足す
			_AnimController->AdvanceTime(_NowTime - endtime, NULL);

			if (_LoopNum != -1 &&		//無限ループではない
				_LoopCount >= _LoopNum)	//カウントが指定した数以上になった
			{
				//アニメーション終了
				_IsPlaying = false;
			}
		}
	}
}