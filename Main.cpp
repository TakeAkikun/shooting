
//ヘッダファイル読み込み
#include "geme.h"       //ゲーム全体のヘッダファイル
#include "keyboard.h"   //キーボードの処理
#include "FPS.h"        //FPSの処理

#include <math.h>       //数学用

//マクロ定義
#define TAMA_DIV_MAX   4//弾のMax値
#define TAMA_MAX      30//弾の総数
#define TEKI_KIND      8//敵の種類
#define TEKI_MAX      10//敵の総数

//=========================================================
//     構造体    
//=========================================================

//画像の構造体
struct IMAGE
{
	int handle = -1;  //画像のハンドル（管理番号）
	char path[255];   //画像の場所（パス）

	int X;            //X位置
	int Y;            //Y位置
	int width;        //幅
	int height;       //高さ

	BOOL IsDraw = FALSE;//画像が描画できる？
};

//キャラクターの構造体
struct CHARACTOR
{
	IMAGE img;        //画像の構造体
	int Xspead = 1;   //移動速度
	int Yspead = 1;   //移動速度
	RECT coll;        //当たり判定の領域(四角)
};

//動画の構造体
struct MOVIE
{
	int handle = -1;    //動画のハンドル
	char path[25];      //動画のパス

	int x;              //X位置
	int y;              //Y位置
	int width;          //幅
	int height;         //高さ

	int Volume = 255;   //ボリューム（0～255）
};

//音楽の構造体
struct AUDIO
{
	int handle = -1;    //音楽のハンドル
	char path[255];     //音楽のパス

	int Volume = -1;    //ボリューム（MIN 0～255 MAX）
	int playType = -1;  //BGM or SE
};

//弾の構造体
struct TAMA
{
	int handle[TAMA_DIV_MAX];  //画像のハンドル
	char path[255];            //画像のパス

	int DivTate;               //分割数（縦）
	int DivYoko;               //分割数（横）
	int DivMax;                //分割数（総数）

	int AnimeCnt = 0;          //アニメーションカウンタ
	int AnimeCntMax = 0;       //アニメーションカウンタMax

	int NowIndex = 0;          //現在の画像の要素数
	
	int startx;                //X初期位置
	int starty;                //Y初期位置

	float radius;              //半径
	float degree;              //角度

	int x;                     //X位置
	int y;                     //Y位置
	int width;                 //幅
	int height;                //高さ

	int spead;                //弾のスピード

	RECT coll;                 //当たり判定(矩形)

	BOOL IsDraw = FALSE;       //描画できる？
};

//=========================================================
//     変数作成     
//=========================================================

//画面の切り替え
BOOL IsFadeOut = FALSE;   //フェードアウト
BOOL IsFadeIn = FALSE;    //フェードイン

int fadeTimeMill = 2000;                    //切り替えミリ秒
int fadeTimeMax = fadeTimeMill / 1000 * 60; //ミリ秒をフレーム秒に変換

//フェードアウト
int fadeOutCntInit = 0;                     //初期値
int fadeOutCnt = fadeOutCntInit;            //フェードアウトのカウンタ
int fadeOutCutMax = fadeTimeMax;            //フェードアウトのカウンタXAX

//フェードイン
int fadeInCntInit = fadeTimeMax;            //初期値
int fadeInCnt = fadeInCntInit;              //フェードインのカウンタ
int fadeInCutMax = fadeTimeMax;             //フェードインのカウンタXAX

//シーンを管理する変数
GAME_SCENE GameScene;       //現在のゲームのシーン
GAME_SCENE OldGameScene;    //前回のゲームのシーン
GAME_SCENE NextGameScene;   //次回のゲームのシーン

//メニュー欄の表示フラグ
BOOL MenuFlag = FALSE;    //メニュー欄を表示するか？

//ヒント欄の表示フラグ
BOOL TipsFlag = FALSE;    //ヒント欄を表示するか？

//エンド画面の時のフラグ
//最初はクリアを入れておきます
int GameEndFlag = GAME_CLEAR;

//弾の画像
TAMA TanaMoto;         //元
TAMA Tama1[TAMA_MAX];  //実際に使う
TAMA Tama2[TAMA_MAX];  //

//弾の発射カウンタ
int tamaShotCnt = 0;
int tamaShotCnt2 = 0;
int tamaShotCntMax = 100;
int tamaShotCntMax2 = 10;
BOOL tamaAutoFlag = FALSE;

//爆発の画像のハンドル
TAMA Explosion;    //爆発のヤツ
int ExplosionChangeCntMax = 30; //画像を変えるタイミングMax

//背景画像
IMAGE back[2]; //背景は2つの画像

//プレイヤー
CHARACTOR Player;

//敵
CHARACTOR Teki_Moto[TEKI_KIND];
CHARACTOR teki[TEKI_MAX];
CHARACTOR Enemy;
int EnemyHP = 30;     //敵の体力

char tekiPath[TEKI_KIND][255] =
{
	{".\\image\\teki_blue.png"},
	{".\\image\\teki_gray.png"},
	{".\\image\\teki_green.png"},
	{".\\image\\teki_mizu.png"},
	{".\\image\\teki_purple.png"},
	{".\\image\\teki_yellow.png"},
	{".\\image\\teki_red.png"},
	{".\\image\\teki_red_big.png"}
};

//敵が出てくるカウント
int TekiAddCnt = 0;
int TekiAddCntMax = 60;  //60FPSで一回・・・1秒で一回

//ゲームのスコア
int Score = 0;

//=========================================================
//     関数     
//=========================================================

//キャメルケース・・・単語の先頭を大文字にする命名規則
//スネークケース・・・単語をアンダーバーでつなぐ命名規則

VOID Title(VOID);     //タイトル画面
VOID TitleProc(VOID); //タイトル画面（処理）
VOID TitleDraw(VOID); //タイトル画面（描画）

VOID Play(VOID);					                             //プレイ画面
VOID PlayProc(VOID);				                             //プレイ画面（処理）
VOID PlayDraw(VOID);				                             //プレイ画面（描画）

VOID End(VOID);						                             //エンド画面
VOID EndProc(VOID);					                             //エンド画面（処理）
VOID EndDraw(VOID);					                             //エンド画面（描画）

