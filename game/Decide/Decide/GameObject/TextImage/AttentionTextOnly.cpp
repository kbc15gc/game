#include "stdafx.h"
#include "AttentionTextOnly.h"
#include "fbEngine\_Object\_GameObject\TextObject.h"

//�R���X�g���N�^�B
AttentionTextOnly::AttentionTextOnly(const char* name) :
GameObject(name)
{
}

//�f�X�g���N�^�B
AttentionTextOnly::~AttentionTextOnly()
{
	DeleteList();
}

//�e�L�X�g���쐬�B
void AttentionTextOnly::CreateText(const wchar_t * string,
	const Vector3& textPos,
	float size,
	const Color& color,
	MoveType type)
{

	TextInfo* info = new TextInfo;

	//�e�L�X�g�̏������B
	TextObject*	Text = INSTANCE(GameObjectManager)->AddNew<TextObject>("AttentionText", 10);
	Text->Initialize(L"", size);
	Text->SetText(string);
	Text->SetAnchor(fbText::TextAnchorE::Center);
	Text->SetBlendColor(color);
	Text->transform->SetPosition(textPos);

	//�����܂Ƃ߂�B
	info->_Text = Text;
	info->_Color = color;
	info->_Dir = MoveTypeS[static_cast<int>(type)];

	//���X�g�ɒǉ��B
	_TextList.push_back(info);
}

//�X�V�B
void AttentionTextOnly::Update() {
	for (auto itr = _TextList.begin(); itr != _TextList.end();)
	{
		//�e�L�X�g�̈ړ��B
		(*itr)->_Text->transform->SetPosition(
			(*itr)->_Text->transform->GetPosition().x + ((*itr)->_Dir.x/**_MoveSpeed*Time::DeltaTime()*/),
			(*itr)->_Text->transform->GetPosition().y + ((*itr)->_Dir.y/**_MoveSpeed*Time::DeltaTime()*/),
			(*itr)->_Text->transform->GetPosition().z + ((*itr)->_Dir.z/**_MoveSpeed*Time::DeltaTime()*/));

		//�e�e�L�X�g�������������ɂ��Ă����B
		(*itr)->_Text->SetBlendColor(
			Color((*itr)->_Color.r,
			(*itr)->_Color.g,
				(*itr)->_Color.b,
				(*itr)->_Color.a-= 1.5f*Time::DeltaTime()));

		//�e�L�X�g�������ɂȂ肫��ƃ��X�g����폜�B
		if ((*itr)->_Color.a < 0.0f) {
			INSTANCE(GameObjectManager)->AddRemoveList((*itr)->_Text);
			itr = _TextList.erase(itr);
		}
		else
		{
			itr++;
		}
	}
}

//�e�L�X�g���X�g�̑S�폜�B
void AttentionTextOnly::DeleteList() {
	for (auto itr = _TextList.begin(); itr != _TextList.end();)
	{
		INSTANCE(GameObjectManager)->AddRemoveList((*itr)->_Text);
		itr = _TextList.erase(itr);
	}
}