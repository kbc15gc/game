#pragma once
#include "fbEngine\_Object\_Component\Component.h"

class CharacterParameter :public Component {
public:
	CharacterParameter(GameObject* g, Transform* t) :Component(g, t, typeid(this).name()) {};
	~CharacterParameter() {};
public:
	// 元素属性(火、水など)。
	enum class Element{None = 0};
	// 物理属性(斬、打、魔など)。
	enum class Physical{None = 0};
	// パラメーター列挙。
	enum Param { HP = 0, MAXHP, MP, MAXMP, ATK, DEF, DEX, AGI, MAX };
	//初期化。
	void ParamInit(int hp,int maxhp,int mp,int maxmp, int atk, int def, int dex, int agi);
	
	// 指定したパラメーターに加算。
	// 引数：	パラメータータイプ。
	//			加算量。
	inline void AddParam(Param idx,const int add) {
		_Param[idx] += add;
	}
	// 指定したパラメーターから減算。
	// 引数：	パラメータータイプ。
	//			減算量。
	inline void SubParam(Param idx, const int sub) {
		_Param[idx] -= sub;
	}
	// 指定したパラメーター取得。
	// 引数：	パラメータータイプ。
	inline int GetParam(Param idx)const {
		return _Param[idx];
	}

	//ダメージの計算。
	//第一引数:攻撃系のパラメーター 第二引数:防御系のパラメーター。
	int DamageMass(int atk, int defidx);
	//死んだかどうかのフラグを取得。
	//tureなら死んでいる。
	inline bool GetDeathFalg()
	{
		return _DeathFlag;
	}

	//与えるダメージの計算。
	inline int AttackDamageMass(int atk = 1)
	{
		return _Param[Param::ATK] * atk;
	}
private:
	int _Param[Param::MAX];
	bool _DeathFlag = false;//死んだかどうかのフラグ。

};