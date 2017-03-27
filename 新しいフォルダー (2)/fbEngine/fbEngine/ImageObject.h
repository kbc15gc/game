#pragma once
#include "GameObject.h"
#include "Sprite.h"

//�摜��\������Q�[���I�u�W�F�N�g
class ImageObject :public GameObject
{
public:
	ImageObject(char* name) :GameObject(name) {}
	void Awake()override;

	void SetTexture(TEXTURE* t);
	TEXTURE* GetTexture();
	void SetPivot(const Vector2& v);
	void SetPivot(const float& x, const float& y);
	//������F�ݒ�
	void SetBlendColor(const Color& c);
	//�폜����F�ݒ�
	void SetClipColor(const Color& c);
	//���̂܂܃Z�b�g
	void SetEffectFlg(const fbSprite::SpriteEffectE& e);
	//true�Ȃ瑫���Afalse�Ȃ����
	void SetEffectFlg(const fbSprite::SpriteEffectE& e,bool f);
private:
	Sprite* _Sprite;
};
