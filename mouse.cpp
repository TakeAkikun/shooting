//�}�E�X�����̃\�[�X�t�@�C��

//�w�b�_�t�@�C���̓ǂݍ���
#include "geme.h"
#include "mouse.h"
#include "FPS.h"

//�O���[�o���ϐ�

//�}�E�X���͂��擾
MOUSE mouse;

//�֐�

VOID MouseUpdate(VOID)
{
	//�}�E�X�̈ȑO�̏����擾
	mouse.OldPoint = mouse.Point;

	//�}�E�X�̈ȑO�̃{�^�����͂��擾
	mouse.OldButton[MOUSE_INPUT_LEFT] = mouse.Button[MOUSE_INPUT_LEFT];
	mouse.OldButton[MOUSE_INPUT_MIDDLE] = mouse.Button[MOUSE_INPUT_MIDDLE];
	mouse.OldButton[MOUSE_INPUT_RIGHT] = mouse.Button[MOUSE_INPUT_RIGHT];

	//�}�E�X�̍��W���擾
	GetMousePoint(&mouse.Point.x, &mouse.Point.y);

	//�}�E�X�̍��W����ʊO�̏ꍇ�́A��ʓ��Ɏ��߂�
	if (mouse.Point.x < 0) { mouse.Point.x = 0; }
	if (mouse.Point.x > GAME_WIDTH) { mouse.Point.x = GAME_WIDTH; }
	if (mouse.Point.y < 0) { mouse.Point.y = 0; }
	if (mouse.Point.y > GAME_HEIGHT) { mouse.Point.x = GAME_HEIGHT; }

	//�}�E�X�̉����Ă���{�^�����擾
	mouse.InputValue = GetMouseInput();

	//�ȉ��Ń}�X�N�������s��
	//���{�^���������Ă��邩�`�F�b�N(TRUE��0�ȊO)
	if ((mouse.InputValue & MOUSE_INPUT_LEFT) == MOUSE_INPUT_LEFT) { mouse.Button[MOUSE_INPUT_LEFT]++; }        //�����Ă���
	if ((mouse.InputValue & MOUSE_INPUT_LEFT) != MOUSE_INPUT_LEFT) { mouse.Button[MOUSE_INPUT_LEFT] = 0; }      //�����Ă��Ȃ�

	//���{�^���������Ă��邩�`�F�b�N(TRUE��0�ȊO)
	if ((mouse.InputValue & MOUSE_INPUT_MIDDLE) == MOUSE_INPUT_MIDDLE) { mouse.Button[MOUSE_INPUT_MIDDLE]++; }  //�����Ă���
	if ((mouse.InputValue & MOUSE_INPUT_MIDDLE) != MOUSE_INPUT_MIDDLE) { mouse.Button[MOUSE_INPUT_MIDDLE] = 0; }//�����Ă��Ȃ�

	//�E�{�^���������Ă��邩�`�F�b�N(TRUE��0�ȊO)
	if ((mouse.InputValue & MOUSE_INPUT_RIGHT) == MOUSE_INPUT_RIGHT) { mouse.Button[MOUSE_INPUT_RIGHT]++; }     //�����Ă���
	if ((mouse.InputValue & MOUSE_INPUT_RIGHT) != MOUSE_INPUT_RIGHT) { mouse.Button[MOUSE_INPUT_RIGHT] = 0; }   //�����Ă��Ȃ�

	//�z�C�[���̉�]�ʂ��擾
	mouse.WheelValue = GetMouseWheelRotVol();

	return;
}

/// <summary>
/// �{�^���������Ă��邩�A�}�E�X�R�[�h�Ŕ��f����
/// </summary>
/// <param name="MOUSE_INPUT_">MOUSE_INPUT_</param>
/// <returns>�{�^���������Ă���FTRUE</returns>
BOOL MouseDown(int MOUSE_INPUT_)
{
	//�}�E�X�R�[�h�̃{�^���������Ă��鎞
	if (mouse.Button[MOUSE_INPUT_] != 0)
	{
		return TRUE;  //�{�^���������Ă���
	}
	else
	{
		return FALSE; //�{�^���������Ă��Ȃ�
	}
}

