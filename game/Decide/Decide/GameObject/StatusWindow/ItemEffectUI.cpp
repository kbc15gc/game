/**
* アイテム効果UIクラスの実装.
*/
#include"stdafx.h"
#include"ItemEffectUI.h"

namespace
{
	const char* ParamName[] =
	{
		"HP : ",
		"ATK : ",
		"MAT : ",
		"DEF : ",
		"MDE : ",
		"DEX : ",
		"CRT : ",
	};

	const char* ParamText[] =
	{
		"%d",
		"%d%%",
		"%d%%",
		"%d%%",
		"%d%%",
		"%d%%",
		"%d",
	};

	CharacterParameter::Param ParamIndex[] =
	{
		CharacterParameter::Param::HP,
		CharacterParameter::Param::ATK,
		CharacterParameter::Param::MAT,
		CharacterParameter::Param::DEF,
		CharacterParameter::Param::MDE,
		CharacterParameter::Param::DEX,
		CharacterParameter::Param::CRT,
	};
}

/**
* 初期化.
*/
void ItemEffectUI::Awake()
{
	transform->SetLocalPosition(g_WindowSize / 2);
	ImageObject* back = INSTANCE(GameObjectManager)->AddNew<ImageObject>("back", this->GetPriorty());
	back->transform->SetParent(transform);
	back->transform->SetLocalPosition(0, 50, 0);
	back->SetTexture(LOADTEXTURE("UI/Panel5.png"));
	back->SetSize(Vector2(490,230));
	back->SetPivot(1.0f, 0.0f);

	for (int i = 0; i < (int)IEParamCode::Count; i++)
	{
		TextObject* paramName = INSTANCE(GameObjectManager)->AddNew<TextObject>("paramName", this->GetPriorty());
		paramName->Initialize(L"", 25);
		paramName->SetText(ParamName[i]);
		paramName->SetAnchor(fbText::TextAnchorE::Left);
		paramName->transform->SetParent(transform);
		paramName->transform->SetLocalPosition(Vector3(-450, 65 + 30 * i, 0));

		TextObject* param = INSTANCE(GameObjectManager)->AddNew<TextObject>("param", this->GetPriorty());
		param->Initialize(L"", 25);
		param->SetAnchor(fbText::TextAnchorE::Right);
		param->transform->SetParent(paramName->transform);
		param->transform->SetLocalPosition(Vector3(100, 0, 0));

		_Param[i] = param;
	}
}

/**
* アイテムデータを設定.
*/
void ItemEffectUI::SetItemData(ConsumptionItem * itemData)
{
	char str[100] = { "" };

	if (itemData)
	{
		Item::ItemInfo* info = (Item::ItemInfo*)itemData->GetInfo();

		for (int i = 0; i < (int)IEParamCode::Count; i++)
		{
			int param = info->effectValue[ParamIndex[i]];
			sprintf(str, ParamText[i], param);
			if (param > 0)
			{
				_Param[i]->SetBlendColor(Color::blue);
			}
			else if(param < 0)
			{
				_Param[i]->SetBlendColor(Color::red);
			}
			else
			{
				_Param[i]->SetBlendColor(Color::white);
			}
			_Param[i]->SetText(str);
		}
	}
	else
	{
		for (int i = 0; i < (int)IEParamCode::Count; i++)
		{
			sprintf(str, ParamText[i], 0);
			_Param[i]->SetText(str);
			_Param[i]->SetBlendColor(Color::white);
		}
	}
}
