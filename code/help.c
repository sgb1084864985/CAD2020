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


/*ȫ�ֱ���*/
static int	  HelpMode = 2;			// ˵��ģʽ��1Ϊʹ�ð�����2Ϊ����ȼ���3Ϊ���ڳ���
enum __page { StartPage_, SavePage_, DrawPage_, HelpPage_, ExamplePage_, LoadPage_, LastPage_ };

/*
* ��������drawButtons
* �ӿڣ�drawButtons(double winwidth, double winheight)
* ����������������������������������������������������
* ����������ڻ��ƴ������½ǵķ��ذ�ť�����ڷ�����һ��
* ���档
*/
int drawButtons(double winwidth, double winheight)
{
	double h, w;  // �ؼ��߶ȿ��
	double x = winwidth * 5 / 6;
	double y = winheight / 6;// �ؼ�λ��

	// �������塢�ֺ�
	SetFont("΢���ź�");
	SetPointSize(18);

	// Ϊ�ؼ��߶ȿ�ȡ�y���긳ֵ
	h = GetFontHeight() * 1.3;
	y += h;
	w = TextStringWidth("����") * 1.5;

	// ������ɫ
	DefineColor("ButtonFrameCommon", 0.07, 0.72, 0.96);
	DefineColor("ButtonFrameHot", 0.20, 0.60, 1.00);
	setButtonColors("ButtonFrameCommon", "White", "ButtonFrameHot", "White", 1);

	// ���ư�ť
	if (button(GenUIID(0), x, y - 2 * h, w, h, "����"))
		return LastPage_;
	else
		return HelpPage_;
}


/*
* ��������ProgramHelp
* �ӿڣ�ProgramHelp(double winwidth, double winheight)
* ����������������������������������������������������
* ����������ڻ��ư�������-ʹ�ð������棬�����û�����
* �����ʹ�ð�����
*/
void ProgramHelp(double winwidth, double winheight) {
	// ���������ġ�������\n��ʾһ�еĽ�����end\n��ʾһ�εĽ�����END\n��ʾȫ�ĵĽ���
	char* ProgramHelpTitle[] = { "������˵��\n","������ģ��\n","����ʾģ��\n","����ɫ����\n","����л\n" };
	char* ProgramHelpText[] = {
		"���������ڻ����㷨����ͼ����Ҫ��Ϊ��������ʾ����ģ�顣�ڿ�ʼ����������ѡ��",
		"\n",
		"�½��ļ���ʼ���ƣ�Ҳ����ѡ���ȡ�Ѵ���õĺ�׺ΪDAT���ļ��������޸����һ",
		"\n",
		"�����ơ�",
		"end\n",
		"��ģ�����ʵ�ֵĹ����У��ٲ����ı���ͼ�Σ��������Ρ�Բ�Ǿ��Ρ�������Բ�Σ�",
		"\n",
		"�������ߣ��ڶ�ͼ�ν�����������ת�ı任���۶�ͼ�Ρ������߽�����ɫ����ϸ����",
		"\n",
		"ʵ����ʽ�������ܶ��ı���ͼ�ν��и��ơ�ճ����������ɾ���Ĳ�����",
		"\n",
		"��Ҫע����Ǳ�ģ���Ϊ����ģʽ���ı�ģʽ��ͼ��ģʽ������ģʽ�ĸ�ģʽ����Ӧ",
		"\n",
		"�Ĺ�����Ҫ������ģʽ�²ſ���ʹ�á�������ʹ�õĹ��ܰ�ť��ʾΪ��ɫ����������",
		"\n",
		"��ģʽ���˵����л�ģʽ�����½ǵ�ģʽ�����ʾ����ǰ����ģʽ��",
		"\n",
		"���·�����Ϣ���ṩ��ǰ�ļ�����״̬��ѡ��Ԫ�ص������Ϣ��",
		"end\n",
		"��ģ�����ʵ�ֵĹ��ܣ���Ϊ���ṩ���͵��㷨��ʾͼ�⣬ͨ�������ʾ�����л���",
		"\n",
		"��ͨ����һ������һ�������Խ����𲽹ۿ���ؿ������ṩ�����չʾ�����ع��ܡ�",
		"end\n",
		"�ڱ�����ĸ�������������ֲ��ŵĹ��ܣ�Ԥ��3�����֣�Ĭ��ѭ�����ţ�����ʱ��",
		"\n",
		"���Ե��ֹͣ���š�",
		"end\n",
		"��л�����ñ����򣬲���л���ṩ�ı�������뽨�飡",
		"END\n" };
	double Interval[2], TextX, TextY;// �����ж����˱��������ġ�����������֮��ļ����TextX��TextY���ڱ�ʾ����λ��
	int i;// ����

	// ����������������塢�ֺţ�Ϊ���������ֵ
	SetFont("΢���ź�");
	SetPointSize(14);
	Interval[0] = GetFontHeight() / 3;
	SetFont("����");
	SetPointSize(18);
	Interval[1] = GetFontHeight() / 2;

	// ���ƴ��ڵ�����
	SetWindowTitle("С���㷨����ͼ���ƹ���-��������-ʹ�ð���");

	// ���Ʊ���
	TextX = TextStringWidth("�ո�");
	TextY = winheight - Interval[1] * 2;// ��ʼλ��
	MovePen(TextX, TextY);
	SetPenColor("Black");// ������ɫΪ��ɫ
	for (i = 0; i < 5; i++) {
		DrawTextString(ProgramHelpTitle[i]);
		switch (i) {// һ����5�����⣬����ÿ�������µ�����������������ó�����������
		case 0:TextY -= (10 * Interval[0] + 4 * Interval[1]); break;
		case 1:TextY -= (22 * Interval[0] +6 * Interval[1]); break;
		case 2:case 3:TextY -= (7 * Interval[0] + 3.5* Interval[1]); break;
		case 4:TextY += (46 * Interval[0] + 15 * Interval[1]); TextX += TextStringWidth("��"); break;// �������һ������󣬷�����������
		}
		MovePen(TextX, TextY);
	}

	// ��������
	SetFont("΢���ź�");
	SetPointSize(18);
	DefineColor("TextBlue", 0.15, 0.25, 0.63);// �Զ���īˮ��
	SetPenColor("TextBlue");
	for (i = 0; ProgramHelpText[i][0] != 'E'; i++) {
		if (ProgramHelpText[i] == "\n") {// ��ĩ����
			TextY -= Interval[0];
		}
		else if (ProgramHelpText[i][0] == 'e') {// ��ĩ����
			TextY -= (2 * Interval[0] + 3 * Interval[1]);
		}
		else {
			DrawTextString(ProgramHelpText[i]);// �������ݻ���
			TextY -= 2 * Interval[0];
		}
		MovePen(TextX, TextY);
	}
}

