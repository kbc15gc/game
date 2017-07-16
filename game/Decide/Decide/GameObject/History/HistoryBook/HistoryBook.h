/**
* ���j���N���X�̒�`.
*/
#pragma once

#include"..\HistoryInfo.h"

#include"HFSM\HistoryBookState.h"

#include"HistoryPage\HistoryPage.h"

/** �v���C���[�N���X. */
class Player;

/** ���j���̏�ԃN���X�̃��j�[�N�|�C���^. */
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
	enum class StateCodeE : int
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
		for (auto& locList : _HistoryPageList)
		{
			for (auto it : locList)
			{
				if (it != nullptr)
				{
					it->SetActive(flag);
				}
			}
		}
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
	* �J������擾.
	*/
	bool GetIsOpenOrClose() const
	{
		return _IsOpenOrClose;
	}

	/**
	* �`�b�v��ǉ�.
	*/
	HistoryPage* PutInChip(ChipID chipID, LocationCodeE code)
	{
		HistoryPage* page = INSTANCE(GameObjectManager)->AddNew<HistoryPage>("HistoryPage",1);
		page->SetHistoryBook(this);
		page->Start(chipID, code);
		_HistoryPageList[(int)code].push_back(page);
		return page;
	}

	/**
	*�������܂ꂽ�y�[�W�̃��X�g���擾�B
	*/
	vector<vector<HistoryPage*>>& GetPageList()
	{
		return _HistoryPageList;
	}

	vector<HistoryPage*>& GetLocationList(LocationCodeE loc)
	{
		return _HistoryPageList[(int)loc];
	}

	void PutOutPage(LocationCodeE loc,HistoryPage* page)
	{
		auto itr = find(_HistoryPageList[(int)loc].begin(), _HistoryPageList[(int)loc].end(), page);
		if (itr != _HistoryPageList[(int)loc].end())
		{
			(*itr)->ChangeState(HistoryPage::StateCodeE::PutOut);
			_HistoryPageList[(int)loc].erase(itr);
		}
	}
	void OpenPage()
	{
		for (auto& locList : _HistoryPageList)
		{
			for (auto it : locList)
			{
				if (it != nullptr)
				{
					it->ChangeState(HistoryPage::StateCodeE::Turn);
				}
			}
		}
	}

	void SetIsOperation(const bool is)
	{
		_IsOperation = is;
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

	bool _IsOpenOrClose = true;

	/** �y�[�W�N���X�̃|�C���^. */
	vector<vector<HistoryPage*>> _HistoryPageList;

	/** ����\�t���O. */
	bool _IsOperation = true;
};