#pragma once

static const float WindowW = 1280.0f;
static const float WindowH = 720.0f;
//大陸の数
static const UINT CONTINENT_NUM = 1;
//歴史チップの数
static const UINT HISTORY_CHIP_NUM = 3;

/** コンティニューフラグ. */
extern bool IS_CONTINUE;

enum Collision_ID
{
	NOT_ID = fbCollisionAttributeE::NOTHING,
	GROUND = fbCollisionAttributeE::GROUND,
	PLAYER = fbCollisionAttributeE::CHARACTER,
	CHARACTER_GHOST = fbCollisionAttributeE::CHARACTER_GHOST,	//キャラクターコントローラ生成時に使用するが、他のオブジェクトに影響を与えたくないときなどに使用(キャラクターコントローラがこの属性のコリジョンと衝突した場合、アトリビュートに関係なく衝突解決を行わないので注意。この属性のキャラクターコントローラが他のオブジェクトに当たったときは衝突解決される、つまり一方通行)。
	ENEMY = BIT(4),	// プレイヤーを押し戻すコリジョンと攻撃判定用のコリジョンとして使用(コリジョンを一つしか持たない単純形状のエネミーならばキャラクターコントローラにもこれを使用する)。
	NOTHITCAMERA = BIT(5),
	ATTACK = BIT(6),
	SPACE = BIT(7),
	BUILDING = BIT(8),
	ITEMRANGE = BIT(9),
	DROPITEM=BIT(10),	//ドロップアイテム用のコリジョンID(地面意外と当たり判定を行わない)。
	BOSS = BIT(11),
};


//#define DEBUG_SPLITSPACE	// 定義で空間分割デバッグオン。
//#define OFF_SPLITSPACE	// 定義で空間分割無効化。
//#define RELEASE_LOW

