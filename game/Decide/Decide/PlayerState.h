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
	//アップデート
	virtual void Update() = 0;
	//入る
	virtual void Enter() = 0;
	//抜ける
	virtual void Leave() = 0;
protected:
	Player* player;
};