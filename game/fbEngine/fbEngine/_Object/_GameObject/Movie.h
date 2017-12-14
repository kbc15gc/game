#pragma once
#include "GameObject.h"
//動画再生クラス
class Movie:public GameObject
{
public:
	enum class MovieState
	{
		Play,
		Pause,
		Stop
	};
public:
	Movie(const char* name) :GameObject(name) {}
	~Movie()
	{
		//インターフェースをリリース
		//作ったときと逆順に解放
		SAFE_RELEASE(_MediaEvent);
		SAFE_RELEASE(_MediaControl);
		SAFE_RELEASE(_GraphBuilder);

		// COM終了
		CoUninitialize();
	}

	void Awake()override;
	void Render()override;

	//動画を読み込む。
	//[in]　動画のパス。
	void LoadVideo(const wstring& filename);
	//動画を再生する。
	void Play();
	//動画を一時停止する。
	void Pause();
	//動画を完全に停止する。
	void Stop();
private:
	//GraphBuilderインターフェース
	IGraphBuilder* _GraphBuilder = nullptr;
	//メディアコントローラー
	IMediaControl* _MediaControl = nullptr;
	//メディアイベント
	IMediaEvent* _MediaEvent = nullptr;
};