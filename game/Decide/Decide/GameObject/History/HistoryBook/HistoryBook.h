/**
* ���j���N���X�̒�`.
*/
#pragma once

#include "GameObject\Player\Player.h"
#include"GameObject\Camera\PlayerCamera.h"
#include"..\HistoryInfo.h"

#include"HFSM\HistoryBookState.h"

#include"HistoryPage\HistoryPage.h"
#include "GameObject\Village\EventManager.h"

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
	* �`��.
	*/
	void Render()override;

	/**
	* ���݂̏�Ԃ��擾.
	*/
	int GetNowState() const
	{
		return _NowState;
	}

	void SetActive(const bool& act, const bool& children = false)override;

	void SetActiveGameObject(const bool& act, const bool& children = false)
	{
		GameObject::SetActive(act, children);
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

	bool GetIsPlayAnim(AnimationCodeE code)
	{
		return _Anim->GetPlayAnimNo() == (int)code;
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
	HistoryPage* PutInChip(ChipID chipID, LocationCodeE code, int index)
	{
		HistoryPage* page = INSTANCE(GameObjectManager)->AddNew<HistoryPage>("HistoryPage",10);
		page->SetHistoryBook(this);

		_HistoryPageList[(int)code].insert(_HistoryPageList[(int)code].begin() + index, page);

		_CalcPagePosition();
		page->Start(chipID, code);

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
			float angle = (*itr)->GetAngle();
			if (angle > 0.0f)
			{
				angle -= 0.1f;
			}
			else if(angle < 0.0f)
			{
				angle += 0.1f;
			}
			(*itr)->Rotation(angle);
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

	void PageInactive()
	{
		for (auto& locList : _HistoryPageList)
		{
			for (auto it : locList)
			{
				if (it != nullptr)
				{
					it->SetActive(false, true);
				}
			}
		}
	}

	void SetIsOperation(const bool is)
	{
		_IsOperation = is;
	}

	bool GetIsOperation()
	{
		return _IsOperation;
	}

	void PlayerStopDisable()
	{
		INSTANCE(EventManager)->NotifyEndEvent();
	}

	/**
	* �����̃`�b�v���������܂�Ă��邩����.
	*/
	bool IsSetChip(ChipID id)
	{
		for (auto& itLoc : _HistoryPageList)
		{
			for (auto itPage : itLoc)
			{
				if (itPage->GetChipID() == id)
				{
					return true;
				}
			}
		}
		return false;
	}

	void SetLocationCode(LocationCodeE code)
	{
		_NowLocationCode = code;
		for (int i = 0; i < 3; i++)
		{
			float angle = -90.0f;
			Color color = Color(0.2f, 0.2f, 0.2f);

			if (i < (int)_NowLocationCode)
			{
				angle = 90.0f;
			}
			if (i == (int)_NowLocationCode)
			{
				color = Color(1.0f, 1.0f, 1.0f);
			}

			for (auto it : _HistoryPageList[i])
			{
				if (it != nullptr)
				{
					it->SetBlendColor(color);
					it->SetRotAngle(angle);
				}
			}
		}
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

	/**
	* �y�[�W�̍��W���v�Z����.
	*/
	void _CalcPagePosition()
	{
		vector<HistoryPage*> pageList;
		for (auto& loc : _HistoryPageList)
		{
			for (auto& itPage : loc)
			{
				pageList.push_back(itPage);
			}
		}

		int size = pageList.size();
		for (int i = 0; i < size; i++)
		{
			int posX = i - size / 2;
			Vector3 pos = Vector3(-0.01f * posX, 0.0f, (0.2f - 0.01f * i));

			//�ʒu.
			pageList[i]->transform->SetLocalPosition(pos);
			pageList[i]->SetInitPos(pos);
		}

	}

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

	PlayerCamera* _PlayerCamera = nullptr;

	LocationCodeE _NowLocationCode = LocationCodeE::Begin;

};