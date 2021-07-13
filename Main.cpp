
//�w�b�_�t�@�C���ǂݍ���
#include "geme.h"       //�Q�[���S�̂̃w�b�_�t�@�C��
#include "keyboard.h"   //�L�[�{�[�h�̏���
#include "FPS.h"        //FPS�̏���

#include <math.h>       //���w�p

//�}�N����`
#define TAMA_DIV_MAX   4//�e��Max�l
#define TAMA_MAX      30//�e�̑���
#define TEKI_KIND      8//�G�̎��
#define TEKI_MAX      10//�G�̑���

//=========================================================
//     �\����    
//=========================================================

//�摜�̍\����
struct IMAGE
{
	int handle = -1;  //�摜�̃n���h���i�Ǘ��ԍ��j
	char path[255];   //�摜�̏ꏊ�i�p�X�j

	int X;            //X�ʒu
	int Y;            //Y�ʒu
	int width;        //��
	int height;       //����

	BOOL IsDraw = FALSE;//�摜���`��ł���H
};

//�L�����N�^�[�̍\����
struct CHARACTOR
{
	IMAGE img;        //�摜�̍\����
	int Xspead = 1;   //�ړ����x
	int Yspead = 1;   //�ړ����x
	RECT coll;        //�����蔻��̗̈�(�l�p)
};

//����̍\����
struct MOVIE
{
	int handle = -1;    //����̃n���h��
	char path[25];      //����̃p�X

	int x;              //X�ʒu
	int y;              //Y�ʒu
	int width;          //��
	int height;         //����

	int Volume = 255;   //�{�����[���i0�`255�j
};

//���y�̍\����
struct AUDIO
{
	int handle = -1;    //���y�̃n���h��
	char path[255];     //���y�̃p�X

	int Volume = -1;    //�{�����[���iMIN 0�`255 MAX�j
	int playType = -1;  //BGM or SE
};

//�e�̍\����
struct TAMA
{
	int handle[TAMA_DIV_MAX];  //�摜�̃n���h��
	char path[255];            //�摜�̃p�X

	int DivTate;               //�������i�c�j
	int DivYoko;               //�������i���j
	int DivMax;                //�������i�����j

	int AnimeCnt = 0;          //�A�j���[�V�����J�E���^
	int AnimeCntMax = 0;       //�A�j���[�V�����J�E���^Max

	int NowIndex = 0;          //���݂̉摜�̗v�f��
	
	int startx;                //X�����ʒu
	int starty;                //Y�����ʒu

	float radius;              //���a
	float degree;              //�p�x

	int x;                     //X�ʒu
	int y;                     //Y�ʒu
	int width;                 //��
	int height;                //����

	int spead;                //�e�̃X�s�[�h

	RECT coll;                 //�����蔻��(��`)

	BOOL IsDraw = FALSE;       //�`��ł���H
};

//=========================================================
//     �ϐ��쐬     
//=========================================================

//��ʂ̐؂�ւ�
BOOL IsFadeOut = FALSE;   //�t�F�[�h�A�E�g
BOOL IsFadeIn = FALSE;    //�t�F�[�h�C��

int fadeTimeMill = 2000;                    //�؂�ւ��~���b
int fadeTimeMax = fadeTimeMill / 1000 * 60; //�~���b���t���[���b�ɕϊ�

//�t�F�[�h�A�E�g
int fadeOutCntInit = 0;                     //�����l
int fadeOutCnt = fadeOutCntInit;            //�t�F�[�h�A�E�g�̃J�E���^
int fadeOutCutMax = fadeTimeMax;            //�t�F�[�h�A�E�g�̃J�E���^XAX

//�t�F�[�h�C��
int fadeInCntInit = fadeTimeMax;            //�����l
int fadeInCnt = fadeInCntInit;              //�t�F�[�h�C���̃J�E���^
int fadeInCutMax = fadeTimeMax;             //�t�F�[�h�C���̃J�E���^XAX

//�V�[�����Ǘ�����ϐ�
GAME_SCENE GameScene;       //���݂̃Q�[���̃V�[��
GAME_SCENE OldGameScene;    //�O��̃Q�[���̃V�[��
GAME_SCENE NextGameScene;   //����̃Q�[���̃V�[��

//���j���[���̕\���t���O
BOOL MenuFlag = FALSE;    //���j���[����\�����邩�H

//�q���g���̕\���t���O
BOOL TipsFlag = FALSE;    //�q���g����\�����邩�H

//�G���h��ʂ̎��̃t���O
//�ŏ��̓N���A�����Ă����܂�
int GameEndFlag = GAME_CLEAR;

//�e�̉摜
TAMA TanaMoto;         //��
TAMA Tama1[TAMA_MAX];  //���ۂɎg��
TAMA Tama2[TAMA_MAX];  //

//�e�̔��˃J�E���^
int tamaShotCnt = 0;
int tamaShotCnt2 = 0;
int tamaShotCntMax = 100;
int tamaShotCntMax2 = 10;
BOOL tamaAutoFlag = FALSE;

//�����̉摜�̃n���h��
TAMA Explosion;    //�����̃��c
int ExplosionChangeCntMax = 30; //�摜��ς���^�C�~���OMax

//�w�i�摜
IMAGE back[2]; //�w�i��2�̉摜

//�v���C���[
CHARACTOR Player;

//�G
CHARACTOR Teki_Moto[TEKI_KIND];
CHARACTOR teki[TEKI_MAX];
CHARACTOR Enemy;
int EnemyHP = 30;     //�G�̗̑�

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

//�G���o�Ă���J�E���g
int TekiAddCnt = 0;
int TekiAddCntMax = 60;  //60FPS�ň��E�E�E1�b�ň��

//�Q�[���̃X�R�A
int Score = 0;

//=========================================================
//     �֐�     
//=========================================================

//�L�������P�[�X�E�E�E�P��̐擪��啶���ɂ��閽���K��
//�X�l�[�N�P�[�X�E�E�E�P����A���_�[�o�[�łȂ������K��

VOID Title(VOID);     //�^�C�g�����
VOID TitleProc(VOID); //�^�C�g����ʁi�����j
VOID TitleDraw(VOID); //�^�C�g����ʁi�`��j

