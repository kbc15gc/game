#include "stdafx.h"
#include "BuffDebuffICon.h"
#include "fbEngine\_Object\_GameObject\ImageObject.h"
#include "GameObject\Player\Player.h"

//初期化。
void BuffDebuffICon::Awake() {
	//HpBarのtransform設定。
	_PlayerHpBarTransform = static_cast<Player*>(INSTANCE(GameObjectManager)->FindObject("Player"))->GetPlayerHpBar()->GetTransform();
}

//バフアイコンの生成。
void BuffDebuffICon::BuffIconCreate(Param param) {

	//アイコンに出すパラメーター以外が来たら何もしない。
	if (param<Param::Atk || param>Param::Max) {
		return;
	}

	//構造体の確保。
	BuffDebuff* buffdebuff = new BuffDebuff;

	//矢印アイコン。
	ImageObject* arrowIconImage = INSTANCE(GameObjectManager)->AddNew<ImageObject>("ArrowIconImage", 10);

	//パラメーターのアイコン。
	ImageObject* buffDebuffTypeIconImage = INSTANCE(GameObjectManager)->AddNew<ImageObject>("BuffDebuffTypeIconImage", 10);

	//バフ矢印のテクスチャを読み込み。
	arrowIconImage->SetTexture(LOADTEXTURE("BuffIArrow.png"));
	arrowIconImage->SetSize(Vector2(20.0f, 30.0f));
	
	//パラメーターを見てパラメーターに合った画像を読み込み。
	buffDebuffTypeIconImage->SetTexture(LOADTEXTURE(TypeIconText[static_cast<int>(param)]));
	buffDebuffTypeIconImage->SetSize(Vector2(64.0f, 64.0f));
	buffDebuffTypeIconImage->transform->SetParent(_PlayerHpBarTransform);
	buffDebuffTypeIconImage->transform->SetLocalPosition(Vector3(-130.0f,-55.0f, 0.0f));

	arrowIconImage->transform->SetParent(buffDebuffTypeIconImage->transform);
	arrowIconImage->transform->SetLocalPosition(Vector3(30.0f, 15.0f, 0.0f));

	//設定したアイコンの情報をまとめる。
	buffdebuff->_ArrowIconImage = arrowIconImage;
	buffdebuff->_BuffDebuffTypeIconImage = buffDebuffTypeIconImage;

	//表示アイコン追加。
	_PlayerBuffDebuffList.push_back(buffdebuff);
}

//デバフアイコンの生成。
void BuffDebuffICon::DebuffIconCreate(Param param) {
	//構造体の初期化。
	BuffDebuff* buffdebuff = new(BuffDebuff);

	//矢印アイコン。
	ImageObject* arrowIconImage = INSTANCE(GameObjectManager)->AddNew<ImageObject>("ArrowIconImage", 10);

	//パラメーターのアイコン。
	ImageObject* buffDebuffTypeIconImage = INSTANCE(GameObjectManager)->AddNew<ImageObject>("BuffDebuffTypeIconImage", 10);

	//アイコンに出すパラメーター以外が来たら何もしない。
	if (param<Param::Atk || param>Param::Max) {
		return;
	}

	//デバフ矢印のテクスチャを読み込み。
	arrowIconImage->SetTexture(LOADTEXTURE("DebuffArrow.png"));
	arrowIconImage->SetSize(Vector2(20.0f, 30.0f));
	//パラメーターを見てパラメーターに合った画像を読み込み。
	buffDebuffTypeIconImage->SetTexture(LOADTEXTURE(TypeIconText[static_cast<int>(param)]));
	buffDebuffTypeIconImage->SetSize(Vector2(64.0f, 64.0f));
	buffDebuffTypeIconImage->transform->SetParent(_PlayerHpBarTransform);
	buffDebuffTypeIconImage->transform->SetLocalPosition(Vector3(-130.0f, -55.0f, 0.0f));

	arrowIconImage->transform->SetParent(buffDebuffTypeIconImage->transform);
	arrowIconImage->transform->SetLocalPosition(Vector3(30.0f, 15.0f, 0.0f));

	//設定したアイコンの情報をまとめる。
	buffdebuff->_ArrowIconImage = arrowIconImage;
	buffdebuff->_BuffDebuffTypeIconImage = buffDebuffTypeIconImage;

	//表示アイコン追加。
	_PlayerBuffDebuffList.push_back(buffdebuff);

}

void BuffDebuffICon::Update() {

	for (int i = 0; i < _PlayerBuffDebuffList.size(); i++) {
		_PlayerBuffDebuffList[i]->_BuffDebuffTypeIconImage->transform->SetLocalPosition(Vector3(-130.0f+i*70.0f, -55.0f, 0.0f));
	}
}