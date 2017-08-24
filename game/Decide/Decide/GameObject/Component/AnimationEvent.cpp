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

void AnimationEventPlayer::Init()
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

void AnimationEventPlayer::Update() {
	if (_animationEvents.size() <= 0) {
		// アニメーションイベントが一つも設定されていない。
		return;
	}
	Animation* work = gameObject->GetComponent<Animation>();
	if (work) {
		//現在再生中のアニメーション番号取得。
		int nowAnim = work->GetPlayAnimNo();
		for (auto eventData : _animationEvents[nowAnim]) {
				//フレームが一致した時イベント呼び出し。
			if (work->NowFrame() == eventData->playFrame)
			{
				// 関数ポインタに設定された関数を実行。
				(eventData->object->*(eventData->Event))();

				////攻撃コリジョン作成。
				//unsigned int priorty = 1;
				//AttackCollision* attack = INSTANCE(GameObjectManager)->AddNew<AttackCollision>("attackCollision", priorty);
				//attack->Create(eventData->info.damage, eventData->info.pos, eventData->info.rot, eventData->info.size, eventData->info.master, eventData->info.life, eventData->info.wait, eventData->info.parent);
				//if (eventData->info.isRemoveParent) {
				//	attack->RemoveParent();
				//	//もしモンスターの向きとコリジョンの向きを合わせたいのなら
				//	//collison->transform->SetAngle(monster->transform->GetAngle());
				//	//みたいなコードが必要。
				//}
			}
		}
		//for (auto soundData : _soundEvents[nowAnim]) {
		//	//フレームが一致した時音再生。
		//	if (work->NowFrame() == soundData->playFrame)
		//	{
		//		soundData->info.source->Play(soundData->info.isLoop);
		//	}
		//}
	}
	else {
		// Animationコンポーネントがアタッチされていない。
		abort();
	}
}