/*
* ��������ShortcutHelp
* �ӿڣ�ShortcutHelp(double winwidth, double winheight)
* ����������������������������������������������������
* ����������ڻ��ư�������-����ȼ����棬�����û�����
* ��������Ŀ�ݼ�������ʾ��
*/
void ShortcutHelp(double winwidth, double winheight) {
	// ���������ġ�������\n��ʾһ�еĽ�����end\n��ʾһ�εĽ�����END\n��ʾȫ�ĵĽ���
	char* ShortcutTitle[] = { "����ʼ����\n","�����ƽ���\n" };
	char* ShortcutText[] = {
		"���ļ����½��� Ctrl + N  |  �򿪣�Ctrl + O",
		"\n",
		"���˳����˳��� Ctrl + Q",
		"end\n",
		"���ļ������棺 Ctrl + S  |  �˳��� Ctrl + Q  |  �򿪣�Ctrl + O  |  ���棺Ctrl + S",
		"\n",
		"���༭�����ƣ� Ctrl + C  |  ճ���� Ctrl + V  |  ���У�Ctrl + X  |  ɾ����Ctrl + D",
		"END\n" };
	double Interval[2], TextX, TextY;// �����ж����˱��������ġ�����������֮��ļ����TextX��TextY���ڱ�ʾ����λ��
	int i;// ����

	// ����������������塢�ֺţ�Ϊ���������ֵ
	SetFont("΢���ź�");
	SetPointSize(18);
	Interval[0] = GetFontHeight() / 2;
	SetFont("����");
	SetPointSize(22);
	Interval[1] = GetFontHeight() / 2;

	// ���ƴ��ڵ�����
	SetWindowTitle("С���㷨����ͼ���ƹ���-��������-����ȼ�");

	// ���Ʊ���
	TextX = TextStringWidth("�ո�");
	TextY = winheight - Interval[1] * 3;
	MovePen(TextX, TextY);
	SetPenColor("Black");
	for (i = 0; i < 2; i++) {
		DrawTextString(ShortcutTitle[i]);
		switch (i) {// һ����3�����⣬����ÿ�������µ�����������������ó�����������
		case 0:TextY -= (7 * Interval[0] + 3 * Interval[1]); break;
		case 1:TextY += (7 * Interval[0] + 1 * Interval[1]); TextX += TextStringWidth("��"); break;
		}
		MovePen(TextX, TextY);
	}

	// ��������
	SetFont("΢���ź�");
	SetPointSize(18);
	DefineColor("TextBlue", 0.15, 0.25, 0.63);
	SetPenColor("TextBlue");
	for (i = 0; ShortcutText[i][0] != 'E'; i++) {
		if (ShortcutText[i] == "\n") {// ��ĩ����
			TextY -= Interval[0];
		}
		else if (ShortcutText[i][0] == 'e') {// ��ĩ����
			TextY -= (2 * Interval[0] + 3 * Interval[1]);
		}
		else {
			DrawTextString(ShortcutText[i]);// ��������
			TextY -= 2 * Interval[0];
		}
		MovePen(TextX, TextY);
	}
}

