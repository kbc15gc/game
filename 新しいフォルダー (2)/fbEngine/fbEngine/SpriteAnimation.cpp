#include "SpriteAnimation.h"

SpriteAnimation::SpriteAnimation(GameObject * g, Transform * t):
	Component(g, t, typeid(this).name())
{

}

void SpriteAnimation::Awake()
{
}

void SpriteAnimation::Start()
{
}

void SpriteAnimation::Update()
{
	//�A�j���[�V�����Đ����Ȃ�
	if (_Play)
	{
		_Timer += Time::DeltaTime();
		//�Đ����Ԃ𒴂���
		if (_Timer > _PlaybackTime)
		{
			//���Z�b�g�H
			_Timer = 0.0f;
		}
		//���Ԃ̌o�߂��猻�݂̖������Z�o
		_NowAnimNum = (_Timer / _PlaybackTime) * _MaxAnimNum;
	}
}

void SpriteAnimation::ImageRender()
{
}

void SpriteAnimation::Play(float time, int start)
{
	//�e�N�X�`�����ݒ肳��Ă���B
	if (_MaxAnimNum > 0)
	{
		_Play = true;
		_PlaybackTime = time;
		_NowAnimNum = start;
		_Timer = ((float)_NowAnimNum / (float)_MaxAnimNum) * _PlaybackTime;
	}
}

void SpriteAnimation::SetTexture(TEXTURE * tex, int X, int Y)
{
	//�A�j���[�V�����̍ő喇���ݒ�
	_MaxAnimNum = X * Y;
}
