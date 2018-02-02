/**
* 血液エフェクトクラスの定義.
*/
#pragma once

#include"fbEngine\_Object\_GameObject\ImageObject.h"

/**
* 血液イメージクラス.
*/
class BloodImage : public GameObject
{
public:

	/**
	* コンストラクタ.
	*/
	BloodImage(char* name) :
		GameObject(name)
	{
	}

	/**
	* デストラクタ.
	*/
	~BloodImage()
	{
	}

	/**
	* 初期化.
	*/
	void Awake()override;

	/**
	* 更新.
	*/
	void LateUpdate()override;

	/**
	* 有効になった.
	*/
	void OnEnable()override;

private:

	Sprite* _Image = nullptr;

	float _LimitTime = 1.0f;
	float _LocalTime = 0.0f;

	float _Alpha = 1;

};

/**
* 血液エフェクトクラス.
*/
class BloodEffect : public GameObject
{
public:

	/**
	* コンストラクタ.
	*/
	BloodEffect(char* name) :
		GameObject(name)
	{
	}

	/**
	* デストラクタ.
	*/
	~BloodEffect()
	{
	}

	/**
	* 初期化.
	*/
	void Awake()override;

	/**
	* ダメージ受けた.
	*/
	void Damage(int maxHp, int damage);

	void OnDisable()override
	{
		AllOff();
	}

	void AllOff()
	{
		for (auto bi : _BloodImageList)
		{
			bi->SetActive(false);
		}
	}

private:

	static const int BIMaxSize = 20;
	/**
	* リスト.
	*/
	BloodImage* _BloodImageList[BIMaxSize] = { nullptr };

};