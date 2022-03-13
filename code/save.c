//save.c
#include "graphics.h"
#include "extgraph.h"
#include "genlib.h"
#include "simpio.h"
#include "conio.h"
#include <string.h>
#include <stdlib.h>
#include <stddef.h>

#include <windows.h>
#include <olectl.h>
#include <mmsystem.h>
#include <wingdi.h>
#include <ole2.h>
#include <ocidl.h>
#include <winuser.h>
#include <stdio.h>
#include <assert.h>
#include <io.h>
#include "imgui.h"
#include "file.h"
#include "save.h"

/*��ȡ�ļ��ͱ�����棬
	֧�������ļ����ڱ���Ŀ¼��ȡ��
	Ҳ֧���������·��+�ļ�����ȡ��
	֧���г�����Ŀ¼���ж�����
	�ļ���.dat�����д�ȡ
*/


//save:
	//�ı��������ļ�����Ĭ��Ϊnew1��ԭ�ļ�����/����·��
	//��ť1��ѡ���ļ���/ѡ�����·��
	//��ť2������
	//��ť3������
	//��ǩ��1�����Ϊ���ļ���׺.prc)
	//��ǩ��2���ļ��������ڱ���Ŀ¼//����ʾ��"C:\\abc\\xx.prc"//�ļ����ظ���

//load:
	//�ı��������ļ�����Ĭ��Ϊnew1��ԭ�ļ�����/����·��
	//��ť1��ѡ���ļ���/ѡ�����·��
	//��ť2�������ļ�(s)
	//��ť3����һ��/��һ��
	//optional:��ť������򿪵��ļ���s)


/*��������*/
#define MaxStrLen 80
#define FileLen  20
#define AddressLen 200

/*������Ա���*/

#define ButtonW (1./7)
#define ButtonH (0.1)
#define FileButtonH (0.04)
#define BackButtonX (1./7)
#define SaveButtonX (5./7)
#define TextBoxW (5./7)
#define TextBoxX (1./7)
#define TextBoxH (0.17)
#define SaveLabelX (3.3/7)
#define EmptyLineH (0.03)
#define EmptyColumn (0.02)
#define FileRow 9

/*ȫ�ֱ�������*/

enum __page { StartPage_, SavePage_, DrawPage_, HelpPage_, ExamplePage_, LoadPage_, LastPage_ };

static char labels[2][MaxStrLen] = { "��ֱ�������ļ������ļ����ڱ���,��׺Ĭ��dat�������·������\"C:\\abc\\xx.dat\"��","�ļ��Ѵ��ڻ��޷��򿪣�" };
static char filenames[FileRow][FileLen],Isfolder[FileRow], __address[AddressLen] = "*.*" ;  //filenames�����ڴ򿪣����أ����洢��һҳ���ļ�����Ϣ��
															//ÿһҳ�������ΪFileRow��ÿһ���ļ������Ȳ�����FileLen��
															//Isfolder���ڼ�¼ĳ�е��ļ��Ƿ���һ���ļ���

/*�����ӿ�*/
int Display_Save(double wholeW, double wholeH, char* filename, Link head, text* thead, cline* HeadLine);
int Display_Load(double wholeW, double wholeH, Link ptrh, text* ptrth, cline* HeadLine, char* Name);
/*�ڲ�����*/
static int SetFileNames_(int page, char* relafname, char* baseAddress);
static void AddressCatch(char* base, char* source);


/*����ʵ��*/

/*
 * ������: Display_Save
 * �ӿ�: Display_Save(wholeW, wholeH,filename,head, textboxhead);
 * ------------------------
 * ���Ʊ��潻�����棬Ĭ���ļ���
 * Ϊfilname,����headָ���
 * textboxheadָ��ʵ�����Ϊ���ܡ�
 * wholeW��wholeHΪ���ڴ�С��
 */