VOID Play(VOID);					                             //�v���C���
VOID PlayProc(VOID);				                             //�v���C��ʁi�����j
VOID PlayDraw(VOID);				                             //�v���C��ʁi�`��j

VOID End(VOID);						                             //�G���h���
VOID EndProc(VOID);					                             //�G���h��ʁi�����j
VOID EndDraw(VOID);					                             //�G���h��ʁi�`��j

VOID Change(VOID);					                             //�؂�ւ����
VOID ChangeProc(VOID);				                             //�؂�ւ���ʁi�����j
VOID ChangeDraw(VOID);				                             //�؂�ւ���ʁi�`��j

VOID DrawMenu(int Volum);			                             //���j���[��ʁi�`��j
VOID DrawTips(VOID);				                             //�q���g��ʁi�`��j

VOID ChangeScene(GAME_SCENE seane);                              //�V�[���؂�ւ�

VOID CollUpdatePlayer(CHARACTOR* chara);                         //�����蔻��̗̈���X�V(�v���C���[)
VOID CollUpdateEnemy(CHARACTOR* chara);                          //�����蔻��̗̈���X�V(�G)
VOID CollUpdate(CHARACTOR* chara);                               //�����蔻��̗̈���X�V
VOID CollUpdateTama(TAMA* tama);                                 //�����蔻��̗̈���X�V(�e)

BOOL OnCollision(RECT coll1, RECT coll2);                        //�������Ă��邩�𒲂ׂ�

VOID ChangeBGM(AUDIO* music);                                    //BGM�̉��ʕύX

void DrawTama(TAMA* tama);                                       //�e(�}�b�v�`�b�v)�̕`��
VOID ShotTama(TAMA* tama, float deg);                            //�e�̔���

BOOL GameLoad(VOID);                                             //�Q�[���S�̂̃f�[�^��ǂݍ���
VOID GameInit(VOID);                                             //�Q�[���f�[�^�̏�����
BOOL ImageInput(IMAGE* Image, const char* path);                 //�Q�[���̉摜�ǂݍ���
BOOL MusicInput(AUDIO* music, const char* path, int volume, int playType);//�Q�[����BGM�ǂݍ���
BOOL LoadImageMem(const char* Path, int* Handle, int xDiv, int yDiv);//�Q�[���̃}�b�v�`�b�v�̓ǂݍ���

//=====================================================================================================================
//          �R�R���烁�C���v���O����          
//=====================================================================================================================

// �v���O������ WinMain ����n�܂�܂�
// Windous�̃v���O���~���O���@�œ����Ă���B�iWinAPI�j
// DxLib��DirectX�Ƃ����Q�[���v���O���~���O���ȒP�Ɏg����d�g��
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	//������͕K��Init���s���O�ɓ��͂���
	SetOutApplicationLogValidFlag(FALSE);                //Log.txt���o�͂��Ȃ�
	ChangeWindowMode(TRUE);                              //�E�B���h�E���[�h�ɐݒ�
	SetMainWindowText(GAME_TITLE);                       //�Q�[���^�C�g����ݒ�
	SetGraphMode(GAME_WIDTH, GAME_HEIGHT, GAME_COLOR);   //�Q�[���̉𑜓x�̐ݒ�
	SetWindowSize(GAME_WIDTH, GAME_HEIGHT);              //�E�B���h�E�̑傫����ݒ�
	SetWindowStyleMode(GAME_WINDOW_BAR);                 //�E�B���h�E�o�[�̏��
	SetBackgroundColor(255, 255, 255);                   //�E�B���h�E�̃f�t�H�̐F��ݒ�
	SetWindowIconID(GAME_ICON_ID);                       //�A�C�R���t�@�C����ǂݍ���
	SetWaitVSyncFlag(TRUE);                              //�f�B�X�v���C�̐���������L���ɂ���
	SetAlwaysRunFlag(TRUE);                              //�E�B���h�E�������ƃA�N�e�B�u�ɂ���



	if (DxLib_Init() == -1)		// �c�w���C�u��������������
	{
		return -1;			    // �G���[���N�����璼���ɏI��
	}

	//�_�u���o�b�t�@�����O�L����
	SetDrawScreen(DX_SCREEN_BACK);

	//�Q�[���S�̂̏�����
	if (!GameLoad())
	{
		//�Q�[���f�[�^�̓ǂݍ��݂Ɏ��s�����Ƃ�
		DxLib_End();  //DxLib�I��
		return -1;    //�ُ�I��
	}

	//�Q�[���f�[�^�̏�����
	GameInit();

	//�ŏ��̃V�[���̓^�C�g����ʂ���
	GameScene = GAME_SCENE_TITLE;

	//�������[�v
	while (1)
	{

		if (ProcessMessage() != 0) { break; }  //���b�Z�[�W���󂯎�葱����
		if (ClearDrawScreen() != 0) { break; } //��ʂ��N���A

		AllKeyUpdate();         //�L�[�{�[�h���͂̍X�V

		//FPS�l�̍X�V
		FPSUpdate();

		//ESC�L�[�ŋ����I��
		if (KeyClick(KEY_INPUT_ESCAPE) == TRUE) { break; }

		//�ȑO�̃V�[�����擾
		if (GameScene != GAME_SCENE_CHANGE)
		{
			OldGameScene = GameScene;
		}

		//�V�[�����Ƃɏ������s��
		switch (GameScene)
		{
		case GAME_SCENE_TITLE:
			Title();            //�^�C�g�����
			break;
		case GAME_SCENE_PLAY:
			Play();             //�v���C���
			break;
		case GAME_SCENE_END:
			End();              //�G���h���
			break;
		case GAME_SCENE_CHANGE:
			Change();           //�؂�ւ����
			break;
		default:
			break;
		}

		//�V�[����؂�ւ���
		if (OldGameScene != GameScene)
		{
			//�ȑO�̃V�[�����؂�ւ���ʂłȂ��Ƃ�
			if (GameScene != GAME_SCENE_CHANGE)
			{
				NextGameScene = GameScene;     //���̃V�[����ۑ�
				GameScene = GAME_SCENE_CHANGE; //��ʐ؂�ւ��V�[���ɕς���
			}
		}

		//FPS�l��`��
		FPSDraw();

		//FPS�l��҂�
		FPSWait();

		ScreenFlip();           //�_�u���o�b�t�@�����O������ʂ�`��
	}

	//�ǂݍ��񂾃}�b�v�`�b�v�����
	for (int i = 0; i < TAMA_DIV_MAX; i++) { DeleteGraph(TanaMoto.handle[i]); }

	//�I���Ƃ��̏���
	DeleteGraph(Player.img.handle);      //�摜���������ォ��폜
	DeleteGraph(Enemy.img.handle);       //
	DeleteGraph(back[0].handle);         //
	DeleteGraph(back[1].handle);         //
	for (int i = 0; i < TEKI_KIND; i++) {
		DeleteGraph(Teki_Moto[i].img.handle);
	}

	DxLib_End();				// �c�w���C�u�����g�p�̏I������

	return 0;				    // �\�t�g�̏I�� 
}


