#include "stdafx.h"
#include "GameObject\Component\AnimationEvent.h"
#include "fbEngine\_Object\_GameObject\SoundSource.h"
#include "fbEngine\_Object\_GameObject\GameObject.h"


AnimationEventPlayer::~AnimationEventPlayer() {
	for (auto& work : _animationEvents) {
		for (auto Event : work) {
			SAFE_DELETE(Event);
		}
		work.clear();
	}
	_animationEvents.clear();
};

void AnimationEventPlayer::Init(int animationNum)
{
	// アニメーションの数だけ配列追加。
	_animationEvents = vector<vector<EventData*>>(animationNum);
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


		for (auto eventData : _animationEvents[nowAnim]) {
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
	else {
		// Animationコンポーネントがアタッチされていない。
		abort();
	}
}

void AnimationEventPlayer::StartAnimation(int animationNo) {
	for (auto eventData : _animationEvents[animationNo]) {
		eventData->isPlay = false;
	}
}