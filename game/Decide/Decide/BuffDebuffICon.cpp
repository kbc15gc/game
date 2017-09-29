#include "stdafx.h"
#include "BuffDebuffICon.h"
#include "fbEngine\_Object\_GameObject\ImageObject.h"
#include "GameObject\Player\Player.h"

//�������B
void BuffDebuffICon::Awake() {
	//HpBar��transform�ݒ�B
	_PlayerHpBarTransform = static_cast<Player*>(INSTANCE(GameObjectManager)->FindObject("Player"))->GetPlayerHpBar()->GetTransform();
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

	//�\���̂̊m�ہB
	BuffDebuff* buffdebuff = new BuffDebuff;

	//���A�C�R���B
	ImageObject* arrowIconImage = INSTANCE(GameObjectManager)->AddNew<ImageObject>("ArrowIconImage", 10);

	//�p�����[�^�[�̃A�C�R���B
	ImageObject* buffDebuffTypeIconImage = INSTANCE(GameObjectManager)->AddNew<ImageObject>("BuffDebuffTypeIconImage", 10);

	//�o�t���̃e�N�X�`����ǂݍ��݁B
	arrowIconImage->SetTexture(LOADTEXTURE("BuffIArrow.png"));
	arrowIconImage->SetSize(Vector2(20.0f, 30.0f));
	
	//�p�����[�^�[�����ăp�����[�^�[�ɍ������摜��ǂݍ��݁B
	buffDebuffTypeIconImage->SetTexture(LOADTEXTURE(TypeIconText[static_cast<int>(param)]));
	buffDebuffTypeIconImage->SetSize(Vector2(64.0f, 64.0f));
	buffDebuffTypeIconImage->transform->SetParent(_PlayerHpBarTransform);
	buffDebuffTypeIconImage->transform->SetLocalPosition(Vector3(-130.0f,-55.0f, 0.0f));

	arrowIconImage->transform->SetParent(buffDebuffTypeIconImage->transform);
	arrowIconImage->transform->SetLocalPosition(Vector3(30.0f, 15.0f, 0.0f));

	//�ݒ肵���A�C�R���̏����܂Ƃ߂�B
	buffdebuff->_ArrowIconImage = arrowIconImage;
	buffdebuff->_BuffDebuffTypeIconImage = buffDebuffTypeIconImage;
	buffdebuff->_Param = param;

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
	ImageObject* arrowIconImage = INSTANCE(GameObjectManager)->AddNew<ImageObject>("ArrowIconImage", 10);

	//�p�����[�^�[�̃A�C�R���B
	ImageObject* buffDebuffTypeIconImage = INSTANCE(GameObjectManager)->AddNew<ImageObject>("BuffDebuffTypeIconImage", 10);

	//�A�C�R���ɏo���p�����[�^�[�ȊO�������牽�����Ȃ��B
	if (param<Param::Atk || param>Param::Max) {
		return;
	}

	//�f�o�t���̃e�N�X�`����ǂݍ��݁B
	arrowIconImage->SetTexture(LOADTEXTURE("DebuffArrow.png"));
	arrowIconImage->SetSize(Vector2(20.0f, 30.0f));
	//�p�����[�^�[�����ăp�����[�^�[�ɍ������摜��ǂݍ��݁B
	buffDebuffTypeIconImage->SetTexture(LOADTEXTURE(TypeIconText[static_cast<int>(param)]));
	buffDebuffTypeIconImage->SetSize(Vector2(64.0f, 64.0f));
	buffDebuffTypeIconImage->transform->SetParent(_PlayerHpBarTransform);
	buffDebuffTypeIconImage->transform->SetLocalPosition(Vector3(-130.0f, -55.0f, 0.0f));

	arrowIconImage->transform->SetParent(buffDebuffTypeIconImage->transform);
	arrowIconImage->transform->SetLocalPosition(Vector3(30.0f, 15.0f, 0.0f));

	//�ݒ肵���A�C�R���̏����܂Ƃ߂�B
	buffdebuff->_ArrowIconImage = arrowIconImage;
	buffdebuff->_BuffDebuffTypeIconImage = buffDebuffTypeIconImage;
	buffdebuff->_Param = param;

	//�\���A�C�R���ǉ��B
	_PlayerBuffDebuffList.push_back(buffdebuff);

}

//�X�V�B
void BuffDebuffICon::Update() {

	for (int i = 0; i < _PlayerBuffDebuffList.size(); i++) {
		_PlayerBuffDebuffList[i]->_BuffDebuffTypeIconImage->transform->SetLocalPosition(Vector3(-130.0f+i*70.0f, -55.0f, 0.0f));
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