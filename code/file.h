#pragma once

#include "draw.h"

#define false 0
#define true 1

typedef _Bool status;
typedef node* Link;

//�����ӿڣ�

status Save(char* filename, Link head, text* thead, cline* LineHead, int len, int SaveCopyAs);  //���溯���������ֱ�Ϊ���ļ�����������ͷָ�룬�ı�����ͷָ�룬������

status Load(char* filename, Link ptrh, text* ptrth, cline* LineHead);            //��ȡ�����������ֱ�Ϊ���ļ�����ָ��������ͷָ���ָ�룬ָ���ı�����ͷָ���ָ��

int GetListLen(Link head);                                     //��������ĳ���

void Close(Link head, text* texthead,cline* LineHead);                         //�ƻ�������˳�����

void _Destroy(Link head, text* texthead, cline* LineHead);

void CLine_Destroy(cline* cl);