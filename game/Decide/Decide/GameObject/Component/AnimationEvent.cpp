#include "stdafx.h"
#include "GameObject\Component\AnimationEvent.h"
#include "fbEngine\_Object\_GameObject\SoundSource.h"
#include "fbEngine\_Object\_GameObject\GameObject.h"


AnimationEventPlayer::~AnimationEventPlayer() {
	for (auto& work : _animationEvents) {
		for (auto& work2 : work) {
			for (auto Event : work2) {
				SAFE_DELETE(Event);
			}
			work2.clear();
		}
		work.clear();
	}
	_animationEvents.clear();
};

void AnimationEventPlayer::Init(int animationNum)
{
	// アニメーションの数だけ配列追加。
	_animationEvents = vector<vector<vector<EventData*>>>(animationNum);
	_nowPlayEventList = vector<int>(animationNum,0);
}

void AnimationEventPlayer::Update() {
	if (_animationEvents.size() <= 0) {
		// アニメーションイベントが一つも設定されていない。
		return;
	}

	Animation* work = gameObject->GetComponent<Animation>();
	if (work) {
		if (!_isSetCallBack) {

			// コールバック関数設定。
			work->AddCallBack(move(unique_ptr<Animation::StartAnimationCallBack>(new Animation::StartAnimationCallBack(this, static_cast<Animation::CallBack>(&AnimationEventPlayer::StartAnimation)))));
			_isSetCallBack = true;
		}

		//現在再生中のアニメーション番号取得。
		int nowAnim = work->GetPlayAnimNo();

		if (_animationEvents[nowAnim].size() > 0) {
			for (auto eventData : _animationEvents[nowAnim][_nowPlayEventList[nowAnim]]) {
				//時間が一致した時イベント呼び出し。
				if (work->GetLocalAnimationTime() >= eventData->playTime)
				{
					if (!eventData->isPlay) {
						// イベントがまだ実行されてない。

						// 関数ポインタに設定された関数を実行。
						(gameObject->*(eventData->Event))();
						eventData->isPlay = true;
					}
				}
			}
		}
	}
	else {
		// Animationコンポーネントがアタッチされていない。
		abort();
	}
}

void AnimationEventPlayer::AddAnimationEvent(int animationNo, const float eventTime, AnimationEvent Event, int eventListNo) {
	EventData* work = nullptr;
	work = new EventData;
	work->playTime = eventTime;
	work->Event = Event;
	work->isPlay = false;

	if (static_cast<int>(_animationEvents[animationNo].size()) <= eventListNo) {
		// 配列の容量が足りないので拡張。
		_animationEvents[animationNo].resize(eventListNo + 1);
	}
	_animationEvents[animationNo][eventListNo].push_back(work);
}

void AnimationEventPlayer::StartAnimation(int animationNo) {
	if (_animationEvents[animationNo].size() > 0) {
		for (auto eventData : _animationEvents[animationNo][_nowPlayEventList[animationNo]]) {
			eventData->isPlay = false;
		}
	}
}