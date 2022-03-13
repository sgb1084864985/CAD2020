#include <stdio.h>
#include <stdlib.h>
#include<math.h>
#include <stddef.h>
#include "graphics.h"
#include <assert.h>
#include "genlib.h"
#include "conio.h"
#include "draw.h"

#include <windows.h>
#include <olectl.h>
#include <stdio.h>
#include <mmsystem.h>
#include <wingdi.h>
#include <ole2.h>
#include <ocidl.h>
#include <winuser.h>

/*
 * ������: ChangeLine
 * �ӿ�: ChangeLine()
 * -------------------------------------------------
 * �ı���������ɫ
 */
void ChangeLine() {
	clinecolor ^= 1;
}

/*
 * ������: forward
 * �ӿ�: forward(double distance)
 * -------------------------------------------------
 * ����
 */
void forward(double distance) {
	//���ýǶȻ���
	double dx = distance * cos(Angle / 180 * pi);
	double dy = distance * sin(Angle / 180 * pi);
	DrawLine(dx, dy);
}

void Lines_Draw(cline* LineHead,cline* CurLine) {
	cline* temp = LineHead->next;
	while (temp) {
		Liner(temp,CurLine);
		temp = temp->next;
	}
}

/*
 * ������: move
 * �ӿ�: move(double distance)
 * -------------------------------------------------
 * �����ƶ�
 */
void move(double distance) {
	double dx = distance * cos(Angle / 180 * pi);
	double dy = distance * sin(Angle / 180 * pi);
	//��ȡ���ʵ�ǰλ��
	double x = GetCurrentX();
	double y = GetCurrentY();
	//�ƶ�����
	MovePen(x + dx, y + dy);
}

/*
 * ������: imLine
 * �ӿ�: imLine(double dis)
 * -------------------------------------------------
 * ������
 */
void imLine(double dis) {
	//����
	int n = floor(fabs(dis / dot));
	double onestep = (dis>=0) ? dot : -dot;
	int flag = 1, i;
	//���һ�����뻭��
	for (i = 0; i < n; i++) {
		if (flag) {
			forward(onestep);
			flag = 0;
		}
		else {
			//�㻮��
			move(onestep);
			flag = 1;
		}
	}
}

/*
 * ������: Drawline
 * �ӿ�: Drawline(int flag,double dis)
 * -------------------------------------------------
 * ���ݲ�����ʵ������
 */
void Drawline(int flag, double dis) {
	//��ʵ������
	if (!flag)forward(dis);
	else imLine(dis);
}

/*
 * ������: Draw
 * �ӿ�: Draw(node *g, int isDraw)
 * -------------------------------------------------
 * ���ݽṹ�廭��ָ��ͼ��
 */
void Draw(node* g, int isDraw) {
	//����ģʽ
	SetPenSize(g->pensize);
	SetPenColor(scolor[g->color]);

	//�Ƿ����
	if (isDraw)SetEraseMode(FALSE);
	else SetEraseMode(TRUE);

	double r, a, R, width, height;
	double x = g->cx, y = g->cy;
	width = g->width;
	height = g->height;

	//ѡ������ͼ��
	switch (g->n) {
	case 1:
		//����
		r = sqrt(width * width + height * height) / 2;
		//��תͼ��
		a = g->angle / 180 * pi + pi - atan(height / width);
		MovePen(x + r * cos(a), y + r * sin(a));
		Angle = g->angle;
		Drawline(g->frame, width);
		//˳ʱ����ת
		Angle -= 90;
		Drawline(g->frame, height);
		Angle -= 90;
		Drawline(g->frame, width);
		Angle -= 90;
		Drawline(g->frame, height);
		break;

	case 2:
		//Բ�Ǿ���
		R = height / 4;
		double w = width - 2 * R, h = height;
		r = sqrt(w * w + h * h) / 2;
		a = g->angle / 180 * pi + pi - atan(h / w);
		//����תͼ��
		MovePen(x + r * cos(a), y + r * sin(a));
		//��Բ��
		DrawArc(R, 90 + g->angle, 90);
		Angle = g->angle - 90;
		//Բ�ǰ뾶Ϊ���1/4
		Drawline(g->frame, h - 2 * R);
		//Բ�ǰ뾶Ϊ���1/4
		DrawArc(R, 180 + g->angle, 90);
		Angle += 90;
		Drawline(g->frame, w);
		DrawArc(R, 270 + g->angle, 90);
		//��ʱ����ת��ͼ��
		Angle += 90;
		Drawline(g->frame, h - 2 * R);
		DrawArc(R, 0 + g->angle, 90);
		Angle += 90;
		Drawline(g->frame, w);
		break;

	case 3:
		//����
		r = sqrt(width * width + height * height) / 2;
		a = g->angle / 180 * pi;
		double x1 = width / 2 * cos(a), y1 = width / 2 * sin(a);
		//�����ھ��α߿���
		MovePen(x + x1, y + y1);
		a = atan(height / width) / pi * 180;
		Angle = g->angle + 180 - a;
		//ǰһ�������Ƿ����
		Drawline(g->frame, r);
		Angle += 2 * a;
		//ǰһ�������Ƿ����
		Drawline(g->frame, r);
		Angle += 180 - 2 * a;
		//ǰһ�������Ƿ����
		Drawline(g->frame, r);
		Angle += 2 * a;
		//ǰһ�������Ƿ����
		Drawline(g->frame, r);
		break;
	}
}

