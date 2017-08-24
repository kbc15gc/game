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

void AnimationEventPlayer::Init()
{
	// �A�j���[�V�����̐������z��ǉ��B
	if (gameObject->GetComponent<Animation>()) {
		_animationEvents = vector<vector<EventData*>>(gameObject->GetComponent<Animation>()->GetNumAnimationSet());
	}
#ifdef _DEBUG
	else {
		// ���A�j���[�V�����R���|�[�l���g���ɒǉ��������B
		abort();
	}
#endif // _DEBUG
	_isFirst = false;
}

void AnimationEventPlayer::Update() {
	if (_animationEvents.size() <= 0) {
		// �A�j���[�V�����C�x���g������ݒ肳��Ă��Ȃ��B
		return;
	}
	Animation* work = gameObject->GetComponent<Animation>();
	if (work) {
		//���ݍĐ����̃A�j���[�V�����ԍ��擾�B
		int nowAnim = work->GetPlayAnimNo();
		for (auto eventData : _animationEvents[nowAnim]) {
				//�t���[������v�������C�x���g�Ăяo���B
			if (work->NowFrame() == eventData->playFrame)
			{
				// �֐��|�C���^�ɐݒ肳�ꂽ�֐������s�B
				(eventData->object->*(eventData->Event))();

				////�U���R���W�����쐬�B
				//unsigned int priorty = 1;
				//AttackCollision* attack = INSTANCE(GameObjectManager)->AddNew<AttackCollision>("attackCollision", priorty);
				//attack->Create(eventData->info.damage, eventData->info.pos, eventData->info.rot, eventData->info.size, eventData->info.master, eventData->info.life, eventData->info.wait, eventData->info.parent);
				//if (eventData->info.isRemoveParent) {
				//	attack->RemoveParent();
				//	//���������X�^�[�̌����ƃR���W�����̌��������킹�����̂Ȃ�
				//	//collison->transform->SetAngle(monster->transform->GetAngle());
				//	//�݂����ȃR�[�h���K�v�B
				//}
			}
		}
		//for (auto soundData : _soundEvents[nowAnim]) {
		//	//�t���[������v���������Đ��B
		//	if (work->NowFrame() == soundData->playFrame)
		//	{
		//		soundData->info.source->Play(soundData->info.isLoop);
		//	}
		//}
	}
	else {
		// Animation�R���|�[�l���g���A�^�b�`����Ă��Ȃ��B
		abort();
	}
}