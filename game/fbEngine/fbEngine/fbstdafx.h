/**
* プリコンパイル済みヘッダの定義.
*/
#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Windows ヘッダーから使用されていない部分を除外します。
// Windows ヘッダー ファイル:
#include <windows.h>
// C ランタイム ヘッダー ファイル
#include <stdlib.h>
#include <malloc.h>
#include <tchar.h>


// TODO: プログラムに必要な追加ヘッダーをここで参照してください
//ダイレクトＸ
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#include <d3d9.h>
#include <d3dx9.h>
//XAudio
#pragma comment(lib, "X3daudio.lib")
#include <X3DAudio.h>
#include <XAudio2.h>
#include <XAudio2fx.h>
//DirectShow
#pragma comment(lib, "Strmiids.lib")
#pragma comment(lib, "Quartz.lib")
#include <dshow.h>
#include <strmif.h>
#include <objbase.h>
#include <vmr9.h>
//COMのコンパイラを通すために必要
#define _WIN32_DCOM
//XInput
#pragma comment(lib, "Xinput.lib")
#include <XInput.h>
//DirectInput
// DirectInputバージョンの宣言
#define	DIRECTINPUT_VERSION	0x0800
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#include <dinput.h>
//たぶん時間のやつ
#pragma comment(lib, "winmm.lib ")

//std
using namespace std;
#include <vector>
#include <map>
#include <list>
#include <typeinfo>
#include <memory>		//スマートポインタ
#include <algorithm>
#include <fstream>		//ファイル操作
#include <functional>	//関数ポインタ
#include <string>

static const float FontSize = 512.0f;

//Include
#include "_Include\Definition.h"

//System
#include "_System\System.h"
#include "_System\GraphicsDevice.h"

#include"_System\Game.h"

//Value
#include "_Value/Vector.h"
#include "_Value/Color.h"

//Input
#include "_Inputs\InputManager.h"

//Support
#include "_Support/Assert.h"
#include "_Support/Support.h"
#include "_Support/Time.h"
#include "_Support/Random.h"

//Resource
#include "_Resource\TextureManager.h"
#include "_Resource\Material.h"
#include "_Object\_Component\_3D\Animation.h"
#include "_Resource\SkinModelData.h"
#include "_Resource\SkinModelManager.h"
#include "MeshColliderManager.h"

//Object
#include "_Support\RenderTargetManager.h"

//Component
#include "_Object\_Component\Transform.h"
#include "_Object\_Component\_3D\SkinModel.h"
#include "_Object\_Component\_Sound\SoundEngine.h"

//GameOject
#include "_Object\_GameObject\GameObjectManager.h"

//Physics
#include "../bulletPhysics/src/btBulletDynamicsCommon.h"
#include "../bulletPhysics/src/BulletCollision/CollisionDispatch/btGhostObject.h"
#include "_Include\CollisionInclude.h"
#include "_Object\_Component\_Physics\Physics.h"

//Scene
#include "_Scene\SceneManager.h"

//json
#include"_Data\Json\picojson.h"
#include"_Data\Json\nkJsonData.h"

extern HWND g_MainWindow;	//ウィンドウハンドル
extern Vector2 g_WindowSize; 
extern Vector2 g_FrameSize;
extern Vector2 g_StartWindowSize;