//=====================================================================================================================
//          �R�R���珉�����֌W          
//=====================================================================================================================


/// <summary>
/// �Q�[���f�[�^�̏�����
/// </summary>
/// <param name=""></param>
VOID GameInit(VOID)
{
	//�Q�[���X�R�A�̏�����
	Score = 0;

	//�v���C���[��������
	Player.img.X = 500;
	Player.img.Y = 500;
	Player.Xspead = 300;
	Player.Yspead = 300;
	Player.img.IsDraw = TRUE;
	
	//�����蔻����X�V
	CollUpdatePlayer(&Player);  //�v���C���[�̃A�h���X

	//�G��������
	Enemy.img.X = 50;
	Enemy.img.Y = 50;
	Enemy.Xspead = 7;
	Enemy.Yspead = 7;
	Enemy.img.IsDraw = TRUE;

	//�����蔻��̍X�V
	CollUpdate(&Enemy);  //�G�̃A�h���X

	//�G�@�̂̏�����
	for (int i = 0; i < TEKI_KIND; i++)
	{
		Teki_Moto[i].img.X = GAME_WIDTH / 2 - Teki_Moto[i].img.width;
		Teki_Moto[i].img.Y = -Teki_Moto[i].img.height;
		CollUpdatePlayer(&Teki_Moto[i]);  //�����蔻��̍X�V
		Teki_Moto[i].img.IsDraw = FALSE;  //�`��͂��Ȃ�
	}

	//�w�i�摜�̈ʒu
	back[0].X = 0;
	back[0].Y = -back[0].height;
	back[0].IsDraw = TRUE;
	back[1].X = 0;
	back[1].Y = 0;
	back[1].IsDraw = TRUE;

	//�e�̈ʒu
	
	//�ʒu�𒲐�
	TanaMoto.x = -10; //��ʊO
	TanaMoto.y = -10; //��ʊO

	//�����蔻��̍X�V
	CollUpdateTama(&TanaMoto);//�e�̃A�h���X

	//�S�Ă̒e�ɏ����R�s�[
	for (int i = 0; i < TAMA_MAX; i++)
	{
		Tama1[i] = TanaMoto;
		Tama2[i] = TanaMoto;
	}
}


/// <summary>
/// �Q�[���S�̂̏�����
/// </summary>
/// <returns>�ǂݍ��ݐ����E�E�ETRUE�b�ǂݍ��ݎ��s�E�E�EFALSE</returns>
BOOL GameLoad()
{
	//�Q�[���X�R�A�̏�����
	Score = 0;

	//�e�̕�������ݒ�
	TanaMoto.DivYoko = 4;
	TanaMoto.DivTate = 1;

	//�e�̃p�X���R�s�[
	strcpyDx(TanaMoto.path, ".\\image\\marudai_green.png");

	//�}�b�v�`�b�v��ǂݍ���
	if (!LoadImageMem(TanaMoto.path, &TanaMoto.handle[0], TanaMoto.DivYoko, TanaMoto.DivTate)) { FALSE; }  //�e

	//�摜�̕��ƍ������擾
	GetGraphSize(TanaMoto.handle[0], &TanaMoto.width, &TanaMoto.height);

	//�ʒu�𒲐�
	TanaMoto.x = -10; //��ʊO
	TanaMoto.y = -10; //��ʊO

	//���x
	TanaMoto.spead = 1;
	
	//�A�j���[�V�����̑��x
	TanaMoto.AnimeCntMax = 10;

	//�����蔻��̍X�V
	CollUpdateTama(&TanaMoto);

	//�摜�͕\�����Ȃ�
	TanaMoto.IsDraw = FALSE;

	//�S�Ă̒e�ɏ����R�s�[
	for (int i = 0; i < TAMA_MAX; i++)
	{
		Tama1[i] = TanaMoto;
		Tama2[i] = TanaMoto;
	}

	//�v���C���[��������
	Player.img.X = 500;
	Player.img.Y = 500;
	Player.Xspead = 300;
	Player.Yspead = 300;
	Player.img.IsDraw = TRUE;

	//�����蔻����X�V
	CollUpdatePlayer(&Player);  //�v���C���[�̓����蔻��̃A�h���X

	//�G��������
	Enemy.img.X = 50;
	Enemy.img.Y = 50;
	Enemy.Xspead = 7;
	Enemy.Yspead = 7;
	Enemy.img.IsDraw = TRUE;

	//�����蔻��̍X�V
	CollUpdate(&Enemy);  //�G�̓����蔻��̃A�h���X

	//�w�i�摜�̈ʒu
	back[0].X = 0;
	back[0].Y = -back[0].height;
	back[0].IsDraw = TRUE;
	back[1].X = 0;
	back[1].Y = 0;
	back[1].IsDraw = TRUE;

	//�摜��ǂݍ���
	if (!ImageInput(&Player.img, ".\\image\\Player.png")) { FALSE; }
	if (!ImageInput(&Enemy.img, ".\\image\\Enemy.jpeg")) { FALSE; }
	if (!ImageInput(&back[0], ".\\image\\hoshi.jpg")) { FALSE; }
	if (!ImageInput(&back[1], ".\\image\\hoshi_rev.jpg")) { FALSE; }

	//�G�̉摜
	for (int i = 0; i < TEKI_KIND; i++)
	{
		if (!ImageInput(&Teki_Moto[i].img, tekiPath[i])) { FALSE; }
		Teki_Moto[i].img.X = GAME_WIDTH / 2 - Teki_Moto[i].img.width;
		Teki_Moto[i].img.Y = -Teki_Moto[i].img.height;
		CollUpdatePlayer(&Teki_Moto[i]);  //�����蔻��̍X�V
		Teki_Moto[i].img.IsDraw = FALSE;  //�`��͂��Ȃ�
	}

	return TRUE;                    //�S���ǂݍ��߂���TRUE
}