VOID Change(VOID);					                             //切り替え画面
VOID ChangeProc(VOID);				                             //切り替え画面（処理）
VOID ChangeDraw(VOID);				                             //切り替え画面（描画）

VOID DrawMenu(int Volum);			                             //メニュー画面（描画）
VOID DrawTips(VOID);				                             //ヒント画面（描画）

VOID ChangeScene(GAME_SCENE seane);                              //シーン切り替え

VOID CollUpdatePlayer(CHARACTOR* chara);                         //当たり判定の領域を更新(プレイヤー)
VOID CollUpdateEnemy(CHARACTOR* chara);                          //当たり判定の領域を更新(敵)
VOID CollUpdate(CHARACTOR* chara);                               //当たり判定の領域を更新
VOID CollUpdateTama(TAMA* tama);                                 //当たり判定の領域を更新(弾)

BOOL OnCollision(RECT coll1, RECT coll2);                        //当たっているかを調べる

VOID ChangeBGM(AUDIO* music);                                    //BGMの音量変更

void DrawTama(TAMA* tama);                                       //弾(マップチップ)の描画
VOID ShotTama(TAMA* tama, float deg);                            //弾の発射

BOOL GameLoad(VOID);                                             //ゲーム全体のデータを読み込み
VOID GameInit(VOID);                                             //ゲームデータの初期化
BOOL ImageInput(IMAGE* Image, const char* path);                 //ゲームの画像読み込み
BOOL MusicInput(AUDIO* music, const char* path, int volume, int playType);//ゲームのBGM読み込み
BOOL LoadImageMem(const char* Path, int* Handle, int xDiv, int yDiv);//ゲームのマップチップの読み込み

//=====================================================================================================================
//          ココからメインプログラム          
//=====================================================================================================================

// プログラムは WinMain から始まります
// Windousのプログラミング方法で動いている。（WinAPI）
// DxLibはDirectXというゲームプログラミングを簡単に使える仕組み
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	//こいつらは必ずInitを行う前に入力する
	SetOutApplicationLogValidFlag(FALSE);                //Log.txtを出力しない
	ChangeWindowMode(TRUE);                              //ウィンドウモードに設定
	SetMainWindowText(GAME_TITLE);                       //ゲームタイトルを設定
	SetGraphMode(GAME_WIDTH, GAME_HEIGHT, GAME_COLOR);   //ゲームの解像度の設定
	SetWindowSize(GAME_WIDTH, GAME_HEIGHT);              //ウィンドウの大きさを設定
	SetWindowStyleMode(GAME_WINDOW_BAR);                 //ウィンドウバーの状態
	SetBackgroundColor(255, 255, 255);                   //ウィンドウのデフォの色を設定
	SetWindowIconID(GAME_ICON_ID);                       //アイコンファイルを読み込み
	SetWaitVSyncFlag(TRUE);                              //ディスプレイの垂直同期を有効にする
	SetAlwaysRunFlag(TRUE);                              //ウィンドウをずっとアクティブにする



	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			    // エラーが起きたら直ちに終了
	}

	//ダブルバッファリング有効化
	SetDrawScreen(DX_SCREEN_BACK);

	//ゲーム全体の初期化
	if (!GameLoad())
	{
		//ゲームデータの読み込みに失敗したとき
		DxLib_End();  //DxLib終了
		return -1;    //異常終了
	}

	//ゲームデータの初期化
	GameInit();

	//最初のシーンはタイトル画面から
	GameScene = GAME_SCENE_TITLE;

	//無限ループ
	while (1)
	{

		if (ProcessMessage() != 0) { break; }  //メッセージを受け取り続ける
		if (ClearDrawScreen() != 0) { break; } //画面をクリア

		AllKeyUpdate();         //キーボード入力の更新

		//FPS値の更新
		FPSUpdate();

		//ESCキーで強制終了
		if (KeyClick(KEY_INPUT_ESCAPE) == TRUE) { break; }

		//以前のシーンを取得
		if (GameScene != GAME_SCENE_CHANGE)
		{
			OldGameScene = GameScene;
		}

		//シーンごとに処理を行う
		switch (GameScene)
		{
		case GAME_SCENE_TITLE:
			Title();            //タイトル画面
			break;
		case GAME_SCENE_PLAY:
			Play();             //プレイ画面
			break;
		case GAME_SCENE_END:
			End();              //エンド画面
			break;
		case GAME_SCENE_CHANGE:
			Change();           //切り替え画面
			break;
		default:
			break;
		}

		//シーンを切り替える
		if (OldGameScene != GameScene)
		{
			//以前のシーンが切り替え画面でないとき
			if (GameScene != GAME_SCENE_CHANGE)
			{
				NextGameScene = GameScene;     //次のシーンを保存
				GameScene = GAME_SCENE_CHANGE; //画面切り替えシーンに変える
			}
		}

		//FPS値を描画
		FPSDraw();

		//FPS値を待つ
		FPSWait();

		ScreenFlip();           //ダブルバッファリングした画面を描画
	}

	//読み込んだマップチップを解放
	for (int i = 0; i < TAMA_DIV_MAX; i++) { DeleteGraph(TanaMoto.handle[i]); }

	//終わるときの処理
	DeleteGraph(Player.img.handle);      //画像をメモリ上から削除
	DeleteGraph(Enemy.img.handle);       //
	DeleteGraph(back[0].handle);         //
	DeleteGraph(back[1].handle);         //
	for (int i = 0; i < TEKI_KIND; i++) {
		DeleteGraph(Teki_Moto[i].img.handle);
	}

	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;				    // ソフトの終了 
}


//=====================================================================================================================
//          ココから初期化関係          
//=====================================================================================================================


