#pragma once
#include "fbEngine\_Object\_Component\Component.h"
#include "fbEngine\_Object\_GameObject\TextObject.h"
#include "fbEngine\fbstdafx.h"
#include "GameObject\ItemManager\HoldItem\HoldWeapon\HoldWeapon.h"

class CharacterParameter :public Component {
public:
	CharacterParameter(GameObject* g, Transform* t) :Component(g, t, typeid(this).name()) {
#ifdef _DEBUG
		mbstowcs_s(nullptr, name, typeid(*this).name(), strlen(typeid(*this).name()));
#endif

	};
	~CharacterParameter() {
	};

	void Awake()override;
public:

#ifdef _DEBUG
	// デバッグ出力用の配列。
	// ※パラメーターの種類を追加したら順番通りに追加して。
	static const wchar_t* ENUM_NAME[];
#endif

	// 元素属性(火、水など)。
	enum class Element{None = 0};
	// 物理属性(斬、打、魔など)。
	enum class Physical{None = 0};
	// パラメーター列挙。
	//			HP。
	//			HP最大値。
	//			MP。
	//			MP最大値。
	//			攻撃力。
	//			魔法攻撃力。
	//			防御力。
	//			魔法防御。
	//			器用度(クリティカル発生率)。
	//			レベル。
	//			経験値。
	//			落とす経験値。
	//			所持金。
	enum Param { MIN = -1,HP = 0, MAXHP, MP, MAXMP, ATK, MAT, DEF, MDE, DEX, LV, EXP, DROPEXP, MONEY, MAX };
	//初期化。
	// 引数：	HP。
	//			HP最大値。
	//			MP。
	//			MP最大値。
	//			攻撃力。
	//			魔法攻撃力。
	//			防御力。
	//			魔法防御。
	//			器用度(クリティカル発生率)。
	//			レベル。
	//			経験値。
	//			落とす経験値。
	//			所持金。
	void ParamInit(int hp,int maxhp,int mp,int maxmp, int atk, int mat, int def, int mde, int dex, int lv, int exp, int dropexp,int money);
	//初期化。
	void ParamInit(int param[Param::MAX]);
	void ParamInit(const vector<int>& param);

	void Update()override;

	// 指定したパラメーターに加算。
	// 引数：	パラメータータイプ。
	//			加算量。
	inline void AddParam(Param idx,const int add) {
		_OutCheck(idx);
		_Param[idx] += add;
	}
	// 指定したパラメーターから減算。
	// 引数：	パラメータータイプ。
	//			減算量。
	inline void SubParam(Param idx, const int sub) {
		_OutCheck(idx);
		_Param[idx] -= sub;
	}
	// 指定したパラメーター取得。
	// 引数：	パラメータータイプ。
	inline int GetParam(Param idx)const {
		_OutCheck(idx);
		return _Param[idx];
	}

	int* GetParamPt(Param param)
	{
		return &_Param[param];
	}

	// 全パラメーター取得。
	inline const vector<int>& GetParams() {
		return _Param;
	}

	//死んだかどうかのフラグを取得。
	//tureなら死んでいる。
	inline bool GetDeathFalg()
	{
		return _DeathFlag;
	}

	// 被ダメージ処理(パラメーターにダメージを与える)。
	// 引数:		敵からのダメージ。
	//				キャラクターの行動で発生する防御率(防御行動などによって変動する値、デフォルトは1)。
	//				装備品の防御力(デフォルトは0)。
	int ReciveDamage(int defaultDamage, int defidx = 1, int Equipment = 0);

	// 被ダメージ計算(計算のみでパラメーターに影響はない)。
	// 引数:		敵からのダメージ。
	//				キャラクターの行動で発生する防御率(防御行動などによって変動する値、デフォルトは1)。
	//				装備品の防御力(デフォルトは0)。
	int ReceiveDamageMass(int defaultDamage, int defidx = 1,int Equipment = 0);

	//与ダメージ計算。
	// 引数：	魔法攻撃か。
	//			武器(デフォルトはnull、武器未装備時はnullを設定)。	
	//			キャラクターの行動で発生する攻率力(攻撃の種類などによって変動する値、デフォルトは1)。
	inline int GiveDamageMass(bool isMagic,HoldWeapon* weapon = nullptr,int atk = 1)
	{
		int damage = 0;
		int weaponDamage = 0;
		int critMax = 100;	// クリティカル率最大。
		int crit = _Param[Param::DEX];// クリティカル率。

		if (isMagic) {
			damage = _Param[Param::MAT];
			if (weapon) {
				//weaponDamage = weapon->GetInfo();
				crit += static_cast<Item::WeaponInfo*>(weapon->GetInfo()->Dex);
			}
		}
		else {
			damage = _Param[Param::ATK];
			if (weapon) {
				weaponDamage = weapon->GetAttack();
			}
		}

		damage += weaponDamage;
		if()
		damage * 

		// クリティカル。
		damage += static_cast<Item::WeaponInfo*>(weapon->GetInfo())->ATK + (static_cast<Item::WeaponInfo*>(weapon->GetInfo())->ATK * weapon->GetAtkRnd() * 0.1f);
		
		return damage * atk;
	}
	// レベルアップ。
	// 引数：		レベルアップに必要な経験値の値。
	void LevelUP(int lvupexp, int hp, int mp, int atk, int mat,int def,int mde, int dex);
private:

	// 配列外にアクセスしてないかチェック。
	inline void _OutCheck(int num)const {
		if (num <= Param::MIN || num >= Param::MAX) 
			abort();
	}

private:
	vector<int> _Param;
	bool _DeathFlag = false;//死んだかどうかのフラグ。
};
