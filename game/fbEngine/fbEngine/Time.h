#pragma once
#pragma comment(lib, "winmm.lib ")
#include <time.h>
#include <mmsystem.h>

//���ԃN���X
class Time
{
public:
	Time(){};
	static void Update();
	//�f���^�^�C���擾
	static const double DeltaTime()
	{
		if (deltaTime > (1.0 / 30.0))
		{
			return (1.0 / 30.0);
		}
		return deltaTime;
	}
	//FPS(��)�擾
	static const float& Fps()
	{
		return fps;
	}
private:
	//�O��̃A�b�v�f�[�g�ƍ��̃A�b�v�f�[�g�Ƃ̎������i�[(�~���b�P��)
	static double deltaTime;
	//�O��̎��Ԋi�[
	static DWORD  beforeTime;
	//�t���[���p�[�Z�J���h(1�b�Ԃɂł�t���[���̐�)
	static float fps;

	//�t���[����
	static float fream;
	static double time;
};