/// <summary>
/// ゲームデータの初期化
/// </summary>
/// <param name=""></param>
VOID GameInit(VOID)
{
	//ゲームスコアの初期化
	Score = 0;

	//プレイヤーを初期化
	Player.img.X = 500;
	Player.img.Y = 500;
	Player.Xspead = 300;
	Player.Yspead = 300;
	Player.img.IsDraw = TRUE;
	
	//当たり判定を更新
	CollUpdatePlayer(&Player);  //プレイヤーのアドレス

	//敵を初期化
	Enemy.img.X = 50;
	Enemy.img.Y = 50;
	Enemy.Xspead = 7;
	Enemy.Yspead = 7;
	Enemy.img.IsDraw = TRUE;

	//当たり判定の更新
	CollUpdate(&Enemy);  //敵のアドレス

	//敵機体の初期化
	for (int i = 0; i < TEKI_KIND; i++)
	{
		Teki_Moto[i].img.X = GAME_WIDTH / 2 - Teki_Moto[i].img.width;
		Teki_Moto[i].img.Y = -Teki_Moto[i].img.height;
		CollUpdatePlayer(&Teki_Moto[i]);  //当たり判定の更新
		Teki_Moto[i].img.IsDraw = FALSE;  //描画はしない
	}

	//背景画像の位置
	back[0].X = 0;
	back[0].Y = -back[0].height;
	back[0].IsDraw = TRUE;
	back[1].X = 0;
	back[1].Y = 0;
	back[1].IsDraw = TRUE;

	//弾の位置
	
	//位置を調整
	TanaMoto.x = -10; //画面外
	TanaMoto.y = -10; //画面外

	//当たり判定の更新
	CollUpdateTama(&TanaMoto);//弾のアドレス

	//全ての弾に情報をコピー
	for (int i = 0; i < TAMA_MAX; i++)
	{
		Tama1[i] = TanaMoto;
		Tama2[i] = TanaMoto;
	}
}


/// <summary>
/// ゲーム全体の初期化
/// </summary>
/// <returns>読み込み成功・・・TRUE｜読み込み失敗・・・FALSE</returns>
BOOL GameLoad()
{
	//ゲームスコアの初期化
	Score = 0;

	//弾の分割数を設定
	TanaMoto.DivYoko = 4;
	TanaMoto.DivTate = 1;

	//弾のパスをコピー
	strcpyDx(TanaMoto.path, ".\\image\\marudai_green.png");

	//マップチップを読み込み
	if (!LoadImageMem(TanaMoto.path, &TanaMoto.handle[0], TanaMoto.DivYoko, TanaMoto.DivTate)) { FALSE; }  //弾

	//画像の幅と高さを取得
	GetGraphSize(TanaMoto.handle[0], &TanaMoto.width, &TanaMoto.height);

	//位置を調整
	TanaMoto.x = -10; //画面外
	TanaMoto.y = -10; //画面外

	//速度
	TanaMoto.spead = 1;
	
	//アニメーションの速度
	TanaMoto.AnimeCntMax = 10;

	//当たり判定の更新
	CollUpdateTama(&TanaMoto);

	//画像は表示しない
	TanaMoto.IsDraw = FALSE;

	//全ての弾に情報をコピー
	for (int i = 0; i < TAMA_MAX; i++)
	{
		Tama1[i] = TanaMoto;
		Tama2[i] = TanaMoto;
	}

	//プレイヤーを初期化
	Player.img.X = 500;
	Player.img.Y = 500;
	Player.Xspead = 300;
	Player.Yspead = 300;
	Player.img.IsDraw = TRUE;

	//当たり判定を更新
	CollUpdatePlayer(&Player);  //プレイヤーの当たり判定のアドレス

	//敵を初期化
	Enemy.img.X = 50;
	Enemy.img.Y = 50;
	Enemy.Xspead = 7;
	Enemy.Yspead = 7;
	Enemy.img.IsDraw = TRUE;

	//当たり判定の更新
	CollUpdate(&Enemy);  //敵の当たり判定のアドレス

	//背景画像の位置
	back[0].X = 0;
	back[0].Y = -back[0].height;
	back[0].IsDraw = TRUE;
	back[1].X = 0;
	back[1].Y = 0;
	back[1].IsDraw = TRUE;

	//画像を読み込み
	if (!ImageInput(&Player.img, ".\\image\\Player.png")) { FALSE; }
	if (!ImageInput(&Enemy.img, ".\\image\\Enemy.jpeg")) { FALSE; }
	if (!ImageInput(&back[0], ".\\image\\hoshi.jpg")) { FALSE; }
	if (!ImageInput(&back[1], ".\\image\\hoshi_rev.jpg")) { FALSE; }

	//敵の画像
	for (int i = 0; i < TEKI_KIND; i++)
	{
		if (!ImageInput(&Teki_Moto[i].img, tekiPath[i])) { FALSE; }
		Teki_Moto[i].img.X = GAME_WIDTH / 2 - Teki_Moto[i].img.width;
		Teki_Moto[i].img.Y = -Teki_Moto[i].img.height;
		CollUpdatePlayer(&Teki_Moto[i]);  //当たり判定の更新
		Teki_Moto[i].img.IsDraw = FALSE;  //描画はしない
	}

	return TRUE;                    //全部読み込めたらTRUE
}


//=====================================================================================================================
//          ココから読み込み関係          
//=====================================================================================================================


/// <summary>
/// 画像をメモリに読み込み
/// </summary>
/// <param name="Image">Image構造体のアドレス</param>
/// <param name="path">Imageの画像パス</param>
/// <returns>TRUE or FALSE</returns>
BOOL ImageInput(IMAGE* Image, const char* path)
{
	//画像を読み込み
	strcpyDx(Image->path, path);  //パスのコピー
	Image->handle = LoadGraph(Image->path);          //画像の読み込み

	//画像が読み込めなかったときは、エラー(－1)が入る
	if (Image->handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),   //メインのウィンドウタイトル
			Image->path,             //メッセージ本文
			"画像読み込みエラー",    //メッセージタイトル
			MB_OK                    //ボタン
		);

		return FALSE;                //エラー終了
	}

	//画面の幅と高さを取得
	GetGraphSize(Image->handle, &Image->width, &Image->height);
}


