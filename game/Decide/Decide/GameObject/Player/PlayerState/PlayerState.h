#pragma once

class Player;

class PlayerState
{
public:
	PlayerState(Player* player)
	{
		this->_Player = player;
	}
	virtual ~PlayerState()
	{
	}
	//アップデート
	virtual void Update() = 0;
	//入る
	virtual void Enter() = 0;
	//抜ける
	virtual void Leave() = 0;
protected:
	Player* _Player;
	const float _Speed = 20.0f;
	const float _JumpSpeed = 15.0f;
};