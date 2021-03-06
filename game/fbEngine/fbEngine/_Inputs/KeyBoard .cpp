#include"fbstdafx.h"
#include "KeyBoard .h"

HRESULT KeyBoard::Initialize(LPDIRECTINPUT8 Dinput)
{
	//デバイス作成
	if (FAILED(Dinput->CreateDevice(
		GUID_SysKeyboard,	// キーボードを受け付ける
		&keyDevice,		// IDirectInputDevice8ポインタ
		NULL)))			// 使わない
	{
		return false;
	}

	//受け取る構造体のフォーマットを設定
	if (FAILED(keyDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return false;
	}


	// 入力デバイスへのアクセス権を取得
	keyDevice->Acquire();

	ZeroMemory(now, sizeof(now));
	ZeroMemory(old, sizeof(old));

	return D3D_OK;
}

void KeyBoard::Update()
{
	//前の情報をコピー
	memcpy(old, now, sizeof(old));
	// キーボード操作情報を受け取る
	keyDevice->GetDeviceState(
		sizeof(now),	// パラメータ バッファサイズ
		&now);		// 受け取る構造体のアドレス
}

bool KeyBoard::isPush(int key)
{
	//前フレームで押されていない &&
	//今押されている
	return (!(old[key] & 0x80) && now[key] & 0x80);
}

bool KeyBoard::isPressed(int key)
{
	//格納されていると先頭のbitが立つので0x80と論理積をとる
	return ((now[key] & 0x80) > 0);
}

bool KeyBoard::KeyRepeat(int key, float interval)
{
	if (isPressed(key))
	{
		if (isPush(key))
		{
			_RepeatTimer[key] = 0.0f;
			return true;
		}
		//時間加算。
		_RepeatTimer[key] += Time::DeltaTime();
		if (_RepeatTimer[key] >= interval)
		{
			_RepeatTimer[key] = 0.0f;
			return true;
		}
	}

	return false;
}
