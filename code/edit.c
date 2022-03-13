#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stddef.h>
#include "graphics.h"
#include "genlib.h"
#include "conio.h"
#include "draw.h"
#include "menu.h"

#include <windows.h>
#include <olectl.h>
#include <stdio.h>
#include <mmsystem.h>
#include <wingdi.h>
#include <ole2.h>
#include <ocidl.h>
#include <winuser.h>
#include <assert.h>
#include <edit.h>

//����������������������������������������������������������������ͼ�κ���
/*
 * ������: Zoom
 * �ӿ�: Zoom(node *p, int dir, double x, double y)
 * -------------------------------------------------
 * ���ŵ�ǰͼ��
 */
void Zoom(node* p, int dir, double x, double y) {

	double r = sqrt((p->cx - x) * (p->cx - x) + (p->cy - y) * (p->cy - y));
	//�����ľ���
	double angle = GetAngle(p->cx, p->cy, x, y) - p->angle / 180 * pi;
	//���������߽Ƕ�
	double w = fabs(r * cos(angle));
	//�ĺ�Ŀ�
	double h = fabs(r * sin(angle));
	//�ĺ�ĸ�

	//�ж����ŷ���
	if (dir == down) {
		p->height = (h - 0.09) * 2;
	}
	else {
		p->width = (w - 0.09) * 2;
	}

}

/*
 * ������: Move
 * �ӿ�: Move(node *p, double prex, double prey, double x, double y)
 * -------------------------------------------------
 * �ƶ���ǰͼ��
 */
void Move(node* p, double prex, double prey, double x, double y) {
	//����������λ�øĳ�
	p->cx += x - prex;
	p->cy += y - prey;
}

/*
 * ������: Rotate
 * �ӿ�: Rotate(node *p, double x, double y)
 * -------------------------------------------------
 * ��ת��ǰͼ��
 */
void Rotate(node* p, double x, double y) {
	//���ݽǶ���ת
	p->angle = GetAngle(p->cx, p->cy, x, y) / pi * 180 - 90;
}

/*
 * ������: Copy
 * �ӿ�: Copy(node *p)
 * -------------------------------------------------
 * ���Ƶ�ǰͼ��
 */
node* Copy(node* p) {
	//�����ڴ�
	node* pt = (node*)malloc(sizeof(node));
	//���Ƶ�ǰͼ����������
	pt->n = p->n;
	pt->width = p->width;
	pt->height = p->height;
	pt->angle = p->angle;
	pt->cx = p->cx+0.16;
	pt->cy = p->cy+0.16;
	pt->pensize = p->pensize;
	pt->color = p->color;
	pt->frame = p->frame;
	pt->filled = p->filled;
	pt->angle = 0;
	return pt;
}

/*
 * ������: Create
 * �ӿ�: Create(node *head, node *t)
 * -------------------------------------------------
 * �����Ѹ���ͼ��
 */
void Create(node* head, node* t) {
	node* p = head;
	//��������
	while (p->next) {
		p = p->next;
	}
	p->next = t;
	t->next = NULL;
}

void AddCline(cline* head, cline* new) {
	if (head&&new) {
		new->next = head->next;
		head->next = new;
	}
}

/*
 * ������: CreateNew
 * �ӿ�: CreateNew(node *head, node *t)
 * -------------------------------------------------
 * �����Ѹ���ͼ�ε�����
 */
node* CreateNew(node* prehead, int n) {
	//�����ڴ�
	node* pt = (node*)malloc(sizeof(node));
	node* temp = prehead;

	//������ͬ����
	pt->n = n;
	pt->width = 2.6;
	pt->height = 1.8;
	pt->angle = 0;
	pt->cx = 4;
	pt->cy = 3;
	pt->pensize = 1;
	pt->color = black;
	pt->frame = 0;
	pt->filled = 0;
	pt->next = NULL;

	while (temp->next) {
		temp = temp->next;
	}
	temp->next = pt;

	//���ش����Ľṹ��
	return pt;
}

