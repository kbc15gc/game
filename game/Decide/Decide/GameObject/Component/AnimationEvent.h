#pragma once
#include "fbEngine\_Object\_Component\Component.h"
#include "fbEngine\fbstdafx.h"
#include "AttackCollision.h"



// �֐��|�C���^��V�����^���Œ�`�B
// ���V�������O��AnimationEvent�B
// ���߂�l��void�A�����͂Ȃ��B
typedef void(GameObject::*AnimationEvent)();

// �A�j���[�V�����C�x���g�Đ��֐��B
class AnimationEventPlayer : public Component {
public:

private:
	struct EventData {
		int playFrame;		// �C�x���g����������t���[���B
		AnimationEvent Event;	// �C�x���g(GameObject���p�������N���X�̃����o�֐��|�C���^)�B
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
	void AddAnimationEvent(int animationNo, const int eventFrame, AnimationEvent Event) {
		if (_isFirst) {
			Init();
		}
		EventData* work = nullptr;
		work = new EventData;
		work->playFrame = eventFrame;
		work->Event = Event;
		_animationEvents[animationNo].push_back(work);
	}

private:
	bool _isFirst = true;	// �ŏ��̒ǉ��������B
	vector<vector<EventData*>> _animationEvents;
};
