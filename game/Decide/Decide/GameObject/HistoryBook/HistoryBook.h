#pragma once
#include "fbEngine\_Object\_GameObject\GameObject.h"

class HistoryBook : public GameObject
{
public:
	//���j���̃X�e�[�g
	enum class State
	{
		//IdolClose = 0,		//������ԁB
		Open = 0,				//�{���J���Ă���B
		IdolOpen,			//�J������ԁB
		Close,				//�{�����Ă���B
	};
	//���j���̃A�j���[�V�����i���o�[
	enum class AnimationNo
	{
		AnimationInvalid = -1,		//����
		AnimationOpen,				//�{���J���A�j���[�V�����B
		AnimationNum,
		
	};
	HistoryBook(const char* name);

	void Awake()override;
	void Start()override;
	void Update()override;
	//�A�j���[�V�����Đ�
	// animno �A�j���[�V�����̃i���o�[
	// interpolatetime ��Ԏ���
	// loopnum ���[�v�� (�f�t�H���g��-1)
	void PlayAnimation(AnimationNo animno, float interpolatetime, int loopnum = -1);
private:
	Animation* _Anim;											//�A�j���[�V�����̃R���|�[�l���g�̃A�h���X�ێ��p�B
	double _AnimationEndTime[(int)AnimationNo::AnimationNum];	//�A�j���[�V�����̏I������
	State HistoryBookState;
	GameObject* _Player = nullptr;
	GameObject* _GameCamera = nullptr;
};