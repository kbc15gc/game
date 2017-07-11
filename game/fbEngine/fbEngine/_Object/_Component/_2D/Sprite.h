#pragma once
#include "_Object\_Component\Component.h"

class Vertex;

namespace fbSprite
{
	//�G�t�F�N�g�̃t���O
	enum class SpriteEffectE : unsigned int
	{
		NONE = BIT(0),		//�Ȃ�				00000000
		FADE = BIT(1),		//�t�F�[�h�t���O	00000010
		FADEOUT = BIT(1),	//�t�F�[�h�A�E�g	00000010
		FADEIN = BIT(1) +1,	//�t�F�[�h�C��		00000011
		SHADOW = BIT(2),	//�e�`��			00000100
		OUTLINE = BIT(3),	//�ӂ��ǂ�			00001000
		//�������@MASK = 16,//�}�X�N����		
	};
}

class Sprite:public Component
{
public:
	Sprite(GameObject* g, Transform* t);
	void Start()override;
	void Update()override;
	void ImageRender()override;
	
	void SetTexture(TEXTURE* tex)
	{
		_Texture = tex;
		if (_Texture)
			_Size = _Texture->Size;
	}
	void SetSize(const Vector2& size)
	{
		_Size = size;
	}

	const Vector2& GetSize()
	{
		return _Size;
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
	//���ʕ`��t���O�Z�b�g
	void SetDoubleSided(const bool& f)
	{
		_DoubleSided = f;
	}
private:
	//�`�悷��
	void _RenderSprite();
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
	//�摜�T�C�Y
	Vector2 _Size;
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
	//���ʕ`��
	bool _DoubleSided;

	//���_
	static Vertex* _Vertex;
};