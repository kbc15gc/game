#include "stdafx.h"
#include "GameObject\Component\AnimationEvent.h"
#include "fbEngine\_Object\_GameObject\SoundSource.h"
#include "fbEngine\_Object\_GameObject\GameObject.h"


AnimationEventPlayer::~AnimationEventPlayer() {
	for (auto& work : _animationEvents) {
		for (auto Event : work) {
			SAFE_DELETE(Event);
		}
		work.clear();
	}
	_animationEvents.clear();
};

void AnimationEventPlayer::Init(int animationNum)
{
	// �A�j���[�V�����̐������z��ǉ��B
	_animationEvents = vector<vector<EventData*>>(animationNum);
}

void AnimationEventPlayer::Update() {
	if (_animationEvents.size() <= 0) {
		// �A�j���[�V�����C�x���g������ݒ肳��Ă��Ȃ��B
		return;
	}

	Animation* work = gameObject->GetComponent<Animation>();
	if (work) {
		if (!_isSetCallBack) {

			// �R�[���o�b�N�֐��ݒ�B
			work->AddCallBack(move(unique_ptr<Animation::StartAnimationCallBack>(new Animation::StartAnimationCallBack(this, static_cast<Animation::CallBack>(&AnimationEventPlayer::StartAnimation)))));
			_isSetCallBack = true;
		}

		//���ݍĐ����̃A�j���[�V�����ԍ��擾�B
		int nowAnim = work->GetPlayAnimNo();


		for (auto eventData : _animationEvents[nowAnim]) {
			//���Ԃ���v�������C�x���g�Ăяo���B
			if (work->GetLocalAnimationTime() >= eventData->playTime)
			{
				if (!eventData->isPlay) {
					// �C�x���g���܂����s����ĂȂ��B

					// �֐��|�C���^�ɐݒ肳�ꂽ�֐������s�B
					(gameObject->*(eventData->Event))();
					eventData->isPlay = true;
				}
			}
		}
	}
	else {
		// Animation�R���|�[�l���g���A�^�b�`����Ă��Ȃ��B
		abort();
	}
}

void AnimationEventPlayer::StartAnimation(int animationNo) {
	for (auto eventData : _animationEvents[animationNo]) {
		eventData->isPlay = false;
	}
}