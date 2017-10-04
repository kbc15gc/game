#include "stdafx.h"
#include "BuffDebuffICon.h"
#include "fbEngine\_Object\_GameObject\ImageObject.h"
#include "GameObject\Enemy\EnemyCharacter.h"
#include "GameObject\ItemManager\ItemManager.h"
#include "GameObject\Player\Player.h"

namespace
{
	//バフアイコンのサイズ
	Vector2 BUFF_DEBUFF_ICON_SIZE = { 41,41 };
	//矢印アイコンのサイズ
	Vector2 ARROW_ICON_SIZE = { 13,19 };
}

//デストラクタ。
BuffDebuffICon::~BuffDebuffICon() {
	for (auto BuffDebuff : _BuffDebuffList)
	{
		delete BuffDebuff;
	}
}

//初期化。
void BuffDebuffICon::Awake() {

}

//バフアイコンの生成。
void BuffDebuffICon::BuffIconCreate(CharacterParameter::Param param) {

	//アイコンに出すパラメーター以外が来たら何もしない。
	if (param == CharacterParameter::Param::HP ||
		param == CharacterParameter::Param::MP ||
		param == CharacterParameter::Param::CRT ||
		param == CharacterParameter::Param::LV) {
		return;
	}

	//すでに追加されているパラメーターかをチェック。
	if (false == _AddCheck(param,true)) {
		return;
	}


	//構造体の確保。
	BuffDebuff* buffdebuff = new BuffDebuff;

	//矢印アイコン。
	ImageObject* arrowIconImage = INSTANCE(GameObjectManager)->AddNew<ImageObject>("ArrowIconImage", 8);

	//パラメーターのアイコン。
	ImageObject* buffDebuffTypeIconImage = INSTANCE(GameObjectManager)->AddNew<ImageObject>("BuffDebuffTypeIconImage", 7);

	switch (_UseIconType)
	{
	case BuffDebuffICon::UseIconType::Player:
		SetPlayerOffset();
		//バフ矢印のテクスチャを読み込み。
		arrowIconImage->SetTexture(LOADTEXTURE(ArrowIconText[static_cast<int>(Arrow::Up)]));
		arrowIconImage->SetSize(ARROW_ICON_SIZE);

		//パラメーターを見てパラメーターに合った画像を読み込み。
		buffDebuffTypeIconImage->SetTexture(LOADTEXTURE(TypeIconText[static_cast<int>(param)]));
		buffDebuffTypeIconImage->SetSize(BUFF_DEBUFF_ICON_SIZE);

		buffDebuffTypeIconImage->transform->SetParent(_HpBarTransform);
		buffDebuffTypeIconImage->transform->SetPosition(_IconPosOffset);

		arrowIconImage->transform->SetParent(buffDebuffTypeIconImage->transform);
		arrowIconImage->transform->SetLocalPosition(_ArrowPosOffet);
		break;
	case BuffDebuffICon::UseIconType::Enemy:
		SetEnemyOffset();
		//バフ矢印のテクスチャを読み込み。
		arrowIconImage->SetTexture(LOADTEXTURE(ArrowIconText[static_cast<int>(Arrow::Up)]));
		arrowIconImage->SetSize(ARROW_ICON_SIZE);
		//パラメーターを見てパラメーターに合った画像を読み込み。
		buffDebuffTypeIconImage->SetTexture(LOADTEXTURE(TypeIconText[static_cast<int>(param)]));
		buffDebuffTypeIconImage->SetSize(BUFF_DEBUFF_ICON_SIZE);

		//エネミー専用処理。
		{
			buffDebuffTypeIconImage->transform->SetParent(_HpBarTransform);
			_ScreenPos = INSTANCE(GameObjectManager)->mainCamera->WorldToScreen(_HpBarTransform->GetPosition());
			buffDebuffTypeIconImage->transform->SetPosition((Vector3(_ScreenPos, 0.0f)));
		}

		arrowIconImage->transform->SetParent(buffDebuffTypeIconImage->transform);
		arrowIconImage->transform->SetLocalPosition(_ArrowPosOffet);

		break;
	default:
		break;
	}

	//設定したアイコンの情報をまとめる。
	buffdebuff->_ArrowIconImage = arrowIconImage;
	buffdebuff->_BuffDebuffTypeIconImage = buffDebuffTypeIconImage;
	buffdebuff->_Param = param;
	buffdebuff->_isBuff = true;

	//表示アイコン追加。
	_BuffDebuffList.push_back(buffdebuff);
}

