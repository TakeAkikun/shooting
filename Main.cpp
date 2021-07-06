
//ヘッダファイル読み込み
#include "geme.h"       //ゲーム全体のヘッダファイル
#include "keyboard.h"   //キーボードの処理
#include "FPS.h"        //FPSの処理

//マクロ定義
#define TAMA_DIV_MAX   4//弾のMax値
#define TAMA_MAX      10//弾の総数

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

	int Volume = 255;   //ボリューム（0〜255）
};

//音楽の構造体
struct AUDIO
{
	int handle = -1;    //音楽のハンドル
	char path[255];     //音楽のパス

	int Volume = -1;    //ボリューム（MIN 0〜255 MAX）
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
	
	int x;                     //X位置
	int y;                     //Y位置
	int width;                 //幅
	int height;                //高さ

	int Xspead;                //弾のスピード(ｘ)
	int Yspead;                //弾のスピード(ｙ)

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

//弾の発射カウンタ
int tamaShotCnt = 0;
int tamaShotCntMax = 10;

//爆発の画像のハンドル
TAMA Explosion;    //爆発のヤツ
int ExplosionChangeCntMax = 30; //画像を変えるタイミングMax

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
VOID CollUpdate(CHARACTOR* chara);                               //当たり判定の領域を更新
VOID CollUpdateTama(TAMA* tama);                                 //当たり判定の領域を更新(弾)

BOOL OnCollision(RECT coll1, RECT coll2);                        //当たっているかを調べる

VOID ChangeBGM(AUDIO* music);                                    //BGMの音量変更

void DrawTama(TAMA* tama);                                       //弾(マップチップ)の描画

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
	
	


}


/// <summary>
/// ゲーム全体の初期化
/// </summary>
/// <returns>読み込み成功・・・TRUE｜読み込み失敗・・・FALSE</returns>
BOOL GameLoad()
{
	
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
	TanaMoto.x = GAME_WIDTH / 2 - TanaMoto.width / 2; //画面中央
	TanaMoto.y = GAME_HEIGHT - TanaMoto.height;       //画面下

	//速度
	TanaMoto.Xspead = 1;
	TanaMoto.Yspead = 1;

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

	//画像が読み込めなかったときは、エラー(−1)が入る
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

	//音楽が読み込めなかったときは、エラー(−1)が入る
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
			//弾を発射(描画)する
			for (int i = 0; i < TAMA_MAX; i++)
			{
				if (Tama1[i].IsDraw == FALSE
					&& tamaShotCntMax - 1 == tamaShotCnt)
				{
					//弾を発射する
					Tama1[i].IsDraw = TRUE;

					//弾の位置決め
					Tama1[i].x = GAME_WIDTH / 2 - Tama1[i].width / 2;
					Tama1[i].y = GAME_HEIGHT / 2 - Tama1[i].height / 2;

					//当たり判定の更新
					CollUpdateTama(&Tama1[i]);

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

		//弾を飛ばす
		for (int i = 0; i < TAMA_MAX; i++)
		{
			if (Tama1[i].IsDraw == TRUE)
			{
				//弾１のｘ
				Tama1[i].y -= Tama1[i].Yspead;

				//画面外に出たら描画をやめる
				if (Tama1[i].y + Tama1[i].height < 0 ||
					Tama1[i].y > GAME_HEIGHT ||
					Tama1[i].x + Tama1[i].width < 0 ||
					Tama1[i].x > GAME_WIDTH)
				{
					Tama1[i].IsDraw = FALSE;
				}
			}
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
	//弾の描画
	for (int i = 0; i < TAMA_MAX; i++)
	{
		//描画されているとき
		if (Tama1[i].IsDraw == TRUE)
		{
			DrawTama(&Tama1[i]);
		}
	}


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
	chara->coll.left = chara->img.X;
	chara->coll.top = chara->img.Y;
	chara->coll.right = chara->img.X + chara->img.width;
	chara->coll.bottom = chara->img.Y + chara->img.height;

	return;
}

/// <summary>
/// 当たり判定の領域更新(プレイヤー)
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
	//音量が０〜２５５の時に作動
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