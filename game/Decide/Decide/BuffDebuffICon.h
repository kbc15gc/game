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
		Dex			//クリティカル率。
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

	//バフデバフかを見てアイコンを生成。
	void Buff(Param );

	//どのバフデバフかを見てアイコンを生成。
	void Debuff(Param);
private:
	ImageObject* _ArrowIconImage = nullptr;				//BuffDebuffTypeIconに添える矢印アイコン。
	ImageObject* _BuffDebuffTypeIconImage = nullptr;	//何のステータスが上がっているかを表すアイコン。
};