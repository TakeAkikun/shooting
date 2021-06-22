
//�w�b�_�t�@�C���ǂݍ���
#include "geme.h"       //�Q�[���S�̂̃w�b�_�t�@�C��
#include "keyboard.h"   //�L�[�{�[�h�̏���
#include "FPS.h"        //FPS�̏���

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
VOID CollUpdate(CHARACTOR* chara);                               //�����蔻��̗̈���X�V

BOOL OnCollision(RECT coll1, RECT coll2);                       //�������Ă��邩�𒲂ׂ�

VOID ChangeBGM(AUDIO* music);                                    //BGM�̉��ʕύX

BOOL GameLoad(VOID);                                             //�Q�[���S�̂̃f�[�^��ǂݍ���
VOID GameInit(VOID);                                             //�Q�[���f�[�^�̏�����
BOOL ImageInput(IMAGE* Image, const char* path);                 //�Q�[���̉摜�ǂݍ���
BOOL MusicInput(AUDIO* music, const char* path, int volume, int playType);//�Q�[����BGM�ǂݍ���

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
	
}


/// <summary>
/// �Q�[���S�̂̏�����
/// </summary>
/// <returns>�ǂݍ��ݐ����E�E�ETRUE�b�ǂݍ��ݎ��s�E�E�EFALSE</returns>
BOOL GameLoad()
{

	return TRUE;        //�S���ǂݍ��߂���TRUE
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
	//�S�[���̉摜��ǂݍ���
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
	DrawString(0, 0, "�v���C���", GetColor(0, 0, 0));

	return;
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