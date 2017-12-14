#pragma once
#include "GameObject.h"
//����Đ��N���X
class Movie:public GameObject
{
public:
	enum class MovieState
	{
		Play,
		Pause,
		Stop
	};
public:
	Movie(const char* name) :GameObject(name) {}
	~Movie()
	{
		//�C���^�[�t�F�[�X�������[�X
		//������Ƃ��Ƌt���ɉ��
		SAFE_RELEASE(_MediaEvent);
		SAFE_RELEASE(_MediaControl);
		SAFE_RELEASE(_GraphBuilder);

		// COM�I��
		CoUninitialize();
	}

	void Awake()override;
	void Render()override;

	//�����ǂݍ��ށB
	//[in]�@����̃p�X�B
	void LoadVideo(const wstring& filename);
	//������Đ�����B
	void Play();
	//������ꎞ��~����B
	void Pause();
	//��������S�ɒ�~����B
	void Stop();
private:
	//GraphBuilder�C���^�[�t�F�[�X
	IGraphBuilder* _GraphBuilder = nullptr;
	//���f�B�A�R���g���[���[
	IMediaControl* _MediaControl = nullptr;
	//���f�B�A�C�x���g
	IMediaEvent* _MediaEvent = nullptr;
};