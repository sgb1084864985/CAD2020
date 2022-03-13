#include "graphics.h"
#include "extgraph.h"
#include "genlib.h"
#include "simpio.h"
#include "conio.h"
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

#include "imgui.h"
#include "menu.h"

// ȫ�ֱ���
static int  CodeShowMode = 0;		// ����չʾ״̬
static int  ExampleFlagSelection = -1;// Ӧ���ڼ�¼�û�����˵���ķ���ֵ


void DrawMenu_(double winwidth, double winheight)
{
	// ��������simpleGUI�涨��Ԥ�ȶ����5���˵�������
	static char* menuListFile[] = { "���ز˵�",
		"ȷ�Ϸ���" };
	static char* menuListShow[] = { "��ʾѡ��",
		"Program 1" };
	static char* menuListCode[] = { "����",
		"չʾ����" };
	static char* menuListMusic[] = { "����",
		"�㽭��ѧУ��",
		"The Congress Reel",
		"Yellow",
		"ֹͣ����" };
	static char* menuListProcessure[] = { "����",
		"��ʼ  | Ctrl-Z", };


	double h; // �ؼ��߶�
	double w = TextStringWidth(menuListFile[0]) * 1.5; // �ؼ����
	double x = 0;// ������ť��xλ�ã��𽥵���
	double y = winheight;// ��ť��yλ�ã�����
	double ButtonInterval = 1;// ��ť֮��ļ��
	int    FileSelection, ShowSelection, CodeSelection, MusicSelection, ProcessureSelection;// �ֱ��¼ÿһ����ť�ķ���ֵ
	int    ExampleReturnFlag = 6;// ����ȷ�����ս��ĸ���ť�ķ���ֵ����FlagSelection

	// �Զ�����ɫ
	DefineColor("ButtonFrameCommon", 0.07, 0.72, 0.96);
	DefineColor("ButtonFrameHot", 0.20, 0.60, 1.00);
	setMenuColors("ButtonFrameCommon", "White", "ButtonFrameHot", "White", 1);

	// �����������С
	SetFont("΢���ź�");
	SetPointSize(18);

	//�ڹ涨���������С�󣬶Կؼ��߶ȸ�ֵ
	h = GetFontHeight() * 1.2;

	// ���ƴ��ڱ���
	SetWindowTitle("С���㷨����ͼ���ƹ���-��ʾ����");

	// ���λ��Ʋ˵���������˵���ť
	drawMenuBar(0, y - h, winwidth, h);

	// File �˵�
	FileSelection = menuList(GenUIID(0), x, y - h, w, TextStringWidth(menuListFile[1]) * 1.2, h,
		menuListFile, sizeof(menuListFile) / sizeof(menuListFile[0])) + 10;
	if (FileSelection > 10) {
		ExampleReturnFlag = 1;
	}

	// Show �˵�
	ShowSelection = menuList(GenUIID(0), x + w + ButtonInterval, y - h, w, TextStringWidth(menuListShow[1]) * 1.2, h,
		menuListShow, sizeof(menuListShow) / sizeof(menuListShow[0])) + 20;
	if (ShowSelection > 20) {
		ExampleReturnFlag = 2;
	}

	// Code �˵�
	menuListCode[1] = CodeShowMode ? "���ش���" : "չʾ����";
	CodeSelection = menuList(GenUIID(0), x + 2 * (w + ButtonInterval), y - h, w, TextStringWidth(menuListCode[1]) * 1.2, h,
		menuListCode, sizeof(menuListCode) / sizeof(menuListCode[0])) + 30;
	if (CodeSelection > 30) {
		ExampleReturnFlag = 3;
		if (CodeSelection == 31) CodeShowMode = !CodeShowMode;
	}

	// Music �˵�
	MusicSelection = menuList(GenUIID(0), x + 3 * (w + ButtonInterval), y - h, w, TextStringWidth(menuListMusic[2]) * 1.2, h,
		menuListMusic, sizeof(menuListMusic) / sizeof(menuListMusic[0])) + 40;
	if (MusicSelection > 40) {
		Music(MusicSelection);
		ExampleReturnFlag = 4;
	}

	// Processure �˵�
	ProcessureSelection = menuList(GenUIID(0), x + 5 * (w + ButtonInterval), y - h, w, TextStringWidth(menuListProcessure[1]) * 1.2, h,
		menuListProcessure, sizeof(menuListProcessure) / sizeof(menuListProcessure[0])) + 50;
	if (ProcessureSelection > 50) {
		ExampleReturnFlag = 5;
	}

	// ͨ����ExampleReturnFlag����switch-case����¼ǡ���ķ���ֵ
	switch (ExampleReturnFlag) {
	case 0: ExampleFlagSelection = 0; break;
	case 1: ExampleFlagSelection = FileSelection; break;
	case 2: ExampleFlagSelection = ShowSelection; break;
	case 3: ExampleFlagSelection = CodeSelection; break;
	case 4: ExampleFlagSelection = MusicSelection; break;
	case 5: ExampleFlagSelection = ProcessureSelection; break;
	default: ExampleFlagSelection = -1; break;
	}
	// ����ReturnFlag
	ExampleReturnFlag = 0;
}

int GetExampleSelection() {
	int PresentExampleFlagSelection = ExampleFlagSelection;
	ExampleFlagSelection = -1;
	return PresentExampleFlagSelection;
}