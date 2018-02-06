#include"stdafx.h"
#include "EndingScene.h"
#include "fbEngine/_Object/_GameObject/Movie.h"

void EndingScene::Start()
{
	//エンディング動画。
	auto movie = INSTANCE(GameObjectManager)->AddNew<Movie>("movie", 10);
	movie->LoadVideo(L"ending.wmv");
	movie->Play();
	INSTANCE(SceneManager)->ChangeScene("TitleScene");
}

void EndingScene::Update()
{
}