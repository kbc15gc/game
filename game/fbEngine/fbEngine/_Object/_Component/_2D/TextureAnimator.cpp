#include"fbstdafx.h"
#include "TextureAnimator.h"

TextureAnimator::TextureAnimator(GameObject * g, Transform * t):
	Component(g, t, typeid(this).name())
{

}

void TextureAnimator::Awake()
{
	_isPlaying = false;
	_End = false;
	_LoopNum = 0;
	_LoopCount = 0;
	_Division = Vector2(0, 0);
	_UV = Vector4(0, 1, 0, 1);
	_NowAnimNum = 0;
	_MaxAnimNum = 0;
	_PlaybackTime = 0.0f;
	_TimePerSheet = 0.0f;
	_Timer = 0.0f;
}

void TextureAnimator::Update()
{
	//�A�j���[�V�����Đ��� &&
	//�ݒ肳��Ă���B
	if (_isPlaying && 0 < _MaxAnimNum)
	{
		_Timer += Time::DeltaTime();
		//�Đ����Ԃ𒴂���
		if (_Timer > _TimePerSheet)
		{
			//�����i�񂾂��v�Z
			int add = _Timer / _TimePerSheet;
			//�c�������Ԉ��p��
			_Timer -= _TimePerSheet * add;
			//���̐�
			int NowTmp = _NowAnimNum + add;
			//�J�E���g�i�߂�H
			_LoopCount += NowTmp / (_MaxAnimNum-1);

			if (NowTmp < _MaxAnimNum ||	//�ő吔��菬����
				_LoopNum < 0 ||			//�����[�v
				_LoopCount < _LoopNum)	//loop����
			{
				//������i�߂�
				_NowAnimNum = NowTmp % _MaxAnimNum;
				//UV�v�Z
				_CalcUV();
			}
			else
			{
				_isPlaying = false;
				_End = true;
				_NowAnimNum = 0;
				_UV = Vector4(0, 1, 0, 1);
			}
		}
	}
}

void TextureAnimator::Play(const float& time, const int& loop, const int& start)
{
	_isPlaying = true;
	_End = false;
	_LoopNum = loop;
	_LoopCount = 0;
	_NowAnimNum = min(start, _MaxAnimNum - 1);
	//UV�v�Z
	_CalcUV();

	_PlaybackTime = time;
	_TimePerSheet = _PlaybackTime / (float)_MaxAnimNum;
	_Timer = 0.0f;
}

void TextureAnimator::Pause()
{
	_isPlaying = false;
}

void TextureAnimator::SetSplit(const int& X, const int& Y, const int& max)
{
	_Division.x = X;
	_Division.y = Y;
	//1��؂肠����̕��ƍ���
	_WidthX = 1.0 / (double)_Division.x;
	_WidthY = 1.0 / (double)_Division.y;
	//�A�j���[�V�����̍ő喇���ݒ�
	_MaxAnimNum = max;
}

const Vector4& TextureAnimator::GetUV()
{
	return _UV;
}

const bool & TextureAnimator::GetEnd()
{
	return _End;
}

void TextureAnimator::_CalcUV()
{
	//�c���̉����ڂ�
	int nowX, nowY;
	nowX = _NowAnimNum % (int)_Division.x;
	nowY = _NowAnimNum / (int)_Division.x;
	//UV�v�Z
	_UV.x = nowX * _WidthX;
	_UV.y = _UV.x + _WidthX;
	_UV.z = nowY * _WidthY;
	_UV.w = _UV.z + _WidthY;
}
