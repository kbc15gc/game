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
	//���x���A�b�v���Ăяo���B
	void Init();
private:
	//�`�悳��鎞�ԁB
	float _LifeTime;
	//���݂̎���
	float _Time;
	//���x���A�b�vImage
	ImageObject* _LevelUpImage = nullptr;
	//���W
	Vector3 _Pos;
	//�傫��
	Vector3 _Scale;
};