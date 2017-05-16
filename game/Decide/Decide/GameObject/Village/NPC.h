#pragma once
#include "GameObject\Village\ContinentObject.h"

namespace
{
	//オブジェクトの情報
	struct NPCInfo : public ObjectInfo
	{
		int MesseageID;	//メッセージのID
		bool ShowTitle;	//タイトルを見せるかどうか？
	};

	//メンバ変数の情報設定
	const Support::DATARECORD NPCInfoData[6] =
	{
		{ "filename",Support::DataTypeE::STRING, offsetof(struct NPCInfo,filename),sizeof(char) * 256 },
		{ "pos",Support::DataTypeE::VECTOR3, offsetof(struct NPCInfo,pos),sizeof(Vector3) },
		{ "ang",Support::DataTypeE::VECTOR3, offsetof(struct NPCInfo,ang),sizeof(Vector3) },
		{ "sca",Support::DataTypeE::VECTOR3, offsetof(struct NPCInfo,sca),sizeof(Vector3) },
		{ "MesseageID",Support::DataTypeE::INT, offsetof(struct NPCInfo,MesseageID),sizeof(int) },
		{ "ShowTitle",Support::DataTypeE::INT, offsetof(struct NPCInfo,ShowTitle),sizeof(bool) },
	};
}

class TextBox;
class Player;

//会話のできるNPC
class NPC :public ContinentObject
{
public:
	NPC(const char* name);
	~NPC();
	void Awake()override;
	void Update()override;
	void SetMesseage(const int& id,const bool show);
	void LateUpdate()override;
private:
	//NPCの身長(モデルのサイズを計算してもいいかもしれない。)
	float _Height;
	//テキストボックス
	TextBox* _TextBox;
	//プレイヤーのアドレス
	Player* _Player;
	//プレイヤーと会話可能な距離
	float _Radius;
};