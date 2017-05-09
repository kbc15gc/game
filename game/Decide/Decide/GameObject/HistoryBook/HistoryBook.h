#pragma once
#include "fbEngine\_Object\_GameObject\GameObject.h"

class HistoryBook : public GameObject
{
public:
	//歴史書のステート
	enum class State
	{
		//IdolClose = 0,		//閉じた状態。
		Open = 0,				//本が開いている。
		IdolOpen,			//開いた状態。
		Close,				//本が閉じている。
	};
	//歴史書のアニメーションナンバー
	enum class AnimationNo
	{
		AnimationInvalid = -1,		//無効
		AnimationOpen,				//本が開くアニメーション。
		AnimationNum,
		
	};
	HistoryBook(const char* name);

	void Awake()override;
	void Start()override;
	void Update()override;
	//アニメーション再生
	// animno アニメーションのナンバー
	// interpolatetime 補間時間
	// loopnum ループ回数 (デフォルトは-1)
	void PlayAnimation(AnimationNo animno, float interpolatetime, int loopnum = -1);
private:
	Animation* _Anim;											//アニメーションのコンポーネントのアドレス保持用。
	double _AnimationEndTime[(int)AnimationNo::AnimationNum];	//アニメーションの終了時間
	State HistoryBookState;
	GameObject* _Player = nullptr;
	GameObject* _GameCamera = nullptr;
};