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
	void SetParam(char* name,  char* iconName , int param,fbText::TextAnchorE anchor = fbText::TextAnchorE::MiddleRight, int buff = 0, int maxParam = INT_MIN, float nameTextSize = 40.0f, const Vector2& iconSize = Vector2(30.0f, 30.0f))
	{
		if (_IconImage->GetTexture() == nullptr) {
			_IconImage->SetTexture(LOADTEXTURE(iconName));
		}
		_IconImage->SetSize(iconSize);
		_ParamNameText->SetFontSize(nameTextSize);
		_ParamName = name;
		_Param = param;
		_ParamText->SetAnchor(anchor);
		_ParamBuff = buff;
		_MaxParam = maxParam;
	}

	// �p�����[�^�e�L�X�g�̈ʒu�ݒ�(���[�J�����W)�B
	inline void SetParamTextPos(const Vector3& localPos) {
		_ParamText->transform->SetLocalPosition(localPos);
	}
	// �p�����[�^�e�L�X�g�̈ʒu�擾�B
	inline const Vector3& GetParamTextPos()const {
		return _ParamText->transform->GetLocalPosition();
	}

	inline ImageObject* GetIconObject()const {
		return _IconImage;
	}
private:

	/** �p�����[�^���\��. */
	TextObject* _ParamNameText = nullptr;
	/** �p�����[�^��. */
	char* _ParamName = "���ݒ�";

	/** �p�����[�^�\��. */
	TextObject* _ParamText = nullptr;
	// �ő�p�����[�^�\���B
	TextObject* _MaxParamText = nullptr;
	/** �p�����[�^. */
	int _Param;
	/** �ő�p�����[�^. */
	int _MaxParam = INT_MIN;

	TextObject* _BuffText = nullptr;

	/** �p�����[�^�o�t�l(�}�C�i�X�Ȃ�f�o�t). */
	int _ParamBuff;

	/** �A�C�R���摜. */
	ImageObject* _IconImage = nullptr;

};