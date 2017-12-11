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
#include "fbEngine/CharacterController.h"
#include "GameObject\History\HistoryBook\HistoryBook.h"
#include "GameObject\ItemManager\ItemManager.h"

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

	//キャラクターコントローラーコンポーネント追加。
	_CCharacterController = AddComponent<CCharacterController>();

	//球体コライダーコンポーネント追加。
	BoxCollider* coll = AddComponent<BoxCollider>();

	//重力設定。
	_Gravity = -0.98f;

	//コライダー作成。
	coll->Create(Vector3(1.0f, 1.0f, 1.0f));

	//スキンモデル作成。
	SkinModelData* modeldata = new SkinModelData();

	//モデルデータ作成(ファイルパスはテスト用)。
	modeldata->CloneModelData(SkinModelManager::LoadModel("Chest.X"));

	//モデル設定。
	_Model->SetModelData(modeldata);
	_Model->SetModelEffect(ModelEffectE::SPECULAR);
	_Model->SetModelEffect(ModelEffectE::ALPHA);

	_Model->SetModelEffect(ModelEffectE::ZENABLE, true);
	_Model->SetModelEffect(ModelEffectE::DITHERING, true);

	//キャラクターライトを設定。
	_TreasureChestLight.SetDiffuseLightDirection(0, Vector3(1.0f, 0.0f, 0.0f));
	_TreasureChestLight.SetDiffuseLightDirection(1, Vector3(0.0f, 1.0f, 0.0f));
	_TreasureChestLight.SetDiffuseLightDirection(2, Vector3(0.0f, 0.0f, 1.0f));
	_TreasureChestLight.SetDiffuseLightDirection(3, Vector3(0.0f, 0.0f, 0.0f));

	_TreasureChestLight.SetDiffuseLightColor(0, Vector4(0.1f, 0.1f, 0.1f, 5.0f));
	_TreasureChestLight.SetDiffuseLightColor(1, Vector4(0.0f, 0.0f, 0.0f, 1.0f));
	_TreasureChestLight.SetDiffuseLightColor(2, Vector4(0.0f, 0.0f, 0.0f, 1.0f));
	_TreasureChestLight.SetDiffuseLightColor(3, Vector4(0.0f, 0.0f, 0.0f, 1.0f));

	_TreasureChestLight.SetAmbientLight(Vector4(0.1f, 0.1f, 0.1f, 1.0f));

	_Model->SetCharacterLight(&_TreasureChestLight);

	//初期座標。
	_DropPos = Vector3(0.0f, 0.0f, 0.0f);
	transform->SetLocalPosition(_DropPos);

	//プレイヤー取得。
	_Player = static_cast<Player*>(INSTANCE(GameObjectManager)->FindObject("Player"));

	//アイコン画像設定(アイコン画像はテスト用)。
	_ButtonIconImage = INSTANCE(GameObjectManager)->AddNew<ImageObject>("DropButtonIconImage", _ButtonIconPriority);
	_ButtonIconImage->SetTexture(LOADTEXTURE("UI/Button_A.png"));
	_ButtonIconImage->SetActive(false, false);

	//最大時間を設定。
	_AppearMaxTime = 5.0f;

	//初期化。
	_DropEquipment = nullptr;

	_CCharacterController->Init(Vector3(0.0f, 0.5f, 0.0f), Collision_ID::DROPITEM, coll, _Gravity);
	//以下衝突を取りたい属性(横方向)を指定。
	_CCharacterController->AttributeXZ_AllOff();	//全衝突無視。
	_CCharacterController->AddAttributeXZ(Collision_ID::GROUND);		//地面コリジョンを追加。

	//以下衝突を取りたい属性(縦方向)を指定。
	_CCharacterController->AttributeY_AllOff();		//全衝突無視。
	_CCharacterController->AddAttributeY(Collision_ID::GROUND);		//地面コリジョンを追加。
	_CCharacterController->AddAttributeY(Collision_ID::BUILDING);	//建物のコリジョンを追加。
	
	_CCharacterController->SetGravity(_Gravity);

	//テキストを出す位置。
	_TextPos = Vector3(600.0f, 260.0f, 0.0f);

	//テキストサイズ。
	_TextFontSize = 33.0f;

	_AttentionText = static_cast<AttentionTextOnly*>(INSTANCE(GameObjectManager)->FindObject("AttentionTextOnly"));
}

