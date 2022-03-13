#include "conio.h"

#include <windows.h>
#include <olectl.h>
#include <stdio.h>
#include <mmsystem.h>
#include <wingdi.h>
#include <ole2.h>
#include <ocidl.h>
#include <winuser.h>
#include <stdlib.h>
#include <assert.h>
#include "imgui.h"
#include "edit.h"
#include "draw.h"

/*��������*/
#define false 0
#define true 1

/*���ͱ���*/
typedef _Bool status; //״̬status��Ϊfalse��true����
typedef node* Link;


/*�����ӿ�*/

status Save(char* filename, Link head, text* thead, cline* LineHead, int len, int SaveCopyAs); //���溯���������ֱ�Ϊ���ļ�����������ͷָ�룬�ı�����ͷָ�룬������
status Load(char* filename, Link ptrh, text* ptrth, cline* LineHead);	//��ȡ�����������ֱ�Ϊ���ļ�����ָ��������ͷָ���ָ�룬ָ���ı�����ͷָ���ָ��

int GetListLen(Link head);                                     //��������ĳ���

void Close(Link head, text* texthead,cline* HeadLine);                         //�ƻ�������˳�����

void _Destroy(Link head, text* texthead, cline* LineHead);  //�ƻ�����

void CLine_Destroy(cline* cl);

/*�ڲ��ú���*/

static void TextBox_Destroy(text* head);

static void MainChain_Destroy(Link head);

static void SaveTBox(FILE* fp, text* thead);

static void SaveMainChain(FILE* fp, Link head);

static void SaveLine(FILE* fp, struct ConnectLine* cline);

static text* LoadTBox(FILE* fp);

void AllLines_Destroy(cline* head);

static void AllLines_Save(FILE* fp, cline* head);

static cline* AllLines_Load(FILE* fp);

static struct ConnectLine* LoadLine(fp);

static Link LoadMainChain(FILE* fp, int ListLen);


/*����ʵ��*/

/*
 * ������: Save
 * �ӿ�: Save(filename, head, textboxhead, len, SaveCopyAs);
 * ------------------------
 * �ú�����������ͼ�Σ����Ρ�
 * ���Ρ������ߵ�Ԫ�أ�����
 * ���ı�������д�뵽ָ����
 *���С�
 * ������SaveCopyAsΪ1��
 * Ϊ���Ϊ״̬��������
 * ��ͬһ��ַ��ͬ�ļ�����
 * ���򷵻�false��
 *
 * ��SaveCopyAsΪ0��
 * Ϊ��������״̬,
 * ������֮ǰ��ͬ���ļ���
 * ������ɹ���������true��
 */

status Save(char* filename, Link head, text* thead, cline* LineHead, int len, int SaveCopyAs) {
	if (len == 0) return true;
	if (head && thead && filename) {

		FILE* fp;

		if (SaveCopyAs && (fp = fopen(filename, "rb")) != NULL) {
			fclose(fp);
			return false;
		}

		if ((fp = fopen(filename, "wb")) == NULL)
			return false;

		/*д����������*/
		fwrite(&len, sizeof(int), 1, fp);

		SaveMainChain(fp, head);
		SaveTBox(fp, thead);
		AllLines_Save(fp, LineHead);

		fclose(fp);
		return true;
	}
	else return false;
}

/*
 * ������: SaveTBox
 * �ӿ�: SaveTBox(fp, textboxhead);
 * ------------------------
 * �ú������������ı�������
 * ���䳤�Ȱ���ָ���ļ�ָ��
 * д�뵽�ļ���
 */

static void SaveTBox(FILE* fp, text* thead) {
	text* p = thead->next;

	int len = 0;					   //len ���ڼ�¼������

	long pos = ftell(fp),endpos;		//��pos��¼��ʼλ��

	fwrite(&len, sizeof(int), 1, fp);  //��д��ռ�ݴ���λ��

	while (p != NULL) {
		fwrite(p, sizeof(*p), 1, fp);
		p = p->next;
		len++;                         //�õ�����
	}

	endpos = ftell(fp);    
	fseek(fp, pos, SEEK_SET);            //������ͷ��
	fwrite(&len, sizeof(int), 1, fp);    //д�볤��
	fseek(fp, endpos, SEEK_SET);         //����ĩβ
}

/*
 * ������: SaveSaveLine
 * �ӿ�: SaveLine(FILE* fp, struct ConnectLine* cline);
 * ------------------------
 * �ú���������������������
 * ���������ṹ����ָ���ļ�
 * ָ��д�뵽�ļ���
 */

