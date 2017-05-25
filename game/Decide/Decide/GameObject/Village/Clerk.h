#pragma once
#include "GameObject\Village\NPC.h"

class Shop;

class Clerk:public NPC
{
public:
	Clerk(const char* name);
	void LoadShop(int shopid);
private:
	//Ç®ìXÅB
	Shop* _Shop;
};