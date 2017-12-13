/**
* 謎の光クラスの定義.
*/
#pragma once

#include"fbEngine\_Object\_GameObject\Plate.h"
#include"GameObject\Camera\PlayerCamera.h"
#include"../HistoryMenu/HistoryMenu.h"

/**
* 謎の光クラス.
*/
class MysteryLight : public GameObject
{
public:

	/**
	* コンストラクタ.
	*/
	MysteryLight(char* name) :
		GameObject(name)
	{
	}

	/**
	* デストラクタ.
	*/
	~MysteryLight()
	{
	}

	/**
	* 初期化.
	*/
	void Awake()override;

	/**
	* 更新.
	*/
	void Update()override;

	void SetActive(const bool act, const bool child = false)override;

private:

	float _LightTime = 2.5f;
	float _LocalTime = 0.0f;

	/** ライト用画像. */
	ImageObject* _LightPlate = nullptr;

	PlayerCamera* _PlayerCamera = nullptr;

	HistoryMenu* _HistoryMenu = nullptr;

	float _Alpha = 1.0f;

};