/// <summary>
/// 音楽をメモリに読み込み
/// </summary>
/// <param name="music">Audio構造体のアドレス</param>
/// <param name="path">Audioの音楽パス</param>
/// <param name="volume">ボリューム</param>
/// <param name="playType">DX_PLAYTYPE_LOOP or DX_PLAYTYPE_BACK</param>
/// <returns>TRUE or FALSE</returns>
BOOL MusicInput(AUDIO* music, const char* path, int volume, int playType)
{
	//音楽を読み込み
	strcpyDx(music->path, path);  //パスのコピー
	music->handle = LoadSoundMem(music->path);          //音楽の読み込み

	//音楽が読み込めなかったときは、エラー(－1)が入る
	if (music->handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),       //メインのウィンドウタイトル
			music->path,                 //メッセージ本文
			"音楽読み込みエラー",        //メッセージタイトル
			MB_OK                        //ボタン
		);

		return FALSE;                    //エラー終了
	}

	music->playType = playType;                          //音楽をループさせる
	music->Volume = volume;                              //MAXが255
	ChangeVolumeSoundMem(music->Volume, music->handle);  //BGMのボリュームを変更

	return TRUE;
}


/// <summary>
/// マップチップをメモリに読み込み
/// </summary>
/// <param name="Path">画像のパス</param>
/// <param name="MapTip">ハンドル配列の先頭アドレス</param>
/// <param name="xDiv">画像の横の分割数</param>
/// <param name="yDiv">画像の縦の分割数</param>
/// <returns></returns>
BOOL LoadImageMem(const char* Path, int* Handle, int xDiv, int yDiv)
{
	//弾の読み込み
	int IsTamaLoad = 1;              //画像が読み込めたか？

	//一時的に使う画像のハンドル
	int MaptipHandle = LoadGraph(Path);

	//読み込みエラー
	if (MaptipHandle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),   //ウィンドウハンドル
			Path,              //本文
			"画像読み込みエラー",    //タイトル
			MB_OK                    //ボタン
		);

		return FALSE;                //読み込み失敗
	}

	int tamawidth = 0;
	int tamaheight = 0;
	GetGraphSize(MaptipHandle, &tamawidth, &tamaheight);

	//分割して読み込み
	if (LoadDivGraph(
		Path,                        //画像のパス
		TAMA_DIV_MAX,                //分割総数
		4, 1,                        //横と縦の分割数
		tamawidth / xDiv, tamaheight / yDiv,//分割後の画像一つ分の大きさ(X,Y)
		Handle
	) == FALSE)

	//分割エラー
	if (MaptipHandle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),   //ウィンドウハンドル
			Path,                    //本文
			"画像分割エラー",        //タイトル
			MB_OK                    //ボタン
		);

		return FALSE;                //読み込み失敗
	}

	//一時的に読み込んだ画像を解放
	DeleteGraph(MaptipHandle);

	return TRUE;
}


//=========================================================
//     下記は切り替え関数     
//=========================================================


/// <summary>
/// シーンを切り替える関数
/// </summary>
/// <param name="scene"></param>
VOID ChangeScene(GAME_SCENE scene)
{
	GameScene = scene;  //シーン切り替え
	IsFadeIn = FALSE;             //フェードインしない
	IsFadeOut = TRUE;             //フェードアウトする

	return;
}


//=====================================================================================================================
//           ココからタイトル          
//=====================================================================================================================

/// <summary>
/// タイトル画面
/// </summary>
/// <param name=""></param>
VOID Title(VOID)
{
	TitleProc();    //処理
	TitleDraw();    //描画

	return;
}

/// <summary>
/// タイトル画面の処理
/// </summary>
/// <param name=""></param>
VOID TitleProc(VOID)
{
	//メニュー画面とヒント画面が開いていない時限定
	if (MenuFlag == FALSE && TipsFlag == FALSE)
	{

		//プレイシーンに切り替え
		if (KeyClick(KEY_INPUT_RETURN) == TRUE) {
			//シーン切り替え
			//次のシーンの初期化をココで行うと楽

			//ゲームデータの初期化
			GameInit();

			//プレイ画面に切り替え
			ChangeScene(GAME_SCENE_PLAY);

			return;
		}
	}

	return;
}

/// <summary>
/// タイトル画面の描画
/// </summary>
/// <param name=""></param>
VOID TitleDraw(VOID)
{
	
	DrawString(0, 0, "タイトル画面", GetColor(0, 0, 0));
	return;
}


//=====================================================================================================================
//          ココからプレイ          
//=====================================================================================================================

/// <summary>
/// プレイ画面
/// </summary>
/// <param name=""></param>
VOID Play(VOID)
{
	PlayProc();    //処理
	PlayDraw();    //描画

	return;
}

