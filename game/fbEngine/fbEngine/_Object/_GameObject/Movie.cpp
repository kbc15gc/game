#include"fbstdafx.h"
#include "Movie.h"

void Movie::Awake()
{
	//COMの初期化
	HRESULT hRes = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
	if (FAILED(hRes)) {
		// COM初期化失敗
	}
	
	// IGraphBuilderインターフェイスの取得
	hRes = CoCreateInstance(
		CLSID_FilterGraph,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_IGraphBuilder,
		(void **)(&_GraphBuilder)
	);

	//VMR9作成。
	CoCreateInstance(
		CLSID_VideoMixingRenderer9,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_IBaseFilter,
		(void**)&_BaseFilterVMR9);
	//グラフフィルタに追加
	_GraphBuilder->AddFilter(_BaseFilterVMR9, L"VMR9");

	IVMRFilterConfig *pVMRCfg = NULL;
	_BaseFilterVMR9->QueryInterface(IID_IVMRFilterConfig9, (void**)&pVMRCfg);
	//ウィンドウレスモードへ(フルスクリーン)
	pVMRCfg->SetRenderingMode(VMRMode_Windowless);
	//とりあえず使わないので破棄
	pVMRCfg->Release();

	//グラフィックビルダー作成。
	CoCreateInstance(
		CLSID_CaptureGraphBuilder2,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_ICaptureGraphBuilder2,
		(void**)&_CGB2);
}

void Movie::Init(const wstring& filename)
{
	//出力するウィンドウ設定
	IVMRWindowlessControl *pVMRWndCont = NULL;
	_BaseFilterVMR9->QueryInterface(IID_IVMRWindowlessControl9, (void**)&pVMRWndCont);
	HRESULT hr = pVMRWndCont->SetVideoClippingWindow(g_MainWindow);
	//破棄
	pVMRWndCont->Release();

	wstring path = L"Asset/Movie/" + filename;
	IBaseFilter *pSource = NULL;
	//Source Filterを作成し、フィルタグラフに追加します
	hr = _GraphBuilder->AddSourceFilter(path.c_str(), L"FiltaName", &pSource);

	//初期化
	HRESULT hRes = _CGB2->SetFiltergraph(_GraphBuilder);

	//ソースフィルタをVMR9に接続
	_CGB2->RenderStream(0, 0, pSource, 0, _BaseFilterVMR9);
	//デフォルトオーディオに接続
	_CGB2->RenderStream(0, &MEDIATYPE_Audio, pSource, 0, 0);

	// 描画領域の設定（接続後でないとエラーになる）
	LONG W, H;
	RECT SrcR, DestR;
	//動画の縦横サイズ取得
	hRes = pVMRWndCont->GetNativeVideoSize(&W, &H, NULL, NULL);
	SetRect(&SrcR, 0, 0, W, H);
	GetClientRect(g_MainWindow, &DestR);
	hRes = pVMRWndCont->SetVideoPosition(&SrcR, &DestR);
	pVMRWndCont->Release();           // ウィンドウレスコントロールはもう必要ない


	if (FAILED(hRes))
		return;
}

void Movie::Play()
{
	// メディアコントロールインターフェイスの取得
	//フィルタグラフの流れを管理
	HRESULT hRes = _GraphBuilder->QueryInterface(IID_IMediaControl, (void**)(&_MediaControl));
	if (FAILED(hRes))
		return;

	// メディアイベントインターフェイスの取得
	//再生や停止の際にその乗法をイベントとして通知
	hRes = _GraphBuilder->QueryInterface(IID_IMediaEvent, (void **)(&_MediaEvent));
	if (FAILED(hRes))
		return;

	//動画の再生時間の取得(秒単位)
	REFTIME length;
	IMediaPosition *pMediaPosition;
	_GraphBuilder->QueryInterface(IID_IMediaPosition,
		(LPVOID *)&pMediaPosition);
	pMediaPosition->get_Duration(&length);
	SAFE_RELEASE(pMediaPosition);

	// メディア再生
	if (FAILED(hRes = _MediaControl->Run()))
		return;

	// 待機時間の設定(ミリ秒)
	long pEvCode;
	hRes = _MediaEvent->WaitForCompletion(length*(REFTIME)1000.0, &pEvCode);

	//グラフフィルタ停止
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