/*
 * ������: Trveal
 * �ӿ�: Trveal(cline *cl, double x, double y)
 * -------------------------------------------------
 * ��Ѱ�뵱ǰ������������ͼ��
 */
line* Trveal(cline* cl, double x, double y) {
	if (cl == NULL)return NULL;

	line* temp = cl->begin;
	double cx, cy, w, h;;

	//Ѱ�������ߵ�ͷ�ڵ�
	//���ݷ����жϽڵ�
	while (temp != NULL) {
		if (temp->dir == xaxs) {
			cx = (temp->bx + temp->ex) / 2;
			cy = temp->by;
			w = fabs(temp->bx - temp->ex) - 0.2;
			h = 0.3;
		}
		else {
			cy = (temp->by + temp->ey) / 2;
			cx = temp->bx;
			h = fabs(temp->by - temp->ey) - 0.2;
			w = 0.3;
		}

		//���������
		if (InRect(cx, cy, w, h, x, y)) {
			return temp;
		}

		temp = temp->next;
	}
	return NULL;
}

/*
 * ������: Search
 * �ӿ�: Search(node *lhead, double x, double y)
 * -------------------------------------------------
 * ����������Ѱͼ��
 */
node* Search(node* lhead, double x, double y) {
	node* pt = lhead;
	//��������
	while (pt) {
		if (InRegion(pt->cx, pt->cy, pt->width, pt->height, x, y, pt->angle)) {
			//���ص�ǰͼ��
			return pt;
		}
		pt = pt->next;
	}

	return NULL;
}

/*
 * ������: Delete
 * �ӿ�: Delete(node *head, node *p)
 * -------------------------------------------------
 * ɾ����ǰͼ��
 */
node* Delete(node* head, node* p) {
	if (p == NULL)return NULL;
	//��������ɾ��
	node* t = head;
	while (t->next != p) {
		t = t->next;
	}
	t->next = p->next;
	//�ͷ��ڴ�
	free(p);
	return t;
}

/*
 * ������: JudgeEdge
 * �ӿ�: JudgeEdge(node *p, double x, double y)
 * -------------------------------------------------
 * ���ͼ�ζ˵�
 */
int JudgeEdge(node* p, double x, double y) {
	if (p == NULL)return 0;
	//���ݲ�ͬλ�÷��ز�ͬ����
	double w = p->width / 2, h = p->height / 2;
	if (InArc(p->cx, p->cy, x, y, .4, p->angle, w + 0.06)) { //ԭw+0.06,���¶�����
		//�ҷ���
		return right;
	}
	if (InArc(p->cx, p->cy, x, y, .4, p->angle + 180, w + 0.09)) {
		//����
		return left;
	}
	if (InArc(p->cx, p->cy, x, y, .4, p->angle + 270, h + 0.09)) {
		//�·���
		return down;
	}
	if (InArc(p->cx, p->cy, x, y, .5, p->angle + 90, h + 0.08)) {  //ԭh+0.15
		//�Ϸ���
		return ROTATE;
	}
	return 0;
}

//���������������������������������������������������������������������ߺ���
/*
 * ������: updateline
 * �ӿ�: updateline(node *p)
 * -------------------------------------------------
 * ����������
 */

/*
 * ������: DrawList
 * �ӿ�: DrawList(node *head)
 * -------------------------------------------------
 * ����ͼ������
 */
void DrawList(node* head) {
	node* p = head->next;
	while (p) {
		//��������
		if (p->filled) {
			//�Ƿ����
			Fill(p);
			/*p = p->next;
			continue;*/
		}
		else Draw(p, 1);//��ͼ��

		p = p->next;
	}
}

void MoveWholeLine(cline* LineHead, double dx, double dy) {
	line* temp = LineHead->begin;
	while (temp) {
		temp->bx += dx;
		temp->ex += dx;
		temp->by += dy;
		temp->ey += dy;
		temp = temp->next;
	}
}

