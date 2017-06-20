/**
* ���j���N���X�̒�`.
*/
#pragma once

//���.
#include"HFSM\HistoryBookStateUnused.h"
#include"HFSM\HistoryBookStateIdol.h"
#include"HFSM\HistoryBookStateMove.h"
#include"HFSM\HistoryBookStateOpen.h"
#include"HFSM\HistoryBookStateClose.h"

class Player;

using HistoryBookStatePtr = unique_ptr<IHistoryBookState>;

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
		Invalid = -1,	//!< ����.
		Unused,			//!< ���g�p.
		Idol,			//!< �������.
		Move,			//!< �ړ�.
		Open,			//!< �J��.
		Close,			//!< ����.
		StateNum,		//!< ��Ԃ̐�.
	};

	/**
	* ���j���̃A�j���[�V�����R�[�h.
	*/
	enum class AnimationCodeE
	{
		Invalid = -1,	//!< ����.
		CloseIdol,		//!< �ҋ@_��.
		OpenIdol,		//!< �ҋ@_�J.
		Open,			//!< �J���A�j���[�V����.
		Close,			//!< ����A�j���[�V����.
		AnimationNum,	//!< �A�j���[�V������.
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
	* ���݂̏�Ԃ��擾.
	*/
	int GetNowState() const
	{
		return _NowState;
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
	* @param animNo				�A�j���[�V�����̃i���o�[.
	* @param interpolatetime	��Ԏ���.
	* @param loopnum			���[�v�� (�f�t�H���g��-1).
	*/
	void PlayAnimation(AnimationCodeE animNo, float interpolatetime, int loopnum = -1);
	
	/**
	* ��Ԃ̕ύX.
	*
	* @param nextState	���̏��.
	*/
	void ChangeState(StateCodeE nextState);

	/**
	* �ړI���W��ݒ�.
	* HistoryBookStateMove�N���X�����̒n�_�ֈړ�����.
	*/
	void SetDestPos(const Vector3& dest)
	{
		_DestPos = dest;
	}

	/**
	* �ړI���W���擾.
	*/
	const Vector3& GetDestPos() const
	{
		return _DestPos;
	}
	
	/**
	* �ړI��]��ݒ�.
	*/
	void SetDestRot(const Quaternion& dest)
	{
		_DestRot = dest;
	}

	/**
	* �ړI��]���擾.
	*/
	const Quaternion& GetDestRot() const
	{
		return _DestRot;
	}


	/**
	* �J������擾.
	*/
	bool GetIsOpenOrClose() const
	{
		return _IsOpenOrClose;
	}



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
	
	/** ���j���̌��݂̏��. */
	int _NowState = (int)StateCodeE::Invalid;
	/** ���j���̏�ԃ��X�g, */
	vector<HistoryBookStatePtr> _StateList;
	
	/** �v���C���[�̃|�C���^. */
	Player*	_Player = nullptr;
	/** �v���C���[�̑O����. */
	Vector3	_PlayerFoward = Vector3::zero;

	/** �ړI���W. */
	Vector3 _DestPos = Vector3::zero;
	/** �ړI��]. */
	Quaternion _DestRot = Quaternion::Identity;

	bool _IsOpenOrClose = true;

};