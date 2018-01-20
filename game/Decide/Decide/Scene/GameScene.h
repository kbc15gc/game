#pragma once
#include "fbEngine\_Scene\Scene.h"
#include "fbEngine\_Object\_GameObject\SoundSource.h"
#include "GameObject/Player/Player.h"

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

class SplitSpace;
class HistoryMenu;
class HistoryBook;
class VillageName;

class GameScene : public Scene
{
public:
	enum class BGM
	{
		NONE = -1,
		BOSS1 = 0,
		MATI1,
		MATI2,
		MATI3,
		MAOU1,
		MAOU2,
		MAOU3,
		WORLD,
		DEAD,
		NUM,
	};
	GameScene() {};
	void Start()override;
	void Update()override;

	// ボス戦などでフィールドBGMを止めた後、もういちどフィールドBGMを再生するために使用。
	void ResetBGMIndex() {
		_BGM = BGM::NONE;
	}

	// ボス戦などでフィールドBGMを止めたいときに使用。
	void StopFieldBGM() {
		if (_GameBGM) {
			_GameBGM->Stop();
		}
	}
private:
	//チップを作成する
	void _NewChip();
	//ワールドのBGMを変更する。
	void _ChangeBGM(BGM);
	//BGM初期化
	//BGMState
	//名前
	//ボリューム
	void InitBGM(BGM, char*, float);
	//箱のあたり判定
	//場所変えなきゃ
	// 境界箱（AABB）による当たり判定
	bool _IsCollideBoxAABB(Vector3 vMin1, Vector3 vMax1, Vector3 vMin2, Vector3 vMax2);
private:
	SoundSource* _SoundBGM[static_cast<int>(BGM::NUM)];
	SoundSource* _GameBGM;
	BGM _BGM = BGM::NONE;
	Player* _Player;
	HistoryMenu* _HistoryMenu = nullptr;
	HistoryBook* _HistoryBook = nullptr;
	VillageName* _VillageName = nullptr;
	bool _IsEnding;

	SplitSpace* _splitWorld = nullptr;
	SplitSpace* _splitMaouzyou = nullptr;
	bool _isMaouzyou = false;	// 魔王城にいるか。
	bool _isFirstFrame = true;
};