void Converse(cline *t, cline *p) {
	line *pl = p->end;
	line *tl = (line *)malloc(sizeof(line));
	t->begin = tl;
	while (pl) {

		tl->bx = pl->ex;
		tl->by = pl->ey;
		tl->ex = pl->bx;
		tl->ey = pl->by;
		tl->dir = pl->dir;
		tl->before = pl->next;
		tl->next = pl->before;

		pl = pl->before;
		tl = tl->next;
	}
}


/*
 * ������: TrvealLine
 * �ӿ�: TrvealLine(node *prehead, double x, double y)
 * -------------------------------------------------
 * ����������Ѱѡ��������
 */
cline* TrvealLine(cline* LineHead, double x, double y) {
	cline* temp = LineHead->next;
	//��������
	while (temp) {
		if (Trveal(temp, x, y)) {
			//���ص�ǰ������ͼ��
			return temp;
		}
		temp = temp->next;
	}
	return NULL;
}

/*
 * ������: MoveLine
 * �ӿ�: MoveLine(line *l, double x, double y)
 * -------------------------------------------------
 * �ƶ�ѡ���ƶ�������
 */
void MoveLine(line* l, double x, double y) {
	if (l == NULL)return;
	//x����
	if (l->dir == xaxs) {
		l->by = y;
		l->ey = y;
		//ǰ��ڵ��λ�ö�Ҫ��Ӧ�ı�
		if (l->before != NULL) {
			l->before->ey = y;
		}
		if (l->next != NULL) {
			l->next->by = y;
		}
	}
	else {
		//y����
		l->bx = x;
		l->ex = x;
		//ǰ��ڵ��λ�ö�Ҫ��Ӧ�ı�
		if (l->before != NULL) {
			l->before->ex = x;
		}
		if (l->next != NULL) {
			l->next->bx = x;
		}
	}
}

/*
 * ������: ZoomLine
 * �ӿ�: ZoomLine(line *l, double x, double y)
 * -------------------------------------------------
 * ����ѡ��������
 */
void ZoomLine(line* l, double x, double y) {
	//�жϷ���������Ӧ�ı�
	if (l->dir == xaxs) {
		l->ex = x;
	}
	else {
		l->ey = y;
	}
}

/*
 * ������: SearchLine
 * �ӿ�: SearchLine(node *prehead, double x, double y)
 * -------------------------------------------------
 * Ѱ��ѡ������������
 */
cline* SearchLine(cline* LineHead, double x, double y) {
	cline* temp = LineHead->next;
	while (temp) {
		line* l = temp->end;
		//�ж���ѡ��
		if (/*l->ey < l->by &&*/ InArc(l->ex, l->ey , x, y, 0.2, 0, 0)) {  //��
			return temp;
		}
		//else {
		//	if (InArc(l->ex, l->ey + 0.05, x, y, 0.06, 0, 0))
		//		//���ؽṹ��
		//		return temp;
		//}

		temp = temp->next;
	}
	return NULL;
}


//���������������������������������������������������������������������������㣬��⡭����
/*
 * ������: Commute
 * �ӿ�: Commute(double x, double y, double x0, double y0, double k)
 * -------------------------------------------------
 * ������߾���
 */
double Commute(double x, double y, double x0, double y0, double k) {
	//���ݵ㵽ֱ�߾��빫ʽ�������
	// y = kx + b
	// (x,y)
	double d = fabs((y - y0 + k * (x0 - x)));
	double t = sqrt(k * k + 1);
	double dis = d / t;
	return dis;
}


/*
 * ������: InRect
 * �ӿ�: InRect(double cx, double cy, double w, double h, double x, double y)
 * -------------------------------------------------
 * ����������
 */
int InRect(double cx, double cy, double w, double h, double x, double y) {
	//�����������ľ����Ƿ���
	if (fabs(x - cx) <= w / 2 && fabs(y - cy) <= h / 2) {
		return 1;
	}
	return 0;
}

