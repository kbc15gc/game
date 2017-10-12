#include "stdafx.h"
#include "DropItem.h"
#include "fbEngine\_Object\_Component\_3D\SkinModel.h"
#include "GameObject\Player\Player.h"
#include "fbEngine\_Object\_GameObject\ImageObject.h"
#include "GameObject\Inventory\Inventory.h"
#include "GameObject\ItemManager\HoldItem\HoldItemFactory.h"
#include "GameObject\Component\ParticleEffect.h"
#include "GameObject\TextImage\AttentionTextOnly.h"

//�R���X�g���N�^�B
DropItem::DropItem(const char * name) :
	GameObject(name)
{
}

//�f�X�g���N�^�B
DropItem::~DropItem()
{
}

void DropItem::Awake() {
	//���f���R���|�[�l���g�ǉ��B
	_Model = AddComponent<SkinModel>();

	//�X�L�����f���쐬�B
	SkinModelData* modeldata = new SkinModelData();

	//���f���f�[�^�쐬(�t�@�C���p�X�̓e�X�g�p)�B
	modeldata->CloneModelData(SkinModelManager::LoadModel("Debug/SphereCollision.X"));

	//���f���ݒ�B
	_Model->SetModelData(modeldata);

	//�������W�B
	_DropPos = Vector3(0.0f, 0.0f, 0.0f);
	transform->SetLocalPosition(_DropPos);

	_Player = static_cast<Player*>(INSTANCE(GameObjectManager)->FindObject("Player"));

	//�A�C�R���摜�ݒ�(�A�C�R���摜�̓e�X�g�p)�B
	_ButtonIconImage= INSTANCE(GameObjectManager)->AddNew<ImageObject>("DropButtonIconImage", 9);
	_ButtonIconImage->SetTexture(LOADTEXTURE("UI/up.png"));
	_ButtonIconImage->SetActive(false, false);

	//�����ݒ�(�e�X�g)�B
	_Life = 10.0f;

	//�������B
	_DropEquipment = nullptr;

}

void DropItem::Create(Item::BaseInfo* info, const Vector3& pos, int dropNum) {

	//�A�C�e���̏ꍇ�B
	if (info->TypeID == Item::ItemCodeE::Item) {
		SetInfo(info);
		//�A�C�e���̏ꍇ�͎w�肳�ꂽ�������Ƃ��B
		_DropNum = dropNum;
	}
	else

	//����̏ꍇ�B
	{
		//����͈���������Ȃ��B
		_DropEquipment = static_cast<HoldEquipment*>(HoldItemFactory::CreateItem(static_cast<Item::BaseInfo*>(info), true));
	}

	//�����ꏊ��ݒ�B
	_DropPos = pos;
	transform->SetLocalPosition(_DropPos);

	//�A�C�R���̐e�ݒ�B
	_ButtonIconImage->transform->SetParent(this->transform);

	//�A�C�R���̍��W���������B
	_ButtonIconImage->transform->SetPosition(Vector3::zero);
}

//�X�V�B
void DropItem::Update() {

#ifdef _DEBUG
	Debug();
#endif // _DEBUG


	//���Ԃ����Z�B
	_TotalDeltaTime += Time::DeltaTime();

	//�v���C���[�Ƃ̋������v�Z�B
	Vector3 v = _Player->transform->GetPosition() - _DropPos;
	float len = v.Length();
	Vector2 _ScreenPos = INSTANCE(GameObjectManager)->mainCamera->WorldToScreen(this->transform->GetPosition());
	
	//�߂��Ȃ�A�C�R����\���B
	if (len < 5.0f) {
		_ButtonIconImage->transform->SetPosition(_ScreenPos.x, _ScreenPos.y - 100.0f, 0.0f);
		_ButtonIconImage->SetActive(true, false);

		//�͈͓���A�{�^���������ꂽ��擾�B
		if (VPadInput->IsPush(fbEngine::VPad::ButtonA)) {

			//���g�����鎞�B
			if (_DropItemInfo) {

				//�擾�����A�C�e���̃A�C�e���R�[�h�����ăC���x���g���ɒǉ��B
				switch (_DropItemInfo->TypeID)
				{
					//�A�C�e���B
				case Item::ItemCodeE::Item:
					//�ǉ��B
					if (INSTANCE(Inventory)->AddItem(static_cast<Item::ItemInfo*>(_DropItemInfo), _DropNum) == false) {
						wchar_t ErrorText[256];
						wcscpy_s(ErrorText, wcslen(L"�A�C�e�����E���܂���ł����B") + 1, L"�A�C�e�����E���܂���ł����B");
						static_cast<AttentionTextOnly*>(INSTANCE(GameObjectManager)->FindObject("AttentionTextOnly"))->CreateText(
							ErrorText, 
							Vector3(600.0f, 260.0f, 0.0f),
							33.0f,
							Color::red,
							AttentionTextOnly::MoveType::Up
						);
					}
					break;
					//�h��B
				case Item::ItemCodeE::Armor:
					//����B
				case Item::ItemCodeE::Weapon:
					//�ǉ��B
					if (INSTANCE(Inventory)->AddEquipment(_DropEquipment) == false) {
						wchar_t ErrorText[256];
						if (_DropEquipment->GetInfo()->TypeID==Item::ItemCodeE::Armor) {
							wcscpy_s(ErrorText, wcslen(L"�h����E���܂���ł����B") + 1, L"�h����E���܂���ł����B");
						}
						else
						{
							wcscpy_s(ErrorText, wcslen(L"������E���܂���ł����B") + 1, L"������E���܂���ł����B");
						}
						static_cast<AttentionTextOnly*>(INSTANCE(GameObjectManager)->FindObject("AttentionTextOnly"))->CreateText(
							ErrorText,
							Vector3(600.0f, 260.0f, 0.0f),
							33.0f,
							Color::red,
							AttentionTextOnly::MoveType::Up
						);
					}
					break;
				default:
					break;
				}

				//�A�C�R���ƃI�u�W�F�N�g�������B
				INSTANCE(GameObjectManager)->AddRemoveList(_ButtonIconImage);
				INSTANCE(GameObjectManager)->AddRemoveList(this);
			}
			else
			{
				char error[256];
				sprintf(error, "�h���b�v�A�C�e����Info����ł����B");
				MessageBoxA(0, error, "�h���b�v�A�C�e���̎擾���s", MB_ICONWARNING);
			}
			
		}
	}
	else
	{
		//�͈͊O�Ȃ�A�C�R����\���B
		_ButtonIconImage->SetActive(false, false);
	}

	//�t�B�[���h��ɏo�Ă����莞�Ԍo����������B
	if (_Life < _TotalDeltaTime) {
		//�A�C�R���ƃI�u�W�F�N�g�������B
		INSTANCE(GameObjectManager)->AddRemoveList(_ButtonIconImage);
		INSTANCE(GameObjectManager)->AddRemoveList(this);
	}

}

#ifdef _DEBUG
void DropItem::Debug() {
	if (KeyBoardInput->isPush(DIK_L)) {
		Create(INSTANCE(ItemManager)->GetItemInfo(0, Item::ItemCodeE::Armor), Vector3(-212.0f, 58.0f, -156.0f),2);

	}
}
#endif // _DEBUG
