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

	//�L�����N�^�[�R���g���[���[�R���|�[�l���g�ǉ��B
	_CCharacterController = AddComponent<CCharacterController>();

	//���̃R���C�_�[�R���|�[�l���g�ǉ��B
	BoxCollider* coll = AddComponent<BoxCollider>();

	//�d�͐ݒ�B
	_Gravity = -0.98f;

	//�R���C�_�[�쐬�B
	coll->Create(Vector3(1.0f, 1.0f, 1.0f));

	//�X�L�����f���쐬�B
	SkinModelData* modeldata = new SkinModelData();

	//���f���f�[�^�쐬(�t�@�C���p�X�̓e�X�g�p)�B
	modeldata->CloneModelData(SkinModelManager::LoadModel("Chest.X"));

	//���f���ݒ�B
	_Model->SetModelData(modeldata);
	_Model->SetModelEffect(ModelEffectE::SPECULAR);

	//�L�����N�^�[���C�g��ݒ�B
	_CharacterLight.SetDiffuseLightDirection(0, Vector3(1.0f, 0.0f, 0.0f));
	_CharacterLight.SetDiffuseLightDirection(1, Vector3(0.0f, 1.0f, 0.0f));
	_CharacterLight.SetDiffuseLightDirection(2, Vector3(0.0f, 0.0f, 1.0f));
	_CharacterLight.SetDiffuseLightDirection(3, Vector3(0.0f, 0.0f, 0.0f));

	_CharacterLight.SetDiffuseLightColor(0, Vector4(0.1f, 0.1f, 0.1f, 5.0f));
	_CharacterLight.SetDiffuseLightColor(1, Vector4(0.0f, 0.0f, 0.0f, 1.0f));
	_CharacterLight.SetDiffuseLightColor(2, Vector4(0.0f, 0.0f, 0.0f, 1.0f));
	_CharacterLight.SetDiffuseLightColor(3, Vector4(0.0f, 0.0f, 0.0f, 1.0f));

	_CharacterLight.SetAmbientLight(Vector4(0.1f, 0.1f, 0.1f, 1.0f));

	_Model->SetCharacterLight(&_CharacterLight);

	//�������W�B
	_DropPos = Vector3(0.0f, 0.0f, 0.0f);
	transform->SetLocalPosition(_DropPos);

	//�v���C���[�擾�B
	_Player = static_cast<Player*>(INSTANCE(GameObjectManager)->FindObject("Player"));

	//�A�C�R���摜�ݒ�(�A�C�R���摜�̓e�X�g�p)�B
	_ButtonIconImage = INSTANCE(GameObjectManager)->AddNew<ImageObject>("DropButtonIconImage", _ButtonIconPriority);
	_ButtonIconImage->SetTexture(LOADTEXTURE("UI/Button_A.png"));
	_ButtonIconImage->SetActive(false, false);

	//�ő厞�Ԃ�ݒ�B
	_AppearMaxTime = 5.0f;

	//�������B
	_DropEquipment = nullptr;

	_CCharacterController->Init(Vector3(0.0f, 0.5f, 0.0f), Collision_ID::DROPITEM, coll, _Gravity);
	//�ȉ��Փ˂���肽������(������)���w��B
	_CCharacterController->AttributeXZ_AllOff();	//�S�Փ˖����B
	_CCharacterController->AddAttributeXZ(Collision_ID::GROUND);		//�n�ʃR���W������ǉ��B

	//�ȉ��Փ˂���肽������(�c����)���w��B
	_CCharacterController->AttributeY_AllOff();		//�S�Փ˖����B
	_CCharacterController->AddAttributeY(Collision_ID::GROUND);	//�n�ʃR���W������ǉ��B
	
	_CCharacterController->SetGravity(_Gravity);
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
		_DropEquipment = HoldItemFactory::CreateItem(static_cast<Item::BaseInfo*>(info), true);
	}

	SetInfo(info);

	//�����ꏊ��ݒ�B
	_DropPos = pos;
	_DropPos.y += 0.5f;
	transform->SetLocalPosition(_DropPos);

	//�A�C�R���̐e�ݒ�B
	_ButtonIconImage->transform->SetParent(this->transform);

	//�A�C�R���̍��W���������B
	_ButtonIconImage->transform->SetPosition(Vector3::zero);

	_ModelColor = Color::white;

	//������������̃����N���`�F�b�N���A�����N�ɓK����SE�ƃG�t�F�N�g��I���B
	_EquipmentRankCheck_SelectSEAndEffect();
}

