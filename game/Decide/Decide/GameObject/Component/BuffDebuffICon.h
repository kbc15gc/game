#pragma once
#include "fbEngine\_Object\_Component\Component.h"
#include "GameObject\Component\CharacterParameter.h"
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

	//使うアイコンのタイプ。
	//プレイヤーだとHpBarの下。エネミーだとHpBarの上。
	enum class UseIconType
	{
		Player = 0,
		Enemy
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
		ImageObject*				_ArrowIconImage;			//BuffDebuffTypeIconに添える矢印アイコン。
		ImageObject*				_BuffDebuffTypeIconImage;	//何のステータスが上がっているかを表すアイコン。
		CharacterParameter::Param   _Param;						//どのパラメーターかを保持する用。
		bool						_isBuff;					//バフかデバフかどうかのフラグ。
	};


	//初期化。
	void Awake()override;

	//更新。
	void Update()override;

#ifdef _DEBUG
	void Debug()override;
#endif
	//バフアイコンを生成。
	//引数:バフを掛けるパラメーター(Atk,Matk,Def,MDef,Dex)。
	void BuffIconCreate(CharacterParameter::Param param);

	//デバフアイコンを生成。
	//引数:デバフを掛けるパラメーター(Atk,Matk,Def,MDef,Dex)。
	void DebuffIconCreate(CharacterParameter::Param param);

	//バフアイコンの削除。
	//引数:効果時間が切れたパラメーター(Atk,Matk,Def,MDef,Dex)。
	void DeleteBuffIcon(CharacterParameter::Param param);

	//デバフアイコンの削除。
	//引数:効果時間が切れたパラメーター(Atk,Matk,Def,MDef,Dex)。
	void DeleteDebuffIcon(CharacterParameter::Param param);

	//自身に掛かっている全てのバフデバフアイコンを削除。
	void DeleteAllBuffDebuffIcon();

	//アイコンを描画しない。
	void RenderDisable();

	//アイコンを描画する。
	void RenderEnable();

	//使うアイコンタイプをプレイヤーに設定。
	void SelectUseIconType_Player() {
		_UseIconType = UseIconType::Player;
	}

	//使うアイコンタイプをエネミー設定。
	void SelectUseIconType_Enemy() {
		_UseIconType = UseIconType::Enemy;
	}

	//HpBarのtransformを設定。
	void SetHpBarTransform(Transform* trns) {
		_HpBarTransform= trns;
	}

	//バフデバフアイコンのサイズを設定(サイズを指定しなくてもデフォルトサイズになる)。
	void SetBuffDebfuuIconSize(const Vector2& size) {
		_BuffDebfuuIconSize = size;
	}

	//矢印のサイズを設定(サイズを指定しなくてもデフォルトサイズになる)。
	void ArrowIconSize(const Vector2& size) {
		_ArrowSize = size;
	}
private:
	//追加するパラメーターを追加していいのかをチェック、追加が可能ならtrue、追加出来ないならfalse。
	//引数:パラメーター(Atk,Matk,Def,MDef,Dex)。
	bool _AddCheck(CharacterParameter::Param param,bool isBuff);

	void SetPlayerOffset() {
		_IconOffSet = 60.0f;
		_IconPosOffset = { 50.0f,120.0f, 0.0f };
		_ArrowPosOffet = { 20.0f, 12.0f, 0.0f };
	}

	void SetEnemyOffset() {
		_IconOffSet = 30.0f;
		_IconPosOffset = { -70.0f,-20.0f,0.0f };
		_ArrowPosOffet = { 5.0f,5.0f,0.0f };
	}
private:
	vector<BuffDebuff*>	_BuffDebuffList;								//バフデバフのリスト。
	Transform*			_HpBarTransform		= nullptr;					//HpBarのTransform参照用。
	Vector2				_ScreenPos			= Vector2::zero;			//HpBarのポジションからスクリーン座標に変換したもの。
	Vector2				_BuffDebfuuIconSize = Vector2(40.0f, 40.0f);	//バフデバフアイコンサイズ。
	Vector2				_ArrowSize			= Vector2(13.0f, 20.0f);	//矢印のサイズ。
	UseIconType			_UseIconType		= UseIconType::Player;		//アイコンタイプ。
	float				_IconOffSet			= 0.0f;						//バフ、デバフを重ね掛けする際にずらすために使う。																		//エネミー時のアイコンをHpBarからどれだけ離すかのOFFSET。
	Vector3				_IconPosOffset		= { 0.0f,0.0f,0.0f };		//アイコンをHpBarからどれだけ離すかのOFFSET。
	Vector3				_ArrowPosOffet		= { 0.0f,.0f,0.0f };		//アイコンから矢印をどれだけ離すかのOFFSET。
	int					_Priority			= 0;						//優先度。
};

namespace {
	//表示するステータスアイコン。
	static char* TypeIconText[static_cast<int>(CharacterParameter::Param::MAX)] =
	{
		"Hp.pmg",			//Hp(パラメーターとの項目合わせ用。今の所使わない)
		"Mp.pmg",			//Mp(パラメーターとの項目合わせ用。今の所使わない)
		"sword.png",		//剣。
		"magic.png",		//杖。
		"armor.png",		//鎧。
		"cloaks.png",		//服。
		"UI/S_Light01.png", //クリティカル率。
		"Crt.png",
		"Lv.png",
	};

	//表示する矢印アイコン。
	static char* ArrowIconText[static_cast<int>(BuffDebuffICon::Arrow::Max)] =
	{
		"BuffIArrow.png",	//バフ矢印。
		"DebuffArrow.png"	//デバフ矢印。
	};
}