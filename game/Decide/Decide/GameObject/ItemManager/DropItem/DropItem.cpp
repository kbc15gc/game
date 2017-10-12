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

	//�D��x�ݒ�B
	_ButtonIconPriority = StatusWindow::WindowBackPriorty - 1;

	//���A�h���b�v�p�̃T�E���h�B
	_RareDropSE = INSTANCE(GameObjectManager)->AddNew<SoundSource>("RareDropSE", 0);
	_RareDropSE->Init("Asset/Sound/Drop/RareDropSE.wav");
	_RareDropSE->SetVolume(1.0f);

	//�h���b�v�p�̃T�E���h�B
	_DropSE = INSTANCE(GameObjectManager)->AddNew<SoundSource>("DropSE", 0);
	_DropSE->Init("Asset/Sound/Drop/DropSE.wav");
	_DropSE->SetVolume(1.0f);

	//�p�[�e�B�N���G�t�F�N�g�R���|�[�l���g�ǉ��B
	_RareDropPE = AddComponent<ParticleEffect>();

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

	//�v���C���[�擾�B
	_Player = static_cast<Player*>(INSTANCE(GameObjectManager)->FindObject("Player"));

	//�A�C�R���摜�ݒ�(�A�C�R���摜�̓e�X�g�p)�B
	_ButtonIconImage = INSTANCE(GameObjectManager)->AddNew<ImageObject>("DropButtonIconImage", _ButtonIconPriority);
	_ButtonIconImage->SetTexture(LOADTEXTURE("UI/up.png"));
	_ButtonIconImage->SetActive(false, false);

	//�ő厞�Ԃ�ݒ�B
	_AppearMaxTime = 10.0f;

	//�������B
	_DropEquipment = nullptr;
}

//�h���b�v�A�C�e�����쐬�B
void DropItem::Create(Item::BaseInfo* info, const Vector3& pos, int dropNum) {

	//�A�C�e���̏ꍇ�B
	if (info->TypeID == Item::ItemCodeE::Item) {
		//�A�C�e���̏ꍇ�͎w�肳�ꂽ�������Ƃ��B
		_DropNum = dropNum;
	}
	else
	//����̏ꍇ�B
	{
		//����͈���������Ȃ��B
		_DropEquipment = static_cast<HoldEquipment*>(HoldItemFactory::CreateItem(static_cast<Item::BaseInfo*>(info), true));
	}

	SetInfo(info);

	//�����ꏊ��ݒ�B
	_DropPos = pos;
	transform->SetLocalPosition(_DropPos);

	//�A�C�R���̐e�ݒ�B
	_ButtonIconImage->transform->SetParent(this->transform);

	//�A�C�R���̍��W���������B
	_ButtonIconImage->transform->SetPosition(Vector3::zero);

	//������������̃����N���`�F�b�N���A�����N�ɓK����SE�ƃG�t�F�N�g��I���B
	_EquipmentRankCheck_SelectSEAndEffect();
	
}

//�X�V�B
void DropItem::Update() {

#ifdef _DEBUG
	Debug();
#endif // _DEBUG

	//�o�����Ԃɉ��Z�B
	_TotalAppearTime += Time::DeltaTime();

	//�v���C���[�Ƃ̋������v�Z�B
	Vector3 v = _Player->transform->GetPosition() - _DropPos;
	float len = v.Length();
	Vector2 _ScreenPos = INSTANCE(GameObjectManager)->mainCamera->WorldToScreen(this->transform->GetPosition());
	
	//�߂��Ȃ�A�C�R����\���B
	if (len < _GetLength) {
		_ButtonIconImage->transform->SetPosition(_ScreenPos.x, _ScreenPos.y - _ButtonIconPosOffSet, 0.0f);
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
						_SelectText(_DropItemInfo->TypeID);
						return;
					}
					break;
					//�h��B
				case Item::ItemCodeE::Armor:
					//����B
				case Item::ItemCodeE::Weapon:
					//�ǉ��B
					if (INSTANCE(Inventory)->AddEquipment(_DropEquipment) == false) {
						_SelectText(_DropItemInfo->TypeID);
						return;
					}
					break;
				default:
					break;
				}		
				//�I�u�W�F�N�g�֌W���폜�B
				_Release();
				
			}
			else
			{
				char error[256];
				sprintf(error, "�h���b�v�A�C�e����Info����ł����B");
				MessageBoxA(0, error, "�h���b�v�A�C�e���̎擾���s", MB_ICONWARNING);
			}

			//�I�u�W�F�N�g�֌W���폜�B
			_Release();
		}
	}
	else
	{
		//�͈͊O�Ȃ�A�C�R����\���B
		_ButtonIconImage->SetActive(false, false);
	}

	//�t�B�[���h��ɏo�Ă����莞�Ԍo����������B
	if (_AppearMaxTime < _TotalAppearTime) {
		//�I�u�W�F�N�g�֌W���폜�B
		_Release();
	}

}

//������������̃����N���`�F�b�N���A�����N�ɓK����SE�ƃG�t�F�N�g��I���B
void DropItem::_EquipmentRankCheck_SelectSEAndEffect()
{
	if (_DropEquipment->GetRank() <= HoldEquipment::Rank::B)
	{
		//S�ȏ�Ȃ烌�A�h���b�v�p�̃G�t�F�N�g��SE�J�n�B
		_RareDropPE->RareDropEffect();
		_RareDropSE->Play(false);
	}
	else
	{
		//�h���b�vSE�Đ��B
		_DropSE->Play(false);
	}
}

//���̃N���X��Gameobject�ɓo�^�����I�u�W�F�N�g��S�폜�p�֐��B
void DropItem::_Release()
{
	//�G�t�F�N�g�������B
	_RareDropPE->SetRareDropEffectFlag(false);

	//�{�^���A�C�R���폜�B
	if (_ButtonIconImage) {
		INSTANCE(GameObjectManager)->AddRemoveList(_ButtonIconImage);
	}

	//�h���b�v�pSE���폜�B
	if (_DropSE) {
		INSTANCE(GameObjectManager)->AddRemoveList(_DropSE);
	}

	//���A�h���b�v�p��SE�폜�B
	if (_RareDropSE) {
		INSTANCE(GameObjectManager)->AddRemoveList(_RareDropSE);
	}

	INSTANCE(GameObjectManager)->AddRemoveList(this);
}
//AttentionText�ɃA�C�e���R�[�h�����ēK�����������ݒ肷��B
void DropItem::_SelectText(Item::ItemCodeE code)
{
	wchar_t ErrorText[256];
	//������I���B
	switch (code)
	{
	case Item::ItemCodeE::Item:
		wcscpy_s(ErrorText, wcslen(L"�C���x���g������t�ŃA�C�e�����E���܂���ł����B") + 1, L"�C���x���g������t�ŃA�C�e�����E���܂���ł����B");
		break;
	case Item::ItemCodeE::Armor:
		wcscpy_s(ErrorText, wcslen(L"�C���x���g������t�Ŗh����E���܂���ł����B") + 1, L"�C���x���g������t�Ŗh����E���܂���ł����B");
		break;
	case Item::ItemCodeE::Weapon:
		wcscpy_s(ErrorText, wcslen(L"�C���x���g������t�ŕ�����E���܂���ł����B") + 1, L"�C���x���g������t�ŕ�����E���܂���ł����B");
		break;
	default:
		break;
	}
	//�e�L�X�g�ɐݒ�B
	_SetText(ErrorText);
}

//AttentionText�ɕ������ݒ�B
void DropItem::_SetText(const wchar_t* string) 
{
	//AttentionText�쐬�B
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
