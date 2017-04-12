#pragma once
#include "fbEngine/GameObject.h"
#include "fbEngine/CharacterController.h"

class SkinModel;
class Animation;


class Player : public GameObject
{
public:
	enum class PlayerState
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
private:
	//重力処理
	void _GravityCheck(const float& movey);
	//横のあたり判定
	void _WallCheck(const Vector3& move);
private:
	//コンポーネントとかアドレスの保持が必要なものたち
	SkinModel* _Model;
	Animation* _Anim;
	RigidBody* _Rigid;
	btRigidBody* _RB;
	//リジットボディの高さ
	float _Height;
	//リジットボディの幅
	float _Radius;
	//プレイヤーの色
	Color _PlayerColor;
	//最終的な移動量
	Vector3 _MoveSpeed;
	//進行
	Vector3 _Dir;
	//重力
	float _Gravity;
	//プレイヤーの状態
	PlayerState _State;
	//プレイヤーのレベル
	int _Level;
	//ジャンプフラグ true=ジャンプ中 , false=ジャンプしてない
	bool _Jump;
	//キャラクターコントローラー
	CCharacterController* _CharacterController;
};