//=====================================================================================================================
//          �R�R����ǂݍ��݊֌W          
//=====================================================================================================================


/// <summary>
/// �摜���������ɓǂݍ���
/// </summary>
/// <param name="Image">Image�\���̂̃A�h���X</param>
/// <param name="path">Image�̉摜�p�X</param>
/// <returns>TRUE or FALSE</returns>
BOOL ImageInput(IMAGE* Image, const char* path)
{
	//�摜��ǂݍ���
	strcpyDx(Image->path, path);  //�p�X�̃R�s�[
	Image->handle = LoadGraph(Image->path);          //�摜�̓ǂݍ���

	//�摜���ǂݍ��߂Ȃ������Ƃ��́A�G���[(�|1)������
	if (Image->handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),   //���C���̃E�B���h�E�^�C�g��
			Image->path,             //���b�Z�[�W�{��
			"�摜�ǂݍ��݃G���[",    //���b�Z�[�W�^�C�g��
			MB_OK                    //�{�^��
		);

		return FALSE;                //�G���[�I��
	}

	//��ʂ̕��ƍ������擾
	GetGraphSize(Image->handle, &Image->width, &Image->height);
}


/// <summary>
/// ���y���������ɓǂݍ���
/// </summary>
/// <param name="music">Audio�\���̂̃A�h���X</param>
/// <param name="path">Audio�̉��y�p�X</param>
/// <param name="volume">�{�����[��</param>
/// <param name="playType">DX_PLAYTYPE_LOOP or DX_PLAYTYPE_BACK</param>
/// <returns>TRUE or FALSE</returns>
BOOL MusicInput(AUDIO* music, const char* path, int volume, int playType)
{
	//���y��ǂݍ���
	strcpyDx(music->path, path);  //�p�X�̃R�s�[
	music->handle = LoadSoundMem(music->path);          //���y�̓ǂݍ���

	//���y���ǂݍ��߂Ȃ������Ƃ��́A�G���[(�|1)������
	if (music->handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),       //���C���̃E�B���h�E�^�C�g��
			music->path,                 //���b�Z�[�W�{��
			"���y�ǂݍ��݃G���[",        //���b�Z�[�W�^�C�g��
			MB_OK                        //�{�^��
		);

		return FALSE;                    //�G���[�I��
	}

	music->playType = playType;                          //���y�����[�v������
	music->Volume = volume;                              //MAX��255
	ChangeVolumeSoundMem(music->Volume, music->handle);  //BGM�̃{�����[����ύX

	return TRUE;
}


/// <summary>
/// �}�b�v�`�b�v���������ɓǂݍ���
/// </summary>
/// <param name="Path">�摜�̃p�X</param>
/// <param name="MapTip">�n���h���z��̐擪�A�h���X</param>
/// <param name="xDiv">�摜�̉��̕�����</param>
/// <param name="yDiv">�摜�̏c�̕�����</param>
/// <returns></returns>
BOOL LoadImageMem(const char* Path, int* Handle, int xDiv, int yDiv)
{
	//�e�̓ǂݍ���
	int IsTamaLoad = 1;              //�摜���ǂݍ��߂����H

	//�ꎞ�I�Ɏg���摜�̃n���h��
	int MaptipHandle = LoadGraph(Path);

	//�ǂݍ��݃G���[
	if (MaptipHandle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),   //�E�B���h�E�n���h��
			Path,              //�{��
			"�摜�ǂݍ��݃G���[",    //�^�C�g��
			MB_OK                    //�{�^��
		);

		return FALSE;                //�ǂݍ��ݎ��s
	}

	int tamawidth = 0;
	int tamaheight = 0;
	GetGraphSize(MaptipHandle, &tamawidth, &tamaheight);

	//�������ēǂݍ���
	if (LoadDivGraph(
		Path,                        //�摜�̃p�X
		TAMA_DIV_MAX,                //��������
		4, 1,                        //���Əc�̕�����
		tamawidth / xDiv, tamaheight / yDiv,//������̉摜����̑傫��(X,Y)
		Handle
	) == FALSE)

	//�����G���[
	if (MaptipHandle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),   //�E�B���h�E�n���h��
			Path,                    //�{��
			"�摜�����G���[",        //�^�C�g��
			MB_OK                    //�{�^��
		);

		return FALSE;                //�ǂݍ��ݎ��s
	}

	//�ꎞ�I�ɓǂݍ��񂾉摜�����
	DeleteGraph(MaptipHandle);

	return TRUE;
}


//=========================================================
//     ���L�͐؂�ւ��֐�     
//=========================================================


/// <summary>
/// �V�[����؂�ւ���֐�
/// </summary>
/// <param name="scene"></param>
VOID ChangeScene(GAME_SCENE scene)
{
	GameScene = scene;  //�V�[���؂�ւ�
	IsFadeIn = FALSE;             //�t�F�[�h�C�����Ȃ�
	IsFadeOut = TRUE;             //�t�F�[�h�A�E�g����

	return;
}


//=====================================================================================================================
//           �R�R����^�C�g��          
//=====================================================================================================================

/// <summary>
/// �^�C�g�����
/// </summary>
/// <param name=""></param>
VOID Title(VOID)
{
	TitleProc();    //����
	TitleDraw();    //�`��

	return;
}

/// <summary>
/// �^�C�g����ʂ̏���
/// </summary>
/// <param name=""></param>
VOID TitleProc(VOID)
{
	//���j���[��ʂƃq���g��ʂ��J���Ă��Ȃ�������
	if (MenuFlag == FALSE && TipsFlag == FALSE)
	{

		//�v���C�V�[���ɐ؂�ւ�
		if (KeyClick(KEY_INPUT_RETURN) == TRUE) {
			//�V�[���؂�ւ�
			//���̃V�[���̏��������R�R�ōs���Ɗy

			//�Q�[���f�[�^�̏�����
			GameInit();

			//�v���C��ʂɐ؂�ւ�
			ChangeScene(GAME_SCENE_PLAY);

			return;
		}
	}

	return;
}

