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
	void Start()override;
	void Update()override;
	void Render()override;
private:
	//GraphBuilderインターフェース
	IGraphBuilder* _GraphBuilder;
	//レンダリングを担当するVMR9フィルタ
	IBaseFilter* _BaseFilterVMR9;
	//メディアコントローラー
	IMediaControl* _MediaControl;
	//メディアイベント
	IMediaEvent* _MediaEvent;
	ICaptureGraphBuilder2* _CGB2 = NULL;
};