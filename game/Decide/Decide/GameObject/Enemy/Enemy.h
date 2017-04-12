#pragma once
#include "EnemyCharacter.h"

// 継承クラス。
// 雑魚エネミー(プロト)。
class Enemy :
	public EnemyCharacter
{
public:
	Enemy(const char* name);
	~Enemy();
	void Awake()override;
	void Start()override;
	void Update()override;
private:
	// コリジョン定義関数。
	// コリジョンの形状やパラメータを設定する関数。
	void _ConfigCollision()override;

};