/// <summary>
/// �^�C�g����ʂ̕`��
/// </summary>
/// <param name=""></param>
VOID TitleDraw(VOID)
{
	
	DrawString(0, 0, "�^�C�g�����", GetColor(0, 0, 0));
	return;
}


//=====================================================================================================================
//          �R�R����v���C          
//=====================================================================================================================

/// <summary>
/// �v���C���
/// </summary>
/// <param name=""></param>
VOID Play(VOID)
{
	PlayProc();    //����
	PlayDraw();    //�`��

	return;
}

/// <summary>
/// �v���C��ʂ̏���
/// </summary>
/// <param name=""></param>
VOID PlayProc(VOID)
{
	//���j���[��ʂƃq���g��ʂ��J���Ă��Ȃ�������
	if (MenuFlag == FALSE && TipsFlag == FALSE)
	{
		//�X�y�[�X�L�[�������Ă���Ƃ�
		if (KeyDown(KEY_INPUT_SPACE) == TRUE)
		{
			if (tamaShotCnt == 0)
			{
				//�e�𔭎�(�`��)����
				for (int i = 0; i < TAMA_MAX; i++)
				{
					if (Tama1[i].IsDraw == FALSE)
					{
						//�p�x�����Ă���
						ShotTama(&Tama1[i], 315.0f);

						//�ꔭ�o������E�o
						break;
					}
				}

				//�e�𔭎�(�`��)����
				for (int i = 0; i < TAMA_MAX; i++)
				{
					if (Tama1[i].IsDraw == FALSE)
					{
						//�p�x�����Ă���
						ShotTama(&Tama1[i], 225.0f);

						//�ꔭ�o������E�o
						break;
					}
				}
			}

			//�e�̔��ˑ҂�
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
				//�e�𔭎�(�`��)����
				for (int i = 0; i < TAMA_MAX; i++)
				{
					if (Tama2[i].IsDraw == FALSE)
					{
						//�p�x�����Ă���
						ShotTama(&Tama2[i], 270.0f);

						//�ꔭ�o������E�o
						break;
					}
				}
			}

			//�e�̔��ˑ҂�
			if (tamaShotCnt2 < tamaShotCntMax2)
			{
				tamaShotCnt2++;
			}
			else
			{
				tamaShotCnt2 = 0;
			}
		}

		//�e�̈ړ�
		for (int i = 0; i < TAMA_MAX; i++)
		{
			if (Tama1[i].IsDraw == TRUE)
			{
				if (tamaAutoFlag == FALSE && Tama1[i].y > Tama1[i].starty - 100)
				{
					Tama1[i].x = Tama1[i].startx + cos(Tama1[i].degree * DX_PI / 180.0f) * Tama1[i].radius;
					Tama1[i].y = Tama1[i].starty + sin(Tama1[i].degree * DX_PI / 180.0f) * Tama1[i].radius;

					//���a�𑫂�
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
					�����̂����i����[�߂�ǂ������j
					//���p�O�p�`�̒��
					//((Enemy.img.X + Enemy.img.width / 2) - (Tama1[i].startx + Tama1[i].width / 2))
					//���p�O�p�`�̍���
					//((Tama1[i].starty + Tama1[i].height / 2) - (Enemy.img.Y + Enemy.img.height / 2))
					//���p�O�p�`�̎Ε�(�O�����̒藝���g�p)
					//sqrt((((Enemy.img.X + Enemy.img.width / 2) - (Tama1[i].startx + Tama1[i].width / 2)) * ((Enemy.img.X + Enemy.img.width / 2) - (Tama1[i].startx + Tama1[i].width / 2))) + 
					//(((Tama1[i].starty + Tama1[i].height / 2) - (Enemy.img.Y + Enemy.img.height / 2)) * ((Tama1[i].starty + Tama1[i].height / 2) - (Enemy.img.Y + Enemy.img.height / 2))))

					Tama1[i].x = Tama1[i].startx +
						//��� / �Εӂ�cos�����߂�
						(((Enemy.img.X + Enemy.img.width / 2) - (Tama1[i].startx + Tama1[i].width / 2)) /
							sqrt((((Enemy.img.X + Enemy.img.width / 2) - (Tama1[i].startx + Tama1[i].width / 2)) * ((Enemy.img.X + Enemy.img.width / 2) - (Tama1[i].startx + Tama1[i].width / 2)) +
								(((Tama1[i].starty + Tama1[i].height / 2) - (Enemy.img.Y + Enemy.img.height / 2)) * ((Tama1[i].starty + Tama1[i].height / 2) - (Enemy.img.Y + Enemy.img.height / 2))))))
						* Tama1[i].radius;
					Tama1[i].y = Tama1[i].starty -
						//���� / �Εӂ�sin�����߂�
						(((Tama1[i].starty + Tama1[i].height / 2) - (Enemy.img.Y + Enemy.img.height / 2)) /
							sqrt((((Enemy.img.X + Enemy.img.width / 2) - (Tama1[i].startx + Tama1[i].width / 2)) * ((Enemy.img.X + Enemy.img.width / 2) - (Tama1[i].startx + Tama1[i].width / 2))) +
								(((Tama1[i].starty + Tama1[i].height / 2) - (Enemy.img.Y + Enemy.img.height / 2)) * ((Tama1[i].starty + Tama1[i].height / 2) - (Enemy.img.Y + Enemy.img.height / 2)))))
						* Tama1[i].radius;
					*/

					//atan2�E�E�E���p�O�p�`�̎ΕӈȊO��2�ӂ���p�x���o���Ă���邷�������c
					Tama1[i].degree = (int)atan2(Enemy.img.Y - Tama1[i].starty, Enemy.img.X - Tama1[i].startx) / DX_PI * 180.0f;

					//�e�̈ʒu���C��
								//���S�ʒu�@�@�@�{�@��΂��p�x�˔�΂��������v�Z�@���@����
					Tama1[i].x = Tama1[i].startx + cos(Tama1[i].degree * DX_PI / 180.0f) * Tama1[i].radius;
					Tama1[i].y = Tama1[i].starty + sin(Tama1[i].degree * DX_PI / 180.0f) * Tama1[i].radius;

					//���a�𑫂�
					Tama1[i].radius += Tama1[i].spead;
				}

				//��ʊO�ɏo����`�����߂�
				if (Tama1[i].y + Tama1[i].height < 0 ||
					Tama1[i].y > GAME_HEIGHT ||
					Tama1[i].x + Tama1[i].width < 0 ||
					Tama1[i].x > GAME_WIDTH)
				{
					Tama1[i].IsDraw = FALSE;
					tamaAutoFlag = FALSE;
				}

				//�����蔻��̍X�V
				CollUpdateTama(&Tama1[i]);
			}

			if (Tama2[i].IsDraw == TRUE)
			{
				//�ړ�
				Tama2[i].x = Tama2[i].startx + cos(Tama2[i].degree * DX_PI / 180.0f) * Tama2[i].radius;
				Tama2[i].y = Tama2[i].starty + sin(Tama2[i].degree * DX_PI / 180.0f) * Tama2[i].radius;

				//���a�𑫂�
				Tama2[i].radius += Tama2[i].spead;

				//��ʊO�ɏo����`�����߂�
				if (Tama2[i].y + Tama2[i].height < 0 ||
					Tama2[i].y > GAME_HEIGHT ||
					Tama2[i].x + Tama2[i].width < 0 ||
					Tama2[i].x > GAME_WIDTH)
				{
					Tama2[i].IsDraw = FALSE;
					tamaAutoFlag = FALSE;
				}

				//�����蔻��̍X�V
				CollUpdateTama(&Tama2[i]);
			}
		}

		if (TekiAddCnt < TekiAddCntMax)
		{
			TekiAddCnt++;
		}
		else
		{
			//�J�E���g���Z�b�g
			TekiAddCnt = 0;

			//�G�̐���
			for (int i = 0; i < TEKI_MAX; i++)
			{
				//�`�悳��Ă��Ȃ��G��T��
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

					teki[i].img.IsDraw = TRUE;  //�`�悷��
					break;
				}
			}
		}

		//�G�̏���
		for (int i = 0; i < TEKI_MAX; i++)
		{
			//�`�悵�Ă���G
			if (teki[i].img.IsDraw == TRUE)
			{
				teki[i].img.Y += 1;
				CollUpdateEnemy(&teki[i]);

				//��ʊO�ɏo����`�����߂�
				if (teki[i].img.Y > GAME_HEIGHT)
				{
					teki[i].img.IsDraw = FALSE;
				}
			}
		}

		//�v���C���[�̑���

			//�ǂ�˂������Ȃ��悤��if���𒲐�
		if (KeyDown(KEY_INPUT_UP) == TRUE && Player.img.Y > 0)
		{
			Player.img.Y -= Player.Yspead * fps.DeltaTime;   //��Ɉړ�

			//�X�s�[�h�������Ă߂荞�ނ̂�h�~
			if (Player.img.Y < 0)
			{
				Player.img.Y = 0;
			}
		}

		if (KeyDown(KEY_INPUT_DOWN) == TRUE && Player.img.Y < GAME_HEIGHT - Player.img.height)
		{
			Player.img.Y += Player.Yspead * fps.DeltaTime;   //���Ɉړ�

			//�X�s�[�h�������Ă߂荞�ނ̂�h�~
			if (Player.img.Y > GAME_HEIGHT - Player.img.height)
			{
				Player.img.Y = GAME_HEIGHT - Player.img.height;
			}
		}

		if (KeyDown(KEY_INPUT_LEFT) == TRUE && Player.img.X > 0)
		{
			Player.img.X -= Player.Xspead * fps.DeltaTime;   //���Ɉړ�

			//�X�s�[�h�������Ă߂荞�ނ̂�h�~
			if (Player.img.X < 0)
			{
				Player.img.X = 0;
			}
		}

		if (KeyDown(KEY_INPUT_RIGHT) == TRUE && Player.img.X < GAME_WIDTH - Player.img.width)
		{
			Player.img.X += Player.Xspead * fps.DeltaTime;   //�E�Ɉړ�

			//�X�s�[�h�������Ă߂荞�ނ̂�h�~
			if (Player.img.X > GAME_WIDTH - Player.img.width)
			{
				Player.img.X = GAME_WIDTH - Player.img.width;
			}
		}

		//�G
		Enemy.img.X += Enemy.Xspead;
		if (Enemy.img.X < 0 || Enemy.img.X + Enemy.img.width > GAME_WIDTH)
		{
			Enemy.Xspead = -Enemy.Xspead;
		}

		//�����蔻����X�V
		CollUpdatePlayer(&Player);
		CollUpdateEnemy(&Enemy);

		//Hit����
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

		//���j����
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

			//�G��HP�����Z�b�g
			EnemyHP = 30;

			//�����������I��
			return;
		}

		//�G���h�V�[���ɐ؂�ւ�
		if (KeyClick(KEY_INPUT_RETURN) == TRUE) {
			//�V�[���؂�ւ�
			//���̃V�[���̏��������R�R�ōs���Ɗy

			//�Q�[���f�[�^�̏�����
			GameInit();

			//�G���h��ʂɐ؂�ւ�
			ChangeScene(GAME_SCENE_END);

		}
	}

	return;
}

