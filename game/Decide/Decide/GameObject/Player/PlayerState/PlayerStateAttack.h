#pragma once
#include "PlayerState.h"
#include "fbEngine\_Object\_GameObject\SoundSource.h"

class PlayerStateAttack : public PlayerState
{
public:
	PlayerStateAttack(Player* player);
	~PlayerStateAttack();
	void Update()override;
	void Enter()override;
	void Leave()override;
	//UŒ‚ŠÖ”
	//w’è‚µ‚½ƒtƒŒ[ƒ€”‚ÉUŒ‚‚·‚é
	void Attack(int attackframe);
private:
	SoundSource* _SE;
};