#include"fbstdafx.h"
#include "Movie.h"

void Movie::Awake()
{
	//COM�̏�����
	//HRESULT hRes = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
	//if (FAILED(hRes)) {
	//	// COM���������s
	//}
	//
	//// IGraphBuilder�C���^�[�t�F�C�X�̎擾
	//hRes = CoCreateInstance(
	//	CLSID_FilterGraph,
	//	NULL,
	//	CLSCTX_INPROC_SERVER,
	//	IID_IGraphBuilder,
	//	(void **)(&_GraphBuilder)
	//);

	////VMR9�쐬�B
	//CoCreateInstance(
	//	CLSID_VideoMixingRenderer9,
	//	NULL,
	//	CLSCTX_INPROC_SERVER,
	//	IID_IBaseFilter,
	//	(void**)&_BaseFilterVMR9);
	////�O���t�t�B���^�ɒǉ�
	//_GraphBuilder->AddFilter(_BaseFilterVMR9, L"VMR9");

	//IVMRFilterConfig *pVMRCfg = NULL;
	//_BaseFilterVMR9->QueryInterface(IID_IVMRFilterConfig9, (void**)&pVMRCfg);
	////�E�B���h�E���X���[�h��(�t���X�N���[��)
	//pVMRCfg->SetRenderingMode(VMRMode_Windowless);
	////�Ƃ肠�����g��Ȃ��̂Ŕj��
	//pVMRCfg->Release();

	////�O���t�B�b�N�r���_�[�쐬�B
	//CoCreateInstance(
	//	CLSID_CaptureGraphBuilder2,
	//	NULL,
	//	CLSCTX_INPROC_SERVER,
	//	IID_ICaptureGraphBuilder2,
	//	(void**)&_CGB2);
}

void Movie::Init(const wstring& filename)
{
	//�o�͂���E�B���h�E�ݒ�
	IVMRWindowlessControl *pVMRWndCont = NULL;
	_BaseFilterVMR9->QueryInterface(IID_IVMRWindowlessControl9, (void**)&pVMRWndCont);
	pVMRWndCont->SetVideoClippingWindow(g_MainWindow);
	//�j��
	pVMRWndCont->Release();

	wstring path = L"Asset/Movie/" + filename;
	IBaseFilter *pSource = NULL;
	HRESULT hRes;
	BSTR str = L"Asset/Movie/op.wav";
	_MediaControl->RenderFile(str);
	//Source Filter���쐬���A�t�B���^�O���t�ɒǉ����܂�
	//HRESULT hRes = _GraphBuilder->AddSourceFilter(path.c_str(), L"FiltaName", &pSource);
	//pBuilder->SetOutputFileName(&MEDIASUBTYPE_Avi, L"C:\\Example.avi", &ppf, &pSink);

	//�o�͑��H�o�͂���O���t�B�b�N�r���_�[
	//hRes = _CGB2->SetFiltergraph(_GraphBuilder);

	//�\�[�X�t�B���^��VMR9�ɐڑ�
	//hRes = _CGB2->RenderStream(0, 0, pSource, 0, _BaseFilterVMR9);
	//�f�t�H���g�I�[�f�B�I�ɐڑ�
	//hRes = _CGB2->RenderStream(0, 0, pSource, 0, 0);

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

void Movie::test()
{
	HRESULT hRes;
	// COM��������
	hRes = CoInitialize(NULL);

	// FilterGraph�𐶐�
	hRes = CoCreateInstance(CLSID_FilterGraph,
		NULL,
		CLSCTX_INPROC,
		IID_IGraphBuilder,
		(LPVOID *)&_GraphBuilder);

	// MediaControl�C���^�[�t�F�[�X�擾
	hRes = _GraphBuilder->QueryInterface(IID_IMediaControl,
		(LPVOID *)&_MediaControl);

	hRes = _GraphBuilder->QueryInterface(IID_IMediaEvent,
		(LPVOID *)&_MediaEvent);

	BSTR str = L"Asset/Movie/op.wmv";
	// Graph�𐶐�
	hRes = _MediaControl->RenderFile(str);

	IVideoWindow *pVideoWindow;

	hRes = _GraphBuilder->QueryInterface(IID_IVideoWindow,
		(LPVOID *)&pVideoWindow);
	hRes = pVideoWindow->put_Owner((OAHWND)g_MainWindow);

	hRes = pVideoWindow->put_WindowStyle(WS_CHILD | WS_CLIPSIBLINGS);

	RECT rect;
	GetClientRect(g_MainWindow, &rect);
	hRes = pVideoWindow->SetWindowPosition(0, 0,
		rect.right - rect.left, rect.bottom - rect.top);

	hRes = pVideoWindow->SetWindowForeground(OATRUE);
	hRes = pVideoWindow->put_Visible(OATRUE);

	//����̍Đ����Ԃ̎擾(�b�P��)
	REFTIME length;
	IMediaPosition *pMediaPosition;
	hRes = _GraphBuilder->QueryInterface(IID_IMediaPosition,
		(LPVOID *)&pMediaPosition);
	hRes = pMediaPosition->get_Duration(&length);
	SAFE_RELEASE(pMediaPosition);

	// ���f�B�A�Đ�
	if (FAILED(hRes = _MediaControl->Run()))
		return;

	// �ҋ@���Ԃ̐ݒ�(�~���b)
	long pEvCode;
	hRes = _MediaEvent->WaitForCompletion(length*(REFTIME)1000.0, &pEvCode);

	//�O���t�t�B���^��~
	hRes = _MediaControl->Stop();

	// ���������
	_MediaControl->Release();
	_GraphBuilder->Release();

	// COM�I��
	CoUninitialize();

	return;
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