/// <summary>
/// プレイ画面の処理
/// </summary>
/// <param name=""></param>
VOID PlayProc(VOID)
{
	//メニュー画面とヒント画面が開いていない時限定
	if (MenuFlag == FALSE && TipsFlag == FALSE)
	{
		//スペースキーを押しているとき
		if (KeyDown(KEY_INPUT_SPACE) == TRUE)
		{
			if (tamaShotCnt == 0)
			{
				//弾を発射(描画)する
				for (int i = 0; i < TAMA_MAX; i++)
				{
					if (Tama1[i].IsDraw == FALSE)
					{
						//角度を入れておく
						ShotTama(&Tama1[i], 315.0f);

						//一発出したら脱出
						break;
					}
				}

				//弾を発射(描画)する
				for (int i = 0; i < TAMA_MAX; i++)
				{
					if (Tama1[i].IsDraw == FALSE)
					{
						//角度を入れておく
						ShotTama(&Tama1[i], 225.0f);

						//一発出したら脱出
						break;
					}
				}
			}

			//弾の発射待ち
			if (tamaShotCnt < tamaShotCntMax)
			{
				tamaShotCnt++;
			}
			else
			{
				tamaShotCnt = 0;
			}

			if (tamaShotCnt2 == 0)
			{
				//弾を発射(描画)する
				for (int i = 0; i < TAMA_MAX; i++)
				{
					if (Tama2[i].IsDraw == FALSE)
					{
						//角度を入れておく
						ShotTama(&Tama2[i], 270.0f);

						//一発出したら脱出
						break;
					}
				}
			}

			//弾の発射待ち
			if (tamaShotCnt2 < tamaShotCntMax2)
			{
				tamaShotCnt2++;
			}
			else
			{
				tamaShotCnt2 = 0;
			}
		}

		//弾の移動
		for (int i = 0; i < TAMA_MAX; i++)
		{
			if (Tama1[i].IsDraw == TRUE)
			{
				if (tamaAutoFlag == FALSE && Tama1[i].y > Tama1[i].starty - 100)
				{
					Tama1[i].x = Tama1[i].startx + cos(Tama1[i].degree * DX_PI / 180.0f) * Tama1[i].radius;
					Tama1[i].y = Tama1[i].starty + sin(Tama1[i].degree * DX_PI / 180.0f) * Tama1[i].radius;

					//半径を足す
					Tama1[i].radius += Tama1[i].spead;
				}

				if (tamaAutoFlag == FALSE && Tama1[i].y <= Tama1[i].starty - 100)
				{
					Tama1[i].startx = Tama1[i].x;
					Tama1[i].starty = Tama1[i].y;
					Tama1[i].radius = 0.0f;

					tamaAutoFlag = TRUE;
				}

				if (tamaAutoFlag == TRUE)
				{
					/*
					自分のやり方（ちょーめんどくさい）
					//直角三角形の底辺
					//((Enemy.img.X + Enemy.img.width / 2) - (Tama1[i].startx + Tama1[i].width / 2))
					//直角三角形の高さ
					//((Tama1[i].starty + Tama1[i].height / 2) - (Enemy.img.Y + Enemy.img.height / 2))
					//直角三角形の斜辺(三平方の定理を使用)
					//sqrt((((Enemy.img.X + Enemy.img.width / 2) - (Tama1[i].startx + Tama1[i].width / 2)) * ((Enemy.img.X + Enemy.img.width / 2) - (Tama1[i].startx + Tama1[i].width / 2))) + 
					//(((Tama1[i].starty + Tama1[i].height / 2) - (Enemy.img.Y + Enemy.img.height / 2)) * ((Tama1[i].starty + Tama1[i].height / 2) - (Enemy.img.Y + Enemy.img.height / 2))))

					Tama1[i].x = Tama1[i].startx +
						//底辺 / 斜辺でcosを求める
						(((Enemy.img.X + Enemy.img.width / 2) - (Tama1[i].startx + Tama1[i].width / 2)) /
							sqrt((((Enemy.img.X + Enemy.img.width / 2) - (Tama1[i].startx + Tama1[i].width / 2)) * ((Enemy.img.X + Enemy.img.width / 2) - (Tama1[i].startx + Tama1[i].width / 2)) +
								(((Tama1[i].starty + Tama1[i].height / 2) - (Enemy.img.Y + Enemy.img.height / 2)) * ((Tama1[i].starty + Tama1[i].height / 2) - (Enemy.img.Y + Enemy.img.height / 2))))))
						* Tama1[i].radius;
					Tama1[i].y = Tama1[i].starty -
						//高さ / 斜辺でsinを求める
						(((Tama1[i].starty + Tama1[i].height / 2) - (Enemy.img.Y + Enemy.img.height / 2)) /
							sqrt((((Enemy.img.X + Enemy.img.width / 2) - (Tama1[i].startx + Tama1[i].width / 2)) * ((Enemy.img.X + Enemy.img.width / 2) - (Tama1[i].startx + Tama1[i].width / 2))) +
								(((Tama1[i].starty + Tama1[i].height / 2) - (Enemy.img.Y + Enemy.img.height / 2)) * ((Tama1[i].starty + Tama1[i].height / 2) - (Enemy.img.Y + Enemy.img.height / 2)))))
						* Tama1[i].radius;
					*/

					//atan2・・・直角三角形の斜辺以外の2辺から角度を出してくれるすごいヤツ
					Tama1[i].degree = (int)atan2(Enemy.img.Y - Tama1[i].starty, Enemy.img.X - Tama1[i].startx) / DX_PI * 180.0f;

					//弾の位置を修正
								//中心位置　　　＋　飛ばす角度⇒飛ばす距離を計算　＊　距離
					Tama1[i].x = Tama1[i].startx + cos(Tama1[i].degree * DX_PI / 180.0f) * Tama1[i].radius;
					Tama1[i].y = Tama1[i].starty + sin(Tama1[i].degree * DX_PI / 180.0f) * Tama1[i].radius;

					//半径を足す
					Tama1[i].radius += Tama1[i].spead;
				}

				//画面外に出たら描画をやめる
				if (Tama1[i].y + Tama1[i].height < 0 ||
					Tama1[i].y > GAME_HEIGHT ||
					Tama1[i].x + Tama1[i].width < 0 ||
					Tama1[i].x > GAME_WIDTH)
				{
					Tama1[i].IsDraw = FALSE;
					tamaAutoFlag = FALSE;
				}

				//当たり判定の更新
				CollUpdateTama(&Tama1[i]);
			}

			if (Tama2[i].IsDraw == TRUE)
			{
				//移動
				Tama2[i].x = Tama2[i].startx + cos(Tama2[i].degree * DX_PI / 180.0f) * Tama2[i].radius;
				Tama2[i].y = Tama2[i].starty + sin(Tama2[i].degree * DX_PI / 180.0f) * Tama2[i].radius;

				//半径を足す
				Tama2[i].radius += Tama2[i].spead;

				//画面外に出たら描画をやめる
				if (Tama2[i].y + Tama2[i].height < 0 ||
					Tama2[i].y > GAME_HEIGHT ||
					Tama2[i].x + Tama2[i].width < 0 ||
					Tama2[i].x > GAME_WIDTH)
				{
					Tama2[i].IsDraw = FALSE;
					tamaAutoFlag = FALSE;
				}

				//当たり判定の更新
				CollUpdateTama(&Tama2[i]);
			}
		}

		if (TekiAddCnt < TekiAddCntMax)
		{
			TekiAddCnt++;
		}
		else
		{
			//カウントリセット
			TekiAddCnt = 0;

			//敵の生成
			for (int i = 0; i < TEKI_MAX; i++)
			{
				//描画されていない敵を探す
				if (teki[i].img.IsDraw == FALSE)
				{
					if (Score < 10)
					{
						teki[i] = Teki_Moto[0];
					}
					else if (Score < 20)
					{
						teki[i] = Teki_Moto[1];
					}
					else
					{
						teki[i] = Teki_Moto[GetRand(TEKI_KIND - 1)];
					}

					teki[i].img.X = GetRand(3) * GAME_WIDTH / 4;
					teki[i].img.Y = -teki[i].img.height;

					teki[i].img.IsDraw = TRUE;  //描画する
					break;
				}
			}
		}

		//敵の処理
		for (int i = 0; i < TEKI_MAX; i++)
		{
			//描画している敵
			if (teki[i].img.IsDraw == TRUE)
			{
				teki[i].img.Y += 1;
				CollUpdateEnemy(&teki[i]);

				//画面外に出たら描画をやめる
				if (teki[i].img.Y > GAME_HEIGHT)
				{
					teki[i].img.IsDraw = FALSE;
				}
			}
		}

		//プレイヤーの操作

			//壁を突き抜けないようにif文を調整
		if (KeyDown(KEY_INPUT_UP) == TRUE && Player.img.Y > 0)
		{
			Player.img.Y -= Player.Yspead * fps.DeltaTime;   //上に移動

			//スピード高すぎてめり込むのを防止
			if (Player.img.Y < 0)
			{
				Player.img.Y = 0;
			}
		}

		if (KeyDown(KEY_INPUT_DOWN) == TRUE && Player.img.Y < GAME_HEIGHT - Player.img.height)
		{
			Player.img.Y += Player.Yspead * fps.DeltaTime;   //下に移動

			//スピード高すぎてめり込むのを防止
			if (Player.img.Y > GAME_HEIGHT - Player.img.height)
			{
				Player.img.Y = GAME_HEIGHT - Player.img.height;
			}
		}

		if (KeyDown(KEY_INPUT_LEFT) == TRUE && Player.img.X > 0)
		{
			Player.img.X -= Player.Xspead * fps.DeltaTime;   //左に移動

			//スピード高すぎてめり込むのを防止
			if (Player.img.X < 0)
			{
				Player.img.X = 0;
			}
		}

		if (KeyDown(KEY_INPUT_RIGHT) == TRUE && Player.img.X < GAME_WIDTH - Player.img.width)
		{
			Player.img.X += Player.Xspead * fps.DeltaTime;   //右に移動

			//スピード高すぎてめり込むのを防止
			if (Player.img.X > GAME_WIDTH - Player.img.width)
			{
				Player.img.X = GAME_WIDTH - Player.img.width;
			}
		}

		//敵
		Enemy.img.X += Enemy.Xspead;
		if (Enemy.img.X < 0 || Enemy.img.X + Enemy.img.width > GAME_WIDTH)
		{
			Enemy.Xspead = -Enemy.Xspead;
		}

		//当たり判定を更新
		CollUpdatePlayer(&Player);
		CollUpdateEnemy(&Enemy);

		//Hit判定
		for (int i = 0; i < TAMA_MAX; i++)
		{
			if (Enemy.img.IsDraw == TRUE && Tama1[i].IsDraw == TRUE && OnCollision(Tama1[i].coll, Enemy.coll) == TRUE)
			{
				EnemyHP--;
				Tama1[i].IsDraw = FALSE;
				tamaAutoFlag = FALSE;

				Score++;
			}
			
			if (Enemy.img.IsDraw == TRUE && Tama2[i].IsDraw == TRUE && OnCollision(Tama2[i].coll, Enemy.coll) == TRUE)
			{
				EnemyHP--;
				Tama2[i].IsDraw = FALSE;
				tamaAutoFlag = FALSE;

				Score++;
			}
		}

		//撃破判定
		for (int j = 0; j < TAMA_MAX; j++)
		{
			for (int i = 0; i < TEKI_MAX; i++)
			{
				if (teki[j].img.IsDraw == TRUE && Tama1[i].IsDraw == TRUE && OnCollision(Tama1[i].coll, teki[j].coll) == TRUE)
				{
					Tama1[i].IsDraw = FALSE;
					tamaAutoFlag = FALSE;

					teki[j].img.IsDraw = FALSE;

					Score++;
				}

				if (teki[j].img.IsDraw == TRUE && Tama2[i].IsDraw == TRUE && OnCollision(Tama2[i].coll, teki[j].coll) == TRUE)
				{
					Tama2[i].IsDraw = FALSE;
					tamaAutoFlag = FALSE;

					teki[j].img.IsDraw = FALSE;

					Score++;
				}
			}
		}

		if (EnemyHP <= 0)
		{
			Enemy.img.IsDraw = FALSE;

			//敵のHPをリセット
			EnemyHP = 30;

			//処理を強制終了
			return;
		}

		//エンドシーンに切り替え
		if (KeyClick(KEY_INPUT_RETURN) == TRUE) {
			//シーン切り替え
			//次のシーンの初期化をココで行うと楽

			//ゲームデータの初期化
			GameInit();

			//エンド画面に切り替え
			ChangeScene(GAME_SCENE_END);

		}
	}

	return;
}

