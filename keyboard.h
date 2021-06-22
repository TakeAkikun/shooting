#pragma once

//ヘッダファイルを読み込み
#include "geme.h"

//キーボードのヘッダファイル

//マクロ定義
#define KEY_KIND_MAX 256 //取得するキーの種類

//キーボード構造体
struct KEYBOARD
{
	char TempKeyState[KEY_KIND_MAX];     //入力状態を管理
	int AllKeyState[KEY_KIND_MAX];       //すべてのキーの入力時間を管理
	int OldKeyState[KEY_KIND_MAX];       //以前のすべてのキーの入力時間を管理
};

//外部のグローバル変数
extern KEYBOARD keyboard;

//プロトタイプ宣言
extern VOID AllKeyUpdate(VOID);                         //全てのキーの状態を取得する
extern BOOL KeyDown(int KEY_INPUT);                     //特定のキーを押しているか？
extern BOOL KeyClick(int KEY_INPUT);                    //特定のキーをあげているか？
extern BOOL KeyDownKeep(int KEY_INPUT, int millSec);    //特定のキーを指定のミリ秒押し続けているか？
