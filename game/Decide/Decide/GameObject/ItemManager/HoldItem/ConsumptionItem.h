#pragma once
#include "GameObject\ItemManager\HoldItem\HoldItemBase.h"
#include "fbEngine\_Object\_GameObject\CollisionObject.h"

class GameObject;
class TextObject;
#include"GameObject\Player\Player.h"

//����A�C�e���̃N���X�B
class ConsumptionItem :public HoldItemBase
{
public:
	enum class EffectType{Heel = 0,Buff,Debuff};
	// �A�C�e�����y�ڂ��ΏہB
	enum class Range{
		I = 0,	// 1�́B
		III,	// 3�́B
		V,		// 5�́B
		All,	// �͈͓����ׂāB
		Max
	};

private:
	friend class HoldItemFactory;
	ConsumptionItem(char* name);
public:
	~ConsumptionItem();
	void Awake()override;
	void Start()override;

	//�A�C�e�����g���B
	// �߂�l�F	�A�C�e�����g�p�ł������B
	bool UseItem();


	// �O������ǂݍ��񂾃f�[�^��ݒ�B
	// �����F	CSV�ǂݏ����p�̏��������i�\���̂ւ̃|�C���^�B
	// ��CSV����ǂݍ��񂾃����_���p�����[�^���⑕�������g�p����ۂ͂��̊֐��Ńp�����[�^��ݒ肷��B
	inline void ConfigLoadData(Hold::HoldInfo* info)override {
		SetHoldNum(static_cast<Hold::ConsumptionInfo*>(info)->_HoldNum);
	}

	//�����������Z�B
	// �����F	���₷�ʁB
	// �߂�l�F	���肫��Ȃ������l�B
	inline int AddHoldNum(int add){
		_HoldNum += add;
		if (_HoldNum > holdMax) {
			int ret = _HoldNum - holdMax;
			_HoldNum = holdMax;
			return ret;
		}
		return 0;
	}

	//�����������Z�B
	// �����F	���炷�ʁB
	// �߂�l�F	�����̂��܂�(���̃A�C�e���̏�������0�ɂȂ��Ă������̒l���c���Ă���ꍇ��0�ȊO���A���Ă���)�B
	inline int SubHoldNum(int sub) {
		_HoldNum -= sub;
		if (_HoldNum < 0) {
			int ret = abs(_HoldNum);
			_HoldNum = 0;
			return ret;
		}
		return 0;
	}

	//��������ݒ�B
	// �����F	�ݒ肷��ʁB
	// �߂�l�F	���肫��Ȃ������l�B
	inline int SetHoldNum(int num) {
		if (num > holdMax) {
			int ret = num - holdMax;
			_HoldNum = holdMax;
			return ret;
		}
		_HoldNum = num;
		return 0;
	}

	//���������擾�B
	inline int GetHoldNum() {
		return  _HoldNum;
	}

	inline void SetInfo(Item::BaseInfo* info)override {
		_Info = info;
		if (_Info) {
			_type = static_cast<EffectType>(static_cast<Item::ItemInfo*>(_Info)->type);
			if (_user) {
				if (static_cast<Item::ItemInfo*>(_Info)->type == static_cast<int>(ConsumptionItem::EffectType::Debuff)) {
					_gost = INSTANCE(GameObjectManager)->AddNew<CollisionObject>("ItemRange", 5);	// �A�C�e���̌��ʔ͈̓R���W�����B
					_gost->transform->SetParent(_user->transform);
					_gost->transform->SetLocalPosition(Vector3::zero);
					_gost->Create(Collision_ID::ITEMRANGE, Vector3(_range, _range, _range), false);
#ifdef _DEBUG
					_gost->GetAttachCollision()->GetShape()->RenderDisable();
#endif // _DEBUG
				}
			}
		}
	}
private:

	void SetParticleBuffParam();

	GameObject* _user = nullptr;	// �g�p�ҁB

	/**
	* �v���C���[.
	*/
	Player* _Player = nullptr;

	EffectType _type;

	float _range = 10.0f;	// ���ʂ��y�ڂ��͈�(���͈̔͊O�̂��̂ɂ̓A�C�e���͉e�����y�ڂ��Ȃ�)�B
	int _HoldNum = 0;	// �������B

	CollisionObject* _gost = nullptr;	// �Ώۂ̒T���ɕK�v�B

	const static int holdMax;

	TextObject*	_NoEffectText			= nullptr;			//�A�C�e�����g���Č��ʂ��������ɏo�Ă���e�L�X�g�B
	Vector3		_NoEffectTextPos		= Vector3::zero;	//�e�L�X�g��\������ꏊ�B
	float		_NoEffectTextFontSize	= 0.0f;				//�e�L�X�g�̃T�C�Y�B
	Color		_NoEffectTextColor		= Color::red;		//�e�L�X�g�̃J���[�B
};