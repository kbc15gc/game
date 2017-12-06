/**
* 歴史書クラスの定義.
*/
#pragma once

#include "GameObject\Player\Player.h"
#include"GameObject\Camera\PlayerCamera.h"
#include"..\HistoryInfo.h"

#include"HFSM\HistoryBookState.h"

#include"HistoryPage\HistoryPage.h"
#include "GameObject\Village\EventManager.h"

/** プレイヤークラス. */
class Player;

/** 歴史書の状態クラスのユニークポインタ. */
using HistoryBookStatePtr = unique_ptr<IHistoryBookState>;

/**
* 歴史書クラス.
*/
class HistoryBook : public GameObject
{
public:
	
	/**
	* 歴史書の状態コード.
	*/
	enum class StateCodeE : int
	{
		Invalid = -1,	//!< 無効.
		Unused,			//!< 未使用.
		Idol,			//!< 閉じた状態.
		Move,			//!< 移動.
		Open,			//!< 開く.
		Close,			//!< 閉じる.
		StateNum,		//!< 状態の数.
	};

	/**
	* 歴史書のアニメーションコード.
	*/
	enum class AnimationCodeE
	{
		Invalid = -1,	//!< 無効.
		CloseIdol,		//!< 待機_閉.
		OpenIdol,		//!< 待機_開.
		Open,			//!< 開くアニメーション.
		Close,			//!< 閉じるアニメーション.
		AnimationNum,	//!< アニメーション数.
	};

	/**
	* コンストラクタ.
	*/
	HistoryBook(const char* name) :
		GameObject(name)
	{
	}

	/**
	* デストラクタ.
	*/
	~HistoryBook()
	{
	}

	/**
	* コンストラクタ後の初期化.
	*/
	void Awake()override;

	/**
	* 初期化.
	*/
	void Start()override;

	/**
	* 更新.
	*/
	void Update()override;

	/**
	* 描画.
	*/
	void Render()override;

	/**
	* 現在の状態を取得.
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
	* アニメーションの再生判定を取得.
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
	* アニメーションの再生.
	*
	* @param animNo				アニメーションのナンバー.
	* @param interpolatetime	補間時間.
	* @param loopnum			ループ回数 (デフォルトは-1).
	*/
	void PlayAnimation(AnimationCodeE animNo, float interpolatetime, int loopnum = -1);
	
	/**
	* 状態の変更.
	*
	* @param nextState	次の状態.
	*/
	void ChangeState(StateCodeE nextState);

	/**
	* 目的座標を設定.
	* HistoryBookStateMoveクラスがこの地点へ移動する.
	*/
	void SetDestPos(const Vector3& dest)
	{
		_DestPos = dest;
	}

	/**
	* 目的座標を取得.
	*/
	const Vector3& GetDestPos() const
	{
		return _DestPos;
	}
	
	/**
	* 開閉判定を取得.
	*/
	bool GetIsOpenOrClose() const
	{
		return _IsOpenOrClose;
	}

	/**
	* チップを追加.
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
	*差し込まれたページのリストを取得。
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
	* 引数のチップが差し込まれているか判定.
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
	* 状態リストの初期化.
	*/
	void _InitState();

	/**
	* 歴史書を開いている判定フラグを変更.
	*/
	void _ChangeIsLookAtHistoryFlag();

	/**
	* ページの座標を計算する.
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

			//位置.
			pageList[i]->transform->SetLocalPosition(pos);
			pageList[i]->SetInitPos(pos);
		}

	}

private:

	/** 歴史書のモデル. */
	SkinModel* _Model = nullptr;
	/** 歴史書のアニメーション. */
	Animation* _Anim = nullptr;
	
	/** 歴史書の現在の状態. */
	int _NowState = (int)StateCodeE::Invalid;
	/** 歴史書の状態リスト, */
	vector<HistoryBookStatePtr> _StateList;
	
	/** プレイヤーのポインタ. */
	Player*	_Player = nullptr;
	/** プレイヤーの前方向. */
	Vector3	_PlayerFoward = Vector3::zero;

	/** 目的座標. */
	Vector3 _DestPos = Vector3::zero;

	bool _IsOpenOrClose = true;

	/** ページクラスのポインタ. */
	vector<vector<HistoryPage*>> _HistoryPageList;

	/** 操作可能フラグ. */
	bool _IsOperation = true;

	PlayerCamera* _PlayerCamera = nullptr;

	LocationCodeE _NowLocationCode = LocationCodeE::Begin;

};