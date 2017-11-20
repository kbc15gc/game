#pragma once
#include "GameObject.h"
//����Đ��N���X
class Movie:public GameObject
{
public:
	Movie(const char* name) :GameObject(name) {}
	~Movie()
	{
		//�C���^�[�t�F�[�X�������[�X
		//������Ƃ��Ƌt���ɉ��
		SAFE_RELEASE(_MediaEvent);
		SAFE_RELEASE(_MediaControl);
		SAFE_RELEASE(_BaseFilterVMR9);
		SAFE_RELEASE(_CGB2);
		//SAFE_RELEASE(_GraphBuilder);
		CoUninitialize();
	}

	void Awake()override;
	void Start()override;
	void Update()override;
	void Render()override;
private:
	//GraphBuilder�C���^�[�t�F�[�X
	IGraphBuilder* _GraphBuilder;
	//�����_�����O��S������VMR9�t�B���^
	IBaseFilter* _BaseFilterVMR9;
	//���f�B�A�R���g���[���[
	IMediaControl* _MediaControl;
	//���f�B�A�C�x���g
	IMediaEvent* _MediaEvent;
	ICaptureGraphBuilder2* _CGB2 = NULL;
};