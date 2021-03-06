#pragma once
#include "fbEngine\_Object\_GameObject\GameObject.h"

class TextObject;

//村に入った時に表示される名前。
class VillageName:public GameObject
{
public:
	VillageName(const char* name) :GameObject(name) {};
	void Awake()override;
	void Update()override;

	void Excute(const int location);
private:
	int now = -1;
	double _Timer = 0.0f;
	TextObject* _Text;
};