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
	void SetParam(char* name,char* iconName, int* param, int* maxParam = nullptr)
	{
		_IconImage->SetTexture(LOADTEXTURE(iconName));
		_IconImage->SetSize(Vector2(30.0f, 30.0f));
		_ParamName = name;
		_Param = param;
		_MaxParam = maxParam;
	}

private:

	/** パラメータ名表示. */
	TextObject* _ParamNameText = nullptr;
	/** パラメータ名. */
	char* _ParamName = "未設定";

	/** パラメータ表示. */
	TextObject* _ParamText = nullptr;
	/** パラメータ. */
	int* _Param = nullptr;
	/** 最大パラメータ. */
	int* _MaxParam = nullptr;

	/** アイコン画像. */
	ImageObject* _IconImage = nullptr;

};