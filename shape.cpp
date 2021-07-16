//Copyright(c) 2020
//Hamamatu Mirai Sougou Senmongakkou GameCreate Y.Suzuki
//All rights reserved.

//�}�`�̃\�[�X�t�@�C��

//�w�b�_�t�@�C���ǂݍ���
#include "geme.h"
#include "shape.h"
#include <math.h>

//�֐�

/// <summary>
/// ��`�̈擯�m�̓����蔻��
/// </summary>
/// <param name="a">�̈�a</param>
/// <param name="b">�̈�b</param>
/// <returns></returns>
BOOL CheckCollRectToRect(RECT a, RECT b)
{
	if (a.left < b.right &&
		a.top < b.bottom &&
		a.right > b.left &&
		a.bottom > b.top )
	{
		return TRUE;  //�������Ă���
	}

	return FALSE;  //�������Ă��Ȃ�
}

/// <summary>
/// �l�p�Ɠ_�̓����蔻��
/// </summary>
/// <param name="pt">iPOINT�^�̓_</param>
/// <param name="r">��`�̈�</param>
/// <returns></returns>
BOOL CheckCollPointToRect(iPOINT pt, RECT r)
{
	//�_�̈ʒu���l�p�̒��ɂ���Ƃ�
	if (pt.x > r.left && pt.x < r.right &&
		pt.y > r.top  && pt.y < r.bottom)
	{
		return TRUE;
	}

	return FALSE;
}

/// <summary>
/// �~�Ɠ_�̓����蔻��
/// </summary>
/// <param name="pt">iPOINT�^�̓_</param>
/// <param name="m">�~�̈�</param>
/// <returns></returns>
BOOL CheckCollPointToMaru(iPOINT pt, MARU m)
{
	//�s�^�S���X�̒藝���g�p

	//���p�O�p�`�����߂�
	float a = pt.x - m.center.x;    //���
	float b = pt.y - m.center.y;    //����
	float c = sqrtf(a * a + b * b); //�ΕӁisq rt = square root = �������j

	//�Εӂ̒������~�̔��a�ȉ��Ȃ�΁A�~�̒��ɂ���
	if (c <= m.radius)
	{
		return TRUE;
	}

	return FALSE;
}

/// <summary>
/// �~�Ɖ~�̓����蔻��
/// </summary>
/// <param name="maru1">�~�̈�1</param>
/// <param name="maru2">�~�̈�2</param>
/// <returns></returns>
BOOL CheckCollMaruToMaru(MARU maru1, MARU maru2)
{
	//�s�^�S���X�̒藝���g�p

	//���p�O�p�`�����߂�
	float a = maru1.center.x - maru2.center.x;    //���
	float b = maru1.center.y - maru2.center.y;    //����
	float c = sqrtf(a * a + b * b);               //�ΕӁisq rt = square root = �������j

	//�Εӂ̒������~�̔��a1��2�𑫂��������ȉ��Ȃ�΁A�~�̒��ɂ���
	if (c <= maru1.radius + maru2.radius)
	{
		return TRUE;
	}

	return FALSE;
}

/// <summary>
/// RECT�^���ꎞ�I�ɓn��
/// </summary>
/// <param name="left">��</param>
/// <param name="top">��</param>
/// <param name="right">�E</param>
/// <param name="bottom">��</param>
/// <returns>RECT�^</returns>
RECT GetRect(int left, int top, int right, int bottom)
{
	//�ꎞ�I��RECT�^�̕ϐ�������āA�߂�l�œn��

	RECT rect = { left,top,right,bottom };

	return rect;
}

/// <summary>
/// RECT�𗘗p���Ďl�p��`��
/// </summary>
/// <param name="r">RECT�\����</param>
/// <param name="color">�F</param>
/// <param name="b">�h��Ԃ��FTRUE, �h��Ԃ��Ȃ��FFALSE</param>
void DrawRect(RECT r, unsigned int color, bool b)
{
	//��������ɕ`��
	DrawCircle(r.left, r.top, r.right, r.bottom, color, b);
	return;
}

/// <summary>
/// MARU�𗘗p���Ďl�p��`��
/// </summary>
/// <param name="c">MARU�\����</param>
/// <param name="color">�F</param>
/// <param name="b">�h��Ԃ��FTRUE, �h��Ԃ��Ȃ��FFALSE</param>
/// <param name="thick">���̑���</param>
void DrawMaru(MARU c, unsigned int color, bool b, int thick)
{
	//��������ɕ`��
	DrawCircle(c.center.x, c.center.y, c.radius, color, b, thick);
	return;
}