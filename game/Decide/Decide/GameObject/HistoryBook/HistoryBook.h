#pragma once
#include "fbEngine\_Object\_GameObject\GameObject.h"

class Player;

//歴史書。
class HistoryBook : public GameObject
{
public:
	//歴史書のステート。
	enum class State
	{
		Close= 0,		//閉じた状態。
		Opening,				//本が開いている。
		Open,			//開いた状態。
		Closeing,				//本が閉じている。
	};
	//歴史書のアニメーションナンバー。
	enum class AnimationNo
	{
		AnimationInvalid = -1,	//無効。
		AnimationClose,		//本が閉じた状態。
		AnimationOpening,			//本が開いている状態。
		AnimationOpen,		//本が開いたままの状態。
		AnimationCloseing,			//本が閉じている状態。
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
	//アニメーションコントロール
	void AnimationControl();

	//歴史書を見ているかのフラグの変更。
	void ChangeIsLookAtHistoryFlag();
private:
	SkinModel* _Model;
	Animation* _Anim;											//アニメーションのコンポーネントのアドレス保持用。
	double	_AnimationEndTime[(int)AnimationNo::AnimationNum];	//アニメーションの終了時間。
	State _HistoryBookState;									//歴史書の状態。
	bool _IsLookAtHistoryFlag;									//歴史書を見ているかどうかのフラグ(最初は見ていないのでfalse)。
	Player* _Player;
	Vector3 _PlayerFoward;
};