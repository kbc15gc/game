#include "stdafx.h"
#include "DropItem.h"
#include "fbEngine\_Object\_Component\_3D\SkinModel.h"
#include "GameObject\Player\Player.h"
#include "fbEngine\_Object\_GameObject\ImageObject.h"
#include "GameObject\Inventory\Inventory.h"
#include "GameObject\ItemManager\HoldItem\HoldItemFactory.h"
#include "GameObject\Component\ParticleEffect.h"
#include "GameObject\TextImage\AttentionTextOnly.h"
#include "GameObject\StatusWindow\StatusWindow.h"

//コンストラクタ。
DropItem::DropItem(const char * name) :
	GameObject(name)
{

}

//デストラクタ。
DropItem::~DropItem()
{

}

void DropItem::Awake() {

	//優先度設定。
	_ButtonIconPriority = StatusWindow::WindowBackPriorty - 1;

	//レアドロップ用のサウンド。
	_RareDropSE = INSTANCE(GameObjectManager)->AddNew<SoundSource>("RareDropSE", 0);
	_RareDropSE->Init("Asset/Sound/Drop/RareDropSE.wav");
	_RareDropSE->SetVolume(1.0f);

	//ドロップ用のサウンド。
	_DropSE = INSTANCE(GameObjectManager)->AddNew<SoundSource>("DropSE", 0);
	_DropSE->Init("Asset/Sound/Drop/DropSE.wav");
	_DropSE->SetVolume(1.0f);

	//パーティクルエフェクトコンポーネント追加。
	_RareDropPE = AddComponent<ParticleEffect>();

	//モデルコンポーネント追加。
	_Model = AddComponent<SkinModel>();

	//スキンモデル作成。
	SkinModelData* modeldata = new SkinModelData();

	//モデルデータ作成(ファイルパスはテスト用)。
	modeldata->CloneModelData(SkinModelManager::LoadModel("Debug/SphereCollision.X"));

	//モデル設定。
	_Model->SetModelData(modeldata);

	//初期座標。
	_DropPos = Vector3(0.0f, 0.0f, 0.0f);
	transform->SetLocalPosition(_DropPos);

	//プレイヤー取得。
	_Player = static_cast<Player*>(INSTANCE(GameObjectManager)->FindObject("Player"));

	//アイコン画像設定(アイコン画像はテスト用)。
	_ButtonIconImage = INSTANCE(GameObjectManager)->AddNew<ImageObject>("DropButtonIconImage", _ButtonIconPriority);
	_ButtonIconImage->SetTexture(LOADTEXTURE("UI/up.png"));
	_ButtonIconImage->SetActive(false, false);

	//最大時間を設定。
	_AppearMaxTime = 10.0f;

	//初期化。
	_DropEquipment = nullptr;
}

//ドロップアイテムを作成。
void DropItem::Create(Item::BaseInfo* info, const Vector3& pos, int dropNum) {

	//アイテムの場合。
	if (info->TypeID == Item::ItemCodeE::Item) {
		//アイテムの場合は指定された数分落とす。
		_DropNum = dropNum;
	}
	else
	//武具の場合。
	{
		//武具は一つしか落ちない。
		_DropEquipment = static_cast<HoldEquipment*>(HoldItemFactory::CreateItem(static_cast<Item::BaseInfo*>(info), true));
	}

	SetInfo(info);

	//落下場所を設定。
	_DropPos = pos;
	transform->SetLocalPosition(_DropPos);

	//アイコンの親設定。
	_ButtonIconImage->transform->SetParent(this->transform);

	//アイコンの座標を初期化。
	_ButtonIconImage->transform->SetPosition(Vector3::zero);

	//生成した武具のランクをチェックし、ランクに適したSEとエフェクトを選択。
	_EquipmentRankCheck_SelectSEAndEffect();
	
}

