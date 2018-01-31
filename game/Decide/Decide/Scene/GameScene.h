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
class GameLight;
class Ground;

class GameScene : public Scene
{
public:
	//BGMの種類
	enum class BGM
	{
		NONE = -1,
		BOSS1 = 0,		//ドラリアン
		BOSS2,			//ボスゴーレム
		BOSS3,			//コードネームD
		MATI1,			//始まりの村
		MATI2,			//狩猟の村
		MATI3,			//発展の街	
		MAOU1,			//魔王城1層
		MAOU2,			//魔王城2層
		MAOU3,			//魔王城3層
		WORLD,			//世界	
		DEAD,			//死亡
		BOSS,			//？
		LASTBOSS,		//ﾗｽﾎﾞｽ
		NUM,
	};
	GameScene() {};
	void Start()override;
	void Update()override;

	// ボス戦などでフィールドBGMを止めた後、もういちどフィールドBGMを再生するために使用。
	void ResetBGMIndex() {
		_BGM = BGM::NONE;
	}

	// ボス戦で流したいBGMを指定して再生する。
	void PlayBossBGM(BGM bgm) {
		if (bgm >= BGM::BOSS) {
			_ChangeBGM(bgm);
		}
	}

	// ボス戦などでフィールドBGMを止めたいときに使用。
	void StopFieldBGM() {
		if (_GameBGM) {
			_GameBGM->Stop();
		}
	}

	SplitSpace* GetNowSplitSpace() {
		return _nowSplitSpace;
	}


private:
	// 月の色を補間しながら変更。
	void _MoveMoonColor();

	// 変更開始。
	void _StartMoveMoonColor(const Color& target,float time);

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
	BGM _BefBGM = BGM::NONE;

	GameLight* _GameLight = nullptr;
	Ground* _Ground = nullptr;

	Player* _Player;
	HistoryMenu* _HistoryMenu = nullptr;
	HistoryBook* _HistoryBook = nullptr;
	VillageName* _VillageName = nullptr;
	bool _IsEnding;

	SplitSpace* _splitWorld = nullptr;
	SplitSpace* _splitMaouzyou = nullptr;

	SplitSpace* _nowSplitSpace = nullptr;	// 現在有効になっている空間分割。

	bool _isMaouzyou = false;	// 魔王城にいるか。
	bool _isFirstFrame = true;

	Color _defaultColor;

	//Color _saveMoonColor;	// 月の色を保存。
	Color _targetMoonColor;	// 月の色目標値。
	Color _colorOffset = Color::black;	// 目標の色までの差分。
	float _colorMoveSpeed;				// 色が変化する速さ。
};