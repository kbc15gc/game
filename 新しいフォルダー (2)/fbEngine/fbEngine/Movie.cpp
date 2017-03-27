#include "Movie.h"

void Movie::Awake()
{
	//COM������
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
	//�E�B���h�E���X���[�h��
	pVMRCfg->SetRenderingMode(VMRMode_Windowless);
	//�Ƃ肠�����g��Ȃ��̂Ŕj��
	pVMRCfg->Release();

	//�o�͂���E�B���h�E�ݒ�
	IVMRWindowlessControl *pVMRWndCont = NULL;
	_BaseFilterVMR9->QueryInterface(IID_IVMRWindowlessControl9, (void**)&pVMRWndCont);
	pVMRWndCont->SetVideoClippingWindow(g_MainWindow);
	//�j��
	pVMRWndCont->Release();

	//�\�[�X�t�@�C���l�[��
	WCHAR wFileName[] = L"Asset/Movie/Bemybaby.avi";
	IBaseFilter *pSource = NULL;
	//Source Filter���쐬���A�t�B���^�O���t�ɒǉ����܂�
	_GraphBuilder->AddSourceFilter(wFileName, L"FiltaName", &pSource);

	ICaptureGraphBuilder2 *pCGB2 = NULL;
	HRESULT hr = CoCreateInstance(
		CLSID_CaptureGraphBuilder2,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_ICaptureGraphBuilder2,
		(void**)&pCGB2);
	//������
	hr = pCGB2->SetFiltergraph(_GraphBuilder);

	//�\�[�X�t�B���^��VMR9�ɐڑ�
	pCGB2->RenderStream(0, 0, pSource, 0, _BaseFilterVMR9);
	//�f�t�H���g�I�[�f�B�I�ɐڑ�
	pCGB2->RenderStream(0, &MEDIATYPE_Audio, pSource, 0, 0);

	// �`��̈�̐ݒ�i�ڑ���łȂ��ƃG���[�ɂȂ�j
	LONG W, H;
	RECT SrcR, DestR;
	//����̏c���T�C�Y�擾
	hr = pVMRWndCont->GetNativeVideoSize(&W, &H, NULL, NULL);
	SetRect(&SrcR, 0, 0, W, H);
	GetClientRect(g_MainWindow, &DestR);
	hr = pVMRWndCont->SetVideoPosition(&SrcR, &DestR);
	pVMRWndCont->Release();           // �E�B���h�E���X�R���g���[���͂����K�v�Ȃ�


	if (FAILED(hRes))
		return;

	// ���f�B�A�R���g���[���C���^�[�t�F�C�X�̎擾
	//�t�B���^�O���t�̗�����Ǘ�
	hRes = _GraphBuilder->QueryInterface(IID_IMediaControl, (void**)(&_MediaControl));
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
		return ;

	// �ҋ@���Ԃ̐ݒ�(�~���b)
	long pEvCode;
	hRes = _MediaEvent->WaitForCompletion(length*1000, &pEvCode);

	//�O���t�t�B���^��~
	_MediaControl->Stop();

	//�C���^�[�t�F�[�X�������[�X
	//������Ƃ��Ƌt���ɉ��
	SAFE_RELEASE(_MediaEvent);
	SAFE_RELEASE(_MediaControl);
	SAFE_RELEASE(_BaseFilterVMR9);
	SAFE_RELEASE(pCGB2);
	//SAFE_RELEASE(_GraphBuilder);
	CoUninitialize();
}

void Movie::Start()
{
}

void Movie::Update()
{
}
