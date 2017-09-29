#include "stdafx.h"
#include "BuffDebuffICon.h"
#include "fbEngine\_Object\_GameObject\ImageObject.h"
#include "GameObject\Player\Player.h"

//初期化。
void BuffDebuffICon::Awake() {
	
	//矢印アイコン。
	_ArrowIconImage = INSTANCE(GameObjectManager)->AddNew<ImageObject>("ArrowIconImage", 10);
	
	//パラメーターのアイコン。
	_BuffDebuffTypeIconImage = INSTANCE(GameObjectManager)->AddNew<ImageObject>("BuffDebuffTypeIconImage", 10);

	//構造体の初期化。
	_Buffdebuff = new(BuffDebuff);
	_Buffdebuff->_ArrowIconImage = nullptr;
	_Buffdebuff->_BuffDebuffTypeIconImage = nullptr;

	//HpBarのtransform設定。
	_PlayerHpBarTransform = static_cast<Player*>(INSTANCE(GameObjectManager)->FindObject("Player"))->GetPlayerHpBar()->GetTransform();
}

//バフアイコンの生成。
void BuffDebuffICon::BuffIconCreate(Param param) {

	//アイコンに出すパラメーター以外が来たら何もしない。
	if (param<Param::Atk || param>Param::Max) {
		return;
	}

	//バフ矢印のテクスチャを読み込み。
	_ArrowIconImage->SetTexture(LOADTEXTURE("BuffIArrow.png"));
	_ArrowIconImage->SetSize(Vector2(20.0f, 30.0f));
	
	//パラメーターを見てパラメーターに合った画像を読み込み。
	_BuffDebuffTypeIconImage->SetTexture(LOADTEXTURE(TypeIconText[static_cast<int>(param)]));
	_BuffDebuffTypeIconImage->SetSize(Vector2(64.0f, 64.0f));
	_BuffDebuffTypeIconImage->transform->SetParent(_PlayerHpBarTransform);
	_BuffDebuffTypeIconImage->transform->SetLocalPosition(Vector3(0.0f,0.0f, 0.0f));

	_ArrowIconImage->transform->SetParent(_BuffDebuffTypeIconImage->transform);
	_ArrowIconImage->transform->SetLocalPosition(Vector3(30.0f, -30.0f, 0.0f));

	//設定したアイコンの情報をまとめる。
	_Buffdebuff->_ArrowIconImage = _ArrowIconImage;
	_Buffdebuff->_BuffDebuffTypeIconImage = _BuffDebuffTypeIconImage;

	//表示アイコン追加。
	_PlayerBuffDebuffList.push_back(_Buffdebuff);

	_Buffdebuff->_ArrowIconImage = nullptr;
	_Buffdebuff->_BuffDebuffTypeIconImage = nullptr;
}

//デバフアイコンの生成。
void BuffDebuffICon::DebuffIconCreate(Param param) {
	//アイコンに出すパラメーター以外が来たら何もしない。
	if (param<Param::Atk || param>Param::Max) {
		return;
	}

	//デバフ矢印のテクスチャを読み込み。
	_ArrowIconImage->SetTexture(LOADTEXTURE("DebuffArrow.png"));
	_ArrowIconImage->SetSize(Vector2(20.0f, 30.0f));
	//パラメーターを見てパラメーターに合った画像を読み込み。
	_BuffDebuffTypeIconImage->SetTexture(LOADTEXTURE(TypeIconText[static_cast<int>(param)]));
	_BuffDebuffTypeIconImage->SetSize(Vector2(64.0f, 64.0f));
	_BuffDebuffTypeIconImage->transform->SetParent(_PlayerHpBarTransform);
	_BuffDebuffTypeIconImage->transform->SetLocalPosition(Vector3(0.0f,0.0f, 0.0f));

	_ArrowIconImage->transform->SetParent(_BuffDebuffTypeIconImage->transform);
	_ArrowIconImage->transform->SetLocalPosition(Vector3(-30.0f,-30.0f, 0.0f));

	//設定したアイコンの情報をまとめる。
	_Buffdebuff->_ArrowIconImage = _ArrowIconImage;
	_Buffdebuff->_BuffDebuffTypeIconImage = _BuffDebuffTypeIconImage;

	//表示アイコン追加。
	_PlayerBuffDebuffList.push_back(_Buffdebuff);

	_Buffdebuff->_ArrowIconImage = nullptr;
	_Buffdebuff->_BuffDebuffTypeIconImage = nullptr;
}

void BuffDebuffICon::Update() {
	
}