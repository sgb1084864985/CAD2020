#include "graphics.h"
#include "extgraph.h"
#include "genlib.h"
#include "simpio.h"
#include "conio.h"
#include "draw.h"
#include "edit.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#include <windows.h>
#include <olectl.h>
#include <mmsystem.h>
#include <wingdi.h>
#include <ole2.h>
#include <ocidl.h>
#include <winuser.h>
#include <assert.h>

/*ȫ�ֱ���*/
static double ButtonInterval = 1;// �����水ť���
static char* Title = "С���㷨����ͼ���ƹ���";// ���������
static char* Author = " This Program Is Written By Zhu Lizhen, Yan Tianming and Bao Dezheng. ZJU 2020 Summer";// ������Ϣ�ַ����������׶β�������
enum __page { StartPage_, SavePage_, DrawPage_, HelpPage_, ExamplePage_, LoadPage_, LastPage_ };// ö�����Ͷ������н���

/*
* ��������Start
* �ӿڣ�Start(double winwidth, double winheight)
* ����������������������������������������������
* ����������ڳ�ʼ�����ڱ������������Ľ����ͼ
* ������
*/
void Start(double winwidth, double winheight)
{
	// ��ʼ�����ں�ͼ��ϵͳ
	Display_Start(winwidth, winheight);
}

/*
* ��������DrawStartMenu
* �ӿڣ�DrawStartMenu(double winwidth, double winheight)
* ������������������������������������������������������
* ����������ڻ��ƿ�ʼ���������˵���ť����ʵʱ��⣬��
* result��¼�����������ء�ʵ��ҳ��ת����
*/
int DrawStartMenu(double winwidth, double winheight)
{
	// �Բ˵����ݽ��ж���
	static char* menuListFile[] = { "�ļ�",
		"�½�  | Ctrl-N", // ��ݼ��������[Ctrl-X]��ʽ�������ַ����Ľ�β
		"��ʼ  | Ctrl-R" };
	static char* menuListExample[] = { "��ʾ",
		"������ʾ" };
	static char* menuListHelp[] = { "����",
		"ʹ�ð���",
		"����ȼ�",
		"���ڳ���" };
	static char* menuListExit[] = { "�˳�",
		"ȷ���˳�  | Ctrl-Q" };
	static char* menuListMusic[] = { "����",
		"�㽭��ѧУ��",
		"The Congress Reel",
		"Yellow",
		"ֹͣ����" };

	double h; // �ؼ��߶�
	double w = TextStringWidth(menuListFile[0]) * 1.5; // �ؼ���� 
	double x = winwidth / 2 - (5 * w + 4 * ButtonInterval) / 2;// �ؼ�λ��x����
	double y = winheight / 4;// �ؼ�λ��y����
	int    FileSelection, ExampleSelection, HelpSelection, ExitSelection, MusicSelection, Result = StartPage_;// ��¼�û�ѡ�����Ӧ�����Ľ��

	// �Զ��尴ť��˵���ɫ
	DefineColor("ButtonFrameCommon", 0.07, 0.72, 0.96);
	DefineColor("ButtonFrameHot", 0.20, 0.60, 1.00);
	setMenuColors("ButtonFrameCommon", "White", "ButtonFrameHot", "White", 1);

	// �����������ֺ�
	SetFont("΢���ź�");
	SetPointSize(18);

	h = GetFontHeight() * 1.2;// ��ť�߶ȸ�ֵ

	// ���ƴ��ڱ���
	SetWindowTitle("С���㷨����ͼ���ƹ���-��ʼ����");

	// File �˵�
	FileSelection = menuList(GenUIID(0), x, y - h, w, TextStringWidth(menuListFile[1]) * 1.2, h,
		menuListFile, sizeof(menuListFile) / sizeof(menuListFile[0]));
	switch (FileSelection) {
	case 1: Result = DrawPage_;// ת�����ƽ���
		break;
	case 2: Result = LoadPage_;// ת�����ؽ���
		break;
	default:
		break;
	}

	// Example �˵�
	ExampleSelection = menuList(GenUIID(0), x + w + ButtonInterval, y - h, w, TextStringWidth(menuListExample[1]) * 1.2, h,
		menuListExample, sizeof(menuListExample) / sizeof(menuListExample[0]));
	if (ExampleSelection == 1) Result = ExamplePage_;// ת����ʾ����

	// Help �˵�
	HelpSelection = menuList(GenUIID(0), x + 2 * w + 2 * ButtonInterval, y - h, w, TextStringWidth(menuListHelp[1]) * 1.2, h,
		menuListHelp, sizeof(menuListHelp) / sizeof(menuListHelp[0]));
	switch (HelpSelection) {
	case 1:
		SetHelpMode(1);
		Result = HelpPage_;// ת����������-ʹ�ð���
		break;
	case 2:
		SetHelpMode(2);
		Result = HelpPage_;// ת����������-����ȼ�
		break;
	case 3:
		SetHelpMode(3);
		Result = HelpPage_;// ת����������-���ڳ���
		break;

	}

	// Music �˵�
	MusicSelection = menuList(GenUIID(0), x + 3 * w + 3 * ButtonInterval, y - h, w, TextStringWidth(menuListMusic[2]) * 1.2, h,
		menuListMusic, sizeof(menuListMusic) / sizeof(menuListMusic[0]));
	if (MusicSelection > 0) {
		Music(MusicSelection);// ���Ż��Ʋ�������
	}


	// Exit �˵�
	ExitSelection = menuList(GenUIID(0), x + 4 * w + 4 * ButtonInterval, y - h, w, TextStringWidth(menuListExit[1]) * 1.2, h,
		menuListExit, sizeof(menuListExit) / sizeof(menuListExit[0]));
	if (ExitSelection == 1) exit(-1);// �˳�����


	return Result;
}

