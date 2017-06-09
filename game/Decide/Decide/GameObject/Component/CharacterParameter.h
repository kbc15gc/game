#pragma once
#include "fbEngine\_Object\_Component\Component.h"

class CharacterParameter :public Component {
public:
	CharacterParameter(GameObject* g, Transform* t) :Component(g, t, typeid(this).name()) {};
	~CharacterParameter() {};
public:
	/*初期化
	*/
	void ParamInit(int hp, int mp, int atk, int def, int dex, int agi);
	
	int _HP;	//ヒットポイント
	int _MP;	//マジックポイント
	int _ATK;	//攻撃力
	int _DEF;	//防御力
	int _DEX;	//命中力
	int _AGI;	//回避力
};