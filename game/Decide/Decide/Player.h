#pragma once
#include "fbEngine/GameObject.h"
#include "fbEngine/CharacterController.h"
#include "PlayerStateRun.h"

class SkinModel;
class Animation;


class Player : public GameObject
{
public:
	enum class State
	{
		Wait,
		Run,
		Jump,
		Attack,
	};
	Player(const char* name);
	void Awake()override;
	void Start()override;
	void Update()override;
	//移動
	void Move();
	//ジャンプ
	void Jump();
	//攻撃
	void Attack();
	//状態変更
	//次のステート
	void ChangeState(State nextstate);
	//キャラクターコントローラーゲット
	CCharacterController& GetCharaCon() const
	{
		return* _CharacterController;
	}
private:
	//重力処理
	void _GravityCheck(const float& movey);
	//横のあたり判定
	void _WallCheck(const Vector3& move);
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
};