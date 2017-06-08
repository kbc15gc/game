#pragma once
//�R���W�����Ŏg��enum�񋓌^���`

namespace
{
	//�R���W�����ɐݒ�ł���t�B���^�[(short�^)
	//�g�p����Ƃ��͂��̂܂܂ł͂Ȃ��킩��₷�����O�̗񋓌^�����Ƃ悢�B
	enum class fbCollisionFilterE : short
	{
		NOTHING = 0,
		A = 1,
		B = BIT(1),
		C = BIT(2),
		D = BIT(3),
		E = BIT(4),
		F = BIT(5),
		G = BIT(6),
		H = BIT(7),
		I = BIT(8),
		J = BIT(9),
		K = BIT(10),
		L = BIT(11),
		M = BIT(12),
		N = BIT(13),
		O = BIT(14),
		ALLFILTER = BIT(15) - 1,	//short�^�Ȃ̂ōő吔��32767(2^15 - 1)
	};

	//�R���W�����̑���(int)
	enum class fbCollisionAttributeE : int
	{
		NOTHING = 0,
		CHARACTER = 1,
		GROUND = BIT(1),
		WALL = BIT(2),

		ALL = BIT(30) - 1,	//
	};
}