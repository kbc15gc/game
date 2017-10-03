#include "stdafx.h"
#include "BuffDebuffICon.h"
#include "fbEngine\_Object\_GameObject\ImageObject.h"
#include "GameObject\Player\Player.h"

namespace
{
	//�o�t�A�f�o�t�̃��[�J�����W�A
	Vector3 BUFF_DEBUFF_POS = { -142.0f,88.0f, 0.0f };
	//�o�t�A�f�o�t�̉��ɕ\��������
	Vector3 BUFF_DEBUFF_ARROW_POS = { 25.0f, 15.0f, 0.0f };
	//�o�t���d�ˊ|������ہA���ɂ��炷���߁B
	float OFFSET = 60.0f;
	//�o�t�A�C�R���̃T�C�Y
	Vector2 BUFF_DEBUFF_ICON_SIZE = { 41,41 };
	//���A�C�R���̃T�C�Y
	Vector2 ARROW_ICON_SIZE = { 13,19 };
}

//�f�X�g���N�^�B
BuffDebuffICon::~BuffDebuffICon() {
	for (auto BuffDebuff : _PlayerBuffDebuffList)
	{
		delete BuffDebuff;
	}
}

//�������B
void BuffDebuffICon::Awake() {

}

//�o�t�A�C�R���̐����B
void BuffDebuffICon::BuffIconCreate(Param param) {

	//�A�C�R���ɏo���p�����[�^�[�ȊO�������牽�����Ȃ��B
	if (param<Param::Atk || param>Param::Max) {
		return;
	}

	//���łɒǉ�����Ă���p�����[�^�[�����`�F�b�N�B
	if (false == _AddCheck(param)) {
		return;
	}
	//HpBar��transform�ݒ�B
	_PlayerHpBarTransform = static_cast<Player*>(INSTANCE(GameObjectManager)->FindObject("Player"))->GetPlayerHpBar()->GetTransform();

	//�\���̂̊m�ہB
	BuffDebuff* buffdebuff = new BuffDebuff;

	//���A�C�R���B
	ImageObject* arrowIconImage = INSTANCE(GameObjectManager)->AddNew<ImageObject>("ArrowIconImage", 7);

	//�p�����[�^�[�̃A�C�R���B
	ImageObject* buffDebuffTypeIconImage = INSTANCE(GameObjectManager)->AddNew<ImageObject>("BuffDebuffTypeIconImage", 7);

	//�o�t���̃e�N�X�`����ǂݍ��݁B
	arrowIconImage->SetTexture(LOADTEXTURE(ArrowIconText[static_cast<int>(Arrow::Up)]));
	arrowIconImage->SetSize(ARROW_ICON_SIZE);

	//�p�����[�^�[�����ăp�����[�^�[�ɍ������摜��ǂݍ��݁B
	buffDebuffTypeIconImage->SetTexture(LOADTEXTURE(TypeIconText[static_cast<int>(param)]));
	buffDebuffTypeIconImage->SetSize(BUFF_DEBUFF_ICON_SIZE);
	buffDebuffTypeIconImage->transform->SetParent(_PlayerHpBarTransform);
	buffDebuffTypeIconImage->transform->SetLocalPosition(BUFF_DEBUFF_POS);

	arrowIconImage->transform->SetParent(buffDebuffTypeIconImage->transform);
	arrowIconImage->transform->SetLocalPosition(BUFF_DEBUFF_ARROW_POS);

	//�ݒ肵���A�C�R���̏����܂Ƃ߂�B
	buffdebuff->_ArrowIconImage = arrowIconImage;
	buffdebuff->_BuffDebuffTypeIconImage = buffDebuffTypeIconImage;
	buffdebuff->_Param = param;
	buffdebuff->_isBuff = true;

	//�\���A�C�R���ǉ��B
	_PlayerBuffDebuffList.push_back(buffdebuff);

}

