#include<stdio.h>
#include<windows.h>
#include<mmsystem.h>
#pragma comment(lib, "WINMM.LIB")

/*
* ��������Music
* �ӿڣ�Music(int Choice)
* ������������������������
* �������Ӧ��Windowsϵͳ
* �Ŀ⣬����wav��ʽ���ֵ�
* ������ֹͣ���š�wav����
* Ӧ����exe�ļ�ͬĿ¼�¡�
*/
void Music(int Choice)
{
    switch (Choice) {
    case 1: PlaySound(TEXT("ZJU_Song.wav"), 0, SND_FILENAME | SND_ASYNC | SND_LOOP);// �㽭��ѧУ�裬ѭ������
        break;
    case 2: PlaySound(TEXT("The Congress Reel.wav"), 0, SND_FILENAME | SND_ASYNC | SND_LOOP);// The Congress Reel�����֣�ѭ������
        break;
    case 3: PlaySound(TEXT("Yellow.wav"), 0, SND_FILENAME | SND_ASYNC | SND_LOOP); // Yellow�����֣�ѭ������
        break;
    case 4: PlaySound(TEXT(NULL), 0, SND_FILENAME | SND_PURGE); break;// ֹͣ���ŵ�ǰ����
    default: break;
    }
}
