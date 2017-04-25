#include "AnimationPlate.h"

AnimationPlate::AnimationPlate(char * name) :
	Plate(name),
	_AutoDelete(false)
{
	
}

void AnimationPlate::Awake()
{
	_Primitive = AddComponent<PlatePrimitive>();
	_Animator = AddComponent<TextureAnimator>();
}

void AnimationPlate::Update()
{
	//UV�ݒ�
	_Primitive->SetUV(_Animator->GetUV());
	if(_AutoDelete)
	{
		//�A�j���[�V�����`�F�b�N
		if(_Animator->GetEnd())
		{
			//�I�����Ă���̂Ŏ����ō폜
			INSTANCE(GameObjectManager)->AddRemoveList(this);
		}
	}
}

void AnimationPlate::SetSplit(const int& X, const int& Y, const int& max)
{
	_Animator->SetSplit(X,Y,max);
}

void AnimationPlate::Play(const float& time, const int& loop, const int& start)
{
	_Animator->Play(time,loop,start);
}

void AnimationPlate::Pause()
{
	_Animator->Pause();
}

void AnimationPlate::SetDelete(const bool & f)
{
	_AutoDelete = f;
}
