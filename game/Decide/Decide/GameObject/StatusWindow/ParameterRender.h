/**
* パラメータ表示クラスの定義.
*/
#pragma once

#include"fbEngine\_Object\_GameObject\TextObject.h"
#include"fbEngine\_Object\_GameObject\ImageObject.h"
#include"GameObject\ItemManager\HoldItem\HoldEquipment.h"

/**
* パラメータ表示クラス.
*/
class ParameterRender : public GameObject
{
public:

	/**
	* 表示タイプ.
	*/
	enum ShowType
	{
		Normal,		//!< 通常.
		Max,		//!< 最大値使用.
		Buff,		//!< バフ使用.
		Equip,		//!< 装備使用.
		Rank,		//!< ランク使用.
	};

public:

	/**
	* コンストラクタ.
	*/
	ParameterRender(const char* name) :
		GameObject(name)
	{
	}

	/**
	* デストラクタ.
	*/
	~ParameterRender()
	{
	}

	/**
	* 初期化.
	*/
	void Awake()override;

	/**
	* 更新.
	*/
	void Update()override;

	/**
	* パラメータ設定.
	*/
	void SetParam(char* name, char* iconName, int param, fbText::TextAnchorE paramAnchor = fbText::TextAnchorE::MiddleRight, float nameTextSize = 40.0f, const Vector2& iconSize = defaultIconSize)
	{
		_SetRenderStyle(ShowType::Normal,iconName, iconSize, name,nameTextSize,param,paramAnchor);
	}
	/**
	* パラメータ設定. 
	*/
	void SetParamMax(char* name,char* iconName, int param,int maxParam, fbText::TextAnchorE paramAnchor = fbText::TextAnchorE::MiddleRight, float nameTextSize = 40.0f, const Vector2& iconSize = defaultIconSize)
	{
		_SetRenderStyle(ShowType::Max, iconName, iconSize, name, nameTextSize, param, paramAnchor);
		_MaxParam = maxParam;
	}

	/**
	* パラメータ設定.
	*/
	void SetParamBuff(char* name, char* iconName, int param, int buff, fbText::TextAnchorE paramAnchor = fbText::TextAnchorE::MiddleRight, float nameTextSize = 40.0f, const Vector2& iconSize = defaultIconSize)
	{
		_SetRenderStyle(ShowType::Buff, iconName, iconSize, name, nameTextSize, param, paramAnchor);
		_ParamBuff = buff;
	}
	/**
	* パラメータ設定.
	*/
	void SetParamEquip(char* name, char* iconName, int param, int equip, int newEquip)
	{
		_SetRenderStyle(ShowType::Equip, iconName, defaultIconSize, name, defaultNameTextSize, param, fbText::TextAnchorE::MiddleRight);
		_ParamEquip = equip;
		_ParamNewEquip = newEquip;
	}
	/**
	* パラメータ設定.
	*/
	void SetParamRank(char* name, char* iconName, HoldEquipment::Rank rank, HoldEquipment::Rank newRank)
	{
		_SetRenderStyle(ShowType::Rank, iconName, defaultIconSize, name, defaultNameTextSize, 0, fbText::TextAnchorE::MiddleRight);
		_ParamRank = rank;
		_ParamNewRank = newRank;
	}


	// パラメータテキストの位置設定(ローカル座標)。
	inline void SetParamTextPos(const Vector3& localPos) {
		_ParamText->transform->SetLocalPosition(localPos);
	}
	// パラメータテキストの位置取得。
	inline const Vector3& GetParamTextPos()const {
		return _ParamText->transform->GetLocalPosition();
	}

	inline ImageObject* GetIconObject()const {
		return _IconImage;
	}

private:
	inline void _SetRenderStyle(ShowType type,char* iconName,  const Vector2& iconSize,char* nameText, float nameTextSize, int param, fbText::TextAnchorE paramAnchor) {
		_ShowType = type;
		if (_IconImage->GetTexture() == nullptr) {
			_IconImage->SetTexture(LOADTEXTURE(iconName));
		}
		_IconImage->SetSize(iconSize);
		_ParamName = nameText;
		_Param = param;
		_ParamText->SetAnchor(paramAnchor);
	}

private:

	/** パラメータ名表示. */
	TextObject* _ParamNameText = nullptr;
	/** パラメータ名. */
	char* _ParamName = "未設定";

	/** パラメータ表示. */
	TextObject* _ParamText = nullptr;
	// 最大パラメータ表示。
	TextObject* _MaxParamText = nullptr;
	/** パラメータ. */
	int _Param;
	/** 最大パラメータ. */
	int _MaxParam = INT_MIN;

	TextObject* _BuffText = nullptr;

	/** パラメータバフ値(マイナスならデバフ). */
	int _ParamBuff;

	/** 現在装備. */
	int _ParamEquip;
	/** 候補装備. */
	int _ParamNewEquip;

	HoldEquipment::Rank _ParamRank;
	HoldEquipment::Rank _ParamNewRank;

	/** アイコン画像. */
	ImageObject* _IconImage = nullptr;

	ShowType _ShowType = ShowType::Normal;

	const static float defaultNameTextSize;
	const static Vector2 defaultIconSize;
};