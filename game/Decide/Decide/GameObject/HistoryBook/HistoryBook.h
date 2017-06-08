#pragma once
#include "fbEngine\_Object\_GameObject\GameObject.h"
#include "HFSM\HistoryBookStateClose.h"
#include "HFSM\HistoryBookStateOpening.h"
#include "HFSM\HistoryBookStateOpen.h"
#include "HFSM\HistoryBookStateCloseing.h"

class Player;

//���j���B
class HistoryBook : public GameObject
{
public:
	//���j���̃X�e�[�g�B
	enum class State
	{
		Close= 0,	//������ԁB
		Opening,	//�{���J���Ă���B
		Open,		//�J������ԁB
		Closeing,	//�{�����Ă���B
	};
	//���j���̃A�j���[�V�����i���o�[�B
	enum class AnimationNo
	{
		AnimationInvalid = -1,	//�����B
		AnimationClose,			//�{��������ԁB
		AnimationOpening,		//�{���J���Ă����ԁB
		AnimationOpen,			//�{���J�����܂܂̏�ԁB
		AnimationCloseing,		//�{�����Ă����ԁB
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

	//���j�������Ă��邩�ǂ����̃t���O���擾�B�B
	bool GetIsLookAtHistoryFlag() const
	{
		return _IsLookAtHistoryFlag;
	}

	//���j�������Ă��邩�̃t���O�̕ύX�B
	void ChangeIsLookAtHistoryFlag();

	//�X�e�[�g�̕ύX�B
	void ChangeState(State nextstate);

	//���j���̃R���|�[�l���g��L���ɂ��邩�����ɂ��邩�̃t���O�̐ݒ�B
	void SetEnable(bool flag)
	{
		_Model->enable = flag;
	}

	//�Đ������̃t���O���擾�B
	bool GetIsPlay()
	{
		if (_Anim != nullptr)return _Anim->GetPlaying();
	}
private:
	SkinModel*					_Model = nullptr;									//���f���̃R���|�[�l���g�B�̃A�h���X�ێ��p�B
	Animation*					_Anim = nullptr;									//�A�j���[�V�����̃R���|�[�l���g�̃A�h���X�ێ��p�B
	double						_AnimationEndTime[(int)AnimationNo::AnimationNum] = { 0.0f,0.0f,0.0f,0.0f };	//�A�j���[�V�����̏I�����ԁB
	State						_State;												//���j���̏�ԁB
	bool						_IsLookAtHistoryFlag = false;						//���j�������Ă��邩�ǂ����̃t���O(�ŏ��͌��Ă��Ȃ��̂�false)�B
	Player*						_Player = nullptr;									//�v���C���[�̃R���|�[�l���g�B
	Vector3						_PlayerFoward;
	HistoryBookState*			_CurrentState = nullptr;							//���j���̌��݂̏�ԁB
	HistoryBookStateClose		_CloseState;										//���j���̏�ԁ@������ԁB
	HistoryBookStateOpening		_OpeningState;										//���j���̏�ԁ@�J���Ă����ԁB
	HistoryBookStateOpen		_Open;												//���j���̏�ԁ@�J������ԁB
	HistoryBookStateCloseing	_Closeing;											//���j���̏�ԁ@���Ă����ԁB
	float						_AngleY = 0.0f;
};