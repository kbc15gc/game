#pragma once

#include "fbEngine\_Object\_GameObject\GameObject.h"
#include"GameObject\Player\Player.h"

//ゲームライト
class GameLight :public GameObject
{
public:
	GameLight(const char* name) :GameObject(name) {}
	void Awake()override;

	/**
	* 初期化.
	*/
	void Start()override;

	void Update()override;

private:

	/** プレイヤーのポインタ. */
	Player* _Player = nullptr;

	/** ライト. */
	Light* _Light = nullptr;

};