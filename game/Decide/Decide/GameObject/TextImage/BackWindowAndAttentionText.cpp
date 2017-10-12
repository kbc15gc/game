#include "stdafx.h"
#include "BackWindowAndAttentionText.h"
#include "fbEngine\_Object\_GameObject\ImageObject.h"
#include "fbEngine\_Object\_GameObject\TextObject.h"


//�R���X�g���N�^�B
BackWindowAndAttentionText::BackWindowAndAttentionText(const char* name):
GameObject(name)
{

}

//�f�X�g���N�^�B
BackWindowAndAttentionText::~BackWindowAndAttentionText()
{
	if (_AttentionBackWindowImage) {
		INSTANCE(GameObjectManager)->AddRemoveList(_AttentionBackWindowImage);
	}

	if (_AttentionText) {
		INSTANCE(GameObjectManager)->AddRemoveList(_AttentionText);
	}
}

void BackWindowAndAttentionText::Awake() {
	_AttentionBackWindowImage = INSTANCE(GameObjectManager)->AddNew<ImageObject>("AttentionBackWindowImage", 9);
	_AttentionText	 = INSTANCE(GameObjectManager)->AddNew<TextObject>("AttentionText", 9);

	SetActiveFalse();
}

//�e�L�X�g�Ɣw�i�摜�̍쐬�B
void BackWindowAndAttentionText::Create(const wchar_t * string,
	const Vector3& windowPos,
	const Vector2& windowSize,
	const Vector3& textPos,
	float textSize,
	const Color& color) 
{
	_AttentionBackWindowPos = windowPos;
	_AttentionBackWindowSize = windowSize;
	_AttentionTextPos = textPos;
	_AttentionTextFontSize = textSize;

	//�w�i�E�C���h�E�������B
	_AttentionBackWindowImage->SetTexture(LOADTEXTURE("UI/Panel 5.png"));
	_AttentionBackWindowImage->transform->SetPosition(_AttentionBackWindowPos);
	_AttentionBackWindowImage->SetSize(_AttentionBackWindowSize);

	//�e�L�X�g�̏������B
	_AttentionText->Initialize(L"", _AttentionTextFontSize);
	_AttentionText->SetText(string);
	_AttentionText->SetAnchor(fbText::TextAnchorE::Center);
	_AttentionText->transform->SetParent(_AttentionBackWindowImage->transform);
	_AttentionText->transform->SetLocalPosition(_AttentionTextPos);
	_AttentionText->SetBlendColor(color);

	SetActiveTrue();
}

//�X�V�B
void BackWindowAndAttentionText::Update() {

	if (VPadInput->IsPush(fbEngine::VPad::ButtonA)) {
		SetActiveFalse();
	}

#ifdef _DEBUG
	Debug();
#endif // _DEBUG

}

//�f�o�b�O�B
#ifdef _DEBUG