/// <summary>
/// プレイ画面の描画
/// </summary>
/// <param name=""></param>
VOID PlayDraw(VOID)
{
	//背景の描画
	for (int i = 0; i < 2; i++)
	{
		//背景を描画
		DrawGraph(back[i].X, back[i].Y, back[i].handle, TRUE);

		//画像が下まで下がった時
		if (back[i].Y > GAME_HEIGHT)
		{
			back[i].Y = -back[i].height + 1;
		}

		//画像を動かす
		back[i].Y++;
	}

	//弾の描画
	for (int i = 0; i < TAMA_MAX; i++)
	{
		//描画されているとき
		if (Tama1[i].IsDraw == TRUE)
		{
			DrawTama(&Tama1[i]);

			if (GAME_DEBUG == TRUE)
			{
				//四角を描画
				DrawBox(Tama1[i].coll.left, Tama1[i].coll.top, Tama1[i].coll.right, Tama1[i].coll.bottom, GetColor(255, 0, 0), FALSE);
			}
		}

		//描画されているとき
		if (Tama2[i].IsDraw == TRUE)
		{
			DrawTama(&Tama2[i]);

			if (GAME_DEBUG == TRUE)
			{
				DrawBox(Tama2[i].coll.left, Tama2[i].coll.top, Tama2[i].coll.right, Tama2[i].coll.bottom, GetColor(255, 0, 0), FALSE);
			}
		}
	}

	//敵の描画
	for (int i = 0; i < TEKI_MAX; i++)
	{
		if (teki[i].img.IsDraw == TRUE)
		{
			//画像を描画
			DrawGraph(teki[i].img.X, teki[i].img.Y, teki[i].img.handle, TRUE);

			if (GAME_DEBUG == TRUE)
			{
				//四角を描画
				DrawBox(teki[i].coll.left, teki[i].coll.top, teki[i].coll.right, teki[i].coll.bottom, GetColor(0, 0, 255), FALSE);
			}
		}
	}

	//敵１の描画
	if (Enemy.img.IsDraw == TRUE)
	{
		//画像を描画
		DrawGraph(Enemy.img.X, Enemy.img.Y, Enemy.img.handle, TRUE);

		if (GAME_DEBUG == TRUE)
		{
			//四角を描画
			DrawBox(Enemy.coll.left, Enemy.coll.top, Enemy.coll.right, Enemy.coll.bottom, GetColor(0, 0, 255), FALSE);
		}
	}

	//プレイヤーの描画
	if (Player.img.IsDraw == TRUE)
	{
		//画像を描画
		DrawGraph(Player.img.X, Player.img.Y, Player.img.handle, TRUE);

		if (GAME_DEBUG == TRUE)
		{
			//四角を描画
			DrawBox(Player.coll.left, Player.coll.top, Player.coll.right, Player.coll.bottom, GetColor(255, 0, 0), FALSE);
		}
	}

	//スコアの描画
	int old = GetFontSize();
	SetFontSize(40);
	DrawFormatString(0, 100, GetColor(255, 255, 255), "SCORE：%05d", Score);
	SetFontSize(old);

	DrawString(0, 0, "プレイ画面", GetColor(0, 0, 0));
	return;
}

