#include"fbstdafx.h"
#include "Movie.h"

void Movie::Awake()
{
	HRESULT hRes;

	// COMを初期化
	if ((hRes = CoInitialize(NULL)) == S_FALSE)
	{
		MessageBox(NULL, "COMの初期化に失敗しました。", "動画初期化エラー", MB_ICONERROR);
		return;
	}

	// FilterGraphを生成
	if (FAILED(hRes = CoCreateInstance(CLSID_FilterGraph,
		NULL,
		CLSCTX_INPROC,
		IID_IGraphBuilder,
		(LPVOID *)&_GraphBuilder)))
	{
		MessageBox(NULL, "フィルタグラフィックの生成に失敗しました。", "動画初期化エラー", MB_ICONERROR);
		return;
	}

	// MediaControlインターフェース取得
	if (FAILED(hRes = _GraphBuilder->QueryInterface(IID_IMediaControl,
		(LPVOID *)&_MediaControl)))
	{
		MessageBox(NULL, "メディアコントローラーの生成に失敗しました。", "動画初期化エラー", MB_ICONERROR);
		return;
	}

	if (FAILED(hRes = _GraphBuilder->QueryInterface(IID_IMediaEvent, (LPVOID *)&_MediaEvent)))
	{
		MessageBox(NULL, "メディアイベントの生成に失敗しました。", "動画初期化エラー", MB_ICONERROR);
		return;
	}

	// VMR9フィルタを用意
	CoCreateInstance(CLSID_VideoMixingRenderer9, NULL,
		CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void**)&_VMR9);

	// VMR9フィルタをGraphに追加
	_GraphBuilder->AddFilter(_VMR9, L"VMR9");

	// CaptureGraphBuilder2というキャプチャ用GraphBuilderを生成する
	hRes = CoCreateInstance(CLSID_CaptureGraphBuilder2, NULL, CLSCTX_INPROC,
		IID_ICaptureGraphBuilder2,
		(LPVOID *)&_CaptureGraphBuilder2);

	// FilterGraphをセットする
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
	//パス連結。
	wstring filepath = L"Asset/Movie/" + filename;

	BSTR bstr = const_cast<wchar_t*>(filepath.c_str());
	//動画ファイル読み込み
	if (FAILED(hRes = _GraphBuilder->RenderFile(bstr, NULL)))
	{
		char err[512];
		sprintf(err, "FilePath:%s\nHRESULT:", filepath.c_str());

		MessageBox(NULL, err, "動画読み込みエラー", MB_ICONERROR);
		return;
	}

	// 再生するファイルをSourceFilterとして用意
	//hRes = _GraphBuilder->AddSourceFilter(filepath.c_str(), L"Source", &_Source);	
	//hRes = _GraphBuilder->Connect(_VMR9, _Source);

	// Graphを生成
	//hRes = _CaptureGraphBuilder2->RenderStream(0, 0, _Source, 0, _VMR9);

	//ウィンドウの設定。
	{
		IVideoWindow *pVideoWindow;
		hRes = _GraphBuilder->QueryInterface(IID_IVideoWindow, (LPVOID *)&pVideoWindow);
		//動画を再生するウィンドウを指定。
		hRes = pVideoWindow->put_Owner((OAHWND)g_MainWindow);
		hRes = pVideoWindow->put_WindowStyle(WS_CHILD | WS_CLIPSIBLINGS);

		RECT rect;
		GetClientRect(g_MainWindow, &rect);
		//ウィンドウサイズ？フルスクリーン設定。
		hRes = pVideoWindow->SetWindowPosition(0, 0,
			rect.right - rect.left, rect.bottom - rect.top);

		hRes = pVideoWindow->SetWindowForeground(OATRUE);
		hRes = pVideoWindow->put_Visible(OATRUE);
	}
}

void Movie::Play()
{
	HRESULT hRes;

	//動画の再生時間の取得(秒単位)
	REFTIME length;
	IMediaPosition *pMediaPosition;
	hRes = _GraphBuilder->QueryInterface(IID_IMediaPosition, (LPVOID *)&pMediaPosition);
	hRes = pMediaPosition->get_Duration(&length);
	SAFE_RELEASE(pMediaPosition);

	// メディア再生
	if (FAILED(hRes = _MediaControl->Run()))
	{
		MessageBox(NULL, "動画が読み込まれないかも", "動画再生エラー", MB_ICONERROR);
		return;
	}

	//// 待機時間の設定(ミリ秒)
	//long pEvCode;
	//hRes = _MediaEvent->WaitForCompletion(length*(REFTIME)1000.0, &pEvCode);

	////グラフフィルタ停止
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