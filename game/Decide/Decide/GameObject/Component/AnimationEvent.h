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

	// �ǂ̃C�x���g���X�g���Đ����邩�ݒ肷��֐��B
	// �����F	�A�j���[�V�����ԍ��B
	//			�Đ��������C�x���g���X�g�̔ԍ��B
	void ConfigPlayEventList(int animationNo,int eventListNo) {
		if (eventListNo < 0) {
			// 0��菬�����l��ݒ肷��ȁB
			abort();
		}
		_nowPlayEventList[animationNo] = eventListNo;
	}

	// �A�j���[�V�����C�x���g��o�^����֐��B
	// �����F	�C�x���g��ݒ肷��A�j���[�V�����ԍ��B
	//			�C�x���g��ݒ肷�鎞��(�b)�B
	//			�A�j���[�V�����C�x���g(�֐��|�C���^void(*AnimationEvent)(void))�B
	//			�C�x���g���X�g�ԍ�(�������[�V�������g�p���邪�Đ�����A�j���[�V�����C�x���g�͐؂�ւ������Ƃ��Ɏg�p�A�f�t�H���g��0)�B
	void AddAnimationEvent(int animationNo, const float eventTime, AnimationEvent Event, int eventListNo = 0);

	void StartAnimation(int animationNo);

private:
	vector<vector<vector<EventData*>>> _animationEvents;
	vector<int> _nowPlayEventList;	// �ǂ̃C�x���g���X�g���Đ����邩(�v�f���̓A�j���[�V�����̐�)�B
	bool _isSetCallBack = false;
};
