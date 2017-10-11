#pragma once
#include "fbEngine\_Object\_Component\Component.h"
#include "fbEngine\fbstdafx.h"
#include "AttackCollision.h"



// 関数ポインタを新しい型名で定義。
// ※新しい名前はAnimationEvent。
// ※戻り値はvoid、引数はなし。
typedef void(GameObject::*AnimationEvent)();

// アニメーションイベント再生関数。
class AnimationEventPlayer : public Component {
public:

private:
	struct EventData {
		float playTime;		// イベントが発生する時間。
		AnimationEvent Event;	// イベント(GameObjectを継承したクラスのメンバ関数ポインタ)。
		bool isPlay = false;	// イベント実行済みか。
	};

public:
	AnimationEventPlayer(GameObject* g, Transform* t) :Component(g, t, typeid(this).name()) {
#ifdef _DEBUG
		mbstowcs_s(nullptr, name, typeid(*this).name(), strlen(typeid(*this).name()));
#endif
	};
	~AnimationEventPlayer();

	void Update()override;

	// 初期化。
	// 引数：	このコンポーネントがアタッチされているゲームオブジェクトのアニメーション数。
	void Init(int animationNum);

	// 指定したフレームにアニメーションイベントを設定する関数。
	// 引数：	イベントを設定するアニメーション番号。
	//			イベントを設定する時間(秒)。
	//			アニメーションイベント(関数ポインタvoid(*AnimationEvent)(void))。
	// ※生成されるコリジョン形状はボックスです。
	void AddAnimationEvent(int animationNo, const float eventTime, AnimationEvent Event) {
		EventData* work = nullptr;
		work = new EventData;
		work->playTime = eventTime;
		work->Event = Event;
		work->isPlay = false;
		_animationEvents[animationNo].push_back(work);
	}

private:
	vector<vector<EventData*>> _animationEvents;
	int _playAnimationNo;	// 再生中のアニメーション番号。
	double _nowLocalTime = 0.0f;	// 現在の再生時間。
};
