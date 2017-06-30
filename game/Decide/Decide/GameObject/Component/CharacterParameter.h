#pragma once
#include "fbEngine\_Object\_Component\Component.h"
#include "fbEngine\_Object\_GameObject\TextObject.h"
#include "fbEngine\fbstdafx.h"

class CharacterParameter :public Component {
public:
	CharacterParameter(GameObject* g, Transform* t) :Component(g, t, typeid(this).name()) {
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
	//			防御力。
	//			命中力。
	//			敏捷力。
	enum Param { MIN = -1,HP = 0, MAXHP, MP, MAXMP, ATK, DEF, DEX, AGI, MAX };
	//初期化。
	// 引数：	HP。
	//			HP最大値。
	//			MP。
	//			MP最大値。
	//			攻撃力。
	//			防御力。
	//			命中力。
	//			敏捷力。
	void ParamInit(int hp,int maxhp,int mp,int maxmp, int atk, int def, int dex, int agi);
	//初期化。
	void ParamInit(int param[Param::MAX]);

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
	// 引数：		キャラクターの行動で発生する攻率力(攻撃の種類などによって変動する値、デフォルトは1)。
	inline int GiveDamageMass(int atk = 1)
	{
		return _Param[Param::ATK] * atk;
	}
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
