/**
* パラメータ表示クラスの実装.
*/
#include"stdafx.h"
#include"ParameterRender.h"

const float ParameterRender::defaultNameTextSize = 40.0f;
const Vector2 ParameterRender::defaultIconSize = Vector2(30.0f,30.0f);

/**
* 初期化.
*/
void ParameterRender::Awake()
{
	//パラメータ名初期化.
	_ParamNameText = INSTANCE(GameObjectManager)->AddNew<TextObject>("ParamNameText", 9);
	_ParamNameText->Initialize(L"", defaultNameTextSize);
	_ParamNameText->SetAnchor(fbText::TextAnchorE::UpperLeft);
	_ParamNameText->transform->SetParent(transform);
	_ParamNameText->transform->SetLocalPosition(Vector3(-170.0f, 0.0f, 0.0f));

	//アイコン画像初期化.
	_IconImage = INSTANCE(GameObjectManager)->AddNew<ImageObject>("IconImage", 8);
	_IconImage->transform->SetParent(_ParamNameText->transform);
	_IconImage->transform->SetLocalPosition(Vector3(-20.0f, 12.0f, 0.0f));

	//パラメータ初期化.
	_ParamText = INSTANCE(GameObjectManager)->AddNew<TextObject>("ParamText", 9);
	_ParamText->Initialize(L"", defaultNameTextSize);
	_ParamText->SetAnchor(fbText::TextAnchorE::UpperRight);
	_ParamText->transform->SetParent(_ParamNameText->transform);
	_ParamText->transform->SetLocalPosition(Vector3(370.0f, 0.0f, 0.0f));

	////Maxパラメータ初期化.
	//_MaxParamText = INSTANCE(GameObjectManager)->AddNew<TextObject>("BuffText", 9);
	//_MaxParamText->Initialize(L"", 40.0f);
	//_MaxParamText->SetAnchor(fbText::TextAnchorE::UpperLeft);
	//_MaxParamText->transform->SetParent(_ParamText->transform);
	//_MaxParamText->transform->SetLocalPosition(Vector3(0.0f, 3.5f, 0.0f));


	//バフパラメータ初期化.
	_BuffText = INSTANCE(GameObjectManager)->AddNew<TextObject>("BuffText", 9);
	_BuffText->Initialize(L"", 25.0f);
	_BuffText->SetAnchor(fbText::TextAnchorE::UpperLeft);
	_BuffText->transform->SetParent(_ParamText->transform);
	_BuffText->transform->SetLocalPosition(Vector3(0.0f, 10.0f, 0.0f));
}

/**
* 更新.
*/
void ParameterRender::Update()
{
	_ParamNameText->SetText(_ParamName);

	char param[100] = { "" };

	switch (_ShowType)
	{
		case ParameterRender::Normal:
		{
			sprintf(param, "%d", _Param);
			_BuffText->SetText("");
			_ParamText->SetBlendColor(Color::white);
			break;
		}
		case ParameterRender::Max:
		{
			sprintf(param, "%d", _Param);
			strcat(param, " / ");
			char maxParam[100] = { "" };
			sprintf(maxParam, "%d", _MaxParam);
			strcat(param, maxParam);
			break;
		}
		case ParameterRender::Buff:
		{
			sprintf(param, "%d", _Param);
			char buff[100] = { "" };
			if (_ParamBuff != 0)
			{
				if (_ParamBuff > 0) {
					sprintf(buff, " ↑%d", abs(_ParamBuff));
					_BuffText->SetBlendColor(Color::blue);
					_ParamText->SetBlendColor(Color::yellow);
				}
				else {
					sprintf(buff, " ↓%d", abs(_ParamBuff));
					_BuffText->SetBlendColor(Color::red);
					_ParamText->SetBlendColor(Color::white * 0.3f);
				}
			}
			else
			{
				_ParamText->SetBlendColor(Color::white);
			}
			_BuffText->SetText(buff);

			break;
		}
		case ParameterRender::Equip:
		{
			sprintf(param, "+ %d ( %d ", _ParamNewEquip, _ParamEquip + _Param);
			char p[100] = { "" };
			if (_ParamEquip + _Param > _ParamNewEquip + _Param)
			{
				sprintf(p, "<color=ff0000ff>↓ %d</color>", _ParamNewEquip + _Param);
			}
			else if (_ParamEquip + _Param < _ParamNewEquip + _Param)
			{
				sprintf(p, "<color=0000ffff>↑ %d</color>", _ParamNewEquip + _Param);
			}
			else
			{
				sprintf(p, "→ %d", _ParamNewEquip + _Param);
			}
			strcat(param, p);
			strcat(param, " )");
			break;
		}
		case ParameterRender::Rank:
		{
			if (_ParamRank == HoldEquipment::Rank::None)
			{
				sprintf(param, "%s ", "-");
			}
			else
			{
				sprintf(param, "%s ", RankText[_ParamRank]);
			}

			char p[100] = { "" };
			if (_ParamRank < _ParamNewRank)
			{
				sprintf(p, "<color=ff0000ff>↓ %s</color>", (_ParamNewRank == HoldEquipment::Rank::None) ? "-" : RankText[_ParamNewRank]);
			}
			else if (_ParamRank > _ParamNewRank)
			{
				sprintf(p, "<color=0000ffff>↑ %s</color>", (_ParamNewRank == HoldEquipment::Rank::None) ? "-" : RankText[_ParamNewRank]);
			}
			else
			{
				sprintf(p, "→ %s", (_ParamNewRank == HoldEquipment::Rank::None) ? "-" : RankText[_ParamNewRank]);
			}

			strcat(param, p);

			break;
		}
	}

	_ParamText->SetText(param);
}