//=====================================================================================================================
//          ココから弾の描画          
//=====================================================================================================================

/// <summary>
/// 弾(マップチップ)の描画
/// </summary>
/// <param name="">弾の構造体のパス</param>
void DrawTama(TAMA* tama)
{
	if (tama->IsDraw == TRUE)
	{
		//弾の描画
		DrawGraph(tama->x, tama->y, tama->handle[tama->NowIndex], TRUE);

		if (tama->AnimeCnt < tama->AnimeCntMax)
		{
			(int)tama->AnimeCnt++;
		}
		else
		{
			//弾の添字が弾の分割数の最大よりも小さいとき
			if (tama->NowIndex < TAMA_DIV_MAX - 1)
			{
				tama->NowIndex++;         //次の画像へ
			}
			else
			{
				tama->NowIndex = 0;       //最初に戻す
			}

			tama->AnimeCnt = 0;
		}
	}
}


//=====================================================================================================================
//          ココから弾を飛ばす          
//=====================================================================================================================

/// <summary>
/// 弾を飛ばす関数
/// </summary>
/// <param name="tama">TAMA構造体のアドレス</param>
/// <param name="deg">角度</param>
VOID ShotTama(TAMA* tama, float deg)
{
		//弾を発射する
		tama->IsDraw = TRUE;

		//弾の位置決め
		tama->startx = Player.img.X + Player.img.width / 2 - tama->width / 2;
		tama->starty = Player.img.Y;

		//初めの位置を弾の位置に設定
		tama->x = tama->startx;
		tama->y = tama->starty;

		//弾の速度決め
		tama->spead = 6;

		//弾の角度決め
		tama->degree = deg;

		//弾の半径決め
		tama->radius = 0.0f;

		//弾の当たり判定を更新
		CollUpdateTama(tama);
}


//=====================================================================================================================
//          ココからエンド          
//=====================================================================================================================

/// <summary>
/// エンド画面
/// </summary>
/// <param name=""></param>
VOID End(VOID)
{
	EndProc();    //処理
	EndDraw();    //描画

	return;
}

/// <summary>
/// エンド画面の処理
/// </summary>
/// <param name=""></param>
VOID EndProc(VOID)
{
	//メニュー画面とヒント画面が開いていない時限定
	if (MenuFlag == FALSE && TipsFlag == FALSE)
	{

		//タイトルシーンに切り替え
		if (KeyClick(KEY_INPUT_RETURN) == TRUE) {
			//シーン切り替え
			//次のシーンの初期化をココで行うと楽

			//ゲームデータの初期化
			GameInit();

			//タイトル画面に切り替え
			ChangeScene(GAME_SCENE_TITLE);

		}
	}

	return;
}

/// <summary>
/// エンド画面の描画
/// </summary>
/// <param name=""></param>
VOID EndDraw(VOID)
{
	switch (GameEndFlag)
	{
	case GAME_CLEAR:
		//=======================================================
		//     ゲームクリアの描画     
		//=======================================================

		
		break;


	case GAME_OVER:
		//=======================================================
		//     ゲームオーバーの描画     
		//=======================================================

		
		break;


	default:
		break;
	}

	DrawString(0, 0, "エンド画面", GetColor(0, 0, 0));
	return;
}


//=====================================================================================================================
//          ココからメニュー画面          
//=====================================================================================================================

/// <summary>
/// メニュー欄の表示
/// </summary>
/// <param name="Volum">かかっているBGMの音量</param>
VOID DrawMenu(int Volum)
{
	
	//Mキーが押されたときフラグを反転させる
	if (KeyClick(KEY_INPUT_M) == TRUE)
	{
		if (MenuFlag == TRUE) MenuFlag = FALSE;
		else if (MenuFlag == FALSE) MenuFlag = TRUE;

		
	}

	//メニューキーが立っているときに描画
	if (MenuFlag == TRUE)
	{
		
	}
	return;
}


//=====================================================================================================================
//          ココからヒント画面          
//=====================================================================================================================

VOID DrawTips(VOID)
{
	

	//Hキーが押されたときフラグを反転させる
	if (KeyClick(KEY_INPUT_H) == TRUE)
	{
		if (TipsFlag == TRUE) TipsFlag = FALSE;
		else if (TipsFlag == FALSE) TipsFlag = TRUE;
	}

	if (TipsFlag == TRUE)
	{
		
		switch (GameScene)
		{
		case GAME_SCENE_TITLE:
			//=============================================
			//     タイトルの文章     
			//=============================================

			
			break;
		case GAME_SCENE_PLAY:
			//=============================================
			//     プレイの文章     
			//=============================================

			
			break;
		case GAME_SCENE_END:
			//=============================================
			//     エンドの文章     
			//=============================================


			break;
		default:
			break;
		}
	}
	return;
}


