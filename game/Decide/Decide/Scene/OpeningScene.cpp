#include"stdafx.h"
#include "OpeningScene.h"
#include "fbEngine/_Object/_GameObject/Movie.h"

void OpeningScene::Start()
{
	//�G���f�B���O����B
	auto movie = INSTANCE(GameObjectManager)->AddNew<Movie>("movie", 10);
	movie->LoadVideo(L"op.wmv");
	movie->Play();
	INSTANCE(SceneManager)->ChangeScene("GameScene",true);
}

void OpeningScene::Update()
{
}