/*
* ��������AboutHelp
* �ӿڣ�AboutHelp(double winwidth, double winheight)
* ����������������������������������������������������
* ����������ڻ��ư�������-����ȼ����棬�����û�����
* ��������Ŀ�ݼ�������ʾ��
*/
void AboutHelp(double winwidth, double winheight)
{
	// ���������ġ�������\n��ʾһ�еĽ�����end\n��ʾһ�εĽ�����END\n��ʾȫ�ĵĽ���
	char* AboutHelpTitle[] = { "����������\n","����ϵ��ʽ\n" };
	char* AboutHelpText[] = {
		"������ �� ������ �� ������ �� �㽭��ѧ 2019�� �������ѧ�뼼�� �� �������ר��",
		"end\n",
		"��������뽨�飬��ӭ��ϵ���ǣ�3190105240@zju.edu.cn",
		"END\n" };
	double Interval[2], TextX, TextY;// �����ж����˱��������ġ�����������֮��ļ����TextX��TextY���ڱ�ʾ����λ��
	int i;// ����

	// ����������������塢�ֺţ�Ϊ���������ֵ
	SetFont("΢���ź�");
	SetPointSize(18);
	Interval[0] = GetFontHeight() / 2;
	SetFont("����");
	SetPointSize(22);
	Interval[1] = GetFontHeight() / 2;

	// ���ƴ��ڵ�����
	SetWindowTitle("С���㷨����ͼ���ƹ���-��������-���ڳ���");

	// ���Ʊ���
	TextX = TextStringWidth("�ո�");
	TextY = winheight - Interval[1] * 3;
	MovePen(TextX, TextY);
	SetPenColor("Black");
	for (i = 0; i < 2; i++) {
		DrawTextString(AboutHelpTitle[i]);
		switch (i) {// һ����2�����⣬����ÿ�������µ�����������������ó�����������
		case 0:TextY -= (4 * Interval[0] + 3 * Interval[1]); break;
		case 1:TextY += (4 * Interval[0] + Interval[1]); TextX += TextStringWidth("��"); break;
		}
		MovePen(TextX, TextY);
	}

	// ��������
	SetFont("΢���ź�");
	SetPointSize(18);
	DefineColor("TextBlue", 0.15, 0.25, 0.63);
	SetPenColor("TextBlue");
	for (i = 0; AboutHelpText[i][0] != 'E'; i++) {
		if (AboutHelpText[i] == "\n") {
			TextY -= Interval[0];
		}
		else if (AboutHelpText[i][0] == 'e') {
			TextY -= (2 * Interval[0] + 3 * Interval[1]);
		}
		else {
			DrawTextString(AboutHelpText[i]);
			TextY -= 2 * Interval[0];
		}
		MovePen(TextX, TextY);
	}
}

/*
* ��������Display_Help
* �ӿڣ�Display_Help(double winwidth, double winheight)
* ����������������������������������������������������
* ����������ڸ��ݵ�ǰ��HelpMode���ж�Ӧ��������ĺ���
* ���á�
*/
int Display_Help(double winwidth, double winheight)
{
	// ��ť
	int result = drawButtons(winwidth, winheight);

	// ���ݵ�ǰHelpMode���а��������ѡ�����
	switch (HelpMode) {
	case 1:ProgramHelp(winwidth, winheight); break;
	case 2:ShortcutHelp(winwidth, winheight); break;
	case 3:AboutHelp(winwidth, winheight); break;
	}

	return result;
}

/*
* ��������SetHelpMode
* �ӿڣ�SetHelpMode(int mode)
* ����������������������������
* ����������ڸ��ݴ����ֵ����
* HelpModeֵ�����޸ġ��𵽶԰�
* ������չʾ��ѡ�����á�
*/
void SetHelpMode(int mode) {
	HelpMode = mode;
}