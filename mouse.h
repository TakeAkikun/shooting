#pragma once
//マウス処理のヘッダファイル

//ヘッダファイルの読み込み

#include "geme.h"
#include "shape.h"

//マクロ定義

#define MOUSE_BUTTON_CODE  129  //0x0000~0x0080まで(マウスのボタン)

//構造体

  //マウス構造体
struct MOUSE
{
	int InputValue = 0; //GetMouseInputの値が入る
	int WheelValue = 0; //マウスホイールの回転量(奥：プラス値　手前：マイナス値)
	iPOINT Point;       //マウスの座標が入る
	iPOINT OldPoint;    //マウスの座標が入る
	int OldButton[MOUSE_BUTTON_CODE];  //マウスのボタン入力(直前)が入る
	int Button[MOUSE_BUTTON_CODE];     //マウスのボタン入力が入る
};

//外部のグローバル変数

extern MOUSE mouse;

//プロトタイプ宣言
//externは外部に関数がある場合の目印としてつけている

extern VOID MouseUpdate(VOID);                              //マウスの入力情報を更新する
extern BOOL MouseDown(int MOUSE_INPUT_);                    //ボタンを押しているか、マウスコードで判断する
extern BOOL MouseUp(int MOUSE_INPUT_);                      //ボタンを押し上げたか、マウスコードで判断する
extern BOOL MouseDownKeep(int MOUSE_INPUT_, int milliTime); //ボタンを押し続けているか、マウスコードで判断する
extern BOOL MouseClick(int MOUSE_INPUT_);                   //マウスをクリックしたか、マウスコードで判断する
extern VOID MouseDraw(VOID);                                //マウスの情報を描画する
extern BOOL MouseRectClick(RECT rect, int MOUSE_INPUT_);    //マウスが矩形領域をクリックしたか？
extern BOOL MouseMaruClick(MARU circle, int MOUSE_INPUT_);    //マウスが円領域をクリックしたか？