void BackWindowAndAttentionText::Debug()
{
	//�w�i�E�C���h�E�̈ʒu�����A
	if (KeyBoardInput->isPressed(DIK_H) && KeyBoardInput->isPressed(DIK_UP)) {
		_AttentionBackWindowPos.y -= 1.0f;
		_AttentionBackWindowImage->transform->SetPosition(Vector3(_AttentionBackWindowPos.x, _AttentionBackWindowPos.y, 0.0f));
	}

	if (KeyBoardInput->isPressed(DIK_H) && KeyBoardInput->isPressed(DIK_DOWN)) {
		_AttentionBackWindowPos.y += 1.0f;
		_AttentionBackWindowImage->transform->SetPosition(Vector3(_AttentionBackWindowPos.x, _AttentionBackWindowPos.y, 0.0f));
	}

	if (KeyBoardInput->isPressed(DIK_H) && KeyBoardInput->isPressed(DIK_RIGHT)) {
		_AttentionBackWindowPos.x += 1.0f;
		_AttentionBackWindowImage->transform->SetPosition(Vector3(_AttentionBackWindowPos.x, _AttentionBackWindowPos.y, 0.0f));
	}

	if (KeyBoardInput->isPressed(DIK_H) && KeyBoardInput->isPressed(DIK_LEFT)) {
		_AttentionBackWindowPos.x -= 1.0f;
		_AttentionBackWindowImage->transform->SetPosition(Vector3(_AttentionBackWindowPos.x, _AttentionBackWindowPos.y, 0.0f));
	}

	//�w�i�E�B���h�E�̃T�C�Y�����B
	if (KeyBoardInput->isPressed(DIK_K) && KeyBoardInput->isPressed(DIK_UP)) {
		_AttentionBackWindowSize.y--;
		_AttentionBackWindowImage->SetSize(_AttentionBackWindowSize);
	}

	if (KeyBoardInput->isPressed(DIK_K) && KeyBoardInput->isPressed(DIK_DOWN)) {
		_AttentionBackWindowSize.y++;
		_AttentionBackWindowImage->SetSize(_AttentionBackWindowSize);
	}

	if (KeyBoardInput->isPressed(DIK_K) && KeyBoardInput->isPressed(DIK_RIGHT)) {
		_AttentionBackWindowSize.x++;
		_AttentionBackWindowImage->SetSize(_AttentionBackWindowSize);
	}

	if (KeyBoardInput->isPressed(DIK_K) && KeyBoardInput->isPressed(DIK_LEFT)) {
		_AttentionBackWindowSize.x--;
		_AttentionBackWindowImage->SetSize(_AttentionBackWindowSize);
	}


	//�e�L�X�g�̈ʒu�����B
	if (KeyBoardInput->isPressed(DIK_J) && KeyBoardInput->isPressed(DIK_UP)) {
		_AttentionTextPos.y -= 1.0f;
		_AttentionText->transform->SetLocalPosition(Vector3(_AttentionTextPos.x, _AttentionTextPos.y, 0.0f));
	}

	if (KeyBoardInput->isPressed(DIK_J) && KeyBoardInput->isPressed(DIK_DOWN)) {
		_AttentionTextPos.y += 1.0f;
		_AttentionText->transform->SetLocalPosition(Vector3(_AttentionTextPos.x, _AttentionTextPos.y, 0.0f));
	}

	if (KeyBoardInput->isPressed(DIK_J) && KeyBoardInput->isPressed(DIK_RIGHT)) {
		_AttentionTextPos.x += 1.0f;
		_AttentionText->transform->SetLocalPosition(Vector3(_AttentionTextPos.x, _AttentionTextPos.y, 0.0f));
	}

	if (KeyBoardInput->isPressed(DIK_J) && KeyBoardInput->isPressed(DIK_LEFT)) {
		_AttentionTextPos.x -= 1.0f;
		_AttentionText->transform->SetLocalPosition(Vector3(_AttentionTextPos.x, _AttentionTextPos.y, 0.0f));
	}

	//�e�L�X�g�̃T�C�Y�����B
	if ((KeyBoardInput->isPressed(DIK_L) && KeyBoardInput->isPressed(DIK_UP))) {
		_AttentionTextFontSize++;
		_AttentionText->SetFontSize(_AttentionTextFontSize);
	}

	if ((KeyBoardInput->isPressed(DIK_L) && KeyBoardInput->isPressed(DIK_DOWN))) {
		_AttentionTextFontSize--;
		_AttentionText->SetFontSize(_AttentionTextFontSize);
	}

	//�摜�\���p�B
	if ((KeyBoardInput->isPush(DIK_O))) {
		wchar_t ErrorText[256];
		wcscpy_s(ErrorText, wcslen(L"�����i����t�ŃA�C�e����ǉ��o���܂���ł����B\n�����i�𐮗����Ă��������BA�{�^���܂͂��L�[�{�[�h��P�ŕ���B") + 1, L"�����i����t�ŃA�C�e����ǉ��o���܂���ł����B\n�����i�𐮗����Ă��������BA�{�^���܂͂��L�[�{�[�h��P�ŕ���B");
		Create(ErrorText, Vector3(642.0f, 363.0f, 0.0f), Vector2(793.0f, 229.0f), Vector3(1.0f, -20.0f, 0.0), 33.0f, Color::red);
		SetActiveTrue();
	}

	//�摜�����B
	if ((VPadInput->IsPush(fbEngine::VPad::ButtonA)||(KeyBoardInput->isPush(DIK_P)))) {
		SetActiveFalse();
	}
}
#endif // DEBUG