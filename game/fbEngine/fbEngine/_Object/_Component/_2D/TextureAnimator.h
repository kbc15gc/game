#pragma once
#include "_Object\_Component\Component.h"

//�e�N�X�`�����A�j���[�V���������邽�߂�UV���v�Z����
class TextureAnimator :public Component
{
public:
	TextureAnimator(GameObject* g, Transform* t);
	void Awake()override;
	void Update()override;

	//�A�j���[�V�����Đ�
	//float �Đ�����
	//int ���[�v��(�����́�)
	//int �Đ��J�n����
	void Play(const float& time ,const int& loop = -1, const int& start = 0);
	//�Đ��ꎞ��~
	void Pause();
	//�摜�̕������ݒ�
	//int�@�A�j���[�V�����̉��̖���
	//int�@�A�j���[�V�����̏c�̖���
	//int�@�A�j���[�V�����̖���
	void SetSplit(const int& X, const int& Y, const int& max);
	//uv���擾
	const Vector4& GetUV();
	const bool& GetEnd();
private:
	//UV�v�Z
	void _CalcUV();
private:
	//�Đ���
	bool _isPlaying;
	//�Đ��I������
	bool _End;
	//���[�v��
	int _LoopNum;
	//loop�J�E���g
	int _LoopCount;
	//�c�Ɖ��̕�����
	Vector2 _Division;
	//0�`1�ɐ��K����������1��Ԃ̏c������
	double _WidthX,_WidthY;
	//UVxy��u��zw��v��\���B
	Vector4 _UV;
	//���������ڂ̃A�j���[�V�������H
	int _NowAnimNum;
	//�A�j���[�V�����̍ő喇��
	int _MaxAnimNum;
	//�Đ�����(���b�����čĐ����邩�H)
	float _PlaybackTime;
	//1��������̎���
	float _TimePerSheet;
	//�^�C�}�[
	float _Timer;
};