//�X�V�B
void DropItem::Update() {

#ifdef _DEBUG
	Debug();
#endif // _DEBUG
	
	float deltaTime = Time::DeltaTime();

	Vector3 moveSpeed=Vector3::zero;
	moveSpeed.y = _CCharacterController->GetMoveSpeed().y;

	//�o�����Ԃɉ��Z�B
	_TotalAppearTime += deltaTime;

	//���f����i�X�����ɂ���B
	_ModelColor.a -= 0.1f*deltaTime;
	_Model->SetAllBlend(_ModelColor);
	
	//�v���C���[�Ƃ̋������v�Z�B
	Vector3 v = _Player->transform->GetPosition() - _DropPos;
	float len = v.Length();
	Vector2 _ScreenPos = INSTANCE(GameObjectManager)->mainCamera->WorldToScreen(this->transform->GetPosition());
	
	_CCharacterController->SetMoveSpeed(moveSpeed);
	_CCharacterController->Execute();

	//�߂��Ȃ�A�C�R����\���B
	if (len < _GetLength) {
		_ButtonIconImage->transform->SetPosition(_ScreenPos.x, _ScreenPos.y - _ButtonIconPosOffSet, 0.0f);
		_ButtonIconImage->SetActive(true, false);

		//�A�C�e�����擾���鎞�ɃW�����v���Ȃ��悤�ɐݒ�B
		_Player->PlayerJumpEnable();

		//�͈͓���A�{�^���������ꂽ��擾�B
		if (VPadInput->IsPush(fbEngine::VPad::ButtonA)) {

			//���g�����鎞�B
			if (_DropItemInfo) {

				//�擾�����A�C�e���̃A�C�e���R�[�h�����ăC���x���g����Add�֐��ɑ���B
				if (_AddInventory(_DropItemInfo->TypeID) == true) 
				{
					//�A�C�e���̎擾���o�����Ȃ�I�u�W�F�N�g�֌W���폜�B
					_Release();
				}
				
				//�Ƃ肠�����擾���I������̂Ńv���C���[�̃W�����v���o����悤�ɂ���B
				_Player->PlayerJumpEnable();
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
	if (_AppearMaxTime < _TotalAppearTime) {
		//�I�u�W�F�N�g�֌W���폜�B
		_Release();
	}
}

//������������̃����N���`�F�b�N���A�����N�ɓK����SE�ƃG�t�F�N�g��I���B
void DropItem::_EquipmentRankCheck_SelectSEAndEffect()
{
	if (_DropItemInfo->TypeID != Item::ItemCodeE::Item&& static_cast<HoldEquipment*>(_DropEquipment)->GetRank() <= HoldEquipment::Rank::B)
	{
		//S�ȏ�Ȃ烌�A�h���b�v�p�̃G�t�F�N�g��SE�J�n�B
		_RareDropPE->RareDropEffect();
		_RareDropSE->Play(false);

	}
	else
	{
		//�h���b�vSE�Đ��B
		_DropSE->Play(false);

		_Model->SetModelEffect(ModelEffectE::SPECULAR, false);
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

	_CCharacterController = nullptr;
	_RareDropPE = nullptr;
	_RareDropSE = nullptr;
	_DropSE = nullptr;
	_DropEquipment = nullptr;

	if (this) {
		INSTANCE(GameObjectManager)->AddRemoveList(this);
	}
}
//AttentionText�ɃA�C�e���R�[�h�����ēK�����������ݒ肷��B
void DropItem::_SelectText(Item::ItemCodeE code, bool flag)
{
	wchar_t Text[256];
	//wcscpy_s(Text, wcslen(L"_DropItemInfo->Name�B") + 1, L"_DropItemInfo->Name�B");
	//if (flag == false) {
	//	//�擾�����̕�����I���B
	//	switch (code)
	//	{
	//		
	//	case Item::ItemCodeE::Item:
	//		//wcscpy_s(Text, wcslen(L"�A�C�e���擾���܂����B") + 1, L"�A�C�e�����擾���܂����B");
	//		break;
	//	case Item::ItemCodeE::Armor:
	//		//wcscpy_s(Text, wcslen(L"�h����擾���܂����B") + 1, L"�h����擾���܂����B");
	//		break;
	//	case Item::ItemCodeE::Weapon:
	//		//wcscpy_s(Text, wcslen(L"������擾���܂����B") + 1, L"������擾���܂����B");
	//		break;
	//	default:
	//		break;
	//	}
	//}
	//else
	{
		//�擾���s�̕�����I���B
		switch (code)
		{
		case Item::ItemCodeE::Item:
			wcscpy_s(Text, wcslen(L"�C���x���g������t�ŃA�C�e�����E���܂���ł����B") + 1, L"�C���x���g������t�ŃA�C�e�����E���܂���ł����B");
			break;
		case Item::ItemCodeE::Armor:
			wcscpy_s(Text, wcslen(L"�C���x���g������t�Ŗh����E���܂���ł����B") + 1, L"�C���x���g������t�Ŗh����E���܂���ł����B");
			break;
		case Item::ItemCodeE::Weapon:
			wcscpy_s(Text, wcslen(L"�C���x���g������t�ŕ�����E���܂���ł����B") + 1, L"�C���x���g������t�ŕ�����E���܂���ł����B");
			break;
		default:
			break;
		}
	}

	
	//�e�L�X�g�ɐݒ�B
	_SetText(Text, flag);
}

//AttentionText�ɕ������ݒ�B
void DropItem::_SetText(const wchar_t* string, bool flag)
{
	if (flag == false) {
		//AttentionText�쐬�B
		static_cast<AttentionTextOnly*>(INSTANCE(GameObjectManager)->FindObject("AttentionTextOnly"))->CreateText(
			string,
			Vector3(600.0f, 260.0f, 0.0f),
			33.0f,
			Color::red,
			AttentionTextOnly::MoveType::Up
		);
	}
	else
	{
		//AttentionText�쐬�B
		static_cast<AttentionTextOnly*>(INSTANCE(GameObjectManager)->FindObject("AttentionTextOnly"))->CreateText(
			string,
			Vector3(600.0f, 260.0f, 0.0f),
			33.0f,
			Color::white,
			AttentionTextOnly::MoveType::Up
		);
	}
}

//�E�����A�C�e�����C���x���g����Add�֐��ɑ���B
bool DropItem::_AddInventory(Item::ItemCodeE code) 
{
	switch (code)
	{
		//�A�C�e���B
	case Item::ItemCodeE::Item:
		//�ǉ��B
		if (INSTANCE(Inventory)->AddItem(static_cast<Item::ItemInfo*>(_DropItemInfo), _DropNum) == false) {
			_SelectText(_DropItemInfo->TypeID,false);
			return false;
		}
		else
		{
			_SelectText(_DropItemInfo->TypeID, true);
			return true;
		}
		break;
		//�h��B
	case Item::ItemCodeE::Armor:
		//����B
	case Item::ItemCodeE::Weapon:
		//�ǉ��B
		if (INSTANCE(Inventory)->AddEquipment(static_cast<HoldEquipment*>(_DropEquipment)) == false) {
			_SelectText(_DropItemInfo->TypeID,false);
			return false;
		}
		else
		{
			_SelectText(_DropItemInfo->TypeID, true);
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