/// <summary>
/// �v���C��ʂ̕`��
/// </summary>
/// <param name=""></param>
VOID PlayDraw(VOID)
{
	//�w�i�̕`��
	for (int i = 0; i < 2; i++)
	{
		//�w�i��`��
		DrawGraph(back[i].X, back[i].Y, back[i].handle, TRUE);

		//�摜�����܂ŉ���������
		if (back[i].Y > GAME_HEIGHT)
		{
			back[i].Y = -back[i].height + 1;
		}

		//�摜�𓮂���
		back[i].Y++;
	}

	//�e�̕`��
	for (int i = 0; i < TAMA_MAX; i++)
	{
		//�`�悳��Ă���Ƃ�
		if (Tama1[i].IsDraw == TRUE)
		{
			DrawTama(&Tama1[i]);

			if (GAME_DEBUG == TRUE)
			{
				//�l�p��`��
				DrawBox(Tama1[i].coll.left, Tama1[i].coll.top, Tama1[i].coll.right, Tama1[i].coll.bottom, GetColor(255, 0, 0), FALSE);
			}
		}

		//�`�悳��Ă���Ƃ�
		if (Tama2[i].IsDraw == TRUE)
		{
			DrawTama(&Tama2[i]);

			if (GAME_DEBUG == TRUE)
			{
				DrawBox(Tama2[i].coll.left, Tama2[i].coll.top, Tama2[i].coll.right, Tama2[i].coll.bottom, GetColor(255, 0, 0), FALSE);
			}
		}
	}

	//�G�̕`��
	for (int i = 0; i < TEKI_MAX; i++)
	{
		if (teki[i].img.IsDraw == TRUE)
		{
			//�摜��`��
			DrawGraph(teki[i].img.X, teki[i].img.Y, teki[i].img.handle, TRUE);

			if (GAME_DEBUG == TRUE)
			{
				//�l�p��`��
				DrawBox(teki[i].coll.left, teki[i].coll.top, teki[i].coll.right, teki[i].coll.bottom, GetColor(0, 0, 255), FALSE);
			}
		}
	}

	//�G�P�̕`��
	if (Enemy.img.IsDraw == TRUE)
	{
		//�摜��`��
		DrawGraph(Enemy.img.X, Enemy.img.Y, Enemy.img.handle, TRUE);

		if (GAME_DEBUG == TRUE)
		{
			//�l�p��`��
			DrawBox(Enemy.coll.left, Enemy.coll.top, Enemy.coll.right, Enemy.coll.bottom, GetColor(0, 0, 255), FALSE);
		}
	}

	//�v���C���[�̕`��
	if (Player.img.IsDraw == TRUE)
	{
		//�摜��`��
		DrawGraph(Player.img.X, Player.img.Y, Player.img.handle, TRUE);

		if (GAME_DEBUG == TRUE)
		{
			//�l�p��`��
			DrawBox(Player.coll.left, Player.coll.top, Player.coll.right, Player.coll.bottom, GetColor(255, 0, 0), FALSE);
		}
	}

	//�X�R�A�̕`��
	int old = GetFontSize();
	SetFontSize(40);
	DrawFormatString(0, 100, GetColor(255, 255, 255), "SCORE�F%05d", Score);
	SetFontSize(old);

	DrawString(0, 0, "�v���C���", GetColor(0, 0, 0));
	return;
}

