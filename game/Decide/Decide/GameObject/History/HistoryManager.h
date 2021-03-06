/**
* 歴史を管理するクラスの定義.
*/
#pragma once

#include "HistoryInfo.h"
#include "HistoryMenu\HistoryMenu.h"
#include "HistoryBook\HistoryBook.h"
#include "GameObject\Village\NPC.h"
#include "Effect\MysteryLight.h"
#include "GameObject\Enemy\EnemyManager.h"

/** 各場所の歴史チップの状況. */
struct LocationHistoryInfo;
class Player;

/**
* 歴史を管理するクラス.
*/
class HistoryManager
{
private:
	enum class LoadObjectType{Object = 0,NPC,Enemy,Max};
private:

	/**
	* コンストラクタ.
	*/
	HistoryManager();

	/**
	* デストラクタ.
	*/
	~HistoryManager()
	{
	}

public:

	/**
	* インスタンスの取得.
	*/
	static HistoryManager* Instance()
	{
		if (_Instance == nullptr)
		{
			_Instance = new HistoryManager();
		}
		return _Instance;
	}

	/**
	* 初期化.
	*/
	void Start();

	/**
	* 進化.
	*/
	void Evolution();

	/**
	* 歴史を変える.
	* 
	* @param location	場所ID.
	* @param slot		スロット番号.
	* @param chip		チップID.
	*/
	bool SetHistoryChip(LocationCodeE location, ChipID chip,int index);

	/**
	* 歴史情報を取得.
	*
	* @param location	場所ID.
	*/
	/*LocationHistoryInfo* GetHistory(LocationCodeE location)
	{
		return _LocationHistoryList.at((int)location).get();
	}*/

	/**
	* 新しいチップIDを追加.
	*/
	void AddPossessionChip(ChipID chipID)
	{
		_HistoryMenu->AddChip(chipID);
	}

	//歴史書から指定されたページを削除
	void PutOutPage(LocationCodeE location, vector<HistoryPage*>& list);

	//NPCへのポインタを管理しているベクターを取得。
	vector<vector<NPC*>> GetNPCList()
	{
		return _NPCList;
	}

	/**
	* 引数のチップが差し込まれているか判定.
	* 菅さんこれこれ、これだよ、これ。
	* trueが返ってきたらどちらかに存在してるよ.
	*/
	//HistoryBookが作成された後に使ってください。
	bool IsSetChip(ChipID id)
	{
		if (_HistoryBook->IsSetChip(id) || _HistoryMenu->IsSetChip(id))
		{
			return true;
		}
		return false;
	}

	/**
	* 建物を作成.
	*
	* @param path		フォルダパス.
	* @param Builds		作成した建物を格納する配列.
	*/
	vector<GameObject*>& CreateBuilding(const char* path, vector<GameObject*>& Builds);

	void SetNowLocation(int loc)
	{
		_NowLocationCode = loc;
	}

	/**
	* 現在のロケーションを取得.
	*/
	int GetNowLocation()
	{
		return _NowLocationCode;
	}

	void ChangeLocation(LocationCodeE value);

private:
	
	/**
	* 場所の変化.
	*
	* @param location	場所ID.
	*/
	void _ChangeLocation(LocationCodeE location);

	/**
	* パターン計算.
	*
	* @param info	場所のアドレス.
	*/
	int _CalcPattern(const LocationHistoryInfo* info);

	/**
	* オブジェクトを作成.
	*
	* @param location	場所ID.
	* @param path		フォルダパス.
	* @param type		生成するオブジェクトのタイプ.
	*/
	void _CreateObject(LocationCodeE location, const char* path, HistoryManager::LoadObjectType type);

	/**
	* NPCを作成.
	*
	* @param location	場所ID.
	* @param path		フォルダパス.
	*/
	void _CreateNPC(LocationCodeE location, const char* path);

	/**
	* エネミーを作成.
	*
	* @param location	場所ID.
	* @param path		フォルダパス.
	*/
	void _CreateEnemy(LocationCodeE location, const char * path);

	/**
	* コリジョンを作成.
	*
	* @param location	場所ID.
	* @param path		フォルダパス.
	*/
	void _CreateCollision(int location, const char* path);

private:

	/** インスタンス. */
	static HistoryManager* _Instance;
	
	/** 全場所の歴史の情報リスト. */
	vector<unique_ptr<LocationHistoryInfo>> _LocationHistoryList;

	/** 全場所の歴史によって出されたオブジェクトへのポインタリスト. */
	vector<vector<GameObject*>> _GameObjectList;
	//NPCへのポインタをまとめたもの。
	vector<vector<NPC*>> _NPCList;
	/** ヒストリ―メニュークラスのポインタ. */
	HistoryMenu* _HistoryMenu = nullptr;

	/** 歴史書クラスのポインタ. */
	HistoryBook* _HistoryBook = nullptr;

	MysteryLight* _MysteryLight = nullptr;
	vector<int> _NowGroupIDList;

	Player* _Player = nullptr;

	/** プレイヤーカメラ. */
	PlayerCamera* _PlayerCamera = nullptr;

	int _NowLocationCode = -1;

	/** 進化フラグ. */
	bool _IsEvolution = false;
	/** 進化場所. */
	LocationCodeE _EvolutionLocation = LocationCodeE::None;

};