//デバフアイコンの生成。
void BuffDebuffICon::DebuffIconCreate(CharacterParameter::Param param) {

	//アイコンに出すパラメーター以外が来たら何もしない。
	if (param == CharacterParameter::Param::HP ||
		param == CharacterParameter::Param::MP ||
		param == CharacterParameter::Param::CRT||
		param == CharacterParameter::Param::LV) {
		return;
	}

	//すでに追加されているパラメーターかをチェック。
	if (false == _AddCheck(param,false)) {
		return;
	}
	//構造体の初期化。
	BuffDebuff* buffdebuff = new(BuffDebuff);

	//矢印アイコン。
	ImageObject* arrowIconImage = INSTANCE(GameObjectManager)->AddNew<ImageObject>("ArrowIconImage", 8);

	//パラメーターのアイコン。
	ImageObject* buffDebuffTypeIconImage = INSTANCE(GameObjectManager)->AddNew<ImageObject>("BuffDebuffTypeIconImage", 7);

	switch (_UseIconType)
	{
	case BuffDebuffICon::UseIconType::Player:
		SetPlayerOffset();
		//バフ矢印のテクスチャを読み込み。
		arrowIconImage->SetTexture(LOADTEXTURE(ArrowIconText[static_cast<int>(Arrow::Down)]));
		arrowIconImage->SetSize(ARROW_ICON_SIZE);

		//パラメーターを見てパラメーターに合った画像を読み込み。
		buffDebuffTypeIconImage->SetTexture(LOADTEXTURE(TypeIconText[static_cast<int>(param)]));
		buffDebuffTypeIconImage->SetSize(BUFF_DEBUFF_ICON_SIZE);

		buffDebuffTypeIconImage->transform->SetParent(_HpBarTransform);
		buffDebuffTypeIconImage->transform->SetPosition(_IconPosOffset);

		arrowIconImage->transform->SetParent(buffDebuffTypeIconImage->transform);
		arrowIconImage->transform->SetLocalPosition(_ArrowPosOffet);
		break;

	case BuffDebuffICon::UseIconType::Enemy:

		SetEnemyOffset();
		//デバフ矢印のテクスチャを読み込み。
		arrowIconImage->SetTexture(LOADTEXTURE(ArrowIconText[static_cast<int>(Arrow::Down)]));
		arrowIconImage->SetSize(ARROW_ICON_SIZE);
		//パラメーターを見てパラメーターに合った画像を読み込み。
		buffDebuffTypeIconImage->SetTexture(LOADTEXTURE(TypeIconText[static_cast<int>(param)]));
		buffDebuffTypeIconImage->SetSize(BUFF_DEBUFF_ICON_SIZE);

		//エネミー専用処理。
		{
			buffDebuffTypeIconImage->transform->SetParent(_HpBarTransform);
			_ScreenPos = INSTANCE(GameObjectManager)->mainCamera->WorldToScreen(_HpBarTransform->GetPosition());
			buffDebuffTypeIconImage->transform->SetPosition((Vector3(_ScreenPos, 0.0f)));
		}
	
		arrowIconImage->transform->SetParent(buffDebuffTypeIconImage->transform);
		arrowIconImage->transform->SetLocalPosition(_ArrowPosOffet);
	default:
		break;
	}


	//設定したアイコンの情報をまとめる。
	buffdebuff->_ArrowIconImage = arrowIconImage;
	buffdebuff->_BuffDebuffTypeIconImage = buffDebuffTypeIconImage;
	buffdebuff->_Param = param;
	buffdebuff->_isBuff = false;

	//表示アイコン追加。
	_BuffDebuffList.push_back(buffdebuff);

}

