#pragma once
#include "fbEngine\_Object\_Component\Component.h"

class CharacterParameter :public Component {
public:
	CharacterParameter(GameObject* g, Transform* t) :Component(g, t, typeid(this).name()) {};
	~CharacterParameter() {};
public:
	/*������
	*/
	void ParamInit(int hp, int mp, int atk, int def, int dex, int agi);
	
	int _HP;	//�q�b�g�|�C���g
	int _MP;	//�}�W�b�N�|�C���g
	int _ATK;	//�U����
	int _DEF;	//�h���
	int _DEX;	//������
	int _AGI;	//����
};