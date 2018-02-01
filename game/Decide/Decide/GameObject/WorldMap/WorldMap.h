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

// ワールドマップの開放状況をセーブするための構造体。
struct WorldMapSaveData {
	WorldMapSaveData() {
	}
	WorldMapSaveData(int num) {
		openLocation = num;
	}
	int openLocation = -1;
};

//WorldMapSaveData{構造体の構成フォーマット(メンバ変数)。
// ※1つのレコードは以下の要素で構成される。
// 要素：	変数名。
//			データ型。
//			データの先頭から見た各要素の位置(バイト)。
//			要素一つ分のサイズ(バイト)。
const Support::DATARECORD WorldMapSaveDataDecl[] =
{
	{ "openLocation",Support::DataTypeE::INT, offsetof(struct WorldMapSaveData,openLocation), sizeof(WorldMapSaveData::openLocation) }
};

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

	// 町の名前判明処理。
	void OpenTownName(LocationCodeAll code) {
		if (code < LocationCodeAll::DevilKingdom) {
			for (auto& data : _saveData) {
				if (data->openLocation == static_cast<int>(code)) {
					// すでに開放済み。
					return;
				}
			}

			char text[256];
			sprintf(text, "[%s]", AllLocationNameList[static_cast<int>(code)].c_str());
			_townPoint[static_cast<int>(code)].name->SetText(text);
			//新しく解放された街なのでデータを追加して保存。
			_saveData.push_back(unique_ptr<WorldMapSaveData>(new WorldMapSaveData(static_cast<int>(code))));
			Support::OutputCSV<WorldMapSaveData>(filePath, WorldMapSaveDataDecl, ARRAY_SIZE(WorldMapSaveDataDecl), _saveData);
		}
	}
private:

	/** プレイヤークラスのポインタ. */
	GameObject* _Player = nullptr;
	ThirdPersonCamera* _camera = nullptr;	// マップ用の俯瞰カメラ。
	MapLight* _mapLight = nullptr;
	bool _isChangeFrame = false;	// ワールドマップの開閉状態が切り替わったフレームか。
	Light* _saveLight = nullptr;
	SplitSpace* _split = nullptr;
	GameObject* _maouSiro = nullptr;

	FogInfo _saveFogInfo;

	ImageObject* _playerPoint = nullptr;
	vector<Location> _townPoint = vector<Location>(static_cast<int>(LocationCodeAll::DevilKingdom));

	vector<unique_ptr<WorldMapSaveData>> _saveData;
	char filePath[256] = "C:\\KBCGame\\ReHistory\\SaveData\\WorldMapOpenLocation.csv";	// セーブ先。

																						//効果音再生。
	SoundSource* _openSe = nullptr;
	SoundSource* _closeSe = nullptr;
};

