#pragma once
#include "fbEngine\_Object\_GameObject\GameObject.h"

//���j���B
class HistoryBook : public GameObject
{
public:
	//���j���̃X�e�[�g�B
	enum class State
	{
		IdolClose = 0,		//������ԁB
		Open,				//�{���J���Ă���B
		IdolOpen,			//�J������ԁB
		Close,				//�{�����Ă���B
	};
	//���j���̃A�j���[�V�����i���o�[�B
	enum class AnimationNo
	{
		AnimationInvalid = -1,	//�����B
		AnimationCloseIdol,		//�{��������ԁB
		AnimationOpen,			//�{���J���Ă����ԁB
		AnimationOpenIdol,		//�{���J�����܂܂̏�ԁB
		AnimationClose,			//�{�����Ă����ԁB
		AnimationNum,
		
	};
	HistoryBook(const char* name);
	void Awake()override;
	void Start()override;
	void Update()override;
	//�A�j���[�V�����Đ��B
	// animno �A�j���[�V�����̃i���o�[�B
	// interpolatetime ��Ԏ��ԁB
	// loopnum ���[�v�� (�f�t�H���g��-1)�B
	void PlayAnimation(AnimationNo animno, float interpolatetime, int loopnum = -1);

	//���j�������Ă��邩�ǂ����̃t���O�B
	bool GetIsLookAtHistoryFlag() const
	{
		return _IsLookAtHistoryFlag;
	}
private:
	Animation* _Anim;											//�A�j���[�V�����̃R���|�[�l���g�̃A�h���X�ێ��p�B
	double _AnimationEndTime[(int)AnimationNo::AnimationNum];	//�A�j���[�V�����̏I������
	State _HistoryBookState;										//���j���̏�ԁB
	bool _IsLookAtHistoryFlag = false;							//���j�������Ă��邩�ǂ����̃t���O�B
};