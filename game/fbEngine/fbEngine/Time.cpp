#include "Time.h"


//�����,timeGetTime���g��ver
//���ɂ͍�����\�J�E���^�ƍ�����\���g�����g����,��萸�x�̍������@������(KBCFreamwork�ł݂�)

double Time::deltaTime = 0.0f;
//�p�\�R�����N�����ĉ��~���b�o�߂������擾
DWORD Time::beforeTime = timeGetTime();
float Time::fps = 0;

float Time::fream = 0;
double Time::time = 0;

void Time::Update()
{
	//���̎��Ԃ��擾
	DWORD afterTime = timeGetTime();
	//�O��ƍ��̍����f���^�^�C���ɐݒ�(�~���b��b�ɕϊ�)
	deltaTime = (afterTime - beforeTime) / 1000.0f;
	//before���X�V
	beforeTime = afterTime;
	
	time += deltaTime;
	fream++;

	//1�b�o�߂����Ȃ�
	if (time >= 1.0f)
	{
		//���ߕ�
		double over = (double)1.0f / time;
		//FPS�X�V
		fps = (float)(fream * over);

		time = 0;
		fream = 0;
	}
}