/// <summary>
/// �{�^���������グ�����A�}�E�X�R�[�h�Ŕ��f����
/// </summary>
/// <param name="MOUSE_INPUT_">MOUSE_INPUT_</param>
/// <returns>�{�^���������グ�Ă���FTRUE</returns>
BOOL MouseUp(int MOUSE_INPUT_)
{
	//���O�͉����Ă��邪�A���͉����Ă��Ȃ���
	if (mouse.OldButton[MOUSE_INPUT_] >= 1 &&
		mouse.Button[MOUSE_INPUT_]    == 0)
	{
		return TRUE;  //�{�^���������グ�Ă���
	}
	else
	{
		return FALSE; //�{�^���������グ�Ă��Ȃ�
	}
}

/// <summary>
/// �{�^�������������Ă��邩�A�}�E�X�R�[�h�Ŕ��f����
/// </summary>
/// <param name="MOUSE_INPUT_">MOUSE_INPUT_</param>
/// <param name="milliTime">�{�^�������������Ă��鎞��(�~���b)</param>
/// <returns>�{�^�������������Ă���FTRUE</returns>
BOOL MouseDownKeep(int MOUSE_INPUT_, int milliTime)
{
	float MilliSec = 1000.0f; //1�b��1000�~���b

	//���������Ă��鎞�ԁ��b���~FPS�l
	int UpdateTime = (milliTime / MilliSec) * fps.Value;

	if (mouse.Button[MOUSE_INPUT_] > UpdateTime)
	{
		return TRUE;  //���������Ă���
	}
	else
	{
		return FALSE; //���������Ă��Ȃ�
	}
}

/// <summary>
/// �}�E�X���N���b�N�������A�}�E�X�R�[�h�Ŕ��f����
/// </summary>
/// <param name="MOUSE_INPUT_">MOUSE_INPUT_</param>
/// <returns>�N���b�N�FTRUE</returns>
BOOL MouseClick(int MOUSE_INPUT_)
{
	//���O�͉����Ă��邪�A���͉����Ă��Ȃ���
	if (mouse.OldButton[MOUSE_INPUT_] >= 1 &&
		mouse.Button[MOUSE_INPUT_]    == 0)
	{
		return TRUE;  //�N���b�N�����i�����ˏグ��@�ň��j
	}
	else
	{
		return FALSE; //�N���b�N���Ă��Ȃ�or���������Ă���
	}
}

/// <summary>
/// �}�E�X�̏���`�悷��
/// </summary>
/// <param name=""></param>
VOID MouseDraw(VOID)
{
	if (GAME_DEBUG == TRUE)
	{
		//�}�E�X�̍��W��`��
		DrawFormatString(0, GAME_HEIGHT - 40, GetColor(255, 255, 255), "MOUSE[x:%4d/y:%4d]", mouse.Point.x, mouse.Point.y);
	}

	return;
}

/// <summary>
/// �}�E�X����`�̈���N���b�N�������H
/// </summary>
/// <param name="rect">��`�̈�</param>
/// <param name="MOUSE_INPUT_">�}�E�X�R�[�h</param>
/// <returns></returns>
BOOL MouseRectClick(RECT rect, int MOUSE_INPUT_)
{
	//�_�Ǝl�p�̓����蔻��
	if (CheckCollPointToRect(mouse.Point, rect) == TRUE)
	{
		//�}�E�X���N���b�N���Ă��邩�H
		return MouseClick(MOUSE_INPUT_);
	}

	return FALSE;
}

BOOL MouseMaruClick(MARU circle, int MOUSE_INPUT_)    //�}�E�X���~�̈���N���b�N�������H
{
	//�_�Ɖ~�̓����蔻��
	if (CheckCollPointToMaru(mouse.Point, circle) == TRUE)
	{
		//�}�E�X���N���b�N���Ă��邩�H
		return MouseClick(MOUSE_INPUT_);
	}

	return FALSE;
}