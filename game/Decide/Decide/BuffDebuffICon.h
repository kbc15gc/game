#pragma once
#include "fbEngine\_Object\Object.h"

class ImageObject;

//バフ、デバフを表示するクラス。
class BuffDebuffICon :public GameObject
{
public:
	//表示するアイコンの情報をまとめる用。
	struct BuffDebuff
	{
		ImageObject* _ArrowIconImage;			//BuffDebuffTypeIconに添える矢印アイコン。
		ImageObject* _BuffDebuffTypeIconImage;	//何のステータスが上がっているかを表すアイコン。
	};

	//バフデバフがどのステータスに影響しているかを判断するのに使う。
	enum class Param
	{
		HP,			//HP(キャラクターパラメーターとの項目合わせ用。今の所使わない)。
		MP,			//MP(キャラクターパラメーターとの項目合わせ用。今の所使わない)。
		Atk,		//物理攻撃力。
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

	//更新。
	void Update()override;

	//バフアイコンを生成。
	//引数:バフを掛けるパラメーター(Atk,Matk,Def,MDef,Dex)。
	void BuffIconCreate(Param param);

	//デバフかを見てアイコンを生成。
	//引数:デバフを掛けるパラメーター(Atk,Matk,Def,MDef,Dex)。
	void DebuffIconCreate(Param param);
private:
	vector<BuffDebuff*>	_PlayerBuffDebuffList;					//プレイヤーに掛かっているバフデバフのリスト。
	Transform*			_PlayerHpBarTransform = nullptr;		//プレイヤーのHpBarのTransform参照用。

};

//表示するステータスアイコン。
static char* TypeIconText[static_cast<int>(BuffDebuffICon::Param::Max)] =
{	
	"Hp.pmg",			//Hp(パラメーターとの項目合わせ用。今の所使わない)
	"Mp.pmg",			//Mp(パラメーターとの項目合わせ用。今の所使わない)
	"sword.png",		//剣。
	"magic.png",		//杖。
	"armor.png",		//鎧。
	"cloaks.png",		//服。
	"UI/S_Light01.png"	//クリティカル率。
};