//更新。
void BuffDebuffICon::Update() {
	//バフデバフリストにつまれている分。
	for (int i = 0; i < _BuffDebuffList.size(); i++) {
		//バフを重ね掛けするさい、横にずらす。
		//OFFSETの値で、どれだけ横にずらすか。
		Vector3 iconPos = { 0.0f,0.0f,0.0f };
		switch (_UseIconType)
		{
		case BuffDebuffICon::UseIconType::Player:
			_BuffDebuffList[i]->_BuffDebuffTypeIconImage->transform->SetLocalPosition(_IconPosOffset.x + i*_IconOffSet, _IconPosOffset.y, 0.0f);
			break;
		case BuffDebuffICon::UseIconType::Enemy:
			_ScreenPos = INSTANCE(GameObjectManager)->mainCamera->WorldToScreen(_HpBarTransform->GetPosition());
			_BuffDebuffList[i]->_BuffDebuffTypeIconImage->transform->SetPosition((_ScreenPos.x + _IconPosOffset.x) + i* _IconOffSet, _ScreenPos.y + _IconPosOffset.y, 0.0f);
			_BuffDebuffList[i]->_ArrowIconImage->transform->SetParent(_BuffDebuffList[i]->_BuffDebuffTypeIconImage->transform);
			
			iconPos = _BuffDebuffList[i]->_BuffDebuffTypeIconImage->transform->GetPosition();
			iconPos.x += _ArrowPosOffet.x;
			iconPos.y += _ArrowPosOffet.y;
			_BuffDebuffList[i]->_ArrowIconImage->transform->SetPosition(iconPos);
			break;
		default:
			break;
		}
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
bool BuffDebuffICon::_AddCheck(CharacterParameter::Param param,bool isBuff) {
	for (int i = 0; i < _BuffDebuffList.size(); i++) {
		//同じパラメーターがある。
		if (_BuffDebuffList[i]->_Param == param&&_BuffDebuffList[i]->_isBuff==isBuff) {
			return false;
		}
	}

	//同じパラメーターが無かった
	return true;
}

//アイコンを描画しない。
void BuffDebuffICon::RenderDisable() {

	for (int i = 0; i < _BuffDebuffList.size(); i++) {
		_BuffDebuffList[i]->_ArrowIconImage->SetActive(false);
		_BuffDebuffList[i]->_BuffDebuffTypeIconImage->SetActive(false);
	}
}

//アイコンを描画する。
void BuffDebuffICon::RenderEnable() {
	for (int i = 0; i < _BuffDebuffList.size(); i++) {
		_BuffDebuffList[i]->_ArrowIconImage->SetActive(true);
		_BuffDebuffList[i]->_BuffDebuffTypeIconImage->SetActive(true);
	}
}

//バフアイコンの削除。
void BuffDebuffICon::DeleteBuffIcon(CharacterParameter::Param param) {

	//アイコンに出すパラメーター以外が来たら何もしない。
	if (param == CharacterParameter::Param::HP ||
		param == CharacterParameter::Param::MP ||
		param == CharacterParameter::Param::CRT||
		param == CharacterParameter::Param::LV) {
		return;
	}

	//バフアイコンを削除する処理。
	for (auto itr = _BuffDebuffList.begin(); itr != _BuffDebuffList.end();) {

		//バフアイコン。
		if ((*itr)->_Param == param && (*itr)->_isBuff == true) {

			//削除。
			INSTANCE(GameObjectManager)->AddRemoveList((*itr)->_ArrowIconImage);
			INSTANCE(GameObjectManager)->AddRemoveList((*itr)->_BuffDebuffTypeIconImage);
			itr = _BuffDebuffList.erase(itr);
		}
		else
		{
			//バフアイコン以外。
			itr++;
		}
	}
}

//デバフアイコンの削除。
void BuffDebuffICon::DeleteDebuffIcon(CharacterParameter::Param param) {

	//アイコンに出すパラメーター以外が来たら何もしない。
	if (param == CharacterParameter::Param::HP ||
		param == CharacterParameter::Param::MP ||
		param == CharacterParameter::Param::CRT ||
		param == CharacterParameter::Param::LV) {
		return;
	}

	//デバフアイコンを削除する処理。
	for (auto itr = _BuffDebuffList.begin(); itr != _BuffDebuffList.end();) {

		//デバフアイコン。
		if ((*itr)->_Param == param && (*itr)->_isBuff == false) {

			//削除。
			INSTANCE(GameObjectManager)->AddRemoveList((*itr)->_ArrowIconImage);
			INSTANCE(GameObjectManager)->AddRemoveList((*itr)->_BuffDebuffTypeIconImage);
			itr = _BuffDebuffList.erase(itr);
		}
		else
		{

			//デバフアイコン以外。
			itr++;
		}
	}
}

void BuffDebuffICon::DeleteAllBuffDebuffIcon() {
	//デバフアイコンを削除する処理。
	for (auto itr = _BuffDebuffList.begin(); itr != _BuffDebuffList.end();itr++) {

		//削除。
		INSTANCE(GameObjectManager)->AddRemoveList((*itr)->_ArrowIconImage);
		INSTANCE(GameObjectManager)->AddRemoveList((*itr)->_BuffDebuffTypeIconImage);
		itr = _BuffDebuffList.erase(itr);
	}
}

#ifdef _DEBUG

void BuffDebuffICon::Debug() 
{
	if (KeyBoardInput->isPush(DIK_I)) {
		vector<GameObject*> list;
		INSTANCE(GameObjectManager)->FindObjects("EnemyProt", list);
		for (auto enemy : list) {
			for (int idx = 0; idx < CharacterParameter::Param::MAX; idx++)
			{
				Item::ItemInfo testInfo;
				testInfo.time = 999.0f;
				testInfo.effectValue[idx] = 100;
				static_cast<EnemyCharacter*>(enemy)->ItemEffect(&testInfo);
			}
			for (int idx = 0; idx < CharacterParameter::Param::MAX; idx++)
			{
				Item::ItemInfo testInfo;
				testInfo.time = 999.0f;
				testInfo.effectValue[idx] = -100;
				static_cast<EnemyCharacter*>(enemy)->ItemEffect(&testInfo);
			}
		}
		//static_cast<EnemyCharacter*>(INSTANCE(GameObjectManager)->FindObject("EnemyProt"))->ItemEffect(static_cast<Item::ItemInfo*>(INSTANCE(ItemManager)->GetItemInfo(8, Item::ItemCodeE::Item)));
		Player* p = static_cast<Player*>(INSTANCE(GameObjectManager)->FindObject("Player"));
		Item::ItemInfo testInfo;
		testInfo.time = 999.0f;

		for (int idx = 0; idx < CharacterParameter::Param::MAX; idx++)
		{
			testInfo.effectValue[idx] = 100;
		}
		p->ItemEffect(&testInfo);

		for (int idx = 0; idx < CharacterParameter::Param::MAX; idx++)
		{
			testInfo.effectValue[idx] = -100;
		}
		p->ItemEffect(&testInfo);

	}
}


#endif // DEBUG
