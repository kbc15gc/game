/**
* パラメータ表示クラスの定義.
*/
#pragma once

#include"fbEngine\_Object\_GameObject\TextObject.h"
#include"fbEngine\_Object\_GameObject\ImageObject.h"

/**
* パラメータ表示クラス.
*/
class ParameterRender : public GameObject
{
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
	void SetParam(char* name,char* iconName, int param,int buff = 0,int maxParam = INT_MIN)
	{
		_IconImage->SetTexture(LOADTEXTURE(iconName));
		_IconImage->SetSize(Vector2(30.0f, 30.0f));
		_ParamName = name;
		_Param = param;
		_ParamBuff = buff;
		_MaxParam = maxParam;
	}

	// パラメータテキストの位置設定(ローカル座標)。
	inline void SetParamTextPos(const Vector3& localPos) {
		_ParamText->transform->SetLocalPosition(localPos);
	}
	// パラメータテキストの位置取得。
	inline const Vector3& GetParamTextPos()const {
		return _ParamText->transform->GetLocalPosition();
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

	/** アイコン画像. */
	ImageObject* _IconImage = nullptr;

};