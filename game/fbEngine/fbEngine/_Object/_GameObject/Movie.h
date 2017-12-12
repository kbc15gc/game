#pragma once
#include "GameObject.h"
//動画再生クラス
class Movie:public GameObject
{
public:
	Movie(const char* name) :GameObject(name) {}
	~Movie()
	{
		//インターフェースをリリース
		//作ったときと逆順に解放
		SAFE_RELEASE(_MediaEvent);
		SAFE_RELEASE(_MediaControl);
		SAFE_RELEASE(_BaseFilterVMR9);
		SAFE_RELEASE(_CGB2);
		//SAFE_RELEASE(_GraphBuilder);
		CoUninitialize();
	}

	void Awake()override;

	void Init(const wstring& filename);
	void Play();
	void test();
private:
	//GraphBuilderインターフェース
	IGraphBuilder* _GraphBuilder = nullptr;
	//レンダリングを担当するVMR9フィルタ
	IBaseFilter* _BaseFilterVMR9 = nullptr;
	//メディアコントローラー
	IMediaControl* _MediaControl = nullptr;
	//メディアイベント
	IMediaEvent* _MediaEvent = nullptr;
	ICaptureGraphBuilder2* _CGB2 = nullptr;
};