/*
* ��������DrawTitle
* �ӿڣ�DrawTitle(double winwidth, double winheight)
* ��������������������������������������������������
* ����������ڻ��ƿ�ʼ��������ı��⡣
*/
void DrawTitle(double winwidth, double winheight)
{
	int OriginSize = GetPointSize();// ����ԭʼ�ֺ�
	char* OriginFont = GetFont(), * OriginColor = GetPenColor();// ����ԭʼ������������ɫ
	double TitleX, TitleY;// �����λ������

	// �����ֺš���������ɫ
	SetPointSize(40);
	SetFont("����");
	SetPenColor("black");

	// Ϊ����λ�ø�ֵ
	TitleX = (winwidth - TextStringWidth(Title)) / 2;
	TitleY = winheight * 2 / 3;

	// ���Ʊ���
	MovePen(TitleX, TitleY);
	DrawTextString(Title);

	// �ָ�ԭʼ����
	SetPointSize(OriginSize);
	SetFont(OriginFont);
	SetPenColor(OriginColor);
}

/*
* ��������DrawAuthor
* �ӿڣ�DrawAuthor()
* ������������������
* ����������ڻ��ƿ�
* ʼ�������½ǵ�����
* ��Ϣ�������׶β���
* ʾ�����Ϣ����
*/
void DrawAuthor() {
	int OriginSize = GetPointSize();// ����ԭʼ�ֺ�
	char* OriginFont = GetFont(), * OriginColor = GetPenColor();// ����ԭʼ������������ɫ
	double AuthorX = 0, AuthorY;// ������Ϣ��λ������

	// �����ֺš���������ɫ
	SetPointSize(12);
	SetFont("����");
	SetPenColor("gray");

	// Ϊ������Ϣ��ֵ
	AuthorY = GetFontHeight();

	// ����������Ϣ
	MovePen(AuthorX, AuthorY);
	DrawTextString(Author);

	// �ָ�ԭʼ�ֺš���������ɫ
	SetPointSize(OriginSize);
	SetFont(OriginFont);
	SetPenColor(OriginColor);
}

/*
* ��������Display_Start
* �ӿڣ�Display_Start(double winwidth, double winheight)
* ������������������������������������������������������
* ����������������������ƺ�������˳����ƿ�ʼ����ĸ���
* Ԫ�ء������ز˵���ť������Ӧ��ת��Ŀ��ҳ����Ϣ��
*/
int Display_Start(double winwidth, double winheight)
{
	// ���Ʊ���������
	DrawTitle(winwidth, winheight);
	DrawAuthor();

	// ���ƺʹ���˵�
	return DrawStartMenu(winwidth, winheight);
}