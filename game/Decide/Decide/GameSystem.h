#pragma once

static const float WindowW = 1280.0f;
static const float WindowH = 720.0f;
//‘å—¤‚Ì”
static const UINT CONTINENT_NUM = 1;
//—ğjƒ`ƒbƒv‚Ì”
static const UINT HISTORY_CHIP_NUM = 3;

enum Collision_ID
{
	GROUND = fbCollisionAttributeE::GROUND,
	PLAYER = fbCollisionAttributeE::CHARACTER,
	ENEMY = BIT(3),
	BOSS = BIT(4),
	ATTACK = BIT(5),
};