//�f�o�t�A�C�R���̐����B
void BuffDebuffICon::DebuffIconCreate(Param param) {

	//�A�C�R���ɏo���p�����[�^�[�ȊO�������牽�����Ȃ��B
	if (param<Param::Atk || param>Param::Max) {
		return;
	}

	//���łɒǉ�����Ă���p�����[�^�[�����`�F�b�N�B
	if (false == _AddCheck(param)) {
		return;
	}

	//�\���̂̏������B
	BuffDebuff* buffdebuff = new(BuffDebuff);

	//���A�C�R���B
	ImageObject* arrowIconImage = INSTANCE(GameObjectManager)->AddNew<ImageObject>("ArrowIconImage", 7);

	//�p�����[�^�[�̃A�C�R���B
	ImageObject* buffDebuffTypeIconImage = INSTANCE(GameObjectManager)->AddNew<ImageObject>("BuffDebuffTypeIconImage", 7);

	//�A�C�R���ɏo���p�����[�^�[�ȊO�������牽�����Ȃ��B
	if (param<Param::Atk || param>Param::Max) {
		return;
	}

	//�f�o�t���̃e�N�X�`����ǂݍ��݁B
	arrowIconImage->SetTexture(LOADTEXTURE(ArrowIconText[static_cast<int>(Arrow::Down)]));
	arrowIconImage->SetSize(ARROW_ICON_SIZE);
	//�p�����[�^�[�����ăp�����[�^�[�ɍ������摜��ǂݍ��݁B
	buffDebuffTypeIconImage->SetTexture(LOADTEXTURE(TypeIconText[static_cast<int>(param)]));
	buffDebuffTypeIconImage->SetSize(BUFF_DEBUFF_ICON_SIZE);
	buffDebuffTypeIconImage->transform->SetParent(_PlayerHpBarTransform);
	buffDebuffTypeIconImage->transform->SetLocalPosition(BUFF_DEBUFF_POS);

	arrowIconImage->transform->SetParent(buffDebuffTypeIconImage->transform);
	arrowIconImage->transform->SetLocalPosition(BUFF_DEBUFF_ARROW_POS);

	//�ݒ肵���A�C�R���̏����܂Ƃ߂�B
	buffdebuff->_ArrowIconImage = arrowIconImage;
	buffdebuff->_BuffDebuffTypeIconImage = buffDebuffTypeIconImage;
	buffdebuff->_Param = param;
	buffdebuff->_isBuff = false;

	//�\���A�C�R���ǉ��B
	_PlayerBuffDebuffList.push_back(buffdebuff);

}

//�X�V�B
void BuffDebuffICon::Update() {
	//�o�t�f�o�t���X�g�ɂ܂�Ă��鐔���B
	for (int i = 0; i < _PlayerBuffDebuffList.size(); i++) {
		//�o�t���d�ˊ|�����邳���A���ɂ��炷�B
		//OFFSET�̒l�ŁA�ǂꂾ�����ɂ��炷���B
		_PlayerBuffDebuffList[i]->_BuffDebuffTypeIconImage->transform->SetLocalPosition(Vector3(BUFF_DEBUFF_POS.x + i*OFFSET, BUFF_DEBUFF_POS.y, BUFF_DEBUFF_POS.z));
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
bool BuffDebuffICon::_AddCheck(Param param) {
	for (int i = 0; i < _PlayerBuffDebuffList.size(); i++) {
		//�����p�����[�^�[������B
		if (_PlayerBuffDebuffList[i]->_Param == param) {
			return false;
		}
	}

	//�����p�����[�^�[����������
	return true;
}

//�A�C�R����`�悵�Ȃ��B
void BuffDebuffICon::RenderDisable() {

	for (int i = 0; i < _PlayerBuffDebuffList.size(); i++) {
		_PlayerBuffDebuffList[i]->_ArrowIconImage->SetActive(false);
		_PlayerBuffDebuffList[i]->_BuffDebuffTypeIconImage->SetActive(false);
	}
}

//�A�C�R����`�悷��B
void BuffDebuffICon::RenderEnable() {
	for (int i = 0; i < _PlayerBuffDebuffList.size(); i++) {
		_PlayerBuffDebuffList[i]->_ArrowIconImage->SetActive(true);
		_PlayerBuffDebuffList[i]->_BuffDebuffTypeIconImage->SetActive(true);
	}
}

//�o�t�A�C�R���̍폜�B
void BuffDebuffICon::DeleteBuffIcon(Param param) {

	//�A�C�R���ɏo���p�����[�^�[�ȊO�������牽�����Ȃ��B
	if (param<Param::Atk || param>Param::Max) {
		return;
	}

	//�o�t�A�C�R�����폜���鏈���B
	for (auto itr = _PlayerBuffDebuffList.begin(); itr != _PlayerBuffDebuffList.end();) {

		//�o�t�A�C�R���B
		if ((*itr)->_Param == param && (*itr)->_isBuff == true) {

			//�폜�B
			INSTANCE(GameObjectManager)->AddRemoveList((*itr)->_ArrowIconImage);
			INSTANCE(GameObjectManager)->AddRemoveList((*itr)->_BuffDebuffTypeIconImage);
			itr = _PlayerBuffDebuffList.erase(itr);
		}
		else
		{
			//�o�t�A�C�R���ȊO�B
			itr++;
		}
	}
}

//�f�o�t�A�C�R���̍폜�B
void BuffDebuffICon::DeleteDebuffIcon(Param param) {

	//�A�C�R���ɏo���p�����[�^�[�ȊO�������牽�����Ȃ��B
	if (param<Param::Atk || param>Param::Max) {
		return;
	}

	//�f�o�t�A�C�R�����폜���鏈���B
	for (auto itr = _PlayerBuffDebuffList.begin(); itr != _PlayerBuffDebuffList.end();) {

		//�f�o�t�A�C�R���B
		if ((*itr)->_Param == param && (*itr)->_isBuff == false) {

			//�폜�B
			INSTANCE(GameObjectManager)->AddRemoveList((*itr)->_ArrowIconImage);
			INSTANCE(GameObjectManager)->AddRemoveList((*itr)->_BuffDebuffTypeIconImage);
			itr = _PlayerBuffDebuffList.erase(itr);
		}
		else
		{

			//�f�o�t�A�C�R���ȊO�B
			itr++;
		}
	}
}