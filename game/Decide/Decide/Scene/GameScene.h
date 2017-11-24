#pragma once
#include "fbEngine\_Scene\Scene.h"
#include "fbEngine\_Object\_GameObject\SoundSource.h"
#include "GameObject/Player/Player.h"
class VillageName;

namespace 
{
	//オブジェクトの詳細
	struct SCollisionInfo {
		const char* name;
		Vector3 pos;
		Quaternion angle;
		Vector3 scale;
	};
}


class GameScene : public Scene
{
public:
	enum class BGM
	{
		BOSS1 = 0,
		MATI1,
		MATI2,
		MATI3,
		WORLD,
		DEAD,
	};
	GameScene() {};
	void Start()override;
	void Update()override;
private:
	//チップを作成する
	void _NewChip();
	//ワールドのBGMを変更する。
	void _ChangeBGM(BGM);
	//箱のあたり判定
	//場所変えなきゃ
	// 境界箱（AABB）による当たり判定
	bool _IsCollideBoxAABB(Vector3 vMin1, Vector3 vMax1, Vector3 vMin2, Vector3 vMax2);
private:
	//通常流れてる音楽。
	SoundSource* _WorldBGM;
	SoundSource* _BossBGM;
	SoundSource* _MatiBGM;
	SoundSource* _Mati2BGM;
	SoundSource* _GameBGM;
	SoundSource* _DeadBGM;
	BGM _BGM;
	Player* _Player;

	VillageName* _VillageName;
};