/**
* プレイヤーパラメータUIクラスの実装.
*/
#include"stdafx.h"
#include"PlayerParameterUI.h"

/**
* 初期化.
*/
void PlayerParameterUI::Awake()
{
	_Player = (Player*)INSTANCE(GameObjectManager)->FindObject("Player");

	ImageObject* frame = INSTANCE(GameObjectManager)->AddNew<ImageObject>("Frame", this->GetPriorty());
	frame->transform->SetParent(transform);
	frame->transform->SetLocalPosition(5, 5, 0);
	frame->SetPivot(0, 0);
	frame->SetTexture(LOADTEXTURE("UI/PlayerParameterFrame.png"));
	frame->SetSize(frame->GetSize() * 1.5f);

	_HpBar = INSTANCE(GameObjectManager)->AddNew<ImageObject>("_HpBar", this->GetPriorty());
	_HpBar->transform->SetParent(transform);
	_HpBar->transform->SetLocalPosition(70, 46, 0);
	_HpBar->SetPivot(0.0f, 0.5f);
	_HpBar->SetTexture(LOADTEXTURE("UI/HPBar.png"));
	_HpBar->SetSize(_HpBar->GetSize() * 1.5f);

	_ExpBar = INSTANCE(GameObjectManager)->AddNew<ImageObject>("_ExpBar", this->GetPriorty());
	_ExpBar->transform->SetParent(transform);
	_ExpBar->transform->SetLocalPosition(40, 73, 0);
	_ExpBar->SetPivot(0.5f, 1.0f);
	_ExpBar->SetTexture(LOADTEXTURE("UI/ExpBar.png"));
	_ExpBar->SetSize(_ExpBar->GetSize() * 1.5f);

	_LvText = INSTANCE(GameObjectManager)->AddNew<TextObject>("LvText", this->GetPriorty());
	_LvText->transform->SetParent(transform);
	_LvText->transform->SetLocalPosition(40, 30, 0);
	_LvText->SetAnchor(fbText::TextAnchorE::Left);
	_LvText->Initialize(L"", 35);

	TextObject* lv = INSTANCE(GameObjectManager)->AddNew<TextObject>("LvText", this->GetPriorty());
	lv->transform->SetParent(transform);
	lv->transform->SetLocalPosition(40, 20, 0);
	lv->SetAnchor(fbText::TextAnchorE::Left);
	lv->Initialize(L"Lv", 15);

	_NextHpRate = _NowHpRate = (float)_Player->GetParam(CharacterParameter::Param::HP) / (float)_Player->GetMaxHP();
	_NextExpRate = _NowExpRate = (float)_Player->GetExp() / (float)_Player->GetNextLevelExp();
}

/**
* 更新.
*/
void PlayerParameterUI::LateUpdate()
{
	_NextHpRate = (float)_Player->GetParam(CharacterParameter::Param::HP) / (float)_Player->GetMaxHP();
	float Difference = _NextHpRate - _NowHpRate;

	float time = Time::DeltaTime() * 0.5f;

	if (Difference >= time)
	{
		_NowHpRate += time;
	}
	else if (Difference <= -time)
	{
		_NowHpRate -= time;
	}
	else
	{
		_NowHpRate = _NextHpRate;
	}
	_HpBar->SetUV(Vector4(0, _NowHpRate, 0, 1));
	_HpBar->transform->SetScale(_NowHpRate, 1, 1);

	_NextExpRate = (float)_Player->GetExp() / (float)_Player->GetNextLevelExp();
	Difference = _NextExpRate - _NowExpRate;
	if (Difference >= time)
	{
		_NowExpRate += time;
	}
	else
	{
		_NowExpRate = _NextExpRate;
	}
	_ExpBar->SetUV(Vector4(0, 1, 1.0f - _NowExpRate, 1));
	_ExpBar->transform->SetScale(1, _NowExpRate, 1);

	int lv = _Player->GetParam(CharacterParameter::Param::LV);
	char lvText[100] = { "" };
	sprintf(lvText, "%d", lv);

	_LvText->SetText(lvText);
}
