#pragma once

static const float WindowW = 1280.0f;
static const float WindowH = 720.0f;
//�嗤�̐�
static const UINT CONTINENT_NUM = 1;
//���j�`�b�v�̐�
static const UINT HISTORY_CHIP_NUM = 3;

enum Collision_ID
{
	GROUND = fbCollisionAttributeE::GROUND,
	PLAYER = fbCollisionAttributeE::CHARACTER,
	ENEMY = BIT(3),
	BOSS = BIT(4),
	ATTACK = BIT(5),
	SPACE = BIT(6),
	BUILDING = BIT(7),
};


//#define OFF_SPLITSPACE	// ��`�ŋ�ԕ����������B
#define DEBUG_SPLITSPACE	// ��`�ŋ�ԕ����f�o�b�O�I���B
