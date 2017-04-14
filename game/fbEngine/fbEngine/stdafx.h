// stdafx.h : �W���̃V�X�e�� �C���N���[�h �t�@�C���̃C���N���[�h �t�@�C���A�܂���
// �Q�Ɖ񐔂������A�����܂�ύX����Ȃ��A�v���W�F�N�g��p�̃C���N���[�h �t�@�C��
// ���L�q���܂��B
//

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
#include <memory>


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
//�R���p�C����ʂ����߂ɕK�v
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

using namespace std;

#include <vector>
#include <map>
#include <list>
#include <typeinfo>

#define SAFE_RELEASE(p)			{ if(p) { (p)->Release(); (p) = NULL; } }	//�C���X�^���X�����[�X
#define SAFE_DELETE_ARRAY(p)	{ if(p!=nullptr) { delete[] (p);   (p) = nullptr; } }	//�C���X�^���X�z��폜
#define SAFE_DELETE(p)			{ if(p!=nullptr) { delete (p);     (p) = nullptr; } }	//�C���X�^���X�폜

//�ϐ�count���g����num��J��Ԃ�
#define FOR(count,num) for(int count = 0;count < (int)num;count++)
//�V���O���g���擾
#define INSTANCE(p) p::Instance()
//�󂯎�����l�𕶎���ɕϊ�(enum��ϐ������\�B)
#define STRING(var) #var

static const float FontSize = 300.0f;

#include "System.h"
#include "Noncopyable.h"
#include "Assert.h"
#include "Time.h"
#include "Random.h"
#include "Vector.h"
#include "Color.h"
#include "Support.h"
#include "Transform.h"
#include "GameObject.h"

#include "SoundEngine.h"
#include "GraphicsDevice.h"		//Direct3D�Ǘ��w�b�_�C���N���[�h
#include "GameObjectManager.h"
#include "SceneManager.h"
#include "EffectManager.h"
#include "TextureManager.h"
#include "SkinModelManager.h"
#include "RenderTargetManager.h"
#include "InputManager.h"
//�����n
//���̂Ȃ��łق��̂����C���N���[�h���Ă���B
#include "../bulletPhysics/src/btBulletDynamicsCommon.h"
#include "../bulletPhysics/src/BulletCollision/CollisionDispatch/btGhostObject.h"
#include "Physics.h"

extern HWND g_MainWindow;	//�E�B���h�E�n���h��
extern Vector2 g_WindowSize;