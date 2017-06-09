#pragma once
#include "fbEngine\_Object\_GameObject\GameObject.h"
#include "HFSM\HistoryBookStateClose.h"
#include "HFSM\HistoryBookStateOpening.h"
#include "HFSM\HistoryBookStateOpen.h"
#include "HFSM\HistoryBookStateCloseing.h"

class Player;

//歴史書。
class HistoryBook : public GameObject
{
public:
	//歴史書のステート。
	enum class State
	{
		Close= 0,	//閉じた状態。
		Opening,	//本が開いている。
		Open,		//開いた状態。
		Closeing,	//本が閉じている。
	};
	//歴史書のアニメーションナンバー。
	enum class AnimationNo
	{
		AnimationInvalid = -1,	//無効。
		AnimationClose,			//本が閉じた状態。
		AnimationOpening,		//本が開いている状態。
		AnimationOpen,			//本が開いたままの状態。
		AnimationCloseing,		//本が閉じている状態。
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

	//歴史書を見ているかどうかのフラグを取得。。
	bool GetIsLookAtHistoryFlag() const
	{
		return _IsLookAtHistoryFlag;
	}

	//歴史書を見ているかのフラグの変更。
	void ChangeIsLookAtHistoryFlag();

	//ステートの変更。
	void ChangeState(State nextstate);

	//歴史書のコンポーネントを有効にするか無効にするかのフラグの設定。
	void SetEnable(bool flag)
	{
		_Model->enable = flag;
	}

	//再生中かのフラグを取得。
	bool GetIsPlay()
	{
		if (_Anim != nullptr)return _Anim->GetPlaying();
	}
private:
	SkinModel*					_Model = nullptr;									//モデルのコンポーネント。のアドレス保持用。
	Animation*					_Anim = nullptr;									//アニメーションのコンポーネントのアドレス保持用。
	double						_AnimationEndTime[(int)AnimationNo::AnimationNum] = { 0.0f,0.0f,0.0f,0.0f };	//アニメーションの終了時間。
	State						_State;												//歴史書の状態。
	bool						_IsLookAtHistoryFlag = false;						//歴史書を見ているかどうかのフラグ(最初は見ていないのでfalse)。
	Player*						_Player = nullptr;									//プレイヤーのコンポーネント。
	Vector3						_PlayerFoward;
	HistoryBookState*			_CurrentState = nullptr;							//歴史書の現在の状態。
	HistoryBookStateClose		_CloseState;										//歴史書の状態　閉じた状態。
	HistoryBookStateOpening		_OpeningState;										//歴史書の状態　開いている状態。
	HistoryBookStateOpen		_Open;												//歴史書の状態　開いた状態。
	HistoryBookStateCloseing	_Closeing;											//歴史書の状態　閉じている状態。
	float						_AngleY = 0.0f;
};