//ドロップアイテムを作成。
void DropItem::Create(int id, int typeId, const Vector3& pos, int dropNum) {

	_DropItemInfo = INSTANCE(ItemManager)->GetItemInfo(id, static_cast<Item::ItemCodeE>(typeId));
	//アイテムの場合。
	if (_DropItemInfo->TypeID == Item::ItemCodeE::Item) {
		//アイテムの場合は指定された数分落とす。
		_DropNum = dropNum;
	}
	else
	//武具の場合。
	{
		//武具は一つしか落ちない。
		_DropEquipment = HoldItemFactory::CreateItem(_DropItemInfo, true);
	}

	//落下場所を設定。
	_DropPos = pos;
	_DropPos.y += 0.5f;
	transform->SetLocalPosition(_DropPos);

	//アイコンの親設定。
	_ButtonIconImage->transform->SetParent(this->transform);

	//アイコンの座標を初期化。
	_ButtonIconImage->transform->SetPosition(Vector3::zero);

	_ModelColor = Color::white;

	//生成した武具のランクをチェックし、ランクに適したSEとエフェクトを選択。
	_EquipmentRankCheck_SelectSEAndEffect();
}

//更新。
void DropItem::Update() {

#ifdef _DEBUG
	Debug();
#endif // _DEBUG
	
	float deltaTime = Time::DeltaTime();

	Vector3 moveSpeed = Vector3::zero;

	//出現時間に加算。
	_TotalAppearTime += deltaTime;

	//モデルを段々透明にする。
	_ModelColor.a -= deltaTime * 0.1f;
	_Model->SetAllBlend(_ModelColor);
	
	//プレイヤーとの距離を計算。
	Vector3 v = _Player->transform->GetPosition() - _DropPos;
	float len = v.Length();
	Vector2 _ScreenPos = INSTANCE(GameObjectManager)->mainCamera->WorldToScreen(this->transform->GetPosition());
	
	_CCharacterController->SetMoveSpeed(moveSpeed);
	_CCharacterController->Execute();

	//近くならアイコンを表示。
	if (len < _GetLength) {
		_ButtonIconImage->transform->SetPosition(_ScreenPos.x, _ScreenPos.y - _ButtonIconPosOffSet, 0.0f);
		_ButtonIconImage->SetActive(true, false);

		//アイテムを取得する時にジャンプしないように設定。
		_Player->PlayerJumpEnable();

		//範囲内でAボタンを押されたら取得。
		if (VPadInput->IsPush(fbEngine::VPad::ButtonA)) {

			StatusWindow* status = (StatusWindow*)INSTANCE(GameObjectManager)->FindObject("StatusWindow");
			HistoryBook* book = (HistoryBook*)INSTANCE(GameObjectManager)->FindObject("HistoryBook");

			//ステータス画面も歴史書も開いていない時にアイテム拾う。
			if (status->GetActive() == false && book->GetActive() == false) {

				//中身がある時。
				if (_DropItemInfo) {

					//取得したアイテムのアイテムコードを見てインベントリのAdd関数に送る。
					if (_AddInventory(_DropItemInfo->TypeID) == true)
					{
						//アイテムの取得が出来たならオブジェクト関係を削除。
						_Release();
					}

					//とりあえず取得が終わったのでプレイヤーのジャンプを出来るようにする。
					_Player->PlayerJumpEnable();
				}
				else
				{
					char error[256];
					sprintf(error, "ドロップアイテムのInfoが空でした。");
					MessageBoxA(0, error, "ドロップアイテムの取得失敗", MB_ICONWARNING);
				}
			}
		}
	}
	else
	{
		//範囲外ならアイコン非表示。
		_ButtonIconImage->SetActive(false, false);
	}

	//フィールド上に出てから一定時間経ったら消す。
	if (_AppearMaxTime < _TotalAppearTime)
	{
		if (_DitheRocalTime >= _DitheTime)
		{
			//オブジェクト関係を削除。
			_Release();
		}
		_DitheRocalTime += Time::DeltaTime();
		_Model->SetDitherCoefficient((_DitheRocalTime / _DitheTime) * 65.0f);
	}
}

