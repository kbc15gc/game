#pragma once
//���������֐�
class Random
{
public:
	//�͈͓��̒l��Ԃ�
	static int Range(int min, int max);
	//0.0~1.0�̊Ԃ̒l��Ԃ�
	static double RandDouble();
	//������Ԃ�
	static int RandInt();
	//�����V�[�h�ݒ�
	static void Seed(int seed)
	{
		srand(seed);
	}
};