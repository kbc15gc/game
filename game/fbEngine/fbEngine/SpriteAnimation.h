#pragma once
#include "Component.h"
#include "Sprite.h"

class SpriteAnimation:public Component
{
public:
	SpriteAnimation(GameObject* g, Transform* t);
	void Awake()override;
	void Start()override;
	void Update()override;
	void ImageRender()override;

	//�A�j���[�V�����Đ�
	//�Đ�����
	//�Đ��J�n����
	void Play(float time ,int start = 0);
	void SetTexture(TEXTURE* tex,int X,int Y);
private:
	//�X�v���C�g
	Sprite* _Sprite;
	//�Đ�
	bool _Play;
	//���[�v
	bool _Loop;
	//�c�Ɖ��̕�����
	Vector2 _Division;
	//�A�j���[�V�����ꖇ�������uv
	Vector2 _UV;
	//���������ڂ̃A�j���[�V�������H
	int _NowAnimNum;
	//�A�j���[�V�����̍ő喇��
	int _MaxAnimNum;
	//�Đ�����(���b�����čĐ����邩�H)
	float _PlaybackTime;
	//�^�C�}�[
	float _Timer;
};