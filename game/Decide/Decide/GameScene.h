#pragma once
#include "fbEngine/Scene.h"
class ImageObject;
class TextObject;
class Player;
class SoundSource;

class GameScene : public Scene
{
public:
	GameScene() {};
	void Start()override;
	void Update()override;
private:
};