//更新。
void DropItem::Update() {

#ifdef _DEBUG
	Debug();
#endif // _DEBUG

	//出現時間に加算。
	_TotalAppearTime += Time::DeltaTime();

	//プレイヤーとの距離を計算。
	Vector3 v = _Player->transform->GetPosition() - _DropPos;
	float len = v.Length();
	Vector2 _ScreenPos = INSTANCE(GameObjectManager)->mainCamera->WorldToScreen(this->transform->GetPosition());
	
	//近くならアイコンを表示。
	if (len < _GetLength) {
		_ButtonIconImage->transform->SetPosition(_ScreenPos.x, _ScreenPos.y - _ButtonIconPosOffSet, 0.0f);
		_ButtonIconImage->SetActive(true, false);

		//範囲内でAボタンを押されたら取得。
		if (VPadInput->IsPush(fbEngine::VPad::ButtonA)) {

			//中身がある時。
			if (_DropItemInfo) {

				//取得したアイテムのアイテムコードを見てインベントリに追加。
				switch (_DropItemInfo->TypeID)
				{
					//アイテム。
				case Item::ItemCodeE::Item:
					//追加。
					if (INSTANCE(Inventory)->AddItem(static_cast<Item::ItemInfo*>(_DropItemInfo), _DropNum) == false) {
						_SelectText(_DropItemInfo->TypeID);
						return;
					}
					break;
					//防具。
				case Item::ItemCodeE::Armor:
					//武器。
				case Item::ItemCodeE::Weapon:
					//追加。
					if (INSTANCE(Inventory)->AddEquipment(_DropEquipment) == false) {
						_SelectText(_DropItemInfo->TypeID);
						return;
					}
					break;
				default:
					break;
				}		
				//オブジェクト関係を削除。
				_Release();
				
			}
			else
			{
				char error[256];
				sprintf(error, "ドロップアイテムのInfoが空でした。");
				MessageBoxA(0, error, "ドロップアイテムの取得失敗", MB_ICONWARNING);
			}

			//オブジェクト関係を削除。
			_Release();
		}
	}
	else
	{
		//範囲外ならアイコン非表示。
		_ButtonIconImage->SetActive(false, false);
	}

	//フィールド上に出てから一定時間経ったら消す。
	if (_AppearMaxTime < _TotalAppearTime) {
		//オブジェクト関係を削除。
		_Release();
	}

}

//生成した武具のランクをチェックし、ランクに適したSEとエフェクトを選択。
void DropItem::_EquipmentRankCheck_SelectSEAndEffect()
{
	if (_DropEquipment->GetRank() <= HoldEquipment::Rank::B)
	{
		//S以上ならレアドロップ用のエフェクトとSE開始。
		_RareDropPE->RareDropEffect();
		_RareDropSE->Play(false);
	}
	else
	{
		//ドロップSE再生。
		_DropSE->Play(false);
	}
}

//このクラスでGameobjectに登録したオブジェクトを全削除用関数。
void DropItem::_Release()
{
	//エフェクトを消す。
	_RareDropPE->SetRareDropEffectFlag(false);

	//ボタンアイコン削除。
	if (_ButtonIconImage) {
		INSTANCE(GameObjectManager)->AddRemoveList(_ButtonIconImage);
	}

	//ドロップ用SEを削除。
	if (_DropSE) {
		INSTANCE(GameObjectManager)->AddRemoveList(_DropSE);
	}

	//レアドロップ用のSE削除。
	if (_RareDropSE) {
		INSTANCE(GameObjectManager)->AddRemoveList(_RareDropSE);
	}

	INSTANCE(GameObjectManager)->AddRemoveList(this);
}
//AttentionTextにアイテムコードを見て適した文字列を設定する。
void DropItem::_SelectText(Item::ItemCodeE code)
{
	wchar_t ErrorText[256];
	//文字列選択。
	switch (code)
	{
	case Item::ItemCodeE::Item:
		wcscpy_s(ErrorText, wcslen(L"インベントリが一杯でアイテムを拾えませんでした。") + 1, L"インベントリが一杯でアイテムを拾えませんでした。");
		break;
	case Item::ItemCodeE::Armor:
		wcscpy_s(ErrorText, wcslen(L"インベントリが一杯で防具を拾えませんでした。") + 1, L"インベントリが一杯で防具を拾えませんでした。");
		break;
	case Item::ItemCodeE::Weapon:
		wcscpy_s(ErrorText, wcslen(L"インベントリが一杯で武器を拾えませんでした。") + 1, L"インベントリが一杯で武器を拾えませんでした。");
		break;
	default:
		break;
	}
	//テキストに設定。
	_SetText(ErrorText);
}

//AttentionTextに文字列を設定。
void DropItem::_SetText(const wchar_t* string) 
{
	//AttentionText作成。
	static_cast<AttentionTextOnly*>(INSTANCE(GameObjectManager)->FindObject("AttentionTextOnly"))->CreateText(
		string,
		Vector3(600.0f, 260.0f, 0.0f),
		33.0f,
		Color::red,
		AttentionTextOnly::MoveType::Up
	);
}

#ifdef _DEBUG
void DropItem::Debug() {
	if (KeyBoardInput->isPush(DIK_L)) {
	
	}
}
#endif // _DEBUG