/*
 * ������: DrawArrow
 * �ӿ�: DrawArrow(double bx, double by, double angle,  double length)
 * -------------------------------------------------
 * ����ͷ
 */
void DrawArrow(double bx, double by, double angle, double length) {
	//���ýǶ�
	Angle = angle;
	//����ģʽ
	SetPenSize(1);
	//������ɫ
	SetPenColor(scolor[black]);
	MovePen(bx, by);
	//ͼ���
	forward(length);
	double x = GetCurrentX(), y = GetCurrentY();
	//������ͷ������
	Angle += 160;
	forward(0.2);
	Angle -= 320;
	MovePen(x, y);
	forward(0.2);
}

/*
 * ������: DrawFrame
 * �ӿ�: DrawFrame(node *g, int color)
 * -------------------------------------------------
 * ��ͼ�α߿�
 */
void DrawFrame(node *g, int color)
{
	//���߿�
	SetPenSize(1);
	//����ɫ
	SetPenColor(scolor[color]);
	double w = g->width + 0.09, h = g->height + 0.09;

	//���ݽǶȻ����߿�
	MovePen(g->cx, g->cy);
	Angle = g->angle + 90;
	move(h / 2);
	//�Ƕ�˳ʱ����ת90
	Angle -= 90;
	Drawline(g->frame, w / 2);
	//�Ƕ�˳ʱ����ת90
	Angle -= 90;
	Drawline(g->frame, h);
	//�Ƕ�˳ʱ����ת90
	Angle -= 90;
	Drawline(g->frame, w);
	//�Ƕ�˳ʱ����ת90
	Angle -= 90;
	Drawline(g->frame, h);
	//�Ƕ�˳ʱ����ת90
	Angle -= 90;
	Drawline(g->frame, w / 2);
}

/*
 * ������: Drawedge
 * �ӿ�: Drawedge(node *g)
 * -------------------------------------------------
 * ��ͼ��ͼ��
 */
void Drawedge(node* g) {
	//ͼ��
	if (g == NULL)return;
	double h = g->height / 2, w = g->width / 2;
	//��תͼ��
	MovePen(g->cx, g->cy);
	Angle = g->angle + 90;
	move(h);
	SetPenSize(1);
	//��ɫΪ��ɫ
	SetPenColor(scolor[Red]);
	//�����С
	forward(0.12);
	move(0.12);
	DrawArc(0.06, Angle, 360);

	//��ɫ��ɫ
	//����ͼ��
	//����ͼ��
	SetPenColor(scolor[black]);
	Angle -= 90;
	MovePen(g->cx, g->cy);
	//��С
	//����
	//Բ��
	move(w);
	forward(0.05);
	move(0.08);
	DrawArc(0.04, Angle, 360);
	//�·�ͼ��
	Angle -= 90;
	MovePen(g->cx, g->cy);
	move(h);
	//��С
	//����
	//Բ��
	forward(0.05);
	move(0.08);
	DrawArc(0.04, Angle, 360);
	//�ұ�ͼ��
	Angle -= 90;
	MovePen(g->cx, g->cy);
	//��С
	//����
	//Բ��
	move(w);
	forward(0.05);
	move(0.08);
	DrawArc(0.04, Angle, 360);
}

