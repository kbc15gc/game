#pragma once
#include "PlayerState.h"
#include "fbEngine\_Object\_GameObject\SoundSource.h"

class PlayerStateAttack : public PlayerState
{
public:
	//攻撃コリジョンパラメータ。
	struct  AttackCollisionParameter
	{
		Vector3 pos;	//発生位置。
		Quaternion rot;	//回転。
		Vector3 scale;	//大きさ。
		int attackframe;//攻撃するフレーム。
		float lifetime;	//コリジョンの発生時間。
		float atk;		//攻撃モーション別の攻撃力。
	};
	PlayerStateAttack(Player* player);
	~PlayerStateAttack();
	void Update()override;
	void Enter()override;
	void Leave()override;
	//攻撃コリジョンパラメータを渡す。
	void Attack(AttackCollisionParameter pram);
	//方向転換
	void Dir();
private:
	//攻撃時のSE
	SoundSource* _SE = nullptr;
	//攻撃コリジョン
	std::vector<AttackCollisionParameter> _AttackPram;
};