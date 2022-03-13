#ifndef _draw_h
#define _draw_h

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stddef.h>
#include <assert.h>
#include "graphics.h"
#include "genlib.h"
#include "conio.h"

#include <windows.h>
#include <olectl.h>
#include <stdio.h>
#include <mmsystem.h>
#include <wingdi.h>
#include <ole2.h>
#include <ocidl.h>
#include <winuser.h>

#define pi 3.1415926539
#define dot 0.07

#define white 0
#define Red 1
#define Green 2
#define Blue 3
#define black 4
#define yellow 5
#define pupple 6
#define pink 7
#define orange 8
#define cyan 9
#define deepgreen 10
#define gray 11

#define _TEXT 1
#define LINE 2
#define GRAPH 3
#define FREE 4

#define left 1
#define right 2
#define down 3
#define up 4
#define xaxs 5
#define yaxs 6
#define ROTATE 7
#define Textzoom 1
#define Textmove 2


//��������

static char fontstyle[][20] = { "����","source code pro","consolas","����_GB2312" };
//�����

static double Angle;
//ȫ�ֽǶ�
static int clinecolor;
//��������ɫ
static char scolor[20][10] = { "white","red","green","blue","black" ,"yellow",
				"pupple" ,"pink","orange","cyan","deepgreen","gray" };
//��ɫ��


typedef struct Line {
	//˫������
	double bx, by;
	//��ʼλ��
	double ex, ey;
	//����λ��
	int dir;
	//����
	struct Line* next;
	//��һ��
	struct Line* before;
	//��һ��
}line;

typedef struct ConnectLine {
	//��ʼ
	line* begin;
	////�м�
	//line* middle;                //��֪����
	//����
	line* end;
	//����������
	int num;
	//��ɫ
	//��С
	//��ʵ
	//�Ƿ����
	int color, size, solid, erase;
	//��ʼ����
	//��������
	int blineangle, elineangle;

	struct ConnectLine* next;
}cline;

typedef struct graphics {
	//ͼ��ģʽ
	int n;
	//ͼ���С�����αհ���
	double width, height;
	//��ת����
	double angle;
	//����λ��
	double cx, cy;
	//��ɫ
	int color;
	//��С
	int pensize;
	//�߿�
	int frame;
	//���
	int filled;
	//��һ��
	struct graphics* next;
	////������
	//cline* cl;
	////����ͼ��
	//struct graphics* connect;
}node, * Link;

typedef struct Textbox {
	//���Ͻ�λ��
	double cx, cy;
	//��С
	double width, height;
	//�ı�
	char s[100];
	int show; //��ʾ��� showcuror
	int blink; //�����˸
	int position; //���λ��
	int linenum; //ÿ���ַ���
	int precolor; //�趨��ɫ
	int color; //��ǰ��ɫ
	int fontstyle; //����
	struct Textbox* next; //��һ��
}text;

void ChangeLine();

void forward(double distance);

void move(double distance);

void imLine(double dis);

void Drawline(int flag, double dis);

void Draw(node *g, int isDraw);

void DrawArrow(double bx, double by, double angle, double length);

void DrawFrame(node* g, int color);

void Drawedge(node *g);

void Lines_Draw(cline* LineHead, cline* CurLine);

double GetAngle(double ax, double ay, double bx, double by);

void Liner(cline *l,cline* CurLine);

int Judge(int dir, line *pre, double x, double y);

line * AddLine(int dir, line *pre, double x, double y);

void Arrow(int dir, line *pre, double prex, double prey, double x, double y, int erase);

void Fill(node *p);

void DrawTextEdge(text *t);

#endif
