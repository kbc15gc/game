#include"fbstdafx.h"
#include "Movie.h"

void Movie::Awake()
{
	HRESULT hRes;

	// COM��������
	if ((hRes = CoInitialize(NULL)) == S_FALSE)
	{
		MessageBox(NULL, "COM�̏������Ɏ��s���܂����B", "���揉�����G���[", MB_ICONERROR);
		return;
	}

	// FilterGraph�𐶐�
	if (FAILED(hRes = CoCreateInstance(CLSID_FilterGraph,
		NULL,
		CLSCTX_INPROC,
		IID_IGraphBuilder,
		(LPVOID *)&_GraphBuilder)))
	{
		MessageBox(NULL, "�t�B���^�O���t�B�b�N�̐����Ɏ��s���܂����B", "���揉�����G���[", MB_ICONERROR);
		return;
	}

	// MediaControl�C���^�[�t�F�[�X�擾
	if (FAILED(hRes = _GraphBuilder->QueryInterface(IID_IMediaControl,
		(LPVOID *)&_MediaControl)))
	{
		MessageBox(NULL, "���f�B�A�R���g���[���[�̐����Ɏ��s���܂����B", "���揉�����G���[", MB_ICONERROR);
		return;
	}

	if (FAILED(hRes = _GraphBuilder->QueryInterface(IID_IMediaEvent, (LPVOID *)&_MediaEvent)))
	{
		MessageBox(NULL, "���f�B�A�C�x���g�̐����Ɏ��s���܂����B", "���揉�����G���[", MB_ICONERROR);
		return;
	}

	// VMR9�t�B���^��p��
	CoCreateInstance(CLSID_VideoMixingRenderer9, NULL,
		CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void**)&_VMR9);

	// VMR9�t�B���^��Graph�ɒǉ�
	_GraphBuilder->AddFilter(_VMR9, L"VMR9");

	// CaptureGraphBuilder2�Ƃ����L���v�`���pGraphBuilder�𐶐�����
	hRes = CoCreateInstance(CLSID_CaptureGraphBuilder2, NULL, CLSCTX_INPROC,
		IID_ICaptureGraphBuilder2,
		(LPVOID *)&_CaptureGraphBuilder2);

	// FilterGraph���Z�b�g����
	hRes = _CaptureGraphBuilder2->SetFiltergraph(_GraphBuilder);
}

void Movie::Render()
{
	_MediaControl->Pause();
	_MediaControl->Run();
}

void Movie::LoadVideo(const wstring & filename)
{
	HRESULT hRes;
	//�p�X�A���B
	wstring filepath = L"Asset/Movie/" + filename;

	BSTR bstr = const_cast<wchar_t*>(filepath.c_str());
	//����t�@�C���ǂݍ���
	if (FAILED(hRes = _GraphBuilder->RenderFile(bstr, NULL)))
	{
		char err[512];
		sprintf(err, "FilePath:%s\nHRESULT:", filepath.c_str());

		MessageBox(NULL, err, "����ǂݍ��݃G���[", MB_ICONERROR);
		return;
	}

	// �Đ�����t�@�C����SourceFilter�Ƃ��ėp��
	//hRes = _GraphBuilder->AddSourceFilter(filepath.c_str(), L"Source", &_Source);	
	//hRes = _GraphBuilder->Connect(_VMR9, _Source);

	// Graph�𐶐�
	//hRes = _CaptureGraphBuilder2->RenderStream(0, 0, _Source, 0, _VMR9);

	//�E�B���h�E�̐ݒ�B
	{
		IVideoWindow *pVideoWindow;
		hRes = _GraphBuilder->QueryInterface(IID_IVideoWindow, (LPVOID *)&pVideoWindow);
		//������Đ�����E�B���h�E���w��B
		hRes = pVideoWindow->put_Owner((OAHWND)g_MainWindow);
		hRes = pVideoWindow->put_WindowStyle(WS_CHILD | WS_CLIPSIBLINGS);

		RECT rect;
		GetClientRect(g_MainWindow, &rect);
		//�E�B���h�E�T�C�Y�H�t���X�N���[���ݒ�B
		hRes = pVideoWindow->SetWindowPosition(0, 0,
			rect.right - rect.left, rect.bottom - rect.top);

		hRes = pVideoWindow->SetWindowForeground(OATRUE);
		hRes = pVideoWindow->put_Visible(OATRUE);
	}
}

void Movie::Play()
{
	HRESULT hRes;

	//����̍Đ����Ԃ̎擾(�b�P��)
	REFTIME length;
	IMediaPosition *pMediaPosition;
	hRes = _GraphBuilder->QueryInterface(IID_IMediaPosition, (LPVOID *)&pMediaPosition);
	hRes = pMediaPosition->get_Duration(&length);
	SAFE_RELEASE(pMediaPosition);

	// ���f�B�A�Đ�
	if (FAILED(hRes = _MediaControl->Run()))
	{
		MessageBox(NULL, "���悪�ǂݍ��܂�Ȃ�����", "����Đ��G���[", MB_ICONERROR);
		return;
	}

	//// �ҋ@���Ԃ̐ݒ�(�~���b)
	//long pEvCode;
	//hRes = _MediaEvent->WaitForCompletion(length*(REFTIME)1000.0, &pEvCode);

	////�O���t�t�B���^��~
	//hRes = _MediaControl->Stop();
}

void Movie::Pause()
{
}

void Movie::Stop()
{
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