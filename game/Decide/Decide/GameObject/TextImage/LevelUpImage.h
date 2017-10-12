#pragma once
#include "fbEngine\_Object\_GameObject\ImageObject.h"

class LevelUpImage : public GameObject
{
public:
	LevelUpImage(const char * name) :GameObject(name)
	{
	}
	~LevelUpImage() {
	}

	void Awake()override;
	void Update()override;
	//レベルアップ時呼び出す。
	void Init();
private:
	//描画される時間。
	float _LifeTime;
	//現在の時間
	float _Time;
	//レベルアップImage
	ImageObject* _LevelUpImage = nullptr;
	//座標
	Vector3 _Pos;
	//大きさ
	Vector3 _Scale;
};