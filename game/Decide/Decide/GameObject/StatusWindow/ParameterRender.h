/**
* �p�����[�^�\���N���X�̒�`.
*/
#pragma once

#include"fbEngine\_Object\_GameObject\TextObject.h"
#include"fbEngine\_Object\_GameObject\ImageObject.h"

/**
* �p�����[�^�\���N���X.
*/
class ParameterRender : public GameObject
{
public:

	/**
	* �R���X�g���N�^.
	*/
	ParameterRender(const char* name) :
		GameObject(name)
	{
	}

	/**
	* �f�X�g���N�^.
	*/
	~ParameterRender()
	{
	}

	/**
	* ������.
	*/
	void Awake()override;

	/**
	* �X�V.
	*/
	void Update()override;

	/**
	* �p�����[�^�ݒ�. 
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

	/** �p�����[�^���\��. */
	TextObject* _ParamNameText = nullptr;
	/** �p�����[�^��. */
	char* _ParamName = "���ݒ�";

	/** �p�����[�^�\��. */
	TextObject* _ParamText = nullptr;
	/** �p�����[�^. */
	int* _Param = nullptr;
	/** �ő�p�����[�^. */
	int* _MaxParam = nullptr;

	/** �A�C�R���摜. */
	ImageObject* _IconImage = nullptr;

};