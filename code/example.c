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
#include <math.h>
#include <ole2.h>
#include <ocidl.h>
#include <winuser.h>
#include <assert.h>

#include "imgui.h"

#include "draw.h"
#include "edit.h"
#include "file.h"
#include "ExampleMenu.h"

/*��������*/
#define TIME_ANIMA 1
#define TIME_INTERNAL 1000

/*�ṹ����*/
typedef struct myline_ {
	double bx, by, ex, ey;
	int end;
}MyLine;

/*���ݳ�ʼ��*/
MyLine lines1[] = { {  4.606397774687065,   6.6536856745479831,   4.606397774687065,   6.3310152990264257,   1}, {  4.6397774687065372,   5.7969401947148818,
	  4.6397774687065372,   5.3630041724617525,   1}, {  4.6397774687065372,   4.7732962447844223,   4.6397774687065372,   4.4617524339360219,   1},
  {  4.6175243393602221,   3.927677329624478,   4.6175243393602221,   3.5938803894297635,   1}, {  4.6286509040333801,   3.0375521557719054,
	  4.6286509040333801,   2.8929068150208623,   0}, {  4.6286509040333801,   2.8929068150208623,   3.3824756606397774,   2.8929068150208623,   0},
  {  3.3824756606397774,   2.8929068150208623,   3.3824756606397774,   5.6077885952712094,   0}, {  3.3824756606397774,   5.6077885952712094,
	  4.6509040333796943,   5.6077885952712094,   1}, {  5.3630041724617525,   5.0625869262865084,   6.2976356050069544,   5.0625869262865084,   0},
  {  6.2976356050069544,   5.0625869262865084,   6.2976356050069544,   2.8595271210013911,   0}, {  6.2976356050069544,   2.8595271210013911,
	  5.0848400556328235,   2.8595271210013911,   0}, {  5.0848400556328235,   2.8595271210013911,   5.0848400556328235,   2.6369958275382475,   1},
  {  4.6509040333796943,   2.2698191933240608,   4.6509040333796943,   1.9582753824756605,   1} };

int cur1 = 13;

