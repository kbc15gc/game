#include "stdafx.h"
#include "ConsumptionItem.h"
#include "fbEngine\_Object\_Component\_Physics\GostCollision.h"
#include "fbEngine\_Object\_Component\_Physics\SphereCollider.h"
#include "GameObject\Component\ParticleEffect.h"
#include "BuffDebuffICon.h"
#include"GameObject\Enemy\EnemyCharacter.h"

namespace {
	// ���ʂ��y�ڂ��l���̃e�[�u���B
	static short effectNumTable[static_cast<int>(ConsumptionItem::Range::Max)] = {
		1,3,5,SHRT_MAX
	};
}

// ��g�̍ő及�����B
const int ConsumptionItem::holdMax = 99;

ConsumptionItem::ConsumptionItem(char* name) :HoldItemBase(name)
{
}


ConsumptionItem::~ConsumptionItem()
{
}

void ConsumptionItem::Awake() {
	// �V���b�v�ōw�������A�C�e���Řg���ǉ����ꂽ�ꍇ�AStart���Ă΂�Ȃ��̂ł�����ł��ĂԁB

	_user = _Player = (Player*)INSTANCE(GameObjectManager)->FindObject("Player");	// �Ƃ肠�����g�p�҂͌Œ�Ńv���C���[�B
}

void ConsumptionItem::Start() {
	// Awake���Ƀ��[�U�[����������Ă��Ȃ��\��������̂ł�����ł��ĂԁB
	Awake();
}


//�A�C�e�����g���B
bool ConsumptionItem::UseItem() {

	if (static_cast<EffectType>(static_cast<Item::ItemInfo*>(_Info)->type) == EffectType::Heel || static_cast<EffectType>(static_cast<Item::ItemInfo*>(_Info)->type) == EffectType::Buff) {
		// �񕜂������̓o�t�A�C�e���B

		// �g�p�Ҏ��g���^�[�Q�b�g�ɂ���B
		Item::ItemInfo* info = static_cast<Item::ItemInfo*>(_Info);

//<<<<<<< HEAD
//		bool ret = false;
//		// �b�菈���B
//		// ���Ƃ肠�������o�͍l�����Ă��Ȃ��B
//		if (param->HeelHP(info->effectValue[CharacterParameter::Param::HP])) {	// HP�񕜏����B
//
//			//Hp�񕜂̃G�t�F�N�g�B
//			if (effect) {
//				effect->HeelHpEffect();
//			}
//#ifdef  _DEBUG
//			else {
//				// �G�t�F�N�g�R���|�[�l���g�Ȃ���B
//				abort();
//			}
//#endif //  _DEBUG
//
//			ret = true;
//		}
//
//		if (param->HeelMP(info->effectValue[CharacterParameter::Param::MP])) {	// MP�񕜏����B	
//			//Mp�񕜂̃G�t�F�N�g�B
//
//			if (effect) {
//				effect->HeelMpEffect();
//			}
//#ifdef  _DEBUG
//			else {
//				// �G�t�F�N�g�R���|�[�l���g�Ȃ���B
//				abort();
//			}
//#endif //  _DEBUG
//
//			ret = true;
//		}
//
//		for (int idx = static_cast<int>(CharacterParameter::Param::ATK); idx < CharacterParameter::MAX; idx++) {
//			int value = info->effectValue[idx];
//			if (value > 0) {
//				// �o�t�B
//				if (effect) {
//					effect->BuffEffect();
//					BuffDebuffICon* icon = (BuffDebuffICon*)INSTANCE(GameObjectManager)->FindObject("BuffDebuffICon");
//					icon->BuffIconCreate(static_cast<BuffDebuffICon::Param>(idx));
//				}
//#ifdef  _DEBUG
//				else {
//					// �G�t�F�N�g�R���|�[�l���g�Ȃ���B
//					abort();
//				}
//#endif //  _DEBUG
//				param->Buff(static_cast<CharacterParameter::Param>(idx), static_cast<unsigned short>(value), info->time);
//				ret = true;
//			}
//			else if (value < 0) {
//				// �f�o�t(�f�����b�g)�B
//				if (effect) {
//					BuffDebuffICon* icon = (BuffDebuffICon*)INSTANCE(GameObjectManager)->FindObject("BuffDebuffICon");
//					icon->DebuffIconCreate(static_cast<BuffDebuffICon::Param>(idx));
//					effect->DeBuffEffect();
//				}
//#ifdef  _DEBUG
//				else {
//					// �G�t�F�N�g�R���|�[�l���g�Ȃ���B
//					abort();
//				}
//#endif //  _DEBUG
//				param->Debuff(static_cast<CharacterParameter::Param>(idx), static_cast<unsigned short>(abs(value)), info->time);
//				ret = true;
//			}
//		}
//=======
		// �v���C���[�ɃA�C�e�����ʔ���.
		bool ret = _Player->ItemEffect(info);
//>>>>>>> 64c75be9e0556a80abc3ea251b7433ac4712f6b5

		// �b�菈���B
		// ���Q�[�����ŉ��Ƃ����ʂ��Ȃ����Ƃ����m�点���ׂ��B
		if (!ret) {
			char error[256];
			sprintf(error, "���̐��ʂ������܂���ł��������I�I");
			MessageBoxA(0, error, "�o�t���񕜂��ł��Ȃ���I", MB_ICONWARNING);
			return false;
		}

		return true;
	}
	else 
	{
		vector<EnemyCharacter*> targets;	// ����A�C�e�����g���ΏہB

		// �f�o�t�A�C�e���B
		// ���ʔ͈͓��̃G�l�~�[���擾�B
		int attr = Collision_ID::ENEMY;
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
				targets.push_back((EnemyCharacter*)minObj);
			}
		}
		else {
			for (auto coll : hit) {
				// �ΏۂɎ擾�����I�u�W�F�N�g��ǉ��B
				targets.push_back((EnemyCharacter*)coll->gameObject);
			}
		}

		if (targets.size() <= 0) {

			// �b�菈���B
			// ���Q�[�����ŉ��Ƃ����ʂ��Ȃ����Ƃ����m�点���ׂ��B
			char error[256];
			sprintf(error, "���̐��ʂ������܂���ł��������I�I");
			MessageBoxA(0, error, "�f�o�t�Ȃ̂Ɍ��ʔ͈͓��ɓG�����Ȃ���", MB_ICONWARNING);

			return false;
		}
		else {
			// �����A�C�e�����l�����ĂƂ肠�����S�����s����B
			for (auto target : targets) {
				Item::ItemInfo* info = static_cast<Item::ItemInfo*>(_Info);

				target->ItemEffect(info);

			}
			targets.clear();
		}

		return true;
	}
}