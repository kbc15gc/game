#include "stdafx.h"
#include "BuffDebuffICon.h"
#include "fbEngine\_Object\_GameObject\ImageObject.h"
#include "GameObject\Player\Player.h"

namespace
{
	//バフ、デバフのローカル座標、
	Vector3 BUFF_DEBUFF_POS = { -142.0f,88.0f, 0.0f };
	//バフ、デバフの横に表示する矢印
	Vector3 BUFF_DEBUFF_ARROW_POS = { 25.0f, 15.0f, 0.0f };
	//バフを重ね掛けする際、横にずらすため。
	float OFFSET = 60.0f;
	//バフアイコンのサイズ
	Vector2 BUFF_DEBUFF_ICON_SIZE = { 41,41 };
	//矢印アイコンのサイズ
	Vector2 ARROW_ICON_SIZE = { 13,19 };
}

//デストラクタ。
BuffDebuffICon::~BuffDebuffICon() {
	for (auto BuffDebuff : _PlayerBuffDebuffList)
	{
		delete BuffDebuff;
	}
}

//初期化。
void BuffDebuffICon::Awake() {

}

//バフアイコンの生成。
void BuffDebuffICon::BuffIconCreate(Param param) {

	//アイコンに出すパラメーター以外が来たら何もしない。
	if (param<Param::Atk || param>Param::Max) {
		return;
	}

	//すでに追加されているパラメーターかをチェック。
	if (false == _AddCheck(param)) {
		return;
	}
	//HpBarのtransform設定。
	_PlayerHpBarTransform = static_cast<Player*>(INSTANCE(GameObjectManager)->FindObject("Player"))->GetPlayerHpBar()->GetTransform();

	//構造体の確保。
	BuffDebuff* buffdebuff = new BuffDebuff;

	//矢印アイコン。
	ImageObject* arrowIconImage = INSTANCE(GameObjectManager)->AddNew<ImageObject>("ArrowIconImage", 7);

	//パラメーターのアイコン。
	ImageObject* buffDebuffTypeIconImage = INSTANCE(GameObjectManager)->AddNew<ImageObject>("BuffDebuffTypeIconImage", 7);

	//バフ矢印のテクスチャを読み込み。
	arrowIconImage->SetTexture(LOADTEXTURE(ArrowIconText[static_cast<int>(Arrow::Up)]));
	arrowIconImage->SetSize(ARROW_ICON_SIZE);

	//パラメーターを見てパラメーターに合った画像を読み込み。
	buffDebuffTypeIconImage->SetTexture(LOADTEXTURE(TypeIconText[static_cast<int>(param)]));
	buffDebuffTypeIconImage->SetSize(BUFF_DEBUFF_ICON_SIZE);
	buffDebuffTypeIconImage->transform->SetParent(_PlayerHpBarTransform);
	buffDebuffTypeIconImage->transform->SetLocalPosition(BUFF_DEBUFF_POS);

	arrowIconImage->transform->SetParent(buffDebuffTypeIconImage->transform);
	arrowIconImage->transform->SetLocalPosition(BUFF_DEBUFF_ARROW_POS);

	//設定したアイコンの情報をまとめる。
	buffdebuff->_ArrowIconImage = arrowIconImage;
	buffdebuff->_BuffDebuffTypeIconImage = buffDebuffTypeIconImage;
	buffdebuff->_Param = param;
	buffdebuff->_isBuff = true;

	//表示アイコン追加。
	_PlayerBuffDebuffList.push_back(buffdebuff);

}

//デバフアイコンの生成。
void BuffDebuffICon::DebuffIconCreate(Param param) {

	//アイコンに出すパラメーター以外が来たら何もしない。
	if (param<Param::Atk || param>Param::Max) {
		return;
	}

	//すでに追加されているパラメーターかをチェック。
	if (false == _AddCheck(param)) {
		return;
	}

	//構造体の初期化。
	BuffDebuff* buffdebuff = new(BuffDebuff);

	//矢印アイコン。
	ImageObject* arrowIconImage = INSTANCE(GameObjectManager)->AddNew<ImageObject>("ArrowIconImage", 7);

	//パラメーターのアイコン。
	ImageObject* buffDebuffTypeIconImage = INSTANCE(GameObjectManager)->AddNew<ImageObject>("BuffDebuffTypeIconImage", 7);

	//アイコンに出すパラメーター以外が来たら何もしない。
	if (param<Param::Atk || param>Param::Max) {
		return;
	}

	//デバフ矢印のテクスチャを読み込み。
	arrowIconImage->SetTexture(LOADTEXTURE(ArrowIconText[static_cast<int>(Arrow::Down)]));
	arrowIconImage->SetSize(ARROW_ICON_SIZE);
	//パラメーターを見てパラメーターに合った画像を読み込み。
	buffDebuffTypeIconImage->SetTexture(LOADTEXTURE(TypeIconText[static_cast<int>(param)]));
	buffDebuffTypeIconImage->SetSize(BUFF_DEBUFF_ICON_SIZE);
	buffDebuffTypeIconImage->transform->SetParent(_PlayerHpBarTransform);
	buffDebuffTypeIconImage->transform->SetLocalPosition(BUFF_DEBUFF_POS);

	arrowIconImage->transform->SetParent(buffDebuffTypeIconImage->transform);
	arrowIconImage->transform->SetLocalPosition(BUFF_DEBUFF_ARROW_POS);

	//設定したアイコンの情報をまとめる。
	buffdebuff->_ArrowIconImage = arrowIconImage;
	buffdebuff->_BuffDebuffTypeIconImage = buffDebuffTypeIconImage;
	buffdebuff->_Param = param;
	buffdebuff->_isBuff = false;

	//表示アイコン追加。
	_PlayerBuffDebuffList.push_back(buffdebuff);

}

