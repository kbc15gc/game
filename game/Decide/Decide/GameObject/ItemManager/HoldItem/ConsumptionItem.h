#pragma once
#include "GameObject\ItemManager\HoldItem\HoldItemBase.h"
#include "fbEngine\_Object\_GameObject\CollisionObject.h"

class GameObject;

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
public:
	ConsumptionItem(char* name);
	~ConsumptionItem();
	void Awake()override;
	void Start()override;

	//�A�C�e�����g���B
	// �߂�l�F	�A�C�e�����g�p�ł������B
	bool UseItem();

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

	//���������擾�B
	inline int GetHoldNum() {
		return  _HoldNum;
	}

	inline void SetInfo(Item::BaseInfo* info)override {
		if (_user) {
			if (_Info) {
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

	float _range = 10.0f;	// ���ʂ��y�ڂ��͈�(���͈̔͊O�̂��̂ɂ̓A�C�e���͉e�����y�ڂ��Ȃ�)�B
	int _HoldNum = 0;	// �������B

	CollisionObject* _gost = nullptr;	// �Ώۂ̒T���ɕK�v�B

	const static int holdMax;
};