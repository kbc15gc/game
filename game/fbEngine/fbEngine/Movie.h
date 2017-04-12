#pragma once
#include "GameObject.h"
//����Đ��N���X
class Movie:public GameObject
{
public:
	Movie(const char* name) :GameObject(name) {}

	void Awake()override;
	void Start()override;
	void Update()override;
private:
	//GraphBuilder�C���^�[�t�F�[�X
	IGraphBuilder* _GraphBuilder;
	//�����_�����O��S������VMR9�t�B���^
	IBaseFilter* _BaseFilterVMR9;
	//���f�B�A�R���g���[���[
	IMediaControl* _MediaControl;
	//���f�B�A�C�x���g
	IMediaEvent* _MediaEvent;
};