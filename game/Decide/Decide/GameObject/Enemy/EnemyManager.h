#pragma once

#include "fbEngine\fbstdafx.h"
#include "GameObject\Component\CharacterParameter.h"
#include "GameObject\Enemy\EnemyCharacter.h"

class SplitSpace;

namespace LoadEnemyInfo{

	const int dropMax = 5;
	const int ColorNum = 4;

	// CSVから読み込むエネミーのデータ形式。
	struct EnemyInfo : Noncopyable {
		int type;		// 種別。
		//char fileName[FILENAME_MAX];	// モデルデータのファイル名。
		int param[CharacterParameter::Param::MAX];	// 各種パラメータ。
		int exp;	// 落とす経験値。
		int money;	// 落とす金額。
		Vector3 position;	// 位置。
		Quaternion rotation;	// 回転。
		Vector3 scale;	// 拡縮。
		int item[dropMax];
		int armor[dropMax];
		int weapon[dropMax];
		float probability[dropMax * 3];	//ドロップ確率*3つ分。
		int colorflag;
		Vector4 color;
		float viewAngle;
		float viewRange;
		float wanderingRange;
		float discoveryRange;
		float respawnTime;
	};

	//EnemyInfo構造体の構成フォーマット(メンバ変数)。
	// ※1つのレコードは以下の要素で構成される。
	// 要素：	変数名。
	//			データ型。
	//			データの先頭から見た各要素の位置(バイト)。
	//			要素一つ分のサイズ(バイト)。
	const Support::DATARECORD EnemyInfoDecl[] =
	{
		{ "type",Support::DataTypeE::INT, offsetof(struct EnemyInfo,type),	sizeof(EnemyCharacter::EnemyType) },
		{ "param",	Support::DataTypeE::INT_ARRAY, offsetof(struct EnemyInfo,param),	sizeof(EnemyInfo::param) },
		{ "exp",	Support::DataTypeE::INT, offsetof(struct EnemyInfo,exp),	sizeof(int) },
		{ "money",	Support::DataTypeE::INT, offsetof(struct EnemyInfo,money),	sizeof(int) },
		{ "position",	Support::DataTypeE::VECTOR3, offsetof(struct EnemyInfo,position),	sizeof(Vector3) },
		{ "rotation",	Support::DataTypeE::QUATERNION, offsetof(struct EnemyInfo,rotation),	sizeof(Quaternion) },
		{ "scale",	Support::DataTypeE::VECTOR3, offsetof(struct EnemyInfo,scale),	sizeof(Vector3) },
		{ "item",	Support::DataTypeE::INT_ARRAY, offsetof(struct EnemyInfo,item),	sizeof(EnemyInfo::item) },
		{ "armor",	Support::DataTypeE::INT_ARRAY, offsetof(struct EnemyInfo,armor),	sizeof(EnemyInfo::armor) },
		{ "weapon",	Support::DataTypeE::INT_ARRAY, offsetof(struct EnemyInfo,weapon),	sizeof(EnemyInfo::weapon) },
		{ "probability",	Support::DataTypeE::FLOAT_ARRAY, offsetof(struct EnemyInfo,probability),	sizeof(EnemyInfo::probability) },
		{ "colorflag",Support::DataTypeE::INT, offsetof(struct EnemyInfo,colorflag),	sizeof(EnemyInfo::colorflag) },
		{ "color",	Support::DataTypeE::VECTOR4, offsetof(struct EnemyInfo,color),	sizeof(Vector4) },
		{ "viewAngle",	Support::DataTypeE::FLOAT, offsetof(struct EnemyInfo,viewAngle),	sizeof(EnemyInfo::viewAngle) },
		{ "viewRange",	Support::DataTypeE::FLOAT, offsetof(struct EnemyInfo,viewRange),	sizeof(EnemyInfo::viewRange) },
		{ "wanderingRange",	Support::DataTypeE::FLOAT, offsetof(struct EnemyInfo,wanderingRange),	sizeof(EnemyInfo::wanderingRange) },
		{ "discoveryRange",Support::DataTypeE::FLOAT, offsetof(struct EnemyInfo,discoveryRange),	sizeof(EnemyInfo::discoveryRange) },
		{ "respawnTime",	Support::DataTypeE::FLOAT, offsetof(struct EnemyInfo,respawnTime),	sizeof(EnemyInfo::respawnTime) },

	};
}


class EnemyManager {
private:
	// オブジェクト管理用構造体。
	struct ManagingData {
		//ManagingData(EnemyCharacter* obj,  unique_ptr<EnemyInfo> info) {
		//	Object = obj;
		//	InfoData = move(info);	// ユニークポインタの所有権を譲渡。
		//}
		EnemyCharacter* Object = nullptr;	// InfoDataをもとに生成されたオブジェクトデータ。
		unique_ptr<LoadEnemyInfo::EnemyInfo> InfoData;	// CSVファイルから読み込んだ設定データ。
		Spawner* Respawner = nullptr;	// エネミーを復活させるためのスポナー。
	};
private:
	EnemyManager();
	~EnemyManager();
public:
	
	// インスタンスの取得.
	static EnemyManager* Instance()
	{
		if (_instance == nullptr)
		{
			_instance = new EnemyManager();
		}
		return _instance;
	}

	// 初期化.
	void Start();
	
	// テーブルに登録されている情報をもとにエネミーを作成。
	// 引数：	どの場所のエネミーを作成するか。
	//			エネミー情報の配列。
	void CreateEnemys(LocationCodeE location,vector<unique_ptr<LoadEnemyInfo::EnemyInfo>>& infos);

	// エネミー死亡関数。
	// ※スポナーコンポーネントがあれば自動でリスポーンする。
	void DeathEnemy(EnemyCharacter* object);


private:
	vector<vector<unique_ptr<ManagingData>>> _enemys;	// エネミー。
private:
	static EnemyManager* _instance;
};

