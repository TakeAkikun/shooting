#pragma once

//ゲーム全体のヘッダファイル
#include "DxLib.h"      //DxLibのヘッダファイル（必須）


//マクロ定義
#define GAME_TITLE "ゲームタイトル" //ゲームタイトル
#define GAME_WIDTH   1280            //ゲーム画面の幅
#define GAME_HEIGHT  720             //ゲーム画面の幅
#define GAME_COLOR   32              //ゲーム画面の色数

#define GAME_OVER    44444           //ゲームオーバーの時のフラグ
#define GAME_CLEAR   77777           //ゲームクリアの時のフラグ

#define GAME_ICON_ID 333             //ゲームのアイコンのID

#define GAME_WINDOW_BAR 0            //ウィンドウバーの種類

#define GAME_DEBUG TRUE              //デバッグモード

//列挙型
enum GAME_SCENE
{
	GAME_SCENE_TITLE,
	GAME_SCENE_PLAY,
	GAME_SCENE_END,
	GAME_SCENE_CHANGE
};  //ゲームのシーン