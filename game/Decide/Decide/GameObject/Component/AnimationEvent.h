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

	// どのイベントリストを再生するか設定する関数。
	// 引数：	アニメーション番号。
	//			再生したいイベントリストの番号。
	void ConfigPlayEventList(int animationNo,int eventListNo) {
		if (eventListNo < 0) {
			// 0より小さい値を設定すんな。
			abort();
		}
		_nowPlayEventList[animationNo] = eventListNo;
	}

	// アニメーションイベントを登録する関数。
	// 引数：	イベントを設定するアニメーション番号。
	//			イベントを設定する時間(秒)。
	//			アニメーションイベント(関数ポインタvoid(*AnimationEvent)(void))。
	//			イベントリスト番号(同じモーションを使用するが再生するアニメーションイベントは切り替えたいときに使用、デフォルトは0)。
	void AddAnimationEvent(int animationNo, const float eventTime, AnimationEvent Event, int eventListNo = 0);

	void StartAnimation(int animationNo);

private:
	vector<vector<vector<EventData*>>> _animationEvents;
	vector<int> _nowPlayEventList;	// どのイベントリストを再生するか(要素数はアニメーションの数)。
	bool _isSetCallBack = false;
};
