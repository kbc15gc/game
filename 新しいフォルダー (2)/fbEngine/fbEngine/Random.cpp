#include "Random.h"

int Random::Range(int min,int max) {
	max += 1;
	//�傫�����t
	if (min > max)
	{
		int w = max;
		max = min;
		min = w;
	}
	int ret;

	//�}�C�i�X���ǂ���
	bool a, b;
	a = b = false;
	if (min < 0)
		a = true;

	if (max < 0)
		b = true;


	//-
	if (a)
	{
		//-
		if (b)
		{
			int dif = max - min;
			ret = -(rand() % dif) + min;
		}//+
		else
		{
			ret = (rand() % max) + min;
		}
	}
	//+
	else
	{
		//+
		if (!b)
		{
			int dif = max - min;
			ret = (rand() % dif) + min;
		}
	}
	return ret;
}

double Random::RandDouble()
{
	//15��
	int d = rand();
	double dou = (float)d / 32767.0f;
	return dou;
}

int Random::RandInt()
{
	return rand();
}
