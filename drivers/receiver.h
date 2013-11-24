/*------------------------------------------------------
FILE NAME   : receiver.h
DESCRIPTION : event file header
VERSION     : 0.0.0 (C)Lynx
AUTHOR      : Lynx
CREATE DATE : 2013-11-3
MODIFY DATE :
LOGS        :-\
--------------------------------------------------------*/
#ifndef __receiver_h__
#define __receiver_h__

#include <board.h>

//------------------- Define ---------------------------//
//#define receiverA	1
//#define receiverB	2
//#define receiverC	3

 

//----------------- Typedef------------------------------//
typedef struct{
		int (* initialize)(void);
	
	
		int ReceiverIntErr;  //�ۼƼ�ʱ�жϴ���Ĵ���
		int ReceiverOK;  //���ջ�״̬ 0Ϊ���ź�
		int StickMoveLR;  //��ԭ�ĸ�ҡ�˵�λ��
		int StickPower;
		int StickMoveBF;
		int StickTurnLR;
		int SwitchEnable;
}receiver_T;
 
//----------------- Extern ------------------------------//
extern receiver_T receiver;

#endif //__receiver_h__