//更新。
void BuffDebuffICon::Update() {
	//バフデバフリストにつまれている数分。
	for (int i = 0; i < _PlayerBuffDebuffList.size(); i++) {
		//バフを重ね掛けするさい、横にずらす。
		//OFFSETの値で、どれだけ横にずらすか。
		_PlayerBuffDebuffList[i]->_BuffDebuffTypeIconImage->transform->SetLocalPosition(Vector3(BUFF_DEBUFF_POS.x + i*OFFSET, BUFF_DEBUFF_POS.y, BUFF_DEBUFF_POS.z));
	}

	//デバッグ用アイコンの描画切り替え。
#ifdef _DEBUG
	if (KeyBoardInput->isPressed(DIK_P) && KeyBoardInput->isPush(DIK_N))
	{
		RenderDisable();
	}

	if (KeyBoardInput->isPressed(DIK_P) && KeyBoardInput->isPush(DIK_M))
	{
		RenderEnable();
	}
#endif // _DEBUG
}

//追加するパラメーターを追加していいのかをチェック。
bool BuffDebuffICon::_AddCheck(Param param) {
	for (int i = 0; i < _PlayerBuffDebuffList.size(); i++) {
		//同じパラメーターがある。
		if (_PlayerBuffDebuffList[i]->_Param == param) {
			return false;
		}
	}

	//同じパラメーターが無かった
	return true;
}

//アイコンを描画しない。
void BuffDebuffICon::RenderDisable() {

	for (int i = 0; i < _PlayerBuffDebuffList.size(); i++) {
		_PlayerBuffDebuffList[i]->_ArrowIconImage->SetActive(false);
		_PlayerBuffDebuffList[i]->_BuffDebuffTypeIconImage->SetActive(false);
	}
}

//アイコンを描画する。
void BuffDebuffICon::RenderEnable() {
	for (int i = 0; i < _PlayerBuffDebuffList.size(); i++) {
		_PlayerBuffDebuffList[i]->_ArrowIconImage->SetActive(true);
		_PlayerBuffDebuffList[i]->_BuffDebuffTypeIconImage->SetActive(true);
	}
}

//バフアイコンの削除。
void BuffDebuffICon::DeleteBuffIcon(Param param) {

	//アイコンに出すパラメーター以外が来たら何もしない。
	if (param<Param::Atk || param>Param::Max) {
		return;
	}

	//バフアイコンを削除する処理。
	for (auto itr = _PlayerBuffDebuffList.begin(); itr != _PlayerBuffDebuffList.end();) {

		//バフアイコン。
		if ((*itr)->_Param == param && (*itr)->_isBuff == true) {

			//削除。
			INSTANCE(GameObjectManager)->AddRemoveList((*itr)->_ArrowIconImage);
			INSTANCE(GameObjectManager)->AddRemoveList((*itr)->_BuffDebuffTypeIconImage);
			itr = _PlayerBuffDebuffList.erase(itr);
		}
		else
		{
			//バフアイコン以外。
			itr++;
		}
	}
}

//デバフアイコンの削除。
void BuffDebuffICon::DeleteDebuffIcon(Param param) {

	//アイコンに出すパラメーター以外が来たら何もしない。
	if (param<Param::Atk || param>Param::Max) {
		return;
	}

	//デバフアイコンを削除する処理。
	for (auto itr = _PlayerBuffDebuffList.begin(); itr != _PlayerBuffDebuffList.end();) {

		//デバフアイコン。
		if ((*itr)->_Param == param && (*itr)->_isBuff == false) {

			//削除。
			INSTANCE(GameObjectManager)->AddRemoveList((*itr)->_ArrowIconImage);
			INSTANCE(GameObjectManager)->AddRemoveList((*itr)->_BuffDebuffTypeIconImage);
			itr = _PlayerBuffDebuffList.erase(itr);
		}
		else
		{

			//デバフアイコン以外。
			itr++;
		}
	}
}