/*
 * ������: InRegion
 * �ӿ�: InRegion(double cx, double cy, double w, double h, double x, double y, double angle)
 * -------------------------------------------------
 * ���Ǿ�������
 */
int InRegion(double cx, double cy, double w, double h, double x, double y, double angle) {
	int a = angle;
	if (a % 90 == 0) {
		//tan(90)�����ڣ���Ҫ��������
		return InRect(cx, cy, w, h, x, y);
	}
	else {
		//���ѡ����ת���ͼ��
		//ͨ���Ƕȵ�����ֵ
		double k1 = tan(angle / 180 * pi);
		double k2 = -1 / k1;
		double dh = Commute(cx, cy, x, y, k1);
		double dw = Commute(cx, cy, x, y, k2);
		//�������ĵ�ƽ���߼��
		if (dh < h / 2 && dw < w / 2)return 1;
	}
	return 0;
}

/*
 * ������: InArc
 * �ӿ�: InArc(double cx, double cy, double x, double y, double r, double angle, double d)
 * -------------------------------------------------
 * ���Բ������
 */
int InArc(double cx, double cy, double x, double y, double r, double angle, double d) {
	//�������
	double dx = d * cos(angle / 180 * pi);
	double dy = d * sin(angle / 180 * pi);
	x -= dx;
	y -= dy;
	//�жϾ����Ƿ���ڰ뾶
	double dis = sqrt((x - cx) * (x - cx) + (y - cy) * (y - cy));
	if (dis > r)return 0;
	else return 1;
}

//�����������������������������������������������������������������ı�����
/*
 * ������: AddText
 * �ӿ�: AddText(text *pretext, double cx, double cy)
 * -------------------------------------------------
 * �������ı���
 */
text* AddText(text* pretext, double cx, double cy) {
	//�����ڴ�
	text* t = (text*)malloc(sizeof(text));
	//���������ı�
	*t = sample;
	t->cx = cx;
	t->cy = cy;

	text* temp = pretext;
	//��������
	while (temp->next) {
		temp = temp->next;
	}
	temp->next = t;
	return t;
}

/*
 * ������: DeleteText
 * �ӿ�: DeleteText(text *pretext, text *tt)
 * -------------------------------------------------
 * ɾ����ǰ�ı���
 */
text* DeleteText(text* pretext, text* tt) {
	if (tt == NULL)return NULL;
	text* t = pretext;
	while (t->next != tt) {
		//ɾ����ǰ�ڵ�
		t = t->next;
		//if (t == NULL)return NULL;
	}
	t->next = tt->next;
	//�ͷ��ڴ�
	free(tt);
	return t;
}

/*
 * ������: SearchText
 * �ӿ�: SearchText(text *pretext, double x, double y)
 * -------------------------------------------------
 * ��Ѱ��ǰ�ı���
 */
text* SearchText(text* pretext, double x, double y) {
	text* t = pretext->next;
	double cx, cy;
	while (t) {
		cx = t->cx + t->width / 2;
		cy = t->cy - t->height / 2;
		//�ж��Ƿ��ھ����ı�����
		if (InRect(cx, cy, t->width + 0.1, t->height + 0.1, x, y))
			return t;
		t = t->next;
	}
	//�������򷵻ؿ�ָ��
	return NULL;
}

/*
 * ������: JudgeText
 * �ӿ�: JudgeText(text *t,double x, double y)
 * -------------------------------------------------
 * ����ƶ���������
 */
int JudgeText(text* t, double x, double y) {
	//��ȡ����
	double cx = t->cx + t->width / 2;
	double cy = t->cy - t->height / 2;
	if (!InRect(cx - 0.03, cy + 0.03, t->width - 0.06, t->height - 0.06, x, y) && InRect(cx + 0.03, cy - 0.03, t->width + 0.06, t->height + 0.06, x, y))
	{
		//�ڲ�������ִ���ƶ�����
		if (fabs(x - t->cx - t->width) <= 0.3)
			return xaxs;
		else return yaxs;
	}
	//�ⲿ������ִ�����Ų���
	else return Textmove;
}