int Display_Save(double wholeW, double wholeH,char* filename,Link head, text* thead,cline* HeadLine) {             //����0���˳�������1�˳�

	static char textbuf[FileLen];         //textbuf Ϊ�ı����ϵ��ı�������

	static int labelmode, backflag = 1;   //labelmodeΪ�ֽ�labels[]�ַ��������У�
	                                      //Ӧ��ѡ����ʾ���ַ����Ķ�Ӧ�������±ꣻ
										  //backflagΪ1�����˳�����������뱣����棬
										  //Ϊ0�����������ָý���

	DefineColor("ButtonFrameCommon", 0.07, 0.72, 0.96);
	DefineColor("ButtonFrameHot", 0.20, 0.60, 1.00);
	setButtonColors("ButtonFrameCommon", "White", "ButtonFrameHot", "White", 1);

	SetFont("΢���ź�");
	SetPointSize(18);

	if (backflag == 1) {                  //�ոս���ñ��ֽ�����г�ʼ��
		backflag = 0;
		strcpy(textbuf, filename);        //�����ļ���
		labelmode = 0;
	}

	if (textbox(GenUIID(0), TextBoxX * wholeW, (8* EmptyLineH + 4* ButtonH) * wholeH, TextBoxW * wholeW, TextBoxH* wholeH, textbuf, FileLen))
		labelmode = 0;//�ı�����������Ҫ�����ļ��ľ��Ի���Ե�ַ

	drawLabel(SaveLabelX * wholeW, (9 * EmptyLineH + 5 * ButtonH) * wholeH, "���Ϊ");     //�������
	drawLabel(TextBoxX * wholeW, (7 * EmptyLineH + ButtonH) * wholeH, labels[labelmode]);  //��ʾ�����ĳ��״̬


	if (button(GenUIID(0), BackButtonX * wholeW, (6*EmptyLineH)*wholeH, ButtonW*wholeW, FileButtonH*wholeH, "ȡ��")) {      //����
		backflag = 1;
	}

	if (button(GenUIID(0), SaveButtonX * wholeW, (6*EmptyLineH)*wholeH, ButtonW * wholeW, FileButtonH * wholeH, "����")) {  //����
		
		if (Save(textbuf, head, thead, HeadLine, GetListLen(head), 1)) {  //�籣��ɹ����򷵻�
			backflag = 1;
			besaved();
		}
		else {
			labelmode = 1; //������ʾ����ʧ��
		}

	}

	if (backflag) return LastPage_;  //��������
	return SavePage_;                //���ֽ���
}

/*
 * ������: Display_Load
 * �ӿ�: Display_Load(wholeW, wholeH,head, textboxhead,filename);
 * ------------------------
 * ���ƴ򿪽������棬Ĭ���ļ���Ϊfilname,
 * ����headָ���textboxheadָ��ʵ��
 * wholeW��wholeHΪ���ڴ�С��
 * �򿪺󣬽��ļ�������filename;
 */

int Display_Load(double wholeW, double wholeH, Link ptrh, text* ptrth,cline* HeadLine, char* Name) {

	static char textbuf[AddressLen];
	char tmp[AddressLen];
	static int labelmode, backflag = 1,Isfull,page=1; //Isfull��������ʾ���ļ��Ǹõ�ַ�����һҳ��page����ҳ����������������ͬ��

	DefineColor("ButtonFrameCommon", 0.07, 0.72, 0.96);
	DefineColor("ButtonFrameHot", 0.20, 0.60, 1.00);
	setButtonColors("ButtonFrameCommon", "White", "ButtonFrameHot", "White", 1);

	SetFont("΢���ź�");
	SetPointSize(18);

	//���´򿪸Ľ����ĳ�ʼ������
	if (backflag == 1) {
		backflag = 0;
		strcpy(textbuf, "\0");
		memset(filenames[0], 0, 8*sizeof(char));      //��ջ����ļ�����Ϣ
		Isfull=SetFileNames_(page,"RESTART",__address);
		page = 1;
	}

	/*��һҳ*/
	if (!Isfull && button(GenUIID(0), (SaveButtonX) * wholeW, (5.5* EmptyLineH) * wholeH, ButtonW * wholeW, FileButtonH * wholeH, "��һҳ")) {
		Isfull = SetFileNames_(++page,NULL, __address);
	}

	/*��һҳ*/
	if (page>1 && button(GenUIID(0), (BackButtonX)*wholeW, (5.5* EmptyLineH) * wholeH, ButtonW * wholeW, FileButtonH * wholeH, "��һҳ")) {
		Isfull = SetFileNames_(--page,NULL, __address);
	}

	/*��ʾ��ǰ�ļ����е��ļ���һҳ*/
	usePredefinedColors(3);
	for (int i = 0; i < FileRow; i++) {
		strcpy(tmp, filenames[i]);
		if (Isfolder[i]) sprintf(tmp, "%s%10s", tmp, "Folder");

		if (button(													GenUIID(i), ( BackButtonX ) * wholeW,
																	((19) * EmptyLineH-i* FileButtonH) * wholeH,
																	(5*ButtonW) * wholeW, FileButtonH * wholeH, 
																	tmp))
		{
			if (Isfolder[i])
				Isfull = SetFileNames_(page = 1, filenames[i],__address);
			else
				sprintf(textbuf, "%s\\%s", __address, filenames[i]);
		}
		if (filenames[i + 1][0] == '\0') break;
	}

	setButtonColors("ButtonFrameCommon", "White", "ButtonFrameHot", "White", 1);

	/*�ı��������������������ʾҪ������ļ���ַ*/
	if (textbox(GenUIID(0), TextBoxX * wholeW, (8 * EmptyLineH + 4 * ButtonH) * wholeH, TextBoxW * wholeW, TextBoxH * wholeH, textbuf, MaxStrLen))
		labelmode = 0;

	/*������*/
	drawLabel(SaveLabelX * wholeW, (9 * EmptyLineH + 5 * ButtonH) * wholeH, "��ȡ");

	/*��ʾ��ǰ����״̬*/
	drawLabel(TextBoxX * wholeW, (1 * EmptyLineH + ButtonH) * wholeH, labels[labelmode]);

	if (button(GenUIID(0), BackButtonX * wholeW, (2 * EmptyLineH) * wholeH, ButtonW * wholeW, FileButtonH * wholeH, "ȡ��")) {  //����
		backflag = 1;
	}

	if (button(GenUIID(0), SaveButtonX * wholeW, (2 * EmptyLineH) * wholeH, ButtonW * wholeW, FileButtonH * wholeH, "��")) {  //����

		if (Load(textbuf, ptrh, ptrth,HeadLine)) {
			backflag = 1;
			assert(Name);
			strcpy(Name, textbuf);    //�����ļ���
			return DrawPage_;
		}
		else {
			labelmode = 1;            //��ʧ������ʾ����
		}

	}

	if (backflag) return LastPage_;
	else return LoadPage_;
}

