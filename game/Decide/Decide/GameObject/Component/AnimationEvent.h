#pragma once
#include "fbEngine\_Object\_Component\Component.h"
#include "fbEngine\fbstdafx.h"
#include "AttackCollision.h"

class AnimationEvent : public Component {
public:
	// �U���R���W�����̃p�����[�^�B
	// ��ConfigAnimationEvent�Ăяo�����Ɏg���ĂˁB
	struct AttackEventInfo {
	public:
		// �����F	�e(�f�t�H���g��null)�B
		//			��������ɐe�q�֌W���O�����B
		AttackEventInfo(Transform* tr, bool isRemoveParent) {
			parent = tr;
			this->isRemoveParent = isRemoveParent;
		}
		AttackCollision::CollisionMaster master;	// �N�̍U�����B
		int damage;		// �_���[�W�ʁB
		Vector3 pos;	// �ʒu�B
		Quaternion rot;	// ��]�B
		Vector3 size;	// �T�C�Y�B
		float life = -1.0f;	// ����(0.0��菬�����l�Ŗ���)�B
		float wait = 0.0f;	// �C�x���g�Ăяo�����牽�b��ɐ������邩�B
		Transform* parent = nullptr;	// �e(�f�t�H���g��null)�B
		bool isRemoveParent = true;		// ��������ɐe�q�֌W���O�����B
	};

private:
	struct EventData {
		int createFrame;		// ��������t���[���B
		AttackEventInfo info = AttackEventInfo(nullptr,true);	// ��������R���W�����̃p�����[�^�B
	};
public:
	AnimationEvent(GameObject* g, Transform* t) :Component(g, t, typeid(this).name()) {
#ifdef _DEBUG
		mbstowcs_s(nullptr, name, typeid(*this).name(), strlen(typeid(*this).name()));
#endif
	};
	~AnimationEvent();

	void Update()override;

	void Init();

	// �w�肵���t���[���ɍU������p�R���W�����쐬�C�x���g��ǉ�����֐��B
	// �����F	�C�x���g�𔭐����������A�j���[�V�����ԍ��B
	//			�R���W�����𔭐�������t���[���B
	//			�쐬����R���W�����̃p�����[�^�B
	// �����������R���W�����`��̓{�b�N�X�ł��B
	void AddAnimationEvent(int animationNo,const int eventFrame, const AttackEventInfo& info) {
		if (_isFirst) {
			Init();
		}
		EventData* work = nullptr;
		work = new EventData;
		work->createFrame = eventFrame;
		work->info = info;
		_animationEvents[animationNo].push_back(move(work));
	}

private:
	bool _isFirst = true;	// �ŏ��̒ǉ��������B
	vector<vector<EventData*>> _animationEvents;
};