/*
 * ������: GetAngle
 * �ӿ�: GetAngle(double ax, double ay, double bx, double by)
 * -------------------------------------------------
 * ��ȡ��ǰֱ�߽Ƕ�
 */
double GetAngle(double ax, double ay, double bx, double by) {
	//x��������Ļ���ֵ(a->b)
	double x = bx - ax;
	double y = by - ay;
	if (x == 0) {
		//ƽ����y����⿼��
		if (y > 0)return pi / 4;
		else return pi * 0.75;
	}
	//��������������м���
	if (x > 0)return atan(y / x);
	else return atan(y / x) + pi;
}

/*
 * ������: Liner
 * �ӿ�: Liner(cline *l)
 * -------------------------------------------------
 * ������ǰ������
 */
void Liner(cline *l,cline* curline) {
	//����������
	if (l == NULL)return;
	//���û���ģʽ
	SetEraseMode(l->erase);
	SetPenSize(l->size);
	SetPenColor(scolor[l->color]);

	MovePen(l->begin->bx, l->begin->by);
	line *temp = l->begin;
	do {
		//������ʵ����������
		if(l->solid)
			DrawLine(temp->ex - temp->bx, temp->ey - temp->by);
		else {
			if (temp->dir == xaxs) {
				Angle = 0;
				imLine(temp->ex - temp->bx);
			}
			else {
				Angle = 90;
				imLine(temp->ey - temp->by);
			}
		}
		temp = temp->next;
	} while (temp != NULL);

	//���ݷ�����
	if (l->end->dir == xaxs) {
		if (l->end->ex > l->end->bx) {
			if(l==curline) DrawArc(0.05, 180, 360);
			Angle = 160;
			forward(0.2);
			move(-.2);
			Angle -= 320;
			forward(.2);
		}
		else {
			if (l == curline) DrawArc(0.05, 0, 360);
			Angle = 20;
			forward(0.2);
			move(-.2);
			Angle -= 40;
			forward(.2);
		}
	}
	else {
		//����ĩβ�߶�
		if (l->end->ey < l->end->by) {
			if (l == curline) DrawArc(0.05, 90, 360);
			Angle = 70;
			forward(0.2);
			move(-.2);
			Angle += 40;
			forward(.2);
		}
		else {
			if (l == curline) DrawArc(0.05, 270, 360);
			Angle = -70;
			forward(0.2);
			move(-.2);
			Angle -= 40;
			forward(.2);
		}
	}
}

int Judge(int dir, line *pre, double x, double y) {
	//�ж��������ƶ�
	double cx = pre->ex, cy = pre->ey;

	if (dir == yaxs) {
		if (x - cx > 0.2 || cx - x > 0.2)
			return xaxs;
		else return yaxs;
	}
	else {
		if (y - cy > 0.2 || cy - y > 0.2)
			return yaxs;
		else return xaxs;
	}
}

/*
 * ������: AddLine
 * �ӿ�: AddLine(int dir,line *pre, double x, double y)
 * -------------------------------------------------
 * ����������
 */
line* AddLine(int dir, line* pre, double x, double y) {
	//��ȡ�ռ�
	line* l = (line*)malloc(sizeof(line));
	//ǰ�ýڵ�
	l->before = pre;
	pre->next = l;
	//�ṹ�帳ֵ
	l->bx = pre->ex;
	l->by = pre->ey;
	l->ex = x;
	l->ey = y;
	l->dir = dir;
	l->next = NULL;
	return l;
}

/*
 * ������: Arrow
 * �ӿ�: Arrow(int dir, line *pre, double prex, double prey, double x, double y, int erase)
 * -------------------------------------------------
 * ��̬����������
 */