//=====================================================================================================================
//          �R�R����e�̕`��          
//=====================================================================================================================

/// <summary>
/// �e(�}�b�v�`�b�v)�̕`��
/// </summary>
/// <param name="">�e�̍\���̂̃p�X</param>
void DrawTama(TAMA* tama)
{
	if (tama->IsDraw == TRUE)
	{
		//�e�̕`��
		DrawGraph(tama->x, tama->y, tama->handle[tama->NowIndex], TRUE);

		if (tama->AnimeCnt < tama->AnimeCntMax)
		{
			(int)tama->AnimeCnt++;
		}
		else
		{
			//�e�̓Y�����e�̕������̍ő�����������Ƃ�
			if (tama->NowIndex < TAMA_DIV_MAX - 1)
			{
				tama->NowIndex++;         //���̉摜��
			}
			else
			{
				tama->NowIndex = 0;       //�ŏ��ɖ߂�
			}

			tama->AnimeCnt = 0;
		}
	}
}


//=====================================================================================================================
//          �R�R����e���΂�          
//=====================================================================================================================

/// <summary>
/// �e���΂��֐�
/// </summary>
/// <param name="tama">TAMA�\���̂̃A�h���X</param>
/// <param name="deg">�p�x</param>
VOID ShotTama(TAMA* tama, float deg)
{
		//�e�𔭎˂���
		tama->IsDraw = TRUE;

		//�e�̈ʒu����
		tama->startx = Player.img.X + Player.img.width / 2 - tama->width / 2;
		tama->starty = Player.img.Y;

		//���߂̈ʒu��e�̈ʒu�ɐݒ�
		tama->x = tama->startx;
		tama->y = tama->starty;

		//�e�̑��x����
		tama->spead = 6;

		//�e�̊p�x����
		tama->degree = deg;

		//�e�̔��a����
		tama->radius = 0.0f;

		//�e�̓����蔻����X�V
		CollUpdateTama(tama);
}


//=====================================================================================================================
//          �R�R����G���h          
//=====================================================================================================================

/// <summary>
/// �G���h���
/// </summary>
/// <param name=""></param>
VOID End(VOID)
{
	EndProc();    //����
	EndDraw();    //�`��

	return;
}

/// <summary>
/// �G���h��ʂ̏���
/// </summary>
/// <param name=""></param>
VOID EndProc(VOID)
{
	//���j���[��ʂƃq���g��ʂ��J���Ă��Ȃ�������
	if (MenuFlag == FALSE && TipsFlag == FALSE)
	{

		//�^�C�g���V�[���ɐ؂�ւ�
		if (KeyClick(KEY_INPUT_RETURN) == TRUE) {
			//�V�[���؂�ւ�
			//���̃V�[���̏��������R�R�ōs���Ɗy

			//�Q�[���f�[�^�̏�����
			GameInit();

			//�^�C�g����ʂɐ؂�ւ�
			ChangeScene(GAME_SCENE_TITLE);

		}
	}

	return;
}

/// <summary>
/// �G���h��ʂ̕`��
/// </summary>
/// <param name=""></param>
VOID EndDraw(VOID)
{
	switch (GameEndFlag)
	{
	case GAME_CLEAR:
		//=======================================================
		//     �Q�[���N���A�̕`��     
		//=======================================================

		
		break;


	case GAME_OVER:
		//=======================================================
		//     �Q�[���I�[�o�[�̕`��     
		//=======================================================

		
		break;


	default:
		break;
	}

	DrawString(0, 0, "�G���h���", GetColor(0, 0, 0));
	return;
}


//=====================================================================================================================
//          �R�R���烁�j���[���          
//=====================================================================================================================

/// <summary>
/// ���j���[���̕\��
/// </summary>
/// <param name="Volum">�������Ă���BGM�̉���</param>
VOID DrawMenu(int Volum)
{
	
	//M�L�[�������ꂽ�Ƃ��t���O�𔽓]������
	if (KeyClick(KEY_INPUT_M) == TRUE)
	{
		if (MenuFlag == TRUE) MenuFlag = FALSE;
		else if (MenuFlag == FALSE) MenuFlag = TRUE;

		
	}

	//���j���[�L�[�������Ă���Ƃ��ɕ`��
	if (MenuFlag == TRUE)
	{
		
	}
	return;
}


