#pragma once
#include "GameObject\Village\ContinentObject.h"

namespace npc
{
	//NPCのタイプ
	enum NPCTypeE : int
	{
		VILLAGER,   //普通の村人
		SHOP,       //店員
	};
	//オブジェクトの情報
	struct NPCInfo : public ObjectInfo
	{
	public:
		NPCTypeE NPCType = NPCTypeE::VILLAGER;
		//店のID
		int ShopID = -1;
		int MesseageID;	//メッセージのID
		bool ShowTitle;	//タイトルを見せるかどうか？
	};

	//メンバ変数の情報設定
	const Support::DATARECORD NPCInfoData[] =
	{
		{ "filename",Support::DataTypeE::STRING, offsetof(struct NPCInfo,filename),sizeof(char) * 256 },
		{ "pos",Support::DataTypeE::VECTOR3, offsetof(struct NPCInfo,pos),sizeof(Vector3) },
		{ "ang",Support::DataTypeE::QUATERNION, offsetof(struct NPCInfo,ang),sizeof(Quaternion) },
		{ "sca",Support::DataTypeE::VECTOR3, offsetof(struct NPCInfo,sca),sizeof(Vector3) },
		{ "NPCType",Support::DataTypeE::INT, offsetof(struct NPCInfo,NPCType),sizeof(NPCTypeE) },
		{ "ShopID",Support::DataTypeE::INT, offsetof(struct NPCInfo,ShopID),sizeof(int) },
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

	enum class State
	{
		None = -1,
		Idol,
		Speak,
		Num,
	};

	NPC(const char* name);
	~NPC();
	void Awake()override;
	void Update()override;
	void LateUpdate()override;

	void CreateNPC(const npc::NPCInfo* info);

	void SetMesseage(const int& id, const bool show);

	void SetIsSpeak(bool flag)
	{
		_IsSpeak = flag;
	}

	// 会話終了か取得。
	bool GetIsSpeakEnd()const;

	float GetRadius()
	{
		return _Radius;
	}

	//吹き出しの高さを設定
	void SetHeight(float height)
	{
		_Height = height;
	}

	//アニメーションを流すか。
	void SetAnimation(bool flag)
	{
		_IsAnimation = flag;
	}

protected:
	//話す
	void _Speak();
protected:
	//NPCの身長(モデルのサイズを計算してもいいかもしれない。)
	float _Height;
	//テキストボックス
	TextBox* _TextBox;
	//プレイヤーのアドレス
	Player* _Player;
	//プレイヤーと会話可能な距離
	float _Radius;
	//タイトル表示するかどうか？
	bool _ShowTitle;

	//Playerと話しができるか
	bool _IsSpeak;
private:
	State _State;
	bool _IsAnimation = true;

};