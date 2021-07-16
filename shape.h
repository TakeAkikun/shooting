#pragma once
//Copyright(c) 2020
//Hamamatu Mirai Sougou Senmongakkou GameCreate Y.Suzuki
//All rights reserved.

//図形のヘッダファイル
#include "geme.h"

//構造体

  //int型のPOINT構造体
struct iPOINT
{
	int x = -1;//座標の初期化
	int y = -1;
};

  //円の構造体
struct MARU
{
	iPOINT center;  //中心点
	int radius;     //半径
};

//プロトタイプ宣言
//externは外部に関数がある場合の目印としてつけている

extern BOOL CheckCollRectToRect(RECT a, RECT b);         //四角領域の当たり判定をチェックする
extern BOOL CheckCollPointToRect(iPOINT pt, RECT r);     //点と四角の当たり判定
extern BOOL CheckCollPointToMaru(iPOINT pt, MARU m);     //点と円の当たり判定
extern BOOL CheckCollMaruToMaru(MARU maru1, MARU maru2); //円と円の当たり判定

extern RECT GetRect(int left, int top, int right, int bottom);          //RECT型を一時的に渡す
extern void DrawRect(RECT r, unsigned int color, bool b);               //RECTを利用して四角を描画
extern void DrawMaru(MARU c, unsigned int color, bool b, int thick = 1);//MARUを利用して円を描画