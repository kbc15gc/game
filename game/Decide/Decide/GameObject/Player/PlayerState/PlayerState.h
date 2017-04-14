#pragma once

class Player;

class PlayerState
{
public:
	PlayerState(Player* player)
	{
		this->player = player;
	}
	virtual ~PlayerState()
	{
	}
	//�A�b�v�f�[�g
	virtual void Update() = 0;
	//����
	virtual void Enter() = 0;
	//������
	virtual void Leave() = 0;
protected:
	Player* player;
	const float _Speed = 20.0f;
	const float _JumpSpeed = 15.0f;
};