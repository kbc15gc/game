#pragma once
#include "PlayerState.h"
#include "fbEngine\_Object\_GameObject\SoundSource.h"

class PlayerStateAttack : public PlayerState
{
public:
	//�U���R���W�����p�����[�^�B
	struct  AttackCollisionParameter
	{
		Vector3 pos;	//�����ʒu�B
		Quaternion rot;	//��]�B
		Vector3 scale;	//�傫���B
		int attackframe;//�U������t���[���B
		float lifetime;	//�R���W�����̔������ԁB
		float atk;		//�U�����[�V�����ʂ̍U���́B
	};
	PlayerStateAttack(Player* player);
	~PlayerStateAttack();
	void Update()override;
	void Enter()override;
	void Leave()override;
	//�U���R���W�����p�����[�^��n���B
	void Attack(AttackCollisionParameter pram);
	//�����]��
	void Dir();
private:
	//�U������SE
	SoundSource* _SE = nullptr;
	//�U���R���W����
	std::vector<AttackCollisionParameter> _AttackPram;
};