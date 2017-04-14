#pragma once
#include "fbEngine/GameObject.h"
#include "fbEngine/CharacterController.h"
#include "PlayerState/PlayerStateRun.h"
#include "PlayerState/PlayerStateIdol.h"
#include "PlayerState/PlayerStateAttack.h"

class SkinModel;
class Animation;


class Player : public GameObject
{
public:
	//プレイヤーのステート
	enum class State
	{
		Idol = 0,			//アイドル
		Run,				//走る
		Attack,				//攻撃
	};
	//アニメーションのナンバー
	enum class AnimationNo
	{
		AnimationInvalid = -1,		//無効
		AnimationDeath,				//死亡
		AnimationAttack02,			//攻撃02
		AnimationAttack01,			//攻撃01
		AnimationJump,				//ジャンプ
		AnimationRun,				//走る
		AnimationWalk,				//歩き
		AnimationIdol,				//アイドル	
		AnimationNum,				//アニメーションの数
	};
	Player(const char* name);
	void Awake()override;
	void Start()override;
	void Update()override;
	//状態変更
	// nextstate 次のステート
	void ChangeState(State nextstate);
	//アニメーション再生
	// animno アニメーションのナンバー
	// interpolatetime 補間時間
	// loopnum ループ回数 (デフォルトは-1)
	void PlayAnimation(AnimationNo animno, float interpolatetime , int loopnum = -1);
	//アニメーションコントロール
	void AnimationControl();
	//セットステート
	void SetState(State state)
	{
		_State = state;
	}
	//ゲットステート
	State GetState()
	{
		return _State;
	}
	//キャラクターコントローラーゲット
	CCharacterController& GetCharaCon() const
	{
		return* _CharacterController;
	}
	//アニメーション再生中フラグゲット
	const bool GetAnimIsPlay() const;
private:
	friend class PlayerStateAttack;

	//コンポーネントとかアドレスの保持が必要なものたち
	SkinModel* _Model;
	Animation* _Anim;
	//高さ
	float _Height;
	//半径
	float _Radius;
	//最終的な移動量
	Vector3 _MoveSpeed;
	//進行
	Vector3 _Dir;
	//重力
	float _Gravity;
	//プレイヤーの状態
	State _State;
	//プレイヤーのレベル
	int _Level;
	//アニメーションの終了時間
	double _AnimationEndTime[(int)AnimationNo::AnimationNum];
	//キャラクターコントローラー
	CCharacterController* _CharacterController;
	//現在のプレイヤーのステート
	PlayerState*	_CurrentState;
	//プレイヤーステートラン
	PlayerStateRun	_RunState;
	//プレイヤーステートアイドル
	PlayerStateIdol	_IdolState;
	//プレイヤーステートアタック
	PlayerStateAttack _AttackState;
};