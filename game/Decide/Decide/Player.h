#pragma once
#include "fbEngine/GameObject.h"
#include "fbEngine/CharacterController.h"
#include "PlayerStateRun.h"
#include "PlayerStateIdol.h"

class SkinModel;
class Animation;


class Player : public GameObject
{
public:
	//プレイヤーのステート
	enum class State
	{
		Idol,
		Run,
		Attack,
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
	};
	Player(const char* name);
	void Awake()override;
	void Start()override;
	void Update()override;
	//状態変更
	//次のステート
	void ChangeState(State nextstate);
	//アニメーション再生
	//アニメーションのナンバー
	//補間時間
	void PlayAnimation(AnimationNo animno, float interpolatetime);
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
private:
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
	//ジャンプフラグ true=ジャンプ中 , false=ジャンプしてない
	bool _Jump;
	//キャラクターコントローラー
	CCharacterController* _CharacterController;
	//現在のプレイヤーのステート
	PlayerState*	_CurrentState;
	//プレイヤーステートラン
	PlayerStateRun	_RunState;
	//プレイヤーステートアイドル
	PlayerStateIdol	_IdolState;
};