static void SaveLine(FILE* fp, struct ConnectLine* cline) {
	struct Line* p = cline->begin;

	fwrite(cline, sizeof(*cline), 1, fp);

	while (p != NULL) {
		fwrite(p, sizeof(*p), 1, fp);
		p = p->next;
	}
}

/*
 * ������: SaveMainChain
 * �ӿ�: SaveMainChain(FILE* fp, Link head);
 * ------------------------
 * �ú�����������ͼ��Ԫ����
 * ����ָ���ļ�ָ��д�뵽
 * �ļ���
 */

static void SaveMainChain(FILE* fp, Link head) {
	if (head) {
		Link p = head->next;
		while (p != NULL) {
			fwrite(p, sizeof(*p), 1, fp);
			p = p->next;
		}
	}
}

/*
 * ������: Load
 * �ӿ�: Load(filename, head, textboxhead);
 * ------------------------
 * �ú�����ָ���ļ���ͼ��
 *�����Ρ����Ρ������ߵ�Ԫ
 * �أ����ݺ��ı������ݶ�
 * �뵽�ڴ��в��γ�����
 * �����ӵ������headָ��
 * ��textboxheadָ���
 * ������ɹ���������true;
 * ���ļ��޷��򿪻����ݼ�
 * ��ʧ�ܣ�������false.
 */

status Load(char* filename, Link ptrh, text* ptrth, cline* LineHead) {

	int ListLen;

	FILE* fp;

	if ((fp = fopen(filename, "rb")) == NULL) return false;//�ļ���ʧ��

	fread(&ListLen, sizeof(int), 1, fp);

	if (ListLen <= 0 || ListLen >= 100) return false;

	ptrh->next = LoadMainChain(fp, ListLen);

	ptrth->next = LoadTBox(fp);

	LineHead->next = AllLines_Load(fp);

	fclose(fp);
	return true;
}

/*
 * ������: LoadMainChain
 * �ӿ�: LoadMainChain(fp, ListLen);
 * ------------------------
 * �ú������������ȣ�ListLen)
 * ��ͼ�Σ����Ρ����Ρ�������
 * ��Ԫ�أ����ݴ�ָ���ļ�ָ��
 * �ж��뵽�ڴ��в��γ�����
 * ������ͷ��ָ�롣
 * ������Ϊ0���򷵻�NULL.
 */

static Link LoadMainChain(FILE* fp, int ListLen) {
	Link last = NULL, head = NULL, result = NULL;

	for (int j = 0; j < ListLen; j++) {
		assert(head = (Link)malloc(sizeof(*head)));

		fread(head, sizeof(*head), 1, fp);

		if (j == 0) result = head;

		if (last) last->next = head;

		last = head;

	}

	if (last) last->next = NULL;

	return result;
}

/*
 * ������: LoadLine
 * �ӿ�: LoadLine(fp);
 * ------------------------
 * �ú���������������(struct Line)
 * ���������ṹ��struct ConnectLine)
 * ��ָ���ļ�ָ���ж��뵽
 * �ڴ��в��γ���������
 * ��ͷ��ָ�롣
 * ������Ϊ0���򷵻�NULL.
 */

static struct ConnectLine* LoadLine(FILE* fp) {
	struct ConnectLine* cline = (struct ConnectLine*)malloc(sizeof(*cline));
	assert(cline);

	fread(cline, sizeof(*cline), 1, fp);

	struct Line* last = NULL, * p = NULL;

	cline->begin = NULL;


	for (int j = 0; j < cline->num; j++) {
		p = (struct Line*)malloc(sizeof(struct Line));

		fread(p, sizeof(struct Line), 1, fp);

		if (j == 0) cline->begin = p;

		if (last)  last->next = p;

		p->before = last;

		last = p;
	}

	if(last) last->next = NULL;

	cline->end = last;

	return cline;
}

/*
 * ������: LoadTBox
 * �ӿ�: LoadTBox(fp);
 * ------------------------
 * �ú����ȶ�ȡ������
 *��ListLen)��Ȼ�����
 * �����ȣ�ListLen)��
 * �ı������ݴ�ָ���ļ�
 * ָ���ж��뵽�ڴ��в�
 * �γ�����������ͷ��ָ�롣
 * ������Ϊ0���򷵻�NULL.
 */

static text* LoadTBox(FILE* fp) {
	text* last = NULL, * head = NULL,*p=NULL;

	int ListLen;
	fread(&ListLen, sizeof(int), 1, fp);
	if (ListLen == 0) return NULL;

	

	for (int j = 0; j < ListLen; j++) {

		assert(p = (text*)malloc(sizeof(*head)));

		fread(p, sizeof(*p), 1, fp);

		if (j == 0) head = p;

		if (last) last->next = p;

		last = p;
	}

	if (last) last->next = NULL;

	return head;
}

