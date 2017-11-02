#include "stdafx.h"
#include "GameObject\Component\AnimationEvent.h"
#include "fbEngine\_Object\_GameObject\SoundSource.h"
#include "fbEngine\_Object\_GameObject\GameObject.h"


AnimationEventPlayer::~AnimationEventPlayer() {
	for (auto& work : _animationEvents) {
		for (auto& work2 : work) {
			for (auto Event : work2) {
				SAFE_DELETE(Event);
			}
			work2.clear();
		}
		work.clear();
	}
	_animationEvents.clear();
};

void AnimationEventPlayer::Init(int animationNum)
{
	// �A�j���[�V�����̐������z��ǉ��B
	_animationEvents = vector<vector<vector<EventData*>>>(animationNum);
	_nowPlayEventList = vector<int>(animationNum,0);
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

		if (_animationEvents[nowAnim].size() > 0) {
			for (auto eventData : _animationEvents[nowAnim][_nowPlayEventList[nowAnim]]) {
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
	}
	else {
		// Animation�R���|�[�l���g���A�^�b�`����Ă��Ȃ��B
		abort();
	}
}

void AnimationEventPlayer::AddAnimationEvent(int animationNo, const float eventTime, AnimationEvent Event, int eventListNo) {
	EventData* work = nullptr;
	work = new EventData;
	work->playTime = eventTime;
	work->Event = Event;
	work->isPlay = false;

	if (static_cast<int>(_animationEvents[animationNo].size()) <= eventListNo) {
		// �z��̗e�ʂ�����Ȃ��̂Ŋg���B
		_animationEvents[animationNo].resize(eventListNo + 1);
	}
	_animationEvents[animationNo][eventListNo].push_back(work);
}

void AnimationEventPlayer::StartAnimation(int animationNo) {
	if (_animationEvents[animationNo].size() > 0) {
		for (auto eventData : _animationEvents[animationNo][_nowPlayEventList[animationNo]]) {
			eventData->isPlay = false;
		}
	}
}