/*
 * ������: JudgeChinese
 * �ӿ�: JudgeChinese(text *t)
 * -------------------------------------------------
 * �жϺ���
 */
int JudgeChinese(text* t) {
	int k = 0;
	for (int i = 0; i <= t->position; i++) {
		if (t->s[i] < 0)k++;
	}
	return k % 2;
}

/*
 * ������: MoveText
 * �ӿ�: MoveText(text *t, double prex, double prey, double x, double y)
 * -------------------------------------------------
 * �ƶ��ı���
 */
void MoveText(text* t, double prex, double prey, double x, double y) {
	//�������λ���ƶ��ı���
	t->cx += x - prex;
	t->cy += y - prey;
}

/*
 * ������: ZoomText
 * �ӿ�: ZoomText(text *t, double x, double y, int dir)
 * -------------------------------------------------
 * �����ı���
 */
int ZoomText(text* t, double x, double y, int dir) {
	//�ж��ƶ�����
	if (dir == xaxs) {
		//���ı����С����ʾ
		if (x - t->cx > 0.3) {
			t->width = x - t->cx;
			return 0;
		}
	}
	else {
		//�������λ��ȷ�����ų̶�
		if (t->cy - y > 0.24) {
			t->height = t->cy - y;
			return 0;
		}
	}
	return 1;
}

/*
 * ������: DisplayText
 * �ӿ�: DisplayText(text *pretext, text *tt)
 * -------------------------------------------------
 * �����ı���
 */
void DisplayText(text* pretext, text* tt) {
	text* t = pretext->next;

	while (t) {
		//������Ƥ��ģʽ
		SetEraseMode(0);

		//�����ı�
		DrawString(t);

		t = t->next;
	}

	//���Ƶ�ǰ�ı���ı߿�
	if (tt)DrawTextEdge(tt);
}


/*
 * ������: DrawString
 * �ӿ�: DrawString(text *t)
 * -------------------------------------------------
 * �����ı�
 */
void DrawString(text* t) {
	//���õ�ǰ������ɫ
	SetPenColor(scolor[t->color]);

	int k = 1, j = 0, i = 0, flag = 0;
	int l = strlen(t->s);
	char str[10] = "a";
	//ʹ�õȿ�����
	//��ȡÿ���ַ�����
	double w = TextStringWidth(str);
	t->linenum = (t->width - 0.12) / w + 1;
	//ʹ����ʱ������л���
	char temp[5] = "";


	//���
	if (t->position < 0) {
		if (t->show) {
			if (t->width >= 0.2 && t->height >= 0.2)
				Cursor(t, 0.0, k);
		}
	}

	while (i < l) {
		//�����ַ��ɶԴ���
		if (t->s[i] < 0 && t->s[i + 1] < 0) {
			temp[0] = t->s[i++];
			temp[1] = t->s[i];
			temp[3] = 0;
			//λ��ĩβ
			if (j == t->linenum - 1) {
				k++;
				j = 2;
				MovePen(t->cx + 0.02, t->cy - k * 0.22);
				if(temp) DrawTextString(temp);
			}
			//����λ��
			else {
				MovePen(t->cx + 0.02 + j * w, t->cy - k * 0.22);
				if (temp) DrawTextString(temp);
				j += 2;
				if (j == t->linenum) {
					j = 0;
					k++;
				}
			}
		}
		//�Ǻ����ַ�
		else {
			temp[0] = t->s[i];
			temp[1] = 0;
			MovePen(t->cx + 0.02 + j * w, t->cy - k * 0.22);
			if (temp) DrawTextString(temp);
			j += 1;
			if (j == t->linenum) {
				j = 0;
				k++;
			}
		}
		//���
		if (i == t->position) {
			if (t->show) {
				if (t->width >= 0.2 && t->height >= 0.2)
					Cursor(t, j * w, k);
			}
		}
		i++;
	}

}