/*
 * ������: _Destroy
 * �ӿ�: _Destroy(head, texthead);
 * ------------------------
 * �ú���ͨ�������ͼ
 * ��Ԫ������ͷָ���
 * �ı�������ͷָ����
 * ���ͷ�����ռ䣬����
 * ͷָ�벻�ᱻ�ͷš�
 */

void _Destroy(Link head, text* texthead, cline* LineHead) {
	MainChain_Destroy(head);
	TextBox_Destroy(texthead);
	AllLines_Destroy(LineHead);
}
/*
 * ������: MainChain_Destroy
 * �ӿ�: MainChain_Destroy(head);
 * ------------------------
 * �ú���ͨ�������ͼ
 * ��Ԫ������ͷָ����
 * ���ͷ�����ռ䣬��
 * ��ĳԪ�غ��������ߣ�
 * ���ͷ�������������
 * ����ͷָ�벻�ᱻ�ͷš�
 */

static void MainChain_Destroy(Link head) {
	if (head) {
		Link tmp = NULL, h = head;
		head = head->next;
		while (head) {
			tmp = head;
			head = head->next;
			free(tmp);
		}
		h->next = NULL;
	}
}
/*
 * ������: CLine_Destroy
 * �ӿ�: CLine_Destroy(cl)
 * ------------------------
 * �ú���ͨ�������������
 * ���������ṹָ������
 * ������ռ䡣
 */

void CLine_Destroy(cline* cl) {
	if (cl) {
		line* tmp = NULL, * head = cl->begin;
		while (head) {
			tmp = head;
			head = head->next;
			free(tmp);
		}
		free(cl);
	}
}

/*
 * ������: TextBox_Destroy
 * �ӿ�: TextBox_Destroy(head);
 * ------------------------
 * �ú���ͨ���������
 * ��������ͷָ����
 * �ͷ�����ռ�����ͷ
 * ָ�벻�ᱻ�ͷš�
 */

static void TextBox_Destroy(text* head) {
	if (head) {
		text* tmp = NULL,*h=head;
		head = head->next;
		while (head) {
			tmp = head;
			head = head->next;
			free(tmp);
		}
		h->next = NULL;
	}
}

/*
 * ������: Close
 * �ӿ�: Close(head, texthead);
 * ------------------------
 * �ú���ͨ�������ͼ
 * ��Ԫ������ͷָ���
 * �ı�������ͷָ����
 * ���ͷ�����ռ䣬��
 * ���˳�����
 */

void Close( Link head, text* texthead,cline* LineHead) {
	_Destroy(head,texthead,LineHead);
	free(head);
	free(texthead);
	free(LineHead);
	ExitGraphics();
}

/*
 * ������: GetListLen
 * �ӿ�: GetListLen(head);
 * ------------------------
 * �ú���ͨ�������ͼ
 * ��Ԫ������ͷָ�뷵
 * ��������ܳ��ȡ�
 */

int GetListLen(Link head) {
	assert(head);
	Link p = head->next;
	int i = 0;
	while (p) {
		p = p->next;  //Ұָ��
		i++;
	}
	return i;
}

void AllLines_Destroy(cline* head) {
	cline* temp = head->next,*p;
	while (temp) {
		p = temp->next;
		CLine_Destroy(temp);
		temp = p;
	}
	head->next = NULL;
}

static void AllLines_Save(FILE* fp, cline* head) {
	cline* p = head->next;

	int len = 0;					   //len ���ڼ�¼������

	long pos = ftell(fp), endpos;		//��pos��¼��ʼλ��

	fwrite(&len, sizeof(int), 1, fp);  //��д��ռ�ݴ���λ��

	while (p != NULL) {
		SaveLine(fp, p);
		p = p->next;
		len++;                         //�õ�����
	}

	endpos = ftell(fp);
	fseek(fp, pos, SEEK_SET);            //������ͷ��
	fwrite(&len, sizeof(int), 1, fp);    //д�볤��
	fseek(fp, endpos, SEEK_SET);         //����ĩβ
}

static cline* AllLines_Load(FILE* fp) {
	cline* last = NULL, * head = NULL, * p = NULL;

	int ListLen;
	fread(&ListLen, sizeof(int), 1, fp);
	if (ListLen == 0) return NULL;


	for (int j = 0; j < ListLen; j++) {

		p = LoadLine(fp);

		if (j == 0) head = p;

		if (last) last->next = p;

		last = p;
	}

	if (last) last->next = NULL;

	return head;
}