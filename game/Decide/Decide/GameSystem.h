#pragma once

static const float WindowW = 1280.0f;
static const float WindowH = 720.0f;

enum Collision_ID
{
	GROUND = fbCollisionAttributeE::GROUND,
	PLAYER = fbCollisionAttributeE::CHARACTER,
	ENEMY = BIT(3),
	BOSS = BIT(4),
};