#pragma once
#include "GameObject.h"

class Text;

//FPS�\��
class FPS:public GameObject
{
public:
	FPS(char* name):GameObject(name)
	{
	
	}
	void Start()override;
	void Update()override;
private:
	Text* _Text;
};