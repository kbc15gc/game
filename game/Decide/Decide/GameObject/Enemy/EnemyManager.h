#pragma once

#include "fbEngine\fbstdafx.h"
#include "GameObject\Component\CharacterParameter.h"
#include "GameObject\Enemy\EnemyCharacter.h"

class SplitSpace;

namespace {

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
		int item[5];
		int armor[5];
		int weapon[5];
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
		{ "param",	Support::DataTypeE::INTARRAY, offsetof(struct EnemyInfo,param),	sizeof(EnemyInfo::param) },
		{ "exp",	Support::DataTypeE::INT, offsetof(struct EnemyInfo,exp),	sizeof(int) },
		{ "money",	Support::DataTypeE::INT, offsetof(struct EnemyInfo,money),	sizeof(int) },
		{ "position",	Support::DataTypeE::VECTOR3, offsetof(struct EnemyInfo,position),	sizeof(Vector3) },
		{ "rotation",	Support::DataTypeE::QUATERNION, offsetof(struct EnemyInfo,rotation),	sizeof(Quaternion) },
		{ "scale",	Support::DataTypeE::VECTOR3, offsetof(struct EnemyInfo,scale),	sizeof(Vector3) },
		{ "item",	Support::DataTypeE::INTARRAY, offsetof(struct EnemyInfo,item),	sizeof(EnemyInfo::item) },
		{ "armor",	Support::DataTypeE::INTARRAY, offsetof(struct EnemyInfo,armor),	sizeof(EnemyInfo::armor) },
		{ "weapon",	Support::DataTypeE::INTARRAY, offsetof(struct EnemyInfo,weapon),	sizeof(EnemyInfo::weapon) },
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
		unique_ptr<EnemyInfo> InfoData;	// CSVファイルから読み込んだ設定データ。
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

	// エネミーの位置データなどを外部ファイルから読み込んで保存。
	void LoadEnemyOrigin();
	
	// 読み込んだエネミーをGameObjectManagerに追加。
	void CreateEnemy();

	// エネミー死亡関数。
	// ※スポナーコンポーネントがあれば自動でリスポーンする。
	void DeathEnemy(EnemyCharacter* object);


private:
	vector<ManagingData*> _enemys;
private:
	static EnemyManager* _instance;
};

