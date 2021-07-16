//Copyright(c) 2020
//Hamamatu Mirai Sougou Senmongakkou GameCreate Y.Suzuki
//All rights reserved.

//図形のソースファイル

//ヘッダファイル読み込み
#include "geme.h"
#include "shape.h"
#include <math.h>

//関数

/// <summary>
/// 矩形領域同士の当たり判定
/// </summary>
/// <param name="a">領域a</param>
/// <param name="b">領域b</param>
/// <returns></returns>
BOOL CheckCollRectToRect(RECT a, RECT b)
{
	if (a.left < b.right &&
		a.top < b.bottom &&
		a.right > b.left &&
		a.bottom > b.top )
	{
		return TRUE;  //当たっている
	}

	return FALSE;  //当たっていない
}

/// <summary>
/// 四角と点の当たり判定
/// </summary>
/// <param name="pt">iPOINT型の点</param>
/// <param name="r">矩形領域</param>
/// <returns></returns>
BOOL CheckCollPointToRect(iPOINT pt, RECT r)
{
	//点の位置が四角の中にあるとき
	if (pt.x > r.left && pt.x < r.right &&
		pt.y > r.top  && pt.y < r.bottom)
	{
		return TRUE;
	}

	return FALSE;
}

/// <summary>
/// 円と点の当たり判定
/// </summary>
/// <param name="pt">iPOINT型の点</param>
/// <param name="m">円領域</param>
/// <returns></returns>
BOOL CheckCollPointToMaru(iPOINT pt, MARU m)
{
	//ピタゴラスの定理を使用

	//直角三角形を求める
	float a = pt.x - m.center.x;    //底辺
	float b = pt.y - m.center.y;    //高さ
	float c = sqrtf(a * a + b * b); //斜辺（sq rt = square root = 平方根）

	//斜辺の長さが円の半径以下ならば、円の中にいる
	if (c <= m.radius)
	{
		return TRUE;
	}

	return FALSE;
}

/// <summary>
/// 円と円の当たり判定
/// </summary>
/// <param name="maru1">円領域1</param>
/// <param name="maru2">円領域2</param>
/// <returns></returns>
BOOL CheckCollMaruToMaru(MARU maru1, MARU maru2)
{
	//ピタゴラスの定理を使用

	//直角三角形を求める
	float a = maru1.center.x - maru2.center.x;    //底辺
	float b = maru1.center.y - maru2.center.y;    //高さ
	float c = sqrtf(a * a + b * b);               //斜辺（sq rt = square root = 平方根）

	//斜辺の長さが円の半径1と2を足した長さ以下ならば、円の中にいる
	if (c <= maru1.radius + maru2.radius)
	{
		return TRUE;
	}

	return FALSE;
}

/// <summary>
/// RECT型を一時的に渡す
/// </summary>
/// <param name="left">左</param>
/// <param name="top">上</param>
/// <param name="right">右</param>
/// <param name="bottom">下</param>
/// <returns>RECT型</returns>
RECT GetRect(int left, int top, int right, int bottom)
{
	//一時的にRECT型の変数を作って、戻り値で渡す

	RECT rect = { left,top,right,bottom };

	return rect;
}

/// <summary>
/// RECTを利用して四角を描画
/// </summary>
/// <param name="r">RECT構造体</param>
/// <param name="color">色</param>
/// <param name="b">塗りつぶす：TRUE, 塗りつぶさない：FALSE</param>
void DrawRect(RECT r, unsigned int color, bool b)
{
	//引数を基に描画
	DrawCircle(r.left, r.top, r.right, r.bottom, color, b);
	return;
}

/// <summary>
/// MARUを利用して四角を描画
/// </summary>
/// <param name="c">MARU構造体</param>
/// <param name="color">色</param>
/// <param name="b">塗りつぶす：TRUE, 塗りつぶさない：FALSE</param>
/// <param name="thick">線の太さ</param>
void DrawMaru(MARU c, unsigned int color, bool b, int thick)
{
	//引数を基に描画
	DrawCircle(c.center.x, c.center.y, c.radius, color, b, thick);
	return;
}