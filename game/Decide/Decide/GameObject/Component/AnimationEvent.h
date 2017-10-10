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
		float playTime;		// �C�x���g���������鎞�ԁB
		AnimationEvent Event;	// �C�x���g(GameObject���p�������N���X�̃����o�֐��|�C���^)�B
		bool isPlay = false;	// �C�x���g���s�ς݂��B
	};

public:
	AnimationEventPlayer(GameObject* g, Transform* t) :Component(g, t, typeid(this).name()) {
#ifdef _DEBUG
		mbstowcs_s(nullptr, name, typeid(*this).name(), strlen(typeid(*this).name()));
#endif
	};
	~AnimationEventPlayer();

	void Update()override;

	// �������B
	// �����F	���̃R���|�[�l���g���A�^�b�`����Ă���Q�[���I�u�W�F�N�g�̃A�j���[�V�������B
	void Init(int animationNum);

	// �w�肵���t���[���ɃA�j���[�V�����C�x���g��ݒ肷��֐��B
	// �����F	�C�x���g��ݒ肷��A�j���[�V�����ԍ��B
	//			�C�x���g��ݒ肷�鎞��(�b)�B
	//			�A�j���[�V�����C�x���g(�֐��|�C���^void(*AnimationEvent)(void))�B
	// �����������R���W�����`��̓{�b�N�X�ł��B
	void AddAnimationEvent(int animationNo, const float eventTime, AnimationEvent Event) {
		EventData* work = nullptr;
		work = new EventData;
		work->playTime = eventTime;
		work->Event = Event;
		work->isPlay = false;
		_animationEvents[animationNo].push_back(work);
	}

private:
	vector<vector<EventData*>> _animationEvents;
	int _playAnimationNo;	// �Đ����̃A�j���[�V�����ԍ��B
	double _nowLocalTime = 0.0f;	// ���݂̍Đ����ԁB
};
