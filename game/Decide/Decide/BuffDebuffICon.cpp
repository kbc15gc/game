#include "stdafx.h"
#include "BuffDebuffICon.h"
#include "fbEngine\_Object\_GameObject\ImageObject.h"
#include "GameObject\Player\Player.h"

namespace
{
	//�o�t�̃��[�J�����W�A
	Vector3 BUFF_POS = { -142.0f,88.0f, 0.0f };
	//�o�t�̉��ɕ\��������
	Vector3 BUFF_ARROW_POS = { 25.0f, 15.0f, 0.0f };
	//�o�t���d�ˊ|������ہA���ɂ��炷���߁B
	float OFFSET = 60.0f;
	//�o�t�A�C�R���̃T�C�Y
	Vector2 BUFF_ICON_SIZE = { 41,41 };
	//���A�C�R���̃T�C�Y
	Vector2 ARROW_ICON_SIZE = { 13,19 };
}

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

	//�\���̂̊m�ہB
	BuffDebuff* buffdebuff = new BuffDebuff;

	//���A�C�R���B
	ImageObject* arrowIconImage = INSTANCE(GameObjectManager)->AddNew<ImageObject>("ArrowIconImage", 10);

	//�p�����[�^�[�̃A�C�R���B
	ImageObject* buffDebuffTypeIconImage = INSTANCE(GameObjectManager)->AddNew<ImageObject>("BuffDebuffTypeIconImage", 10);

	//�o�t���̃e�N�X�`����ǂݍ��݁B
	arrowIconImage->SetTexture(LOADTEXTURE("BuffIArrow.png"));
	arrowIconImage->SetSize(ARROW_ICON_SIZE);

	//�p�����[�^�[�����ăp�����[�^�[�ɍ������摜��ǂݍ��݁B
	buffDebuffTypeIconImage->SetTexture(LOADTEXTURE(TypeIconText[static_cast<int>(param)]));
	buffDebuffTypeIconImage->SetSize(BUFF_ICON_SIZE);
	buffDebuffTypeIconImage->transform->SetParent(_PlayerHpBarTransform);
	buffDebuffTypeIconImage->transform->SetLocalPosition(BUFF_POS);

	arrowIconImage->transform->SetParent(buffDebuffTypeIconImage->transform);
	arrowIconImage->transform->SetLocalPosition(BUFF_ARROW_POS);

	//�ݒ肵���A�C�R���̏����܂Ƃ߂�B
	buffdebuff->_ArrowIconImage = arrowIconImage;
	buffdebuff->_BuffDebuffTypeIconImage = buffDebuffTypeIconImage;

	//�\���A�C�R���ǉ��B
	_PlayerBuffDebuffList.push_back(buffdebuff);

}

//�f�o�t�A�C�R���̐����B
void BuffDebuffICon::DebuffIconCreate(Param param) {
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
	arrowIconImage->SetSize(ARROW_ICON_SIZE);
	//�p�����[�^�[�����ăp�����[�^�[�ɍ������摜��ǂݍ��݁B
	buffDebuffTypeIconImage->SetTexture(LOADTEXTURE(TypeIconText[static_cast<int>(param)]));
	buffDebuffTypeIconImage->SetSize(BUFF_ICON_SIZE);
	buffDebuffTypeIconImage->transform->SetParent(_PlayerHpBarTransform);
	buffDebuffTypeIconImage->transform->SetLocalPosition(BUFF_POS);

	arrowIconImage->transform->SetParent(buffDebuffTypeIconImage->transform);
	arrowIconImage->transform->SetLocalPosition(BUFF_ARROW_POS);

	//�ݒ肵���A�C�R���̏����܂Ƃ߂�B
	buffdebuff->_ArrowIconImage = arrowIconImage;
	buffdebuff->_BuffDebuffTypeIconImage = buffDebuffTypeIconImage;

	//�\���A�C�R���ǉ��B
	_PlayerBuffDebuffList.push_back(buffdebuff);

}

void BuffDebuffICon::Update() {
	//�o�t�f�o�t���X�g�ɂ܂�Ă��鐔���B
	for (int i = 0; i < _PlayerBuffDebuffList.size(); i++) {
		//�o�t���d�ˊ|�����邳���A���ɂ��炷�B
		//OFFSET�̒l�ŁA�ǂꂾ�����ɂ��炷���B
		_PlayerBuffDebuffList[i]->_BuffDebuffTypeIconImage->transform->SetLocalPosition(Vector3(BUFF_POS.x + i*OFFSET, BUFF_POS.y, BUFF_POS.z));
	}
}