//=====================================================================================================================
//          �R�R����q���g���          
//=====================================================================================================================

VOID DrawTips(VOID)
{
	

	//H�L�[�������ꂽ�Ƃ��t���O�𔽓]������
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
			//     �^�C�g���̕���     
			//=============================================

			
			break;
		case GAME_SCENE_PLAY:
			//=============================================
			//     �v���C�̕���     
			//=============================================

			
			break;
		case GAME_SCENE_END:
			//=============================================
			//     �G���h�̕���     
			//=============================================


			break;
		default:
			break;
		}
	}
	return;
}


//=====================================================================================================================
//          �R�R����؂�ւ�          
//=====================================================================================================================

/// <summary>
/// �؂�ւ����
/// </summary>
/// <param name=""></param>
VOID Change(VOID)
{
	ChangeProc();    //����
	ChangeDraw();    //�`��

	return;
}

/// <summary>
/// �؂�ւ���ʂ̏���
/// </summary>
/// <param name=""></param>
VOID ChangeProc(VOID)
{
	//�t�F�[�h�C��
	if (IsFadeIn == TRUE)
	{
		if (fadeInCnt > fadeInCutMax)
		{
			fadeInCnt--;               //�J�E���g�����炷
		}
		else
		{
			//�t�F�[�h�C���������I�����
			fadeInCnt = fadeInCntInit; //�J�E���^������
			IsFadeIn = FALSE;          //�t�F�[�h�C�������I��
		}
	}

	//�t�F�[�h�A�E�g
	if (IsFadeOut == TRUE)
	{
		if (fadeOutCnt < fadeOutCutMax)
		{
			fadeOutCnt++;               //�J�E���g�����炷
		}
		else
		{
			//�t�F�[�h�A�E�g�������I�����
			fadeOutCnt = fadeOutCntInit; //�J�E���^������
			IsFadeOut = FALSE;          //�t�F�[�h�A�E�g�����I��
		}
	}

	//�؂�ւ������I�����H
	if (IsFadeIn == FALSE && IsFadeOut == FALSE)
	{
		//�t�F�[�h�C�����t�F�[�h�A�E�g�����Ă��Ȃ���
		GameScene = NextGameScene;  //���̃V�[���ɐ؂�ւ�
		OldGameScene = GameScene;   //�ȑO�̃Q�[���V�[���X�V
	}

	return;
}

/// <summary>
/// �؂�ւ���ʂ̕`��
/// </summary>
/// <param name=""></param>
VOID ChangeDraw(VOID)
{
	//�ȑO�̃V�[����`��
	switch (OldGameScene)
	{
	case GAME_SCENE_TITLE:
		TitleDraw();      //�^�C�g����ʂ̕`��
		break;
	case GAME_SCENE_PLAY:
		PlayDraw();       //�v���C��ʂ̕`��
		break;
	case GAME_SCENE_END:
		EndDraw();        //�G���h��ʂ̕`��
		break;
	default:
		break;
	}

	//�t�F�[�h�C��
	if (IsFadeIn == TRUE)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)fadeInCnt / fadeInCutMax) * 255);
	}

	//�t�F�[�h�A�E�g
	if (IsFadeOut == TRUE)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)fadeOutCnt / fadeOutCutMax) * 255);
	}

	//�l�p�`��`��
	DrawBox(0, 0, GAME_WIDTH, GAME_HEIGHT, GetColor(0, 0, 0), TRUE);

	//�������I��
	SetDrawBlendMode(DX_BLENDGRAPHTYPE_NORMAL, 0);

	DrawString(0, 0, "�؂�ւ����", GetColor(0, 0, 0));

	return;
}


//=====================================================================================================================
//          �R�R���瓖���蔻��֌W          
//=====================================================================================================================

/// <summary>
/// �����蔻��̗̈�X�V(�v���C���[)
/// </summary>
/// <param name="coll">�����蔻��̗̈�</param>
VOID CollUpdatePlayer(CHARACTOR* chara)
{
	chara->coll.left = chara->img.X + 40;
	chara->coll.top = chara->img.Y;
	chara->coll.right = chara->img.X + chara->img.width - 40;
	chara->coll.bottom = chara->img.Y + chara->img.height;

	return;
}

/// <summary>
/// �����蔻��̗̈�X�V(�G)
/// </summary>
/// <param name="coll">�����蔻��̗̈�</param>
VOID CollUpdateEnemy(CHARACTOR* chara)
{
	chara->coll.left = chara->img.X;
	chara->coll.top = chara->img.Y;
	chara->coll.right = chara->img.X + chara->img.width;
	chara->coll.bottom = chara->img.Y + chara->img.height;

	return;
}

/// <summary>
/// �����蔻��̗̈�X�V
/// </summary>
/// <param name="coll">�����蔻��̗̈�</param>
VOID CollUpdate(CHARACTOR* chara)
{
	chara->coll.left = chara->img.X;
	chara->coll.top = chara->img.Y;
	chara->coll.right = chara->img.X + chara->img.width;
	chara->coll.bottom = chara->img.Y + chara->img.height;

	return;
}

/// <summary>
/// �����蔻��̗̈�X�V(�e)
/// </summary>
/// <param name="coll">�����蔻��̗̈�</param>
VOID CollUpdateTama(TAMA* tama)
{
	tama->coll.left = tama->x;
	tama->coll.top = tama->y;
	tama->coll.right = tama->x + tama->width;
	tama->coll.bottom = tama->y + tama->height;

	return;
}

/// <summary>
/// �������Ă��邩�𒲂ׂ�
/// </summary>
/// <param name=""></param>
/// <returns>�������Ă�E�E�ETRUE�b�������Ă��Ȃ��E�E�EFALSE</returns>
BOOL OnCollision(RECT coll1, RECT coll2)
{
	//�������Ă�����TRUE
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
//          �R�R����BGM�֌W          
//=====================================================================================================================

/// <summary>
/// BGM�̉��ʂ�ύX����
/// </summary>
/// <param name="music">�ύX������BGM��AUDIO�\���̂̃A�h���X</param>
VOID ChangeBGM(AUDIO* music)
{
	//���ʂ��O�`�Q�T�T�̎��ɍ쓮
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

	//���ʂ��͈͊O��������͈͓��ɖ߂�
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