#pragma once
/**
* ステータス画面クラスの定義.
*/
#pragma once

#include"GameObject\Player\Player.h"
#include "GameObject\Village\EventManager.h"
#include "GameObject\TextImage\AttentionTextOnly.h"
#include"fbEngine\_Object\_GameObject\ImageObject.h"
#include "GameObject\Camera\ThirdPersonCamera.h"

#include "GameObject\History\HistoryInfo.h"

class SplitSpace;

class MapLight :public GameObject{
public:
	MapLight(const char* name) :GameObject(name) {};
	void Awake()override;

	/**
	* 初期化.
	*/
	void Start()override;

	void Update()override;

	Light* GetLight() {
		return _Light;
	}

	void DefaultAmbient() {
		_Light->SetAmbientLight(_defaultAmbient);
	};
private:
	/** ライト. */
	Light* _Light = nullptr;
	Vector3 _defaultAmbient;
};

/**
* ステータス画面クラス.
*/
class WorldMap : public GameObject
{
private:

	struct Location {
		ImageObject* icon = nullptr;
		TextObject* name = nullptr;
	};
public:

	/**
	* コンストラクタ.
	*/
	WorldMap(const char* name) :
		GameObject(name)
	{
	}

	/**
	* デストラクタ.
	*/
	~WorldMap()
	{
	}

	/**
	* 初期化.
	*/
	void Awake()override;

	void Start()override;

	void PreUpdate()override;

	/**
	* 更新.
	*/
	void Update()override;

	/**
	* マップを開く。
	*/
	void Open();

	// マップを閉じる。
	void Close();

	///**
	//* 有効化.
	//*/
	//void OnEnable()override;

	///**
	//* 無効化.
	//*/
	//void OnDisable()override;

private:

	/** プレイヤークラスのポインタ. */
	GameObject* _Player = nullptr;
	ThirdPersonCamera* _camera = nullptr;	// マップ用の俯瞰カメラ。
	MapLight* _mapLight = nullptr;
	bool _isChangeFrame = false;	// ワールドマップの開閉状態が切り替わったフレームか。
	Light* _saveLight = nullptr;
	SplitSpace* _split = nullptr;
	GameObject* _maouSiro = nullptr;

	ImageObject* _playerPoint = nullptr;
	vector<Location> _townPoint = vector<Location>(static_cast<int>(LocationCodeAll::DevilKingdom));
};