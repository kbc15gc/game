#include "stdafx.h"
#include "DropItem.h"
#include "fbEngine\_Object\_Component\_3D\SkinModel.h"
#include "GameObject\Player\Player.h"
#include "fbEngine\_Object\_GameObject\ImageObject.h"
#include "GameObject\Inventory\Inventory.h"
#include "GameObject\ItemManager\HoldItem\HoldItemFactory.h"
#include "GameObject\Component\ParticleEffect.h"
#include "GameObject\TextImage\AttentionTextOnly.h"

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

	_Player = static_cast<Player*>(INSTANCE(GameObjectManager)->FindObject("Player"));

	//アイコン画像設定(アイコン画像はテスト用)。
	_ButtonIconImage= INSTANCE(GameObjectManager)->AddNew<ImageObject>("DropButtonIconImage", 9);
	_ButtonIconImage->SetTexture(LOADTEXTURE("UI/up.png"));
	_ButtonIconImage->SetActive(false, false);

	//寿命設定(テスト)。
	_Life = 10.0f;

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

	//レアドロップ用のエフェクト開始。
	if (_DropEquipment->GetRank() <= HoldEquipment::Rank::B) 
	{
		_RareDropPE->RareDropEffect();
		_RareDropSE->Play(false);
	}
	else
	{
		_DropSE->Play(false);
	}
}

//更新。
void DropItem::Update() {

//#ifdef _DEBUG
//	Debug();
//#endif // _DEBUG


	//時間を加算。
	_TotalDeltaTime += Time::DeltaTime();

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
						wchar_t ErrorText[256];
						wcscpy_s(ErrorText, wcslen(L"アイテムを拾えませんでした。") + 1, L"アイテムを拾えませんでした。");
						static_cast<AttentionTextOnly*>(INSTANCE(GameObjectManager)->FindObject("AttentionTextOnly"))->CreateText(
							ErrorText, 
							Vector3(600.0f, 260.0f, 0.0f),
							33.0f,
							Color::red,
							AttentionTextOnly::MoveType::Up
						);
						return;
					}
					break;
					//防具。
				case Item::ItemCodeE::Armor:
					//武器。
				case Item::ItemCodeE::Weapon:
					//追加。
					if (INSTANCE(Inventory)->AddEquipment(_DropEquipment) == false) {
						wchar_t ErrorText[256];
						if (_DropEquipment->GetInfo()->TypeID==Item::ItemCodeE::Armor) {
							wcscpy_s(ErrorText, wcslen(L"防具を拾えませんでした。") + 1, L"防具を拾えませんでした。");
						}
						else
						{
							wcscpy_s(ErrorText, wcslen(L"武器を拾えませんでした。") + 1, L"武器を拾えませんでした。");
						}
						static_cast<AttentionTextOnly*>(INSTANCE(GameObjectManager)->FindObject("AttentionTextOnly"))->CreateText(
							ErrorText,
							Vector3(600.0f, 260.0f, 0.0f),
							33.0f,
							Color::red,
							AttentionTextOnly::MoveType::Up
						);
						return;
					}
					break;
				default:
					break;
				}

				_RareDropPE->SetRareDropEffectFlag(false);

				//アイコンとオブジェクトを消す。
				INSTANCE(GameObjectManager)->AddRemoveList(_ButtonIconImage);
				INSTANCE(GameObjectManager)->AddRemoveList(this);
			}
			else
			{
				char error[256];
				sprintf(error, "ドロップアイテムのInfoが空でした。");
				MessageBoxA(0, error, "ドロップアイテムの取得失敗", MB_ICONWARNING);
			}
			
		}
	}
	else
	{
		//範囲外ならアイコン非表示。
		_ButtonIconImage->SetActive(false, false);
	}

	//フィールド上に出てから一定時間経ったら消す。
	if (_Life < _TotalDeltaTime) {
		//アイコンとオブジェクトとエフェクトを消す。
		_RareDropPE->SetRareDropEffectFlag(false);
		INSTANCE(GameObjectManager)->AddRemoveList(_ButtonIconImage);
		INSTANCE(GameObjectManager)->AddRemoveList(this);
	}

}

#ifdef _DEBUG
void DropItem::Debug() {
	if (KeyBoardInput->isPush(DIK_L)) {
	/*	DropItem* item =INSTANCE(GameObjectManager)->AddNew<DropItem>("DropItem", 9);
		item->Create(INSTANCE(ItemManager)->GetItemInfo(0, Item::ItemCodeE::Armor), Vector3(-212.0f, 58.0f, -156.0f),2);*/

	}
}
#endif // _DEBUG
