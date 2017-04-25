#pragma once
#include "_Object\_GameObject\Plate.h"
#include "_Object\_Component\_2D\TextureAnimator.h"

class AnimationPlate :public Plate
{
public:
	AnimationPlate(char* name);
	void Awake()override;
	void Update();

	//�摜�̕������ݒ�
	//int�@�A�j���[�V�����̉��̖���
	//int�@�A�j���[�V�����̏c�̖���
	//int�@�A�j���[�V�����̖���
	void SetSplit(const int& X, const int& Y, const int& max);
	//�A�j���[�V�����Đ�
	//float �Đ�����
	//bool ���[�v���邩�H
	//int �Đ��J�n����
	void Play(const float& time, const int& loop = -1, const int& start = 0);
	//�Đ��ꎞ��~
	void Pause();
	void SetDelete(const bool& f);
private:	
	TextureAnimator* _Animator;
	//�A�j���[�V�������Đ��I�������Ȃ珟��ɍ폜����
	bool _AutoDelete;
};