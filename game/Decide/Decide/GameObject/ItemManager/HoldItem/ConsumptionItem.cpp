#include "stdafx.h"
#include "ConsumptionItem.h"
#include "fbEngine\_Object\_Component\_Physics\GostCollision.h"
#include "fbEngine\_Object\_Component\_Physics\SphereCollider.h"

namespace {
	// ���ʂ��y�ڂ��l���̃e�[�u���B
	static short effectNumTable[static_cast<int>(ConsumptionItem::Range::Max)] = {
		1,3,5,SHRT_MAX
	};
}

ConsumptionItem::ConsumptionItem(char* name) :HoldItemBase(name)
{
}


ConsumptionItem::~ConsumptionItem()
{
}

void ConsumptionItem::Start() {
	_user = INSTANCE(GameObjectManager)->FindObject("Player");	// �Ƃ肠�����g�p�҂͌Œ�Ńv���C���[�B
	_gost = INSTANCE(GameObjectManager)->AddNew<CollisionObject>("ItemRange",5);	// �A�C�e���̌��ʔ͈̓R���W�����B

	_gost->transform->SetParent(_user->transform);
	_gost->transform->SetLocalPosition(Vector3::zero);
	_gost->Create(Collision_ID::ITEMRANGE,Vector3(_range,_range,_range),false);	

	_Effect = AddComponent<ParticleEffect>();

}


//�A�C�e�����g���B
void ConsumptionItem::UseItem() {
	vector<GameObject*> targets;	// ����A�C�e�����g���ΏہB

	if (static_cast<EffectType>(static_cast<Item::ItemInfo*>(_Info)->type) == EffectType::Heel || static_cast<EffectType>(static_cast<Item::ItemInfo*>(_Info)->type) == EffectType::Buff) {
		// �񕜂������̓o�t�A�C�e���B

		// �g�p�Ҏ��g��HP�񕜏����B
		CharacterParameter* param = _user->GetComponent<CharacterParameter>();
		Item::ItemInfo* info = static_cast<Item::ItemInfo*>(_Info);
		// �b�菈���B
		// ���Ƃ肠�������o�͍l�����Ă��Ȃ��B
		_Effect->HeelEffect(_user->transform);

		param->HeelHP(info->effectValue[CharacterParameter::Param::HP]);	// HP�񕜏����B
		param->HeelMP(info->effectValue[CharacterParameter::Param::MP]);	// MP�񕜏����B

		for (int idx = static_cast<int>(CharacterParameter::Param::ATK); idx < CharacterParameter::MAX; idx++) {
			int value = info->effectValue[idx];
			if (value > 0) {
				// �o�t�B
				
				_Effect->BuffEffect(_user->transform);
				param->Buff(static_cast<CharacterParameter::Param>(idx),static_cast<unsigned short>(value),info->time);
			}
			else if (value < 0) {
				// �f�o�t(�f�����b�g)�B

				param->Debuff(static_cast<CharacterParameter::Param>(idx), static_cast<unsigned short>(abs(value)), info->time);
			}
		}
	}
	else {
		// �f�o�t�A�C�e���B
		_Effect->DeBuffEffect(_user->transform);
		// ���ʔ͈͓��̃G�l�~�[���擾�B
		int attr = Collision_ID::ENEMY | Collision_ID::BOSS;
		vector<Collision*> hit;
		INSTANCE(PhysicsWorld)->AllHitsContactTest(_gost->GetAttachCollision(), hit, attr);

		if (hit.size() > effectNumTable[static_cast<int>(static_cast<Item::ItemInfo*>(_Info)->rangeStrength)]) {
			// �Փ˂����Ώۂ��A�C�e���̌��ʔ͈͂�葽���B

			for (int idx = 0; idx < effectNumTable[static_cast<int>(static_cast<Item::ItemInfo*>(_Info)->rangeStrength)]; idx++) {

				// �Փ˂����Ώۂ̒��̋߂����̂���擾���Ă����B
				float min = FLT_MAX;
				GameObject* minObj = nullptr;
				int eraseIdx = 0;	// �擪���牽�Ԗڂ̗v�f���B
				for (int idx = 0; idx < hit.size(); idx++) {
					if (Vector3(_user->transform->GetPosition() - hit[idx]->transform->GetPosition()).Length() < min) {
						minObj = hit[idx]->gameObject;
						eraseIdx = idx;
					}
				}

				// �擾�����R���W�����͔z�񂩂�Ȃ��B
				hit.erase(hit.begin() + eraseIdx);

				// �ΏۂɎ擾�����I�u�W�F�N�g��ǉ��B
				targets.push_back(minObj);
			}
		}
		else {
			for (auto coll: hit) {
				// �ΏۂɎ擾�����I�u�W�F�N�g��ǉ��B
				targets.push_back(coll->gameObject);
			}
		}

	}

	// �����A�C�e�����l�����ĂƂ肠�����S�����s����B
	for (auto target : targets) {
		CharacterParameter* param = target->GetComponent<CharacterParameter>();
		Item::ItemInfo* info = static_cast<Item::ItemInfo*>(_Info);
		// �b�菈���B
		// ���Ƃ肠�������o�͍l�����Ă��Ȃ��B
		for (int idx = static_cast<int>(CharacterParameter::Param::ATK); idx < CharacterParameter::MAX; idx++) {
			if (param) {
				param->Debuff(static_cast<CharacterParameter::Param>(idx), static_cast<unsigned short>(abs(info->effectValue[idx])), info->time);
			}
		}
	}

	if (targets.size() <= 0 && static_cast<EffectType>(static_cast<Item::ItemInfo*>(_Info)->type) == EffectType::Debuff) {

		// �b�菈���B
		// ���Q�[�����ŉ��Ƃ����ʂ��Ȃ����Ƃ����m�点���ׂ��B
		char error[256];
		sprintf(error, "�f�o�t�Ȃ̂Ɍ��ʔ͈͓��ɓG�����Ȃ���");
		MessageBoxA(0, error, "���̐��ʂ������܂���ł��������I�I", MB_ICONWARNING);
	}

	targets.clear();
}