void Arrow(int dir, line* pre, double prex, double prey, double x, double y, int erase) {
	//��̬��
	if (pre == NULL)return;
	//��С
	SetPenSize(1);
	//��ɫ
	SetPenColor("black");

	double cx = pre->ex, cy = pre->ey;

	//����ǰһ��λ�ø���
	//���ݷ�����
	if (dir == xaxs) {
		//�Ƿ����
		SetEraseMode(TRUE);
		//λ��
		MovePen(cx, cy);
		DrawLine(prex - cx, 0);
		SetEraseMode(erase);
		MovePen(cx, cy);
		DrawLine(x - cx, 0);
	}
	else {
		//������
		SetEraseMode(TRUE);
		//λ��
		MovePen(cx, cy);
		DrawLine(0, prey - cy);
		//�Ƿ����
		SetEraseMode(erase);
		MovePen(cx, cy);
		DrawLine(0, y - cy);
	}
}

/*
 * ������: Fill
 * �ӿ�: Fill(node *g)
 * -------------------------------------------------
 * ���ͼ��
 */
void Fill(node* g) {
	//���
	double r, a, R, width, height;
	double x = g->cx, y = g->cy;
	//��ȡ��С
	width = g->width;
	height = g->height;

	//����ܶ�
	StartFilledRegion(0.5);
	SetPenColor(scolor[g->color]);

	//��Draw����һ��
	//����ͼ��
	//�����������
	switch (g->n) {
	case 1:
		//����
		r = sqrt(width * width + height * height) / 2;
		a = g->angle / 180 * pi + pi - atan(height / width);
		MovePen(x + r * cos(a), y + r * sin(a));
		//��ת��������
		Angle = g->angle;
		Drawline(g->frame, width);
		Angle -= 90;
		Drawline(g->frame, height);
		Angle -= 90;
		Drawline(g->frame, width);
		Angle -= 90;
		Drawline(g->frame, height);
		break;

	case 2:
		//Բ�Ǿ���
		R = height / 4;
		double w = width - 2 * R, h = height;
		r = sqrt(w * w + h * h) / 2;
		a = g->angle / 180 * pi + pi - atan(h / w);
		MovePen(x + r * cos(a), y + r * sin(a));
		//����Բ��
		DrawArc(R, 90 + g->angle, 90);
		Angle = g->angle - 90;
		Drawline(g->frame, h - 2 * R);
		DrawArc(R, 180 + g->angle, 90);
		//����Բ��
		Angle += 90;
		Drawline(g->frame, w);
		DrawArc(R, 270 + g->angle, 90);
		Angle += 90;
		Drawline(g->frame, h - 2 * R);
		//����Բ��
		DrawArc(R, 0 + g->angle, 90);
		Angle += 90;
		Drawline(g->frame, w);
		break;

	case 3:
		//����
		r = sqrt(width * width + height * height) / 2;
		a = g->angle / 180 * pi;
		double x1 = width / 2 * cos(a), y1 = width / 2 * sin(a);
		MovePen(x + x1, y + y1);
		a = atan(height / width) / pi * 180;
		//���ݽǶȻ�������
		Angle = g->angle + 180 - a;
		Drawline(g->frame, r);
		Angle += 2 * a;
		Drawline(g->frame, r);
		Angle += 180 - 2 * a;
		Drawline(g->frame, r);
		Angle += 2 * a;
		Drawline(g->frame, r);
		break;
	}
	//�������
	EndFilledRegion();
}

/*
 * ������: DrawTextEdge
 * �ӿ�: DrawTextEdge(text *t)
 * -------------------------------------------------
 * �����ı���
 */
void DrawTextEdge(text* t) {

	MovePen(t->cx, t->cy);
	//��ɫĬ�Ϻ�ɫ
	SetPenColor(scolor[black]);
	SetPenSize(1);

	//��С���ݽṹ��ı�
	Angle = 0;
	imLine(t->width);
	Angle = -90;
	imLine(t->height);
	Angle = 180;
	imLine(t->width);
	Angle = 90;
	imLine(t->height);
}