/*
 * ������: SetFileNames_
 * �ӿ�: SetFileNames_(page,relafname);
 * ------------------------
 * ���ڲ���ַ�Ļ����ϣ��������
 * ��ַ��relafname)��ʾ���ļ���
 * �µ������ļ��к�.dat�ļ���page
 * ����ҳ����
 * ��һ�ε��øĺ���ʱ���������һ��
 * ���õ���Ե�ַ���ӵ��������ַ�ϡ�
 * ���relafname="RESTART",�������
 * ������ַ
 */

static int SetFileNames_(int page,char* relafname,char* baseAddress) {
	int count = 0;               //��ʾ��ǰ�������ļ���Ŀ

	struct _finddata_t fileinfo; //�����ļ���Ϣ�ṹ

	static char tmp[200];

	/*�ı��ļ���,������"RESTART"�ļ�Ŀ¼����Ϊ������Ŀ¼*/
	if (relafname) {
		if (!strcmp("RESTART", relafname)) {
			GetCurrentDirectory(AddressLen,baseAddress);
		}
		else {
			AddressCatch(baseAddress, relafname);
		}

		memset(filenames[0], 0, sizeof(char) * FileLen * FileRow);
		sprintf(tmp, "%s\\*.*", baseAddress);
		//puts(baseAddress);        //������
	}

	long handle = _findfirst(tmp, &fileinfo);

	assert(handle != -1L);

	/*����ָ��ҳ��֮ǰ���ļ�*/
	while (count < (page-1) * FileRow && _findnext(handle, &fileinfo) != -1) {
		count++;
	}

	/*����ָ��ҳ�����ļ�*/
	do {
		if ((fileinfo.attrib & 0x01) == _A_NORMAL) {  //�ļ�Ҫ��֤Ϊ��ϵͳ�ļ��ҿɼ������޸��ļ�
			if (strstr(fileinfo.name, ".dat")) {                                                    //ɸѡ�����Ϻ�׺�����ļ�
				strcpy(filenames[count - (page - 1) * FileRow], fileinfo.name);
				Isfolder[count - (page - 1) * FileRow] = 0;
				count++;
			}

			else if ((fileinfo.attrib & 0x10) == _A_SUBDIR  && strcmp(".",fileinfo.name)!=0) { //�ж��ļ���
				strcpy(filenames[count - (page - 1) * FileRow], fileinfo.name);
				Isfolder[count - (page - 1) * FileRow] = 1;
				count++;
			}
		}

	} while ((_findnext(handle, &fileinfo) != -1) && count < page * FileRow);

	_findclose(handle);

	return count < page * FileRow; //��������һҳ����1�����Ƿ���0
}

void AddressCatch(char* base, char* source) {
	if (!strcmp("..", source)) {
		int pos = 0, i;
		for (i = 0; base[i]; i++) {
			if (base[i] == '\\') pos = i;
		}
		base[pos] = '\0';
	}
	else sprintf(base,"%s\\%s", base, source);
}