/*
 * ������: Cursor
 * �ӿ�: Cursor(text *t)
 * -------------------------------------------------
 * ���ƹ��
 */
void Cursor(text* t, double w, int k) {
	//��˸��־
	SetEraseMode(t->blink);
	//��ɫΪ��ɫ
	SetPenColor(scolor[Blue]);
	//��СΪ2
	SetPenSize(2);

	//����ʾ���ַ���֮����ʾ���
	MovePen(t->cx + 0.04 + w, t->cy - k * 0.22 - 0.02);
	DrawLine(0, 0.2);
	//�Ļػ���ģʽ
	SetEraseMode(0);
	SetPenColor(scolor[t->color]);
}

/*
 * ������: AddChar
 * �ӿ�: AddChar(text *t, char key)
 * -------------------------------------------------
 * ����ַ�
 */
void AddChar(text* t, char key) {
	int l = strlen(t->s);
	//��ǰλ��+1
	t->position += 1;
	//ѭ������
	for (int i = l; i > t->position; i--) {
		t->s[i] = t->s[i - 1];
	}

	//��ǰλ�ò����ַ�
	t->s[t->position] = key;
	//ĩβ��0
	t->s[l + 1] = 0;

}

/*
 * ������: DeleteCha
 * �ӿ�: DeleteChar(text *t)
 * -------------------------------------------------
 * ɾ���ַ�
 */
void DeleteChar(text* t) {
	//�Ѵ�����ǰ�����޷�ɾ��
	if (t->position < 0)return;
	int l = strlen(t->s);
	int flag = 0;
	//�ж��Ƿ��������ַ�
	if (t->s[t->position] < 0) flag = 1;

	//��ǰλ��ѭ�����ɾ��һ���ַ�
	for (int i = t->position; i < l - 1; i++) {
		t->s[i] = t->s[i + 1];
	}
	//��ǰλ��-1
	t->position -= 1;
	l -= 1;

	//�������ģ���ɾ��һ���ַ�
	if (flag) {
		MovePen(0, 0);
		DrawLine(2, 2);
		for (int i = t->position; i < l - 1; i++) {
			t->s[i] = t->s[i + 1];
		}
		t->position -= 1;
		l -= 1;
	}
	//ĩβ��0
	t->s[l] = 0;
}


text* CopyText(text* t) {
	text* pt = (text*)malloc(sizeof(text));
	pt->blink = 0;
	pt->color =t->color;
	pt->show = 0;
	pt->cx = t->cx + 0.16;
	pt->cy = t->cy - 0.16;
	pt->fontstyle = t->fontstyle;
	pt->width = t->width;
	pt->height = t->height;
	pt->linenum = t->linenum;
	strcpy(pt->s, t->s);
	pt->position = strlen(t->s) - 1;
	pt->precolor = t->precolor;
	pt->next = NULL;
	return pt;
}

void PasteText(text* pretext, text* t) {
	text* temp = pretext;
	while (temp->next) {
		temp = temp->next;
	}
	temp->next = t;
	t->next = NULL;
}

cline* CopyLine(cline* l) {
	if (!l) return NULL;
	cline* newone = (cline*)malloc(sizeof(cline));
	assert(newone);
	*newone = *l;

	line* last = NULL, * p = NULL,*lp=l->begin;

	newone->begin = NULL;

	for (int j = 0; j < newone->num; j++) {
		p = (struct Line*)malloc(sizeof(struct Line));
		assert(p);

		*p = *lp;

		p->bx += 0.16;
		p->by += 0.16;
		p->ex += 0.16;
		p->ey += 0.16;

		if (j == 0) newone->begin = p;

		if (last)  last->next = p;

		p->before = last;

		last = p;
		lp = lp->next;
	}

	if (last) last->next = NULL;

	newone->end = last;
	newone->next = NULL;

	return newone;
}