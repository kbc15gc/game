#pragma once
#include "Component.h"

class Vertex;

namespace fbSprite
{
	//�G�t�F�N�g�̃t���O
	enum class SpriteEffectE : unsigned int
	{
		NONE = 0,		//�Ȃ�				00000000
		FADE = 2,		//�t�F�[�h�t���O	00000010
		FADEOUT = 2,	//�t�F�[�h�A�E�g	00000010
		FADEIN = 3,		//�t�F�[�h�C��		00000011
		SHADOW = 4,		//�e�`��			00000100
		OUTLINE = 8,	//�ӂ��ǂ�			00001000
		//�������@MASK = 16,		//�}�X�N����		
	};
}

class Sprite:public Component
{
public:
	Sprite(GameObject* g, Transform* t);
	void Awake()override;
	void Start()override;
	void Update()override;
	void ImageRender()override;
	
	void SetTexture(TEXTURE* tex)
	{
		_Texture = tex;
	}
	TEXTURE* GetTexture()
	{
		return _Texture;
	}
	void SetPivot(const Vector2& v)
	{
		_Pivot.x = min(v.x, 1.0f);
		_Pivot.y = min(v.y, 1.0f);
	}
	void SetUV(const Vector4& uv)
	{
		_UV = uv;
	}
	void SetBlendColor(const Color& c)
	{
		_BlendColor = c;
	}
	void SetClipColor(const Color& c)
	{
		_ClipColor = c;
	}
	//���̂܂܃Z�b�g
	void SetEffectFlg(const DWORD& e);
	//true�Ȃ瑫���Afalse�Ȃ����
	//�߂�l�͐����������ǂ����H
	bool SetEffectFlg(const DWORD& e, bool f);

private:
	//�t�F�[�h������
	void Fade();
	//�e�̐���
	void _CreateShadow();
	//�ӂ��̐���
	void _CreateOutLine();
private:
	//�G�t�F�N�g
	Effect* _Effect;
	//�e�N�X�`��
	TEXTURE* _Texture;
	//��_
	Vector2 _Pivot;
	//UVxy��u��zw��v��\���B
	Vector4 _UV;
	//������F
	Color _BlendColor;
	//�폜����F
	Color _ClipColor;
	//�����鎞��
	float _FadeTime;
	//�^�C�}�[
	float _FTimer;
	//�����邵�����l
	float _FadeLine;
	//�X�v���C�g�ɂ�����G�t�F�N�g�̃t���O
	DWORD _SpriteEffect;

	//���_
	static Vertex* _Vertex;
};