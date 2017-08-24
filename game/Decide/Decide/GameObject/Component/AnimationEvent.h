#pragma once
#include "fbEngine\_Object\_Component\Component.h"
#include "fbEngine\fbstdafx.h"
#include "AttackCollision.h"


class GameObject;


// 関数ポインタを新しい型名で定義。
// ※新しい名前はAnimationEvent。
// ※戻り値はvoid、引数はなし。
typedef void(GameObject::*AnimationEvent)();

// アニメーションイベント再生関数。
class AnimationEventPlayer : public Component {
public:
	// 攻撃コリジョンのパラメータ。
	// ※ConfigAnimationEvent呼び出し時に使ってね。
	struct AttackEventInfo {
	public:
		// 引数：	親(デフォルトはnull)。
		//			生成直後に親子関係を外すか。
		AttackEventInfo(Transform* tr, bool isRemoveParent) {
			parent = tr;
			this->isRemoveParent = isRemoveParent;
		}
		AttackCollision::CollisionMaster master;	// 誰の攻撃か。
		int damage;		// ダメージ量。
		Vector3 pos;	// 位置。
		Quaternion rot;	// 回転。
		Vector3 size;	// サイズ。
		float life = -1.0f;	// 寿命(0.0より小さい値で無限)。
		float wait = 0.0f;	// イベント呼び出しから何秒後に生成するか。
		Transform* parent = nullptr;	// 親(デフォルトはnull)。
		bool isRemoveParent = true;		// 生成直後に親子関係を外すか。
	};

	struct SoundEventInfo {
		SoundSource* source = nullptr;
		bool is3D = false;
		bool isLoop = false;
	};
private:
	struct EventData {
		int playFrame;		// イベントが発生するフレーム。
		GameObject* object = nullptr;
		AnimationEvent Event;	// イベント(関数ポインタ)。
	};

public:
	AnimationEventPlayer(GameObject* g, Transform* t) :Component(g, t, typeid(this).name()) {
#ifdef _DEBUG
		mbstowcs_s(nullptr, name, typeid(*this).name(), strlen(typeid(*this).name()));
#endif
	};
	~AnimationEventPlayer();

	void Update()override;

	void Init();

	// 指定したフレームにアニメーションイベントを設定する関数。
	// 引数：	イベントを設定するアニメーション番号。
	//			イベントを設定するフレーム。
	//			アニメーションイベント(関数ポインタvoid(*AnimationEvent)(void))。
	// ※生成されるコリジョン形状はボックスです。
	void AddAnimationEvent(int animationNo, const int eventFrame,GameObject* object, AnimationEvent Event) {
		if (_isFirst) {
			Init();
		}
		EventData* work = nullptr;
		work = new EventData;
		work->playFrame = eventFrame;
		work->object = object;
		work->Event = Event;
		_animationEvents[animationNo].push_back(work);
	}

private:
	bool _isFirst = true;	// 最初の追加処理か。
	vector<vector<EventData*>> _animationEvents;
};
