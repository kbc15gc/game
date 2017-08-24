#pragma once
#include "fbEngine\_Object\_Component\Component.h"
#include "fbEngine\fbstdafx.h"
#include "AttackCollision.h"


class GameObject;


// �֐��|�C���^��V�����^���Œ�`�B
// ���V�������O��AnimationEvent�B
// ���߂�l��void�A�����͂Ȃ��B
typedef void(GameObject::*AnimationEvent)();

// �A�j���[�V�����C�x���g�Đ��֐��B
class AnimationEventPlayer : public Component {
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

	struct SoundEventInfo {
		SoundSource* source = nullptr;
		bool is3D = false;
		bool isLoop = false;
	};
private:
	struct EventData {
		int playFrame;		// �C�x���g����������t���[���B
		GameObject* object = nullptr;
		AnimationEvent Event;	// �C�x���g(�֐��|�C���^)�B
	};

public:
	AnimationEventPlayer(GameObject* g, Transform* t) :Component(g, t, typeid(this).name()) {
#ifdef _DEBUG
		mbstowcs_s(nullptr, name, typeid(*this).name(), strlen(typeid(*this).name()));
#endif
	};
	~AnimationEventPlayer();

	void Update()override;

	void Init();

	// �w�肵���t���[���ɃA�j���[�V�����C�x���g��ݒ肷��֐��B
	// �����F	�C�x���g��ݒ肷��A�j���[�V�����ԍ��B
	//			�C�x���g��ݒ肷��t���[���B
	//			�A�j���[�V�����C�x���g(�֐��|�C���^void(*AnimationEvent)(void))�B
	// �����������R���W�����`��̓{�b�N�X�ł��B
	void AddAnimationEvent(int animationNo, const int eventFrame,GameObject* object, AnimationEvent Event) {
		if (_isFirst) {
			Init();
		}
		EventData* work = nullptr;
		work = new EventData;
		work->playFrame = eventFrame;
		work->object = object;
		work->Event = Event;
		_animationEvents[animationNo].push_back(work);
	}

private:
	bool _isFirst = true;	// �ŏ��̒ǉ��������B
	vector<vector<EventData*>> _animationEvents;
};
