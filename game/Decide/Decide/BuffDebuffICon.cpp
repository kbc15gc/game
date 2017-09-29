#include "stdafx.h"
#include "BuffDebuffICon.h"
#include "fbEngine\_Object\_GameObject\ImageObject.h"
#include "GameObject\Player\Player.h"

namespace
{
	//バフのローカル座標、
	Vector3 BUFF_POS = { -142.0f,88.0f, 0.0f };
	//バフの横に表示する矢印
	Vector3 BUFF_ARROW_POS = { 25.0f, 15.0f, 0.0f };
	//バフを重ね掛けする際、横にずらすため。
	float OFFSET = 60.0f;
	//バフアイコンのサイズ
	Vector2 BUFF_ICON_SIZE = { 41,41 };
	//矢印アイコンのサイズ
	Vector2 ARROW_ICON_SIZE = { 13,19 };
}

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
	arrowIconImage->SetSize(ARROW_ICON_SIZE);

	//パラメーターを見てパラメーターに合った画像を読み込み。
	buffDebuffTypeIconImage->SetTexture(LOADTEXTURE(TypeIconText[static_cast<int>(param)]));
	buffDebuffTypeIconImage->SetSize(BUFF_ICON_SIZE);
	buffDebuffTypeIconImage->transform->SetParent(_PlayerHpBarTransform);
	buffDebuffTypeIconImage->transform->SetLocalPosition(BUFF_POS);

	arrowIconImage->transform->SetParent(buffDebuffTypeIconImage->transform);
	arrowIconImage->transform->SetLocalPosition(BUFF_ARROW_POS);

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
	arrowIconImage->SetSize(ARROW_ICON_SIZE);
	//パラメーターを見てパラメーターに合った画像を読み込み。
	buffDebuffTypeIconImage->SetTexture(LOADTEXTURE(TypeIconText[static_cast<int>(param)]));
	buffDebuffTypeIconImage->SetSize(BUFF_ICON_SIZE);
	buffDebuffTypeIconImage->transform->SetParent(_PlayerHpBarTransform);
	buffDebuffTypeIconImage->transform->SetLocalPosition(BUFF_POS);

	arrowIconImage->transform->SetParent(buffDebuffTypeIconImage->transform);
	arrowIconImage->transform->SetLocalPosition(BUFF_ARROW_POS);

	//設定したアイコンの情報をまとめる。
	buffdebuff->_ArrowIconImage = arrowIconImage;
	buffdebuff->_BuffDebuffTypeIconImage = buffDebuffTypeIconImage;

	//表示アイコン追加。
	_PlayerBuffDebuffList.push_back(buffdebuff);

}

void BuffDebuffICon::Update() {
	//バフデバフリストにつまれている数分。
	for (int i = 0; i < _PlayerBuffDebuffList.size(); i++) {
		//バフを重ね掛けするさい、横にずらす。
		//OFFSETの値で、どれだけ横にずらすか。
		_PlayerBuffDebuffList[i]->_BuffDebuffTypeIconImage->transform->SetLocalPosition(Vector3(BUFF_POS.x + i*OFFSET, BUFF_POS.y, BUFF_POS.z));
	}
}