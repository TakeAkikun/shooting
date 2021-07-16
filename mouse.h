#pragma once
//�}�E�X�����̃w�b�_�t�@�C��

//�w�b�_�t�@�C���̓ǂݍ���

#include "geme.h"
#include "shape.h"

//�}�N����`

#define MOUSE_BUTTON_CODE  129  //0x0000~0x0080�܂�(�}�E�X�̃{�^��)

//�\����

  //�}�E�X�\����
struct MOUSE
{
	int InputValue = 0; //GetMouseInput�̒l������
	int WheelValue = 0; //�}�E�X�z�C�[���̉�]��(���F�v���X�l�@��O�F�}�C�i�X�l)
	iPOINT Point;       //�}�E�X�̍��W������
	iPOINT OldPoint;    //�}�E�X�̍��W������
	int OldButton[MOUSE_BUTTON_CODE];  //�}�E�X�̃{�^������(���O)������
	int Button[MOUSE_BUTTON_CODE];     //�}�E�X�̃{�^�����͂�����
};

//�O���̃O���[�o���ϐ�

extern MOUSE mouse;

//�v���g�^�C�v�錾
//extern�͊O���Ɋ֐�������ꍇ�̖ڈ�Ƃ��Ă��Ă���

extern VOID MouseUpdate(VOID);                              //�}�E�X�̓��͏����X�V����
extern BOOL MouseDown(int MOUSE_INPUT_);                    //�{�^���������Ă��邩�A�}�E�X�R�[�h�Ŕ��f����
extern BOOL MouseUp(int MOUSE_INPUT_);                      //�{�^���������グ�����A�}�E�X�R�[�h�Ŕ��f����
extern BOOL MouseDownKeep(int MOUSE_INPUT_, int milliTime); //�{�^�������������Ă��邩�A�}�E�X�R�[�h�Ŕ��f����
extern BOOL MouseClick(int MOUSE_INPUT_);                   //�}�E�X���N���b�N�������A�}�E�X�R�[�h�Ŕ��f����
extern VOID MouseDraw(VOID);                                //�}�E�X�̏���`�悷��
extern BOOL MouseRectClick(RECT rect, int MOUSE_INPUT_);    //�}�E�X����`�̈���N���b�N�������H
extern BOOL MouseMaruClick(MARU circle, int MOUSE_INPUT_);    //�}�E�X���~�̈���N���b�N�������H