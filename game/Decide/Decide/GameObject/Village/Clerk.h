#pragma once
#include "GameObject\Village\NPC.h"

class Shop;

class Clerk:public NPC
{
public:
	Clerk(const char* name);
	void LoadShop(int shopid);
private:
	//���X�B
	Shop* _Shop;
};