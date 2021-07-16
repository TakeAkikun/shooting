#pragma once
//Copyright(c) 2020
//Hamamatu Mirai Sougou Senmongakkou GameCreate Y.Suzuki
//All rights reserved.

//�}�`�̃w�b�_�t�@�C��
#include "geme.h"

//�\����

  //int�^��POINT�\����
struct iPOINT
{
	int x = -1;//���W�̏�����
	int y = -1;
};

  //�~�̍\����
struct MARU
{
	iPOINT center;  //���S�_
	int radius;     //���a
};

//�v���g�^�C�v�錾
//extern�͊O���Ɋ֐�������ꍇ�̖ڈ�Ƃ��Ă��Ă���

extern BOOL CheckCollRectToRect(RECT a, RECT b);         //�l�p�̈�̓����蔻����`�F�b�N����
extern BOOL CheckCollPointToRect(iPOINT pt, RECT r);     //�_�Ǝl�p�̓����蔻��
extern BOOL CheckCollPointToMaru(iPOINT pt, MARU m);     //�_�Ɖ~�̓����蔻��
extern BOOL CheckCollMaruToMaru(MARU maru1, MARU maru2); //�~�Ɖ~�̓����蔻��

extern RECT GetRect(int left, int top, int right, int bottom);          //RECT�^���ꎞ�I�ɓn��
extern void DrawRect(RECT r, unsigned int color, bool b);               //RECT�𗘗p���Ďl�p��`��
extern void DrawMaru(MARU c, unsigned int color, bool b, int thick = 1);//MARU�𗘗p���ĉ~��`��