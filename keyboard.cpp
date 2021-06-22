//キーボードのソースファイル

//ヘッダファイルの読み込み
#include "keyboard.h"

//グローバル変数
KEYBOARD keyboard;

//関数

/// <summary>
/// キーの入力状態を更新する
/// </summary>
VOID AllKeyUpdate(VOID)
{

	//直前のキー入力をとっておく
	for (int i = 0; i < KEY_KIND_MAX; i++)
	{
		keyboard.OldKeyState[i] = keyboard.AllKeyState[i];
	}


	//すべてのキーの入力状態を得る
	GetHitKeyStateAll(&keyboard.TempKeyState[0]);


	//押されているキーの時間を更新する
	for (int i = 0; i < KEY_KIND_MAX; i++)
	{
		//キーコードを押しているとき
		if (keyboard.TempKeyState[i] != 0)
		{
			keyboard.AllKeyState[i]++;   //押している時間を加算
		}
		else
		{
			keyboard.AllKeyState[i] = 0; //押している時間を0に戻す
		}
	}

	return;
}


/// <summary>
/// キーを押しているかキーコードで判断する
/// </summary>
/// <param name="KEY_INPUT">キーコード</param>
/// <returns>押されていたらTRUE</returns>
BOOL KeyDown(int KEY_INPUT_)
{
	if (keyboard.AllKeyState[KEY_INPUT_] != 0)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


/// <summary>
/// キーを上げているかキーコードで判断する
/// </summary>
/// <param name="KEY_INPUT">キーコード</param>
/// <returns>上げていたらTRUE</returns>
BOOL KeyUp(int KEY_INPUT_)
{
	if (keyboard.OldKeyState[KEY_INPUT_] != 0 &&  //直前は押していたが
		keyboard.AllKeyState[KEY_INPUT_] == 0)    //今は押していない時
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


/// <summary>
/// キーをクリックしたかキーコードで判断する
/// </summary>
/// <param name="KEY_INPUT">キーコード</param>
/// <returns>クリックしたらTRUE</returns>
BOOL KeyClick(int KEY_INPUT_)
{
	if (keyboard.OldKeyState[KEY_INPUT_] != 0 &&  //直前は押していたが
		keyboard.AllKeyState[KEY_INPUT_] == 0)    //今は押していない時
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


/// <summary>
/// キーを押し続けているか、キーコードで判断する
/// </summary>
/// <param name="MilliTime">キーコード</param>
/// <returns>キーを押し続けているミリ秒</returns>
BOOL KeyDownKeep(int KEY_INPUT_, int MilliTime)
{
	//1秒は1000ミリ秒
	float MilliSec = 1000.0f;

	//押し続けている時間は、ミリ秒数×FPS値
	//例）1500ミリ秒押す⇒1000ミリは1.5秒×60FPS　=　90
	int UpdateTime = (MilliTime / MilliSec) * 60;

	if (keyboard.AllKeyState[KEY_INPUT_] > UpdateTime)
	{
		return TRUE;   //押し続けている
	}
	else
	{
		return FALSE;  //押し続けていない
	}
}