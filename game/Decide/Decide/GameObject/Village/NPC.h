#pragma once
#include "GameObject\Village\ContinentObject.h"
#include"GameObject\Component\ObjectRotation.h"

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
		int EventNo = -1;//イベントの番号。
		int MesseageID;	//メッセージのID。
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
		{ "EventNo",Support::DataTypeE::INT, offsetof(struct NPCInfo,EventNo),sizeof(int) },
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

	/**
	* アニメーションコード.
	*/
	enum class AnimationCodeE
	{
		Idol,	//!< 待機.
		Speak,	//!< 話す.
		AnimationCount,
	};

	NPC(const char* name);
	~NPC();
	void Awake()override;

	/**
	* 初期化.
	*/
	void Start()override;

	void Update()override;
	void LateUpdate()override;

	void CreateNPC(const npc::NPCInfo* info);

	void SetMesseage(const int& id, const bool show);

	void SetIsSpeak(bool flag)
	{
		_IsSpeak = flag;
	}

	// 会話終了か取得(中断した場合も含む)。
	bool GetIsSpeakEnd()const;

	// 会話終了か取得(話を最後まで聞き終わった場合のみ)。
	bool GetisSpeakEndLastMessage()const;

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

	//初期回転設定
	void SetRotation(Quaternion q)
	{
		_Rot = q;
	}

	void SetActive(const bool act, const bool children = false)override;

private:

	/**
	* 足元を地面に合わせる.
	*/
	void _FitGround();

protected:
	//話す
	void _Speak();
	//アニメーション再生
	void PlayAnimation(AnimationCodeE code, const float interpolateTime, const int lnum = -1)
	{
		_Anim->PlayAnimation((int)code, interpolateTime, lnum);
	}
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

	//回転
	ObjectRotation* _Rotation = nullptr;
	Quaternion _Rot;

private:
	//アニメーション
	State _State;
	bool _IsAnimation = true;
};