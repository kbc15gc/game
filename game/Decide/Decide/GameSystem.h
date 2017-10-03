#pragma once

static const float WindowW = 1280.0f;
static const float WindowH = 720.0f;
//大陸の数
static const UINT CONTINENT_NUM = 1;
//歴史チップの数
static const UINT HISTORY_CHIP_NUM = 3;

enum Collision_ID
{
	GROUND = fbCollisionAttributeE::GROUND,
	PLAYER = fbCollisionAttributeE::CHARACTER,
	ENEMY = BIT(3),
	ATTACK = BIT(4),
	SPACE = BIT(5),
	BUILDING = BIT(6),
	ITEMRANGE = BIT(7),
};


//#define DEBUG_SPLITSPACE	// 定義で空間分割デバッグオン。
//#define OFF_SPLITSPACE	// 定義で空間分割無効化。