node head1[] = { {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
{  2,  1.3825000000000003,  0.54916666666666591,  0,  4.572916666666667,6.9374999999999982,  4,  1,  0,  0,  0 },
{  1,  2.3616666666020367,  0.52833333333333576,  0,  4.6249999999999982,  6.0729166666666687,  4,  1,  0,0,  0},
{  3,  1.4861701480548515,	0.61095668158707306,  -0.855097405069003,  4.6354166666666679,  5.0625,  4,  1,  0,  0,  0 },
{  1,  1.4450000000000018,  0.52833333333333576,  0,4.6229166666666668,  4.1958333333333364,  4,  1,  0,  0,  0 },
{  1,  1.4450000000000018,  0.52833333333333576,  0,  4.6500000000000004,  3.3270833333333352,  4,1,  0,  0,  0 },
{  3,  1.6199049839585435,  0.38205211384234866,  -166.7594800772336,  4.6666666666666679,  2.46875,  4,  1,  0,0,  0},
{  2,  1.3825000000000003,  0.54916666666666591,  0,  4.6187500000000004,1.7020833333333316,  4,  1,  0,  0,  0x0} };

text thead1[] = { { 4.3616133518776081,  6.8762169680111267, -6.2774385622041925e+066,  -6.2774385622041925e+066,  "start",
	 0}, { 4.17,  6.0194714881780254, -6.2774385622041925e+066,  -6.2774385622041925e+066,
	 "int i=0,sum=0;",    0}, { 4.3949930458970794,  5.0180806675938801, -6.2774385622041925e+066,
	 -6.2774385622041925e+066,  "i<10?",   0}, { 5.7413073713490963,  5.1738525730180811,
	-6.2774385622041925e+066,  -6.2774385622041925e+066,  "N",  0}, { 4.350486787204451,
	 4.5841446453407508, -6.2774385622041925e+066,  -6.2774385622041925e+066,  "Y",  0}, {
	 4.5173852573018083,  4.0945757997218362, -6.2774385622041925e+066,  -6.2774385622041925e+066,
	 "i++;",   0}, { 4.4951321279554941,  3.2823365785813632,
	-6.2774385622041925e+066,  -6.2774385622041925e+066,  "sum+=i;",  0}, {
	 4.2837273991655076,  2.4144645340751048, -6.2774385622041925e+066,  -6.2774385622041925e+066,  "Output: sum",
	 0}, { 4.47,  1.624478442280946, -6.2774385622041925e+066,  -6.2774385622041925e+066,
	 "end",  0x0} };

char code1[] = "\
int main(){\n\
    int i = 0, sum = 0;\n\
    for (; i < 10; i++) {\n\
        sum += i;\n\
    }\n\
    //Output: sum\n\
    return 0;\n\
}";

enum __page { StartPage_, SavePage_, DrawPage_, HelpPage_, ExamplePage_, LoadPage_, LastPage_ }; //ҳ��ö�ٱ��������ڷ��غ���ֵ

/*��������*/
void Example_Time_Process(int timerID);
int Display_Example();

double TurnAngle(double bx, double by, double ex, double ey);
double distance(double bx, double by, double ex, double ey);

void ProcessInitial_1();
void Example_Time_Process_1(int timerID);
int ExampleDisplay_1();

void startTimer(int id, int timeinterval);
void cancelTimer(int timerID);
static void ShowCode(double x, double y, char* code);
static void Object_ArrayToChain(Link head, int n, double x, double y);
static void TextBox_ArrayToChain(text* head, int n, double x, double y);
static void _showmytext(text* Mytext);
static void _drawlines_(MyLine* lines, int cur1);
static void Line_Adjust(MyLine* head, int cur, double dx, double dy);

/*����ʵ��*/

/*
 * ������: ProcessInitial_1
 * �ӿ�: ProcessInitial_1();
 * ------------------------
 * �ú���Ϊʾ��1������
 * ��ʼ����������Ҫ����
 * ��ͼ��������ı�������
 * ת��Ϊ��������������
 * �ṹ������ֵ
 */
void ProcessInitial_1() {
	Object_ArrayToChain(head1, sizeof(head1) / sizeof(head1[0]),0,-1);
	TextBox_ArrayToChain(thead1, sizeof(thead1) / sizeof(thead1[0]),0,-1);
	Line_Adjust(lines1,cur1,0,-1);
}

/*
 * ������: Line_Adjust
 * �ӿ�: Line_Adjust(head, cur, dx,  dy)
 * ------------------------
 * �ú���Ϊ������������
 * �������������������
 * Ϊÿһ���߶�Ԫ�ؽ���
 * ����ƽ��
 */
static void Line_Adjust(MyLine* head, int cur, double dx, double dy) {
	int i;
	for (i = 0; i < cur; i++) {
		head[i].bx += dx;
		head[i].ex += dx;
		head[i].by += dy;
		head[i].ey += dy;
	}
}

/*
 * ������: Object_ArrayToChain
 * �ӿ�: Object_ArrayToChain(head,n,x,y)
 * ------------------------
 * �ú���Ϊ����ͼ�νṹ
 * �������������������
 * Ϊÿһ��Ԫ�ؽ�������
 * ƽ�ƣ������ṹ������
 * ������Ϊ����
 */
static void Object_ArrayToChain(Link head, int n,double x,double y) {
	int i;
	for (i = 0; i < n - 1; i++) {
		head[i].next = &head[i + 1];
		head[i].cx += x;
		head[i].cy += y;
	}
	head[i].cx += x;
	head[i].cy += y;
	head[i].next = NULL;
}

/*
 * ������: Object_ArrayToChain
 * �ӿ�: Object_ArrayToChain(head,n,x,y)
 * ------------------------
 * �ú���Ϊ�ı��ṹ
 * �������������������
 * Ϊÿһ��Ԫ�ؽ�������
 * ƽ�ƣ������ṹ������
 * ������Ϊ����
 */
static void TextBox_ArrayToChain(text* head, int n,double x, double y) {
	int i;
	for (i = 0; i < n - 1; i++) {
		head[i].next = &head[i + 1];
		head[i].cx += x;
		head[i].cy += y;
	}
	head[i].next = NULL;
	head[i].cx += x;
	head[i].cy += y;
}

/*
 * ������: _showmytext
 * �ӿ�: _showmytext(Mytext);
 * ------------------------
 * �ú���Ϊ�ı���ʾ����,
 * �����ı�����򵥵���
 * �����֡�
 */
static void _showmytext(text* Mytext) {
	text* p = Mytext;
	while (p) {
		drawLabel(p->cx, p->cy, p->s);
		p = p->next;
	}
}

/*
 * ������: Example_Time_Process_1
 * �ӿ�: Example_Time_Process_1(timerID);
 * ------------------------
 * �ú���Ϊʾ��1�Ķ���
 * ��ʾ���������ڻص�����
 * ���ݸ����Ĺ������ض�
 * ��ͼ�θı���ɫ��
 */
void Example_Time_Process_1(int timerID) {
	if (timerID == TIME_ANIMA) {
		static int times, order, i = 0;

		times++;

		if (times != 1) {
			head1[order].color = black;
			head1[order].filled = 0;
		}

		if (order == 2 || order == 1 || order == 4 || order == 6 || order == 0) order++;
		else if (order == 5) order = 3;
		else if (order == 3) {
			if (i < 10) order = 4;
			else order = 6;
		}
		else if (order == 7) {
			cancelTimer(TIME_ANIMA);
			times = 0;
			order = 0;
			i = 0;
			return;
		}

		if (order == 4) i++;
		head1[order].color = Red;
		head1[order].filled = 1;
		//printf("times=%d order=%d i=%d\n", times, order, i);  //���������
	}
}

/*
 * ������: _drawlines_
 * �ӿ�: _drawlines_(lines, cur1);
 * ------------------------
 * �ú���Ϊ�����߻���
 * ��������������������
 * �ж��Ƿ���Ƽ�ͷ
 */
static void _drawlines_(MyLine* lines, int cur1) {
	for (int i = 0; i < cur1; i++) {
		if (lines[i].end) {
			DrawArrow(lines[i].bx, lines[i].by, TurnAngle(lines[i].bx, lines[i].by, lines[i].ex, lines[i].ey), distance(lines[i].bx, lines[i].by, lines[i].ex, lines[i].ey));
		}
		else
		{
			MovePen(lines[i].bx, lines[i].by);
			DrawLine(lines[i].ex - lines[i].bx, lines[i].ey - lines[i].by);
		}
	}
}

/*
 * ������: TurnAngle
 * �ӿ�: TurnAngle(bx,by,ex,ey);
 * ------------------------
 * ��������˵���߶�
 * ��x������ɵĽǶȣ�
 * ���ԽǶ��Ʒ���ֵ
 */
double TurnAngle(double bx, double by, double ex, double ey) {
	double angle = atan((ey - by) / (ex - bx));
	return angle / pi * 180;
}

/*
 * ������:distance
 * �ӿ�: distance(bx,by,ex,ey);
 * ------------------------
 * ��������˵���߶εĳ���
 */
double distance(double bx, double by, double ex, double ey) {
	return sqrt(pow(by - ey, 2) + pow(bx - ex, 2));
}

/*
 * ������: ExampleDisplay_1
 * �ӿ�: ExampleDisplay_1();
 * ------------------------
 * ʾ��1����ʾ����������
 * ��ʼ�����ܺʹ�����ʾ��
 * ����ͼ��ʾ���ܡ�
 */
int ExampleDisplay_1() {
	static int Back=1,New=1,CODE_;
	if (Back == 1) {
		Back = 0;
	}

	if (New == 1) {
		New = 0;
		ProcessInitial_1();
	}

	double winx = GetWindowWidth();
	double winy = GetWindowHeight();
	int flag;

	DrawMenu_(winx, winy);

	flag = GetExampleSelection();
	switch (flag) {
	case 11: 
		cancelTimer(TIME_ANIMA);
		Back = 1;
		break;
	case 21:
		startTimer(TIME_ANIMA, TIME_INTERNAL);
		break;
	case 31: 
		CODE_ ^= 1;
		break;
	case 51:
		startTimer(TIME_ANIMA, TIME_INTERNAL);
		break;
	}

	DrawList(head1);
	_drawlines_(lines1, cur1);
	_showmytext(thead1);
	if(CODE_) ShowCode(8, 6, code1);
	
	return Back;
}

/*
 * ������: ShowCode
 * �ӿ�: ShowCode(x, y, code);
 * ------------------------
 * ������ʾ����������
 * ����������ʹ�����
 * ����Ӧ����ʾ�����룬
 * ���������еĻ��з�
 * �ͽ�����Ӧ�Ļ��С�
 */
static void ShowCode(double x, double y, char* code) {
	double Y = y;
	int i = 0, j = 0;
	while (code[i] != '\0') {
		if (code[i] == '\n') {
			code[i] = '\0';
			drawLabel(x, Y, &code[j]);
			Y -= .3;
			j = i + 1;
			code[i] = '\n';
		}
		i++;
	}
	drawLabel(x, Y, &code[j]);
}

/*
 * ������: Example_Time_Process
 * �ӿ�: Example_Time_Process(int timerID);
 * ------------------------
 * �ú���Ϊ�ܵ�ʾ������
 * ��ʾ���������ڻص�����
 * ���ݸ����Ĺ������ض�
 * ��ͼ�θı���ɫ��
 */
void Example_Time_Process(int timerID) {
	Example_Time_Process_1(timerID);
}

/*
 * ������: Display_Example
 * �ӿ�: Display_Example();
 * ------------------------
 * �ܵ���ʾ����������
 * ��ʼ�����ܺʹ�����ʾ��
 * ����ͼ��ʾ���ܡ�����
 * �Ӻ�������ֵ�����Ƿ�
 * ���ؼ�������ҳ��ö��
 * ֵ�򷵻�����ֵ������
 * ת
 */
int Display_Example() {

	if (ExampleDisplay_1()) return LastPage_;

	else return ExamplePage_;
}