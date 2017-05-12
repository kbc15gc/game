#pragma once
#include "fbEngine\_Object\_GameObject\GameObject.h"

//歴史書。
class HistoryBook : public GameObject
{
public:
	//歴史書のステート。
	enum class State
	{
		CloseIdol= 0,		//閉じた状態。
		Open,				//本が開いている。
		OpenIdol,			//開いた状態。
		Close,				//本が閉じている。
	};
	//歴史書のアニメーションナンバー。
	enum class AnimationNo
	{
		AnimationInvalid = -1,	//無効。
		AnimationCloseIdol,		//本が閉じた状態。
		AnimationOpen,			//本が開いている状態。
		AnimationOpenIdol,		//本が開いたままの状態。
		AnimationClose,			//本が閉じている状態。
		AnimationNum,
		
	};
	HistoryBook(const char* name);
	void Awake()override;
	void Start()override;
	void Update()override;
	//アニメーション再生。
	// animno アニメーションのナンバー。
	// interpolatetime 補間時間。
	// loopnum ループ回数 (デフォルトは-1)。
	void PlayAnimation(AnimationNo animno, float interpolatetime, int loopnum = -1);

	//歴史書を見ているかどうかのフラグ。
	bool GetIsLookAtHistoryFlag() const
	{
		return _IsLookAtHistoryFlag;
	}

	//歴史書の状態を取得。
	State GetHistoryState()
	{
		return _HistoryBookState;
	}
private:
	SkinModel* _Model;
	Animation* _Anim;											//アニメーションのコンポーネントのアドレス保持用。
	double	_AnimationEndTime[(int)AnimationNo::AnimationNum];	//アニメーションの終了時間
	State _HistoryBookState;									//歴史書の状態。
	bool _IsLookAtHistoryFlag;									//歴史書を見ているかどうかのフラグ(最初は見ていないのでfalse)。
};