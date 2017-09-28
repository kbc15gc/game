#pragma once
#include "fbEngine\_Object\Object.h"
#include "fbEngine\_Object\_GameObject\ImageObject.h"

//バフ、デバフを表示するクラス。
class BuffDebuffICon :public GameObject
{
public:
	//バフデバフがどのステータスに影響しているかを判断するのに使う。
	enum class Param
	{
		Atk = 0,	//物理攻撃力。
		MAtk,		//魔法攻撃力。
		Def,		//物理防御力。
		MDef,		//魔法防御力。
		Dex,		//クリティカル率。
		Max
	};

	//コンストラクタ。
	BuffDebuffICon(char* name) :
		GameObject(name)
	{
	}

	//デストラクタ。
	~BuffDebuffICon()
	{
	}

	//初期化。
	void Awake()override;

	//バフアイコンを生成。
	//引数:バフを掛けるパラメーター(Atk,Matk,Def,MDef,Dex)。
	void BuffIconCreate(Param param);

	//デバフかを見てアイコンを生成。
	//引数:デバフを掛けるパラメーター(Atk,Matk,Def,MDef,Dex)。
	void DebuffIconCreate(Param param);
private:
	ImageObject* _ArrowIconImage = nullptr;				//BuffDebuffTypeIconに添える矢印アイコン。
	ImageObject* _BuffDebuffTypeIconImage = nullptr;	//何のステータスが上がっているかを表すアイコン。
};

//ステータスアイコン。
static const char* TypeIconText[static_cast<int>(BuffDebuffICon::Param::Max)] =
{
	"sword.png",
	"magic.png",
	"armor.png",
	"cloaks.png",
	"UI/S_Light01.png"
};
