/**
* ���j���N���X�̒�`.
*/
#pragma once
#include "fbEngine\_Object\_GameObject\GameObject.h"

//�X�e�[�g.
#include "HFSM\HistoryBookStateClose.h"
#include "HFSM\HistoryBookStateOpening.h"
#include "HFSM\HistoryBookStateOpen.h"
#include "HFSM\HistoryBookStateCloseing.h"

class Player;

/**
* ���j���N���X.
*/
class HistoryBook : public GameObject
{
public:
	
	/**
	* ���j���̏�ԃR�[�h.
	*/
	enum class StateCodeE
	{
		Invalid = -1,	//!< �ݒ�Ȃ�.
		Close,			//!< ������ԁB
		Opening,		//!< �{���J���Ă���B
		Open,			//!< �J������ԁB
		Closeing,		//!< �{�����Ă���B
	};

	/**
	* ���j���̃A�j���[�V�����R�[�h.
	*/
	enum class AnimationNo
	{
		AnimationInvalid = -1,	//!< �����B
		AnimationClose,			//!< �{��������ԁB
		AnimationOpening,		//!< �{���J���Ă����ԁB
		AnimationOpen,			//!< �{���J�����܂܂̏�ԁB
		AnimationCloseing,		//!< �{�����Ă����ԁB
		AnimationNum,			//!< �A�j���[�V������.
	};

	/**
	* �R���X�g���N�^.
	*/
	HistoryBook(const char* name) :
		GameObject(name)
	{
	}

	/**
	* �f�X�g���N�^.
	*/
	~HistoryBook()
	{
	}

	/**
	* �R���X�g���N�^��̏�����.
	*/
	void Awake()override;

	/**
	* ������.
	*/
	void Start()override;

	/**
	* �X�V.
	*/
	void Update()override;

	/**
	* ���j�����J���Ă��锻��t���O���擾.
	*/
	bool GetIsLookAtHistoryFlag() const
	{
		return _IsLookAtHistoryFlag;
	}

	/**
	* ���j����\���t���O��ݒ�.
	*/
	void SetEnable(bool flag)
	{
		_Model->enable = flag;
	}

	/**
	* �A�j���[�V�����̍Đ�������擾.
	*/
	bool GetIsPlay()
	{
		if (_Anim != nullptr)
		{
			return _Anim->GetPlaying();
		}
	}

	/**
	* �A�j���[�V�����̍Đ�.
	*
	* @param animno				�A�j���[�V�����̃i���o�[.
	* @param interpolatetime	��Ԏ���.
	* @param loopnum			���[�v�� (�f�t�H���g��-1).
	*/
	void PlayAnimation(AnimationNo animno, float interpolatetime, int loopnum = -1);
	
	/**
	* ��Ԃ̕ύX.
	*
	* @param nextState	���̏��.
	*/
	void ChangeState(StateCodeE nextState);

private:

	/**
	* ��ԃ��X�g�̏�����.
	*/
	void _InitState();

	/**
	* ���j�����J���Ă��锻��t���O��ύX.
	*/
	void _ChangeIsLookAtHistoryFlag();

private:

	/** ���j���̃��f��. */
	SkinModel* _Model = nullptr;
	/** ���j���̃A�j���[�V����. */
	Animation* _Anim = nullptr;
	/** �A�j���[�V�����̏I������. */
	double _AnimationEndTime[(int)AnimationNo::AnimationNum] = { 0.0f,0.0f,0.0f,0.0f };
	
	/** ���j���̌��݂̏��. */
	int _NowState = (int)StateCodeE::Invalid;
	/** ���j���̏�ԃ��X�g, */
	vector<HistoryBookState*> _StateList;
	
	/** ���̊p�x? */
	float _AngleY = 0.0f;

	/** ���j�������Ă��邩�ǂ����̃t���O(�ŏ��͌��Ă��Ȃ��̂�false). */
	bool _IsLookAtHistoryFlag = false;
	/** �v���C���[�̃|�C���^. */
	Player*	_Player = nullptr;
	/** �v���C���[�̑O����. */
	Vector3	_PlayerFoward = Vector3::zero;

};