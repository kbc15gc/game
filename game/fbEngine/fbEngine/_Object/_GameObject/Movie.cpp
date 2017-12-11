#include"fbstdafx.h"
#include "Movie.h"

void Movie::Awake()
{
	//COM�̏�����
	HRESULT hRes = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
	if (FAILED(hRes)) {
		// COM���������s
	}
	
	// IGraphBuilder�C���^�[�t�F�C�X�̎擾
	hRes = CoCreateInstance(
		CLSID_FilterGraph,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_IGraphBuilder,
		(void **)(&_GraphBuilder)
	);

	//VMR9�쐬�B
	CoCreateInstance(
		CLSID_VideoMixingRenderer9,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_IBaseFilter,
		(void**)&_BaseFilterVMR9);
	//�O���t�t�B���^�ɒǉ�
	_GraphBuilder->AddFilter(_BaseFilterVMR9, L"VMR9");

	IVMRFilterConfig *pVMRCfg = NULL;
	_BaseFilterVMR9->QueryInterface(IID_IVMRFilterConfig9, (void**)&pVMRCfg);
	//�E�B���h�E���X���[�h��(�t���X�N���[��)
	pVMRCfg->SetRenderingMode(VMRMode_Windowless);
	//�Ƃ肠�����g��Ȃ��̂Ŕj��
	pVMRCfg->Release();

	//�O���t�B�b�N�r���_�[�쐬�B
	CoCreateInstance(
		CLSID_CaptureGraphBuilder2,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_ICaptureGraphBuilder2,
		(void**)&_CGB2);
}

void Movie::Init(const wstring& filename)
{
	//�o�͂���E�B���h�E�ݒ�
	IVMRWindowlessControl *pVMRWndCont = NULL;
	_BaseFilterVMR9->QueryInterface(IID_IVMRWindowlessControl9, (void**)&pVMRWndCont);
	HRESULT hr = pVMRWndCont->SetVideoClippingWindow(g_MainWindow);
	//�j��
	pVMRWndCont->Release();

	wstring path = L"Asset/Movie/" + filename;
	IBaseFilter *pSource = NULL;
	//Source Filter���쐬���A�t�B���^�O���t�ɒǉ����܂�
	hr = _GraphBuilder->AddSourceFilter(path.c_str(), L"FiltaName", &pSource);

	//������
	HRESULT hRes = _CGB2->SetFiltergraph(_GraphBuilder);

	//�\�[�X�t�B���^��VMR9�ɐڑ�
	_CGB2->RenderStream(0, 0, pSource, 0, _BaseFilterVMR9);
	//�f�t�H���g�I�[�f�B�I�ɐڑ�
	_CGB2->RenderStream(0, &MEDIATYPE_Audio, pSource, 0, 0);

	// �`��̈�̐ݒ�i�ڑ���łȂ��ƃG���[�ɂȂ�j
	LONG W, H;
	RECT SrcR, DestR;
	//����̏c���T�C�Y�擾
	hRes = pVMRWndCont->GetNativeVideoSize(&W, &H, NULL, NULL);
	SetRect(&SrcR, 0, 0, W, H);
	GetClientRect(g_MainWindow, &DestR);
	hRes = pVMRWndCont->SetVideoPosition(&SrcR, &DestR);
	pVMRWndCont->Release();           // �E�B���h�E���X�R���g���[���͂����K�v�Ȃ�


	if (FAILED(hRes))
		return;
}

void Movie::Play()
{
	// ���f�B�A�R���g���[���C���^�[�t�F�C�X�̎擾
	//�t�B���^�O���t�̗�����Ǘ�
	HRESULT hRes = _GraphBuilder->QueryInterface(IID_IMediaControl, (void**)(&_MediaControl));
	if (FAILED(hRes))
		return;

	// ���f�B�A�C�x���g�C���^�[�t�F�C�X�̎擾
	//�Đ����~�̍ۂɂ��̏�@���C�x���g�Ƃ��Ēʒm
	hRes = _GraphBuilder->QueryInterface(IID_IMediaEvent, (void **)(&_MediaEvent));
	if (FAILED(hRes))
		return;

	//����̍Đ����Ԃ̎擾(�b�P��)
	REFTIME length;
	IMediaPosition *pMediaPosition;
	_GraphBuilder->QueryInterface(IID_IMediaPosition,
		(LPVOID *)&pMediaPosition);
	pMediaPosition->get_Duration(&length);
	SAFE_RELEASE(pMediaPosition);

	// ���f�B�A�Đ�
	if (FAILED(hRes = _MediaControl->Run()))
		return;

	// �ҋ@���Ԃ̐ݒ�(�~���b)
	long pEvCode;
	hRes = _MediaEvent->WaitForCompletion(length*(REFTIME)1000.0, &pEvCode);

	//�O���t�t�B���^��~
	_MediaControl->Stop();
}

//void Movie::Update()
//{
//	if(GetAsyncKeyState(VK_SPACE))
//		_MediaControl->Run();
//}
//
//void Movie::Render()
//{
//	_MediaControl->Pause();
//	_MediaControl->Run();
//}