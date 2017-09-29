#include "stdafx.h"
#include "BuffDebuffICon.h"
#include "fbEngine\_Object\_GameObject\ImageObject.h"
#include "GameObject\Player\Player.h"

//�������B
void BuffDebuffICon::Awake() {
	
	//���A�C�R���B
	_ArrowIconImage = INSTANCE(GameObjectManager)->AddNew<ImageObject>("ArrowIconImage", 10);
	
	//�p�����[�^�[�̃A�C�R���B
	_BuffDebuffTypeIconImage = INSTANCE(GameObjectManager)->AddNew<ImageObject>("BuffDebuffTypeIconImage", 10);

	//�\���̂̏������B
	_Buffdebuff = new(BuffDebuff);
	_Buffdebuff->_ArrowIconImage = nullptr;
	_Buffdebuff->_BuffDebuffTypeIconImage = nullptr;

	//HpBar��transform�ݒ�B
	_PlayerHpBarTransform = static_cast<Player*>(INSTANCE(GameObjectManager)->FindObject("Player"))->GetPlayerHpBar()->GetTransform();
}

//�o�t�A�C�R���̐����B
void BuffDebuffICon::BuffIconCreate(Param param) {

	//�A�C�R���ɏo���p�����[�^�[�ȊO�������牽�����Ȃ��B
	if (param<Param::Atk || param>Param::Max) {
		return;
	}

	//�o�t���̃e�N�X�`����ǂݍ��݁B
	_ArrowIconImage->SetTexture(LOADTEXTURE("BuffIArrow.png"));
	_ArrowIconImage->SetSize(Vector2(20.0f, 30.0f));
	
	//�p�����[�^�[�����ăp�����[�^�[�ɍ������摜��ǂݍ��݁B
	_BuffDebuffTypeIconImage->SetTexture(LOADTEXTURE(TypeIconText[static_cast<int>(param)]));
	_BuffDebuffTypeIconImage->SetSize(Vector2(64.0f, 64.0f));
	_BuffDebuffTypeIconImage->transform->SetParent(_PlayerHpBarTransform);
	_BuffDebuffTypeIconImage->transform->SetLocalPosition(Vector3(0.0f,0.0f, 0.0f));

	_ArrowIconImage->transform->SetParent(_BuffDebuffTypeIconImage->transform);
	_ArrowIconImage->transform->SetLocalPosition(Vector3(30.0f, -30.0f, 0.0f));

	//�ݒ肵���A�C�R���̏����܂Ƃ߂�B
	_Buffdebuff->_ArrowIconImage = _ArrowIconImage;
	_Buffdebuff->_BuffDebuffTypeIconImage = _BuffDebuffTypeIconImage;

	//�\���A�C�R���ǉ��B
	_PlayerBuffDebuffList.push_back(_Buffdebuff);

	_Buffdebuff->_ArrowIconImage = nullptr;
	_Buffdebuff->_BuffDebuffTypeIconImage = nullptr;
}

//�f�o�t�A�C�R���̐����B
void BuffDebuffICon::DebuffIconCreate(Param param) {
	//�A�C�R���ɏo���p�����[�^�[�ȊO�������牽�����Ȃ��B
	if (param<Param::Atk || param>Param::Max) {
		return;
	}

	//�f�o�t���̃e�N�X�`����ǂݍ��݁B
	_ArrowIconImage->SetTexture(LOADTEXTURE("DebuffArrow.png"));
	_ArrowIconImage->SetSize(Vector2(20.0f, 30.0f));
	//�p�����[�^�[�����ăp�����[�^�[�ɍ������摜��ǂݍ��݁B
	_BuffDebuffTypeIconImage->SetTexture(LOADTEXTURE(TypeIconText[static_cast<int>(param)]));
	_BuffDebuffTypeIconImage->SetSize(Vector2(64.0f, 64.0f));
	_BuffDebuffTypeIconImage->transform->SetParent(_PlayerHpBarTransform);
	_BuffDebuffTypeIconImage->transform->SetLocalPosition(Vector3(0.0f,0.0f, 0.0f));

	_ArrowIconImage->transform->SetParent(_BuffDebuffTypeIconImage->transform);
	_ArrowIconImage->transform->SetLocalPosition(Vector3(-30.0f,-30.0f, 0.0f));

	//�ݒ肵���A�C�R���̏����܂Ƃ߂�B
	_Buffdebuff->_ArrowIconImage = _ArrowIconImage;
	_Buffdebuff->_BuffDebuffTypeIconImage = _BuffDebuffTypeIconImage;

	//�\���A�C�R���ǉ��B
	_PlayerBuffDebuffList.push_back(_Buffdebuff);

	_Buffdebuff->_ArrowIconImage = nullptr;
	_Buffdebuff->_BuffDebuffTypeIconImage = nullptr;
}

void BuffDebuffICon::Update() {
	
}