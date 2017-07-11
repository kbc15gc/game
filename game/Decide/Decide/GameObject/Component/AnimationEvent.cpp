#include "stdafx.h"
#include "GameObject\Component\AnimationEvent.h"

AnimationEvent::~AnimationEvent() {
	for (auto& work : _animationEvents) {
		for (auto Event : work) {
			SAFE_DELETE(Event);
		}
		work.clear();
	}
	_animationEvents.clear();
};

void AnimationEvent::Init()
{
	// アニメーションの数だけ配列追加。
	if (gameObject->GetComponent<Animation>()) {
		_animationEvents = vector<vector<EventData*>>(gameObject->GetComponent<Animation>()->GetNumAnimationSet());
	}
#ifdef _DEBUG
	else {
		// ※アニメーションコンポーネントを先に追加したか。
		abort();
	}
#endif // _DEBUG
	_isFirst = false;
}

void AnimationEvent::Update() {
	if (_animationEvents.size() <= 0) {
		// アニメーションイベントが一つも設定されていない。
		return;
	}
	Animation* work = gameObject->GetComponent<Animation>();
	if (work) {
		//現在再生中のアニメーション番号取得。
		int nowAnim = work->GetPlayAnimNo();
		for (auto eventData : _animationEvents[nowAnim]) {
				//フレームが一致した時あたり判定作成。
			if (work->NowFrame() == eventData->createFrame)
			{
				//攻撃コリジョン作成
				unsigned int priorty = 1;
				AttackCollision* attack = INSTANCE(GameObjectManager)->AddNew<AttackCollision>("attackCollision", priorty);
				attack->Create(eventData->info.damage, eventData->info.pos, eventData->info.rot, eventData->info.size, eventData->info.master, eventData->info.life, eventData->info.wait, eventData->info.parent);
				if (eventData->info.isRemoveParent) {
					attack->RemoveParent();
				}
			}
		}
	}
	else {
		// Animationコンポーネントがアタッチされていない。
		abort();
	}
}