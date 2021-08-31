//マウス処理のソースファイル

//ヘッダファイルの読み込み
#include "geme.h"
#include "mouse.h"
#include "FPS.h"

//グローバル変数

//マウス入力を取得
MOUSE mouse;

//関数

VOID MouseUpdate(VOID)
{
	//マウスの以前の情報を取得
	mouse.OldPoint = mouse.Point;

	//マウスの以前のボタン入力を取得
	mouse.OldButton[MOUSE_INPUT_LEFT] = mouse.Button[MOUSE_INPUT_LEFT];
	mouse.OldButton[MOUSE_INPUT_MIDDLE] = mouse.Button[MOUSE_INPUT_MIDDLE];
	mouse.OldButton[MOUSE_INPUT_RIGHT] = mouse.Button[MOUSE_INPUT_RIGHT];

	//マウスの座標を取得
	GetMousePoint(&mouse.Point.x, &mouse.Point.y);

	//マウスの座標が画面外の場合は、画面内に収める
	if (mouse.Point.x < 0) { mouse.Point.x = 0; }
	if (mouse.Point.x > GAME_WIDTH) { mouse.Point.x = GAME_WIDTH; }
	if (mouse.Point.y < 0) { mouse.Point.y = 0; }
	if (mouse.Point.y > GAME_HEIGHT) { mouse.Point.x = GAME_HEIGHT; }

	//マウスの押しているボタンを取得
	mouse.InputValue = GetMouseInput();

	//以下でマスク処理を行う
	//左ボタンを押しているかチェック(TRUEは0以外)
	if ((mouse.InputValue & MOUSE_INPUT_LEFT) == MOUSE_INPUT_LEFT) { mouse.Button[MOUSE_INPUT_LEFT]++; }        //押している
	if ((mouse.InputValue & MOUSE_INPUT_LEFT) != MOUSE_INPUT_LEFT) { mouse.Button[MOUSE_INPUT_LEFT] = 0; }      //押していない

	//中ボタンを押しているかチェック(TRUEは0以外)
	if ((mouse.InputValue & MOUSE_INPUT_MIDDLE) == MOUSE_INPUT_MIDDLE) { mouse.Button[MOUSE_INPUT_MIDDLE]++; }  //押している
	if ((mouse.InputValue & MOUSE_INPUT_MIDDLE) != MOUSE_INPUT_MIDDLE) { mouse.Button[MOUSE_INPUT_MIDDLE] = 0; }//押していない

	//右ボタンを押しているかチェック(TRUEは0以外)
	if ((mouse.InputValue & MOUSE_INPUT_RIGHT) == MOUSE_INPUT_RIGHT) { mouse.Button[MOUSE_INPUT_RIGHT]++; }     //押している
	if ((mouse.InputValue & MOUSE_INPUT_RIGHT) != MOUSE_INPUT_RIGHT) { mouse.Button[MOUSE_INPUT_RIGHT] = 0; }   //押していない

	//ホイールの回転量を取得
	mouse.WheelValue = GetMouseWheelRotVol();

	return;
}

/// <summary>
/// ボタンを押しているか、マウスコードで判断する
/// </summary>
/// <param name="MOUSE_INPUT_">MOUSE_INPUT_</param>
/// <returns>ボタンを押している：TRUE</returns>
BOOL MouseDown(int MOUSE_INPUT_)
{
	//マウスコードのボタンを押している時
	if (mouse.Button[MOUSE_INPUT_] != 0)
	{
		return TRUE;  //ボタンを押している
	}
	else
	{
		return FALSE; //ボタンを押していない
	}
}

/// <summary>
/// ボタンを押し上げたか、マウスコードで判断する
/// </summary>
/// <param name="MOUSE_INPUT_">MOUSE_INPUT_</param>
/// <returns>ボタンを押し上げている：TRUE</returns>
BOOL MouseUp(int MOUSE_INPUT_)
{
	//直前は押しているが、今は押していない時
	if (mouse.OldButton[MOUSE_INPUT_] >= 1 &&
		mouse.Button[MOUSE_INPUT_]    == 0)
	{
		return TRUE;  //ボタンを押し上げている
	}
	else
	{
		return FALSE; //ボタンを押し上げていない
	}
}

/// <summary>
/// ボタンを押し続けているか、マウスコードで判断する
/// </summary>
/// <param name="MOUSE_INPUT_">MOUSE_INPUT_</param>
/// <param name="milliTime">ボタンを押し続けている時間(ミリ秒)</param>
/// <returns>ボタンを押し続けている：TRUE</returns>
BOOL MouseDownKeep(int MOUSE_INPUT_, int milliTime)
{
	float MilliSec = 1000.0f; //1秒は1000ミリ秒

	//押し続けている時間＝秒数×FPS値
	int UpdateTime = (milliTime / MilliSec) * fps.Value;

	if (mouse.Button[MOUSE_INPUT_] > UpdateTime)
	{
		return TRUE;  //押し続けている
	}
	else
	{
		return FALSE; //押し続けていない
	}
}

/// <summary>
/// マウスをクリックしたか、マウスコードで判断する
/// </summary>
/// <param name="MOUSE_INPUT_">MOUSE_INPUT_</param>
/// <returns>クリック：TRUE</returns>
BOOL MouseClick(int MOUSE_INPUT_)
{
	//直前は押しているが、今は押していない時
	if (mouse.OldButton[MOUSE_INPUT_] >= 1 &&
		mouse.Button[MOUSE_INPUT_]    == 0)
	{
		return TRUE;  //クリックした（押す⇒上げる　で一回）
	}
	else
	{
		return FALSE; //クリックしていないor押し続けている
	}
}

/// <summary>
/// マウスの情報を描画する
/// </summary>
/// <param name=""></param>
VOID MouseDraw(VOID)
{
	if (GAME_DEBUG == TRUE)
	{
		//マウスの座標を描画
		DrawFormatString(0, GAME_HEIGHT - 40, GetColor(255, 255, 255), "MOUSE[x:%4d/y:%4d]", mouse.Point.x, mouse.Point.y);
	}

	return;
}

/// <summary>
/// マウスが矩形領域をクリックしたか？
/// </summary>
/// <param name="rect">矩形領域</param>
/// <param name="MOUSE_INPUT_">マウスコード</param>
/// <returns></returns>
BOOL MouseRectClick(RECT rect, int MOUSE_INPUT_)
{
	//点と四角の当たり判定
	if (CheckCollPointToRect(mouse.Point, rect) == TRUE)
	{
		//マウスをクリックしているか？
		return MouseClick(MOUSE_INPUT_);
	}

	return FALSE;
}

BOOL MouseMaruClick(MARU circle, int MOUSE_INPUT_)    //マウスが円領域をクリックしたか？
{
	//点と円の当たり判定
	if (CheckCollPointToMaru(mouse.Point, circle) == TRUE)
	{
		//マウスをクリックしているか？
		return MouseClick(MOUSE_INPUT_);
	}

	return FALSE;
}