//=====================================================================================================================
//          ココから切り替え          
//=====================================================================================================================

/// <summary>
/// 切り替え画面
/// </summary>
/// <param name=""></param>
VOID Change(VOID)
{
	ChangeProc();    //処理
	ChangeDraw();    //描画

	return;
}

/// <summary>
/// 切り替え画面の処理
/// </summary>
/// <param name=""></param>
VOID ChangeProc(VOID)
{
	//フェードイン
	if (IsFadeIn == TRUE)
	{
		if (fadeInCnt > fadeInCutMax)
		{
			fadeInCnt--;               //カウントを減らす
		}
		else
		{
			//フェードイン処理が終わった
			fadeInCnt = fadeInCntInit; //カウンタ初期化
			IsFadeIn = FALSE;          //フェードイン処理終了
		}
	}

	//フェードアウト
	if (IsFadeOut == TRUE)
	{
		if (fadeOutCnt < fadeOutCutMax)
		{
			fadeOutCnt++;               //カウントを減らす
		}
		else
		{
			//フェードアウト処理が終わった
			fadeOutCnt = fadeOutCntInit; //カウンタ初期化
			IsFadeOut = FALSE;          //フェードアウト処理終了
		}
	}

	//切り替え処理終了か？
	if (IsFadeIn == FALSE && IsFadeOut == FALSE)
	{
		//フェードインもフェードアウトもしていない時
		GameScene = NextGameScene;  //次のシーンに切り替え
		OldGameScene = GameScene;   //以前のゲームシーン更新
	}

	return;
}

/// <summary>
/// 切り替え画面の描画
/// </summary>
/// <param name=""></param>
VOID ChangeDraw(VOID)
{
	//以前のシーンを描画
	switch (OldGameScene)
	{
	case GAME_SCENE_TITLE:
		TitleDraw();      //タイトル画面の描画
		break;
	case GAME_SCENE_PLAY:
		PlayDraw();       //プレイ画面の描画
		break;
	case GAME_SCENE_END:
		EndDraw();        //エンド画面の描画
		break;
	default:
		break;
	}

	//フェードイン
	if (IsFadeIn == TRUE)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)fadeInCnt / fadeInCutMax) * 255);
	}

	//フェードアウト
	if (IsFadeOut == TRUE)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)fadeOutCnt / fadeOutCutMax) * 255);
	}

	//四角形を描画
	DrawBox(0, 0, GAME_WIDTH, GAME_HEIGHT, GetColor(0, 0, 0), TRUE);

	//半透明終了
	SetDrawBlendMode(DX_BLENDGRAPHTYPE_NORMAL, 0);

	DrawString(0, 0, "切り替え画面", GetColor(0, 0, 0));

	return;
}


//=====================================================================================================================
//          ココから当たり判定関係          
//=====================================================================================================================

/// <summary>
/// 当たり判定の領域更新(プレイヤー)
/// </summary>
/// <param name="coll">当たり判定の領域</param>
VOID CollUpdatePlayer(CHARACTOR* chara)
{
	chara->coll.left = chara->img.X + 40;
	chara->coll.top = chara->img.Y;
	chara->coll.right = chara->img.X + chara->img.width - 40;
	chara->coll.bottom = chara->img.Y + chara->img.height;

	return;
}

/// <summary>
/// 当たり判定の領域更新(敵)
/// </summary>
/// <param name="coll">当たり判定の領域</param>
VOID CollUpdateEnemy(CHARACTOR* chara)
{
	chara->coll.left = chara->img.X;
	chara->coll.top = chara->img.Y;
	chara->coll.right = chara->img.X + chara->img.width;
	chara->coll.bottom = chara->img.Y + chara->img.height;

	return;
}

/// <summary>
/// 当たり判定の領域更新
/// </summary>
/// <param name="coll">当たり判定の領域</param>
VOID CollUpdate(CHARACTOR* chara)
{
	chara->coll.left = chara->img.X;
	chara->coll.top = chara->img.Y;
	chara->coll.right = chara->img.X + chara->img.width;
	chara->coll.bottom = chara->img.Y + chara->img.height;

	return;
}

/// <summary>
/// 当たり判定の領域更新(弾)
/// </summary>
/// <param name="coll">当たり判定の領域</param>
VOID CollUpdateTama(TAMA* tama)
{
	tama->coll.left = tama->x;
	tama->coll.top = tama->y;
	tama->coll.right = tama->x + tama->width;
	tama->coll.bottom = tama->y + tama->height;

	return;
}

/// <summary>
/// 当たっているかを調べる
/// </summary>
/// <param name=""></param>
/// <returns>当たってる・・・TRUE｜当たっていない・・・FALSE</returns>
BOOL OnCollision(RECT coll1, RECT coll2)
{
	//当たっていたらTRUE
	if (coll1.left < coll2.right &&
		coll1.right > coll2.left &&
		coll1.top < coll2.bottom &&
		coll1.bottom > coll2.top)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


//=====================================================================================================================
//          ココからBGM関係          
//=====================================================================================================================

/// <summary>
/// BGMの音量を変更する
/// </summary>
/// <param name="music">変更したいBGMのAUDIO構造体のアドレス</param>
VOID ChangeBGM(AUDIO* music)
{
	//音量が０～２５５の時に作動
	if (0 <= music->Volume && music->Volume <= 255)
	{
		if (KeyDown(KEY_INPUT_O) == TRUE)
		{
			ChangeVolumeSoundMem(music->Volume + 5, music->handle);
			music->Volume += 5;
		}

		if (KeyDown(KEY_INPUT_P) == TRUE)
		{
			ChangeVolumeSoundMem(music->Volume - 5, music->handle);
			music->Volume -= 5;
		}
	}

	//音量が範囲外だったら範囲内に戻す
	if (0 >= music->Volume)
	{
		ChangeVolumeSoundMem(0, music->handle);
		music->Volume = 0;
	}

	if (music->Volume >= 255)
	{
		ChangeVolumeSoundMem(255, music->handle);
		music->Volume = 255;
	}

	

	return;
}