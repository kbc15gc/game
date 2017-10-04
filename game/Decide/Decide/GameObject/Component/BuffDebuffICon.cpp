#include "stdafx.h"
#include "BuffDebuffICon.h"
#include "fbEngine\_Object\_GameObject\ImageObject.h"
#include "GameObject\Enemy\EnemyCharacter.h"
#include "GameObject\ItemManager\ItemManager.h"
#include "GameObject\Player\Player.h"

namespace
{
	//�o�t�A�C�R���̃T�C�Y
	Vector2 BUFF_DEBUFF_ICON_SIZE = { 41,41 };
	//���A�C�R���̃T�C�Y
	Vector2 ARROW_ICON_SIZE = { 13,19 };
}

//�f�X�g���N�^�B
BuffDebuffICon::~BuffDebuffICon() {
	for (auto BuffDebuff : _BuffDebuffList)
	{
		delete BuffDebuff;
	}
}

//�������B
void BuffDebuffICon::Awake() {

}

//�o�t�A�C�R���̐����B
void BuffDebuffICon::BuffIconCreate(CharacterParameter::Param param) {

	//�A�C�R���ɏo���p�����[�^�[�ȊO�������牽�����Ȃ��B
	if (param == CharacterParameter::Param::HP ||
		param == CharacterParameter::Param::MP ||
		param == CharacterParameter::Param::CRT ||
		param == CharacterParameter::Param::LV) {
		return;
	}

	//���łɒǉ�����Ă���p�����[�^�[�����`�F�b�N�B
	if (false == _AddCheck(param,true)) {
		return;
	}


	//�\���̂̊m�ہB
	BuffDebuff* buffdebuff = new BuffDebuff;

	//���A�C�R���B
	ImageObject* arrowIconImage = INSTANCE(GameObjectManager)->AddNew<ImageObject>("ArrowIconImage", 8);

	//�p�����[�^�[�̃A�C�R���B
	ImageObject* buffDebuffTypeIconImage = INSTANCE(GameObjectManager)->AddNew<ImageObject>("BuffDebuffTypeIconImage", 7);

	switch (_UseIconType)
	{
	case BuffDebuffICon::UseIconType::Player:
		SetPlayerOffset();
		//�o�t���̃e�N�X�`����ǂݍ��݁B
		arrowIconImage->SetTexture(LOADTEXTURE(ArrowIconText[static_cast<int>(Arrow::Up)]));
		arrowIconImage->SetSize(ARROW_ICON_SIZE);

		//�p�����[�^�[�����ăp�����[�^�[�ɍ������摜��ǂݍ��݁B
		buffDebuffTypeIconImage->SetTexture(LOADTEXTURE(TypeIconText[static_cast<int>(param)]));
		buffDebuffTypeIconImage->SetSize(BUFF_DEBUFF_ICON_SIZE);

		buffDebuffTypeIconImage->transform->SetParent(_HpBarTransform);
		buffDebuffTypeIconImage->transform->SetPosition(_IconPosOffset);

		arrowIconImage->transform->SetParent(buffDebuffTypeIconImage->transform);
		arrowIconImage->transform->SetLocalPosition(_ArrowPosOffet);
		break;
	case BuffDebuffICon::UseIconType::Enemy:
		SetEnemyOffset();
		//�o�t���̃e�N�X�`����ǂݍ��݁B
		arrowIconImage->SetTexture(LOADTEXTURE(ArrowIconText[static_cast<int>(Arrow::Up)]));
		arrowIconImage->SetSize(ARROW_ICON_SIZE);
		//�p�����[�^�[�����ăp�����[�^�[�ɍ������摜��ǂݍ��݁B
		buffDebuffTypeIconImage->SetTexture(LOADTEXTURE(TypeIconText[static_cast<int>(param)]));
		buffDebuffTypeIconImage->SetSize(BUFF_DEBUFF_ICON_SIZE);

		//�G�l�~�[��p�����B
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

	//�ݒ肵���A�C�R���̏����܂Ƃ߂�B
	buffdebuff->_ArrowIconImage = arrowIconImage;
	buffdebuff->_BuffDebuffTypeIconImage = buffDebuffTypeIconImage;
	buffdebuff->_Param = param;
	buffdebuff->_isBuff = true;

	//�\���A�C�R���ǉ��B
	_BuffDebuffList.push_back(buffdebuff);
}

//�f�o�t�A�C�R���̐����B
void BuffDebuffICon::DebuffIconCreate(CharacterParameter::Param param) {

	//�A�C�R���ɏo���p�����[�^�[�ȊO�������牽�����Ȃ��B
	if (param == CharacterParameter::Param::HP ||
		param == CharacterParameter::Param::MP ||
		param == CharacterParameter::Param::CRT||
		param == CharacterParameter::Param::LV) {
		return;
	}

	//���łɒǉ�����Ă���p�����[�^�[�����`�F�b�N�B
	if (false == _AddCheck(param,false)) {
		return;
	}
	//�\���̂̏������B
	BuffDebuff* buffdebuff = new(BuffDebuff);

	//���A�C�R���B
	ImageObject* arrowIconImage = INSTANCE(GameObjectManager)->AddNew<ImageObject>("ArrowIconImage", 8);

	//�p�����[�^�[�̃A�C�R���B
	ImageObject* buffDebuffTypeIconImage = INSTANCE(GameObjectManager)->AddNew<ImageObject>("BuffDebuffTypeIconImage", 7);

	switch (_UseIconType)
	{
	case BuffDebuffICon::UseIconType::Player:
		SetPlayerOffset();
		//�o�t���̃e�N�X�`����ǂݍ��݁B
		arrowIconImage->SetTexture(LOADTEXTURE(ArrowIconText[static_cast<int>(Arrow::Down)]));
		arrowIconImage->SetSize(ARROW_ICON_SIZE);

		//�p�����[�^�[�����ăp�����[�^�[�ɍ������摜��ǂݍ��݁B
		buffDebuffTypeIconImage->SetTexture(LOADTEXTURE(TypeIconText[static_cast<int>(param)]));
		buffDebuffTypeIconImage->SetSize(BUFF_DEBUFF_ICON_SIZE);

		buffDebuffTypeIconImage->transform->SetParent(_HpBarTransform);
		buffDebuffTypeIconImage->transform->SetPosition(_IconPosOffset);

		arrowIconImage->transform->SetParent(buffDebuffTypeIconImage->transform);
		arrowIconImage->transform->SetLocalPosition(_ArrowPosOffet);
		break;

	case BuffDebuffICon::UseIconType::Enemy:

		SetEnemyOffset();
		//�f�o�t���̃e�N�X�`����ǂݍ��݁B
		arrowIconImage->SetTexture(LOADTEXTURE(ArrowIconText[static_cast<int>(Arrow::Down)]));
		arrowIconImage->SetSize(ARROW_ICON_SIZE);
		//�p�����[�^�[�����ăp�����[�^�[�ɍ������摜��ǂݍ��݁B
		buffDebuffTypeIconImage->SetTexture(LOADTEXTURE(TypeIconText[static_cast<int>(param)]));
		buffDebuffTypeIconImage->SetSize(BUFF_DEBUFF_ICON_SIZE);

		//�G�l�~�[��p�����B
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


	//�ݒ肵���A�C�R���̏����܂Ƃ߂�B
	buffdebuff->_ArrowIconImage = arrowIconImage;
	buffdebuff->_BuffDebuffTypeIconImage = buffDebuffTypeIconImage;
	buffdebuff->_Param = param;
	buffdebuff->_isBuff = false;

	//�\���A�C�R���ǉ��B
	_BuffDebuffList.push_back(buffdebuff);

}

//�X�V�B
void BuffDebuffICon::Update() {
	//�o�t�f�o�t���X�g�ɂ܂�Ă��镪�B
	for (int i = 0; i < _BuffDebuffList.size(); i++) {
		//�o�t���d�ˊ|�����邳���A���ɂ��炷�B
		//OFFSET�̒l�ŁA�ǂꂾ�����ɂ��炷���B
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

	//�f�o�b�O�p�A�C�R���̕`��؂�ւ��B
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

//�ǉ�����p�����[�^�[��ǉ����Ă����̂����`�F�b�N�B
bool BuffDebuffICon::_AddCheck(CharacterParameter::Param param,bool isBuff) {
	for (int i = 0; i < _BuffDebuffList.size(); i++) {
		//�����p�����[�^�[������B
		if (_BuffDebuffList[i]->_Param == param&&_BuffDebuffList[i]->_isBuff==isBuff) {
			return false;
		}
	}

	//�����p�����[�^�[����������
	return true;
}

//�A�C�R����`�悵�Ȃ��B
void BuffDebuffICon::RenderDisable() {

	for (int i = 0; i < _BuffDebuffList.size(); i++) {
		_BuffDebuffList[i]->_ArrowIconImage->SetActive(false);
		_BuffDebuffList[i]->_BuffDebuffTypeIconImage->SetActive(false);
	}
}

//�A�C�R����`�悷��B
void BuffDebuffICon::RenderEnable() {
	for (int i = 0; i < _BuffDebuffList.size(); i++) {
		_BuffDebuffList[i]->_ArrowIconImage->SetActive(true);
		_BuffDebuffList[i]->_BuffDebuffTypeIconImage->SetActive(true);
	}
}

//�o�t�A�C�R���̍폜�B
void BuffDebuffICon::DeleteBuffIcon(CharacterParameter::Param param) {

	//�A�C�R���ɏo���p�����[�^�[�ȊO�������牽�����Ȃ��B
	if (param == CharacterParameter::Param::HP ||
		param == CharacterParameter::Param::MP ||
		param == CharacterParameter::Param::CRT||
		param == CharacterParameter::Param::LV) {
		return;
	}

	//�o�t�A�C�R�����폜���鏈���B
	for (auto itr = _BuffDebuffList.begin(); itr != _BuffDebuffList.end();) {

		//�o�t�A�C�R���B
		if ((*itr)->_Param == param && (*itr)->_isBuff == true) {

			//�폜�B
			INSTANCE(GameObjectManager)->AddRemoveList((*itr)->_ArrowIconImage);
			INSTANCE(GameObjectManager)->AddRemoveList((*itr)->_BuffDebuffTypeIconImage);
			itr = _BuffDebuffList.erase(itr);
		}
		else
		{
			//�o�t�A�C�R���ȊO�B
			itr++;
		}
	}
}

//�f�o�t�A�C�R���̍폜�B
void BuffDebuffICon::DeleteDebuffIcon(CharacterParameter::Param param) {

	//�A�C�R���ɏo���p�����[�^�[�ȊO�������牽�����Ȃ��B
	if (param == CharacterParameter::Param::HP ||
		param == CharacterParameter::Param::MP ||
		param == CharacterParameter::Param::CRT ||
		param == CharacterParameter::Param::LV) {
		return;
	}

	//�f�o�t�A�C�R�����폜���鏈���B
	for (auto itr = _BuffDebuffList.begin(); itr != _BuffDebuffList.end();) {

		//�f�o�t�A�C�R���B
		if ((*itr)->_Param == param && (*itr)->_isBuff == false) {

			//�폜�B
			INSTANCE(GameObjectManager)->AddRemoveList((*itr)->_ArrowIconImage);
			INSTANCE(GameObjectManager)->AddRemoveList((*itr)->_BuffDebuffTypeIconImage);
			itr = _BuffDebuffList.erase(itr);
		}
		else
		{

			//�f�o�t�A�C�R���ȊO�B
			itr++;
		}
	}
}

void BuffDebuffICon::DeleteAllBuffDebuffIcon() {
	//�f�o�t�A�C�R�����폜���鏈���B
	for (auto itr = _BuffDebuffList.begin(); itr != _BuffDebuffList.end();itr++) {

		//�폜�B
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
