/**
* �v���R���p�C���ς݃w�b�_�̒�`.
*/
#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Windows �w�b�_�[����g�p����Ă��Ȃ����������O���܂��B
// Windows �w�b�_�[ �t�@�C��:
#include <windows.h>
// C �����^�C�� �w�b�_�[ �t�@�C��
#include <stdlib.h>
#include <malloc.h>
#include <tchar.h>


// TODO: �v���O�����ɕK�v�Ȓǉ��w�b�_�[�������ŎQ�Ƃ��Ă�������
//�_�C���N�g�w
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
//COM�̃R���p�C����ʂ����߂ɕK�v
#define _WIN32_DCOM
//XInput
#pragma comment(lib, "Xinput.lib")
#include <XInput.h>
//DirectInput
// DirectInput�o�[�W�����̐錾
#define	DIRECTINPUT_VERSION	0x0800
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#include <dinput.h>
//���Ԃ񎞊Ԃ̂��
#pragma comment(lib, "winmm.lib ")

//std
using namespace std;
#include <vector>
#include <map>
#include <list>
#include <typeinfo>
#include <memory>		//�X�}�[�g�|�C���^
#include <algorithm>
#include <fstream>		//�t�@�C������
#include <functional>	//�֐��|�C���^
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

extern HWND g_MainWindow;	//�E�B���h�E�n���h��
extern Vector2 g_WindowSize; 
extern Vector2 g_FrameSize;
extern Vector2 g_StartWindowSize;