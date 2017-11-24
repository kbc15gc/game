#pragma once
#include "fbEngine\_Object\_GameObject\GameObject.h"

class TextObject;

//ë∫Ç…ì¸Ç¡ÇΩéûÇ…ï\é¶Ç≥ÇÍÇÈñºëOÅB
class VillageName:public GameObject
{
public:
	VillageName(const char* name) :GameObject(name) {};
	void Start()override;
	void Update()override;

	void Excute(const int location);
private:
	int now = -1;
	double _Timer = 0.0f;
	TextObject* _Text;
};