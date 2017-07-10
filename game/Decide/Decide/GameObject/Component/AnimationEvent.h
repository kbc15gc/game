#pragma once
#include "fbEngine\_Object\_Component\Component.h"
#include "fbEngine\fbstdafx.h"
#include "AttackCollision.h"

class AnimationEvent : public Component {
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

private:
	struct EventData {
		int createFrame;		// 生成するフレーム。
		AttackEventInfo info = AttackEventInfo(nullptr,true);	// 生成するコリジョンのパラメータ。
	};
public:
	AnimationEvent(GameObject* g, Transform* t) :Component(g, t, typeid(this).name()) {
#ifdef _DEBUG
		mbstowcs_s(nullptr, name, typeid(*this).name(), strlen(typeid(*this).name()));
#endif
	};
	~AnimationEvent();

	void Update()override;

	void Init();

	// 指定したフレームに攻撃判定用コリジョン作成イベントを追加する関数。
	// 引数：	イベントを発生させたいアニメーション番号。
	//			コリジョンを発生させるフレーム。
	//			作成するコリジョンのパラメータ。
	// ※生成されるコリジョン形状はボックスです。
	void AddAnimationEvent(int animationNo,const int eventFrame, const AttackEventInfo& info) {
		if (_isFirst) {
			Init();
		}
		EventData* work = nullptr;
		work = new EventData;
		work->createFrame = eventFrame;
		work->info = info;
		_animationEvents[animationNo].push_back(move(work));
	}

private:
	bool _isFirst = true;	// 最初の追加処理か。
	vector<vector<EventData*>> _animationEvents;
};