//生成した武具のランクをチェックし、ランクに適したSEとエフェクトを選択。
void DropItem::_EquipmentRankCheck_SelectSEAndEffect()
{
	if (_DropItemInfo->TypeID != Item::ItemCodeE::Item&& static_cast<HoldEquipment*>(_DropEquipment)->GetRank() <= HoldEquipment::Rank::A)
	{
		//S以上ならレアドロップ用のエフェクトとSE開始。
		_RareDropPE->RareDropEffect();
		_RareDropSE->Play(false);

	}
	else
	{
		//ドロップSE再生。
		_DropSE->Play(false);

		_Model->SetModelEffect(ModelEffectE::SPECULAR, false);
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

	if (this) {
		INSTANCE(GameObjectManager)->AddRemoveList(this);
	}
}

//flagを見て取得成功か、失敗の適した文字列を決める。
void DropItem::_SelectText(bool flag)
{
	wchar_t WText[256];
	char	CText[256];

	
	if (flag == true) {
		//アイテムの名前をコピー。
		strcpy(CText, _DropItemInfo->Name);
		//取得成功の文字列選択。
		strcat(CText, "を入手しました。");
	}
	else
	{
		//取得失敗の文字列選択。
		strcpy(CText, "アイテムを入手できませんでした。");
	}

	//chatrからwchra_tに変換。
	mbstowcs(WText, CText, sizeof(CText));

	//テキストに設定。
	_SetText(WText, flag);
}

//AttentionTextに文字列を設定。
void DropItem::_SetText(const wchar_t* string, bool flag)
{
	if (flag == false) {
		//AttentionText作成。
		static_cast<AttentionTextOnly*>(INSTANCE(GameObjectManager)->FindObject("AttentionTextOnly"))->CreateText(
			string,
			_TextPos,
			_TextFontSize,
			Color::red,
			AttentionTextOnly::MoveType::Up
		);
	}
	else
	{
		//AttentionText作成。
		static_cast<AttentionTextOnly*>(INSTANCE(GameObjectManager)->FindObject("AttentionTextOnly"))->CreateText(
			string,
			_TextPos,
			_TextFontSize,
			Color::white,
			AttentionTextOnly::MoveType::Down
		);
	}
}

//拾ったアイテムをインベントリのAdd関数に送る。
bool DropItem::_AddInventory(Item::ItemCodeE code) 
{
	switch (code)
	{
		//アイテム。
	case Item::ItemCodeE::Item:
		//追加。
		if (INSTANCE(Inventory)->AddItem(static_cast<Item::ItemInfo*>(_DropItemInfo), _DropNum) == false) {
			//取得失敗。
			_SelectText(false);
			return false;
		}
		else
		{
			//取得成功。
			_SelectText(true);
			return true;
		}
		break;
		//防具。
	case Item::ItemCodeE::Armor:
		//武器。
	case Item::ItemCodeE::Weapon:
		//追加。
		if (INSTANCE(Inventory)->AddEquipment(static_cast<HoldEquipment*>(_DropEquipment)) == false) {
			//取得失敗。
			_SelectText(false);
			return false;
		}
		else
		{
			//取得成功。
			_SelectText(true);
			return true;
		}
		break;
	default:
		break;
	}
}

#ifdef _DEBUG
void DropItem::Debug() {
	
}
#endif // _DEBUG
