#pragma once
#include "fbEngine\_Object\_Component\Component.h"
class ImageObject;

//バフ、デバフを表示するクラス。
class BuffDebuffICon :public Component
{
public:

	BuffDebuffICon(GameObject* g, Transform* t) :Component(g, t, typeid(this).name()) {
#ifdef _DEBUG
		mbstowcs_s(nullptr, name, typeid(*this).name(), strlen(typeid(*this).name()));
#endif

	};
	~BuffDebuffICon();
public:
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

	//矢印の種類。
	enum class Arrow
	{
		Up,		//バフ矢印。
		Down,	//デバフ矢印。
		Max
	};

	//表示するアイコンの情報をまとめる用。
	struct BuffDebuff
	{
		ImageObject* _ArrowIconImage;			//BuffDebuffTypeIconに添える矢印アイコン。
		ImageObject* _BuffDebuffTypeIconImage;	//何のステータスが上がっているかを表すアイコン。
		Param        _Param;					//どのパラメーターかを保持する用。
		bool		 _isBuff;					//バフかデバフかどうかのフラグ。
	};


	//初期化。
	void Awake()override;

	//更新。
	void Update()override;

	//バフアイコンを生成。
	//引数:バフを掛けるパラメーター(Atk,Matk,Def,MDef,Dex)。
	void BuffIconCreate(Param param);

	//デバフアイコンを生成。
	//引数:デバフを掛けるパラメーター(Atk,Matk,Def,MDef,Dex)。
	void DebuffIconCreate(Param param);

	//バフアイコンの削除。
	//引数:効果時間が切れたパラメーター(Atk,Matk,Def,MDef,Dex)。
	void DeleteBuffIcon(Param param);

	//デバフアイコンの削除。
	//引数:効果時間が切れたパラメーター(Atk,Matk,Def,MDef,Dex)。
	void DeleteDebuffIcon(Param param);

	//アイコンを描画しない。
	void RenderDisable();

	//アイコンを描画する。
	void RenderEnable();

	//HpBarのtransformを設定。
	void SetHpBarTransform(Transform* trns) {
		_HpBarTransform = trns;
	}
private:
	//追加するパラメーターを追加していいのかをチェック、追加が可能ならtrue、追加出来ないならfalse。
	//引数:パラメーター(Atk,Matk,Def,MDef,Dex)。
	bool _AddCheck(Param param);
private:
	vector<BuffDebuff*>	_BuffDebuffList;				//バフデバフのリスト。
	Transform*			_HpBarTransform = nullptr;		//HpBarのTransform参照用。
};

namespace {
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

	//表示する矢印アイコン。
	static char* ArrowIconText[static_cast<int>(BuffDebuffICon::Arrow::Max)] =
	{
		"BuffIArrow.png",	//バフ矢印。
		"DebuffArrow.png"	//デバフ矢印。
	};
}