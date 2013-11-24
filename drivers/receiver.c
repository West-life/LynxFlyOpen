/*
 * FILE								: receiver.c
 * DESCRIPTION				: This file is RC receiver driver.Ŀǰ��û�����øù���
 * Author							: Lynx@sia 84693469@qq.com
 * Copyright					:
 *
 * History
 * --------------------
 * Rev								: 0.00
 * Date								: 11/03/2013
 *
 * create.
 * --------------------
 */

//---------------- Include files ------------------------//
#include "receiver.h"


#define TIMPSC 180    //����Ԥ��Ƶʵ��ϵ����δ��һ�����ı�ʱ�ֶ����¶���ͨ������
#define TIMARR_M 400     //�����ʱ�������ı�ʱ�ֶ����¶���ͨ�����ޣ���ҪС�����������С��32768
#define TIMARR_C 30     //�����ʱ��ѭ�����ڣ�ms��
#define TIMARR_A (TIMARR_M*TIMARR_C)     //�����ʱ��ʵ�ʵ������ֵ
#define STICK_M_LR_SUP 751  //�������ͨ���������С��ʱ��ֵ��ע�����ֵ�ڼ�ʱ����������Ԥ��Ƶϵ����ʱҪ�ֶ��ı�
#define STICK_M_LR_MID 585  
#define STICK_M_LR_INF 422  
#define STICK_P_SUP 751  
#define STICK_P_MID 582  
#define STICK_P_INF 422  
#define STICK_M_BF_SUP 748  
#define STICK_M_BF_MID 589  
#define STICK_M_BF_INF 421  
#define STICK_T_LR_SUP 756  
#define STICK_T_LR_MID 589  
#define STICK_T_LR_INF 427  

#define SWITCH_ENABLE_SUP 790   //ң�������Ͻ��Ǹ��󿪹�ͨ��5�����趨����ʹ��ң�������⼸��������������õ�
#define SWITCH_ENABLE_MID_UP 620    //������ʩ���ش�������
#define SWITCH_ENABLE_MID 589  
#define SWITCH_ENABLE_MID_DOWN 550  
#define SWITCH_ENABLE_INF 400  

#define STICK_ERR_SUP 800  
#define STICK_ERR_INF 380  

//---------------- Function Prototype -------------------//

static int initialize(void);
// static int TIM3initialize(int ms);
// static int TIM3enable(void);
// static int TIM3disable(void);

//---------------- Variable -----------------------------//

receiver_T receiver = {
	.initialize = initialize,
	.ReceiverIntErr = 0,  //�ۼƼ�ʱ�жϴ���Ĵ���
	.ReceiverOK = 0,  //���ջ�״̬ 0Ϊ���ź�
	.StickMoveLR = 0,  //��ԭ�ĸ�ҡ�˵�λ��
	.StickPower = 0,
	.StickMoveBF = 0,
	.StickTurnLR = 0,
	.SwitchEnable = 0
};

// static int FoundSthThisTime = 0;  //ÿ��ͨ�������ݾ͸���
// static int CH1HTimer = 0;
// static int CH2HTimer = 0;
// static int CH3HTimer = 0;
// static int CH4HTimer = 0;
// static int CH5HTimer = 0;

// static int CH6HTimer = 0;
// static int CH7HTimer = 0;
// static int CH8HTimer = 0;


//-----------------Function------------------------------//

/*
 * Name										: initialize
 * Description						: ---
 * Author									: Lynx@sia 84693469@qq.com.
 *
 * History
 * ----------------------
 * Rev										: 0.00
 * Date										: 03/05/2012
 *
 * create.
 * ----------------------
 */
static int
initialize(void)
{
	//�������ڽ��ջ���û��������������ֱ�ӳ�ʼ��Ϊ��ʹ��
	receiver.ReceiverOK = 0;
	receiver.SwitchEnable = 0;
	
	
	
// 	//PB9---------------------------------------
// 	//PB9 as interrupt
// 	GPIOB->CRH.B.MODE9 = 0;  //����ģʽ
// 	GPIOB->CRH.B.CNF9 = 2; //����������
// 	GPIOB->ODR.B.IO9 = 1; //��������
// 	
// 	//���ⲿ�ж� PB9
// 	AFIO->EXTICR3.B.EXTI9 = 1; //��PB9�ж�����

// 	EXTI->IMR.B.MR9 = 1; //���� PB9 ���ϵ��ж�����
// 	EXTI->RTSR.B.TR9 = 1; //�����ش���
// 	EXTI->FTSR.B.TR9 = 1; //�½��ش���
// 	//PB8---------------------------------------
// 	//PB8 as interrupt
// 	GPIOB->CRH.B.MODE8 = 0;  //����ģʽ
// 	GPIOB->CRH.B.CNF8 = 2; //����������
// 	GPIOB->ODR.B.IO8 = 1; //��������
// 	
// 	//���ⲿ�ж� PB8
// 	AFIO->EXTICR3.B.EXTI8 = 1; //��PB9�ж�����

// 	EXTI->IMR.B.MR8 = 1; //���� PB9 ���ϵ��ж�����
// 	EXTI->RTSR.B.TR8 = 1; //�����ش���
// 	EXTI->FTSR.B.TR8 = 1; //�½��ش���
// 	//PB7---------------------------------------
// 	//PB7 as interrupt
// 	GPIOB->CRL.B.MODE7 = 0;  //����ģʽ
// 	GPIOB->CRL.B.CNF7 = 2; //����������
// 	GPIOB->ODR.B.IO7 = 1; //��������
// 	
// 	//���ⲿ�ж� PB7
// 	AFIO->EXTICR2.B.EXTI7 = 1; //��PB9�ж�����

// 	EXTI->IMR.B.MR7 = 1; //���� PB9 ���ϵ��ж�����
// 	EXTI->RTSR.B.TR7 = 1; //�����ش���
// 	EXTI->FTSR.B.TR7 = 1; //�½��ش���
// 	//PB6---------------------------------------
// 	//PB6 as interrupt
// 	GPIOB->CRL.B.MODE6 = 0;  //����ģʽ
// 	GPIOB->CRL.B.CNF6 = 2; //����������
// 	GPIOB->ODR.B.IO6 = 1; //��������
// 	
// 	//���ⲿ�ж� PB6
// 	AFIO->EXTICR2.B.EXTI6 = 1; //��PB9�ж�����

// 	EXTI->IMR.B.MR6 = 1; //���� PB9 ���ϵ��ж�����
// 	EXTI->RTSR.B.TR6 = 1; //�����ش���
// 	EXTI->FTSR.B.TR6 = 1; //�½��ش���

// 	NVIC->ISER1.B.EXTI9_5 = 1;
// 	NVIC->IPR6.B.EXTI9_5 = 0;
// 	//PB5---------------------------------------
// 	//PB5 as interrupt
// 	GPIOB->CRL.B.MODE5 = 0;  //����ģʽ
// 	GPIOB->CRL.B.CNF5 = 2; //����������
// 	GPIOB->ODR.B.IO5 = 1; //��������
// 	
// 	//���ⲿ�ж� PB6
// 	AFIO->EXTICR2.B.EXTI5 = 1; //��PB9�ж�����

// 	EXTI->IMR.B.MR5 = 1; //���� PB9 ���ϵ��ж�����
// 	EXTI->RTSR.B.TR5 = 1; //�����ش���
// 	EXTI->FTSR.B.TR5 = 1; //�½��ش���

// 	NVIC->ISER1.B.EXTI9_5 = 1;
// 	NVIC->IPR6.B.EXTI9_5 = 0;
// 	
// 	TIM3initialize(TIMARR_C);  //��TIMER3���趨��ʱΪǰ��define��100ms
// 	TIM3enable();  //ʹ��
// 	
	return 0;
}

/*
 * Name										: EXTI9_5_IRQHandler
 * Description						: ---
 * Author									: Lynx@sia 84693469@qq.com.
 *
 * History
 * ----------------------
 * Rev										: 0.00
 * Date										: 03/05/2012
 *
 * create.
 * ----------------------
 */
// void EXTI9_5_IRQHandler(void)
// {
// 	//�������ڼ���Խ��ʱ��ļ�ʱ��̬����
// 	static int CH1UP = 0;
// 	static int CH1DOWN = 0;
// 	static int CH2UP = 0;
// 	static int CH2DOWN = 0;
// 	static int CH3UP = 0;
// 	static int CH3DOWN = 0;
// 	static int CH4UP = 0;
// 	static int CH4DOWN = 0;
//  	static int CH5UP = 0;
//  	static int CH5DOWN = 0;
// // 	static int CH6UP = 0;
// // 	static int CH6DOWN = 0;
// // 	static int CH7UP = 0;
// // 	static int CH7DOWN = 0;
// // 	static int CH8UP = 0;
// // 	static int CH8DOWN = 0;
// 	
// 	//������ʱ���̾ͼ�¼�õļ�ʱֵ��Ϊ������޶ȵļ������ʱ�䣩
// 	int FastSave;  //���ﲻ����ֵ��Ϊ����OS������֤Ϊ��һ������
// 	OSIntEnter();
// 	
// 	FastSave = TIM3->CNT;  //���ټ�¼ʱ��
// 	
// 	//����ÿһif����ѯ��һ�Σ�Ϊ�����жϱ���������IO���ж�ͬʱ�����ʱ���ܱ�֤����޶ȵĶ�ȡ�ɹ�
// 	
// 	if(EXTI->PR.B.PR9){
// 		if(GPIOB->IDR.B.IO9) {  //�ߵ�ƽ˵����������
// 			CH1UP = FastSave; //����Ӧ��ͨ����ֵ
// 		}else{  //�½���
// 			CH1DOWN = FastSave; //����Ӧ��ͨ����ֵ
// 			CH1HTimer = (TIMARR_A+CH1DOWN-CH1UP)%TIMARR_A;   //���½���ʱ����ߵ�ƽԽ�ɵ�ʱ��
// 			FoundSthThisTime++;
// 		}
// 		EXTI->PR.B.PR9 = 1; //CLEARע��STM32����Ǹ�1����ѯҲ��1 ��
// 	}
// 	if(EXTI->PR.B.PR8){
// 		if(GPIOB->IDR.B.IO8) {  //�ߵ�ƽ˵����������
// 			CH2UP = FastSave; //����Ӧ��ͨ����ֵ
// 		}else{  //�½���
// 			CH2DOWN = FastSave; //����Ӧ��ͨ����ֵ
// 			CH2HTimer = (TIMARR_A+CH2DOWN-CH2UP)%TIMARR_A;   //���½���ʱ����ߵ�ƽԽ�ɵ�ʱ��
// 			FoundSthThisTime++;
// 		}
// 		EXTI->PR.B.PR8 = 1; //CLEARע��STM32����Ǹ�1����ѯҲ��1 ��
// 	}
// 	if(EXTI->PR.B.PR7){
// 		if(GPIOB->IDR.B.IO7) {  //�ߵ�ƽ˵����������
// 			CH3UP = FastSave; //����Ӧ��ͨ����ֵ
// 		}else{  //�½���
// 			CH3DOWN = FastSave; //����Ӧ��ͨ����ֵ
// 			CH3HTimer = (TIMARR_A+CH3DOWN-CH3UP)%TIMARR_A;   //���½���ʱ����ߵ�ƽԽ�ɵ�ʱ��
// 			FoundSthThisTime++;
// 		}
// 		EXTI->PR.B.PR7 = 1; //CLEARע��STM32����Ǹ�1����ѯҲ��1 ��
// 	}
// 	if(EXTI->PR.B.PR6){
// 		if(GPIOB->IDR.B.IO6) {  //�ߵ�ƽ˵����������
// 			CH4UP = FastSave; //����Ӧ��ͨ����ֵ
// 		}else{  //�½���
// 			CH4DOWN = FastSave; //����Ӧ��ͨ����ֵ
// 			CH4HTimer = (TIMARR_A+CH4DOWN-CH4UP)%TIMARR_A;   //���½���ʱ����ߵ�ƽԽ�ɵ�ʱ��
// 			FoundSthThisTime++;
// 		}
// 		EXTI->PR.B.PR6 = 1; //CLEARע��STM32����Ǹ�1����ѯҲ��1 ��
// 	}
// 	if(EXTI->PR.B.PR5){
// 		if(GPIOB->IDR.B.IO5) {  //�ߵ�ƽ˵����������
// 			CH5UP = FastSave; //����Ӧ��ͨ����ֵ
// 		}else{  //�½���
// 			CH5DOWN = FastSave; //����Ӧ��ͨ����ֵ
// 			CH5HTimer = (TIMARR_A+CH5DOWN-CH5UP)%TIMARR_A;   //���½���ʱ����ߵ�ƽԽ�ɵ�ʱ��
// 			FoundSthThisTime++;
// 		}
// 		EXTI->PR.B.PR5 = 1; //CLEARע��STM32����Ǹ�1����ѯҲ��1 ��
// 	}

// 	
// 	OSIntExit();
// }
/*
 * Name				: TIM3initialize
 * Description		: ---
 * Author			: Lynx@sia 84693469@qq.com.
 *
 * History
 * ----------------------
 * Rev	: 0.00
 * Date	: 2013.11.03
 *
 * create.
 * ----------------------
 */
// static int TIM3initialize(int ms)
// {
// 	TIM3->CR1.B.ARPE = 1;
// 	// TIM3->CR1.B.URS = 1;

// 	TIM3->CR2.B.MMS = 1;  //CEN TRIG

// 	TIM3->PSC = TIMPSC - 1;

// 	TIM3->ARR = ms * TIMARR_M - 1;

// 	TIM3->DIER.B.TIE = 1;
// 	TIM3->DIER.B.UIE = 1;

// 	TIM3->CR1.B.DIR = 0;  //up

// 	TIM3->CNT = 0;

// 	TIM3->CR1.B.CEN = 0;
// 	//timer3.timeout_flag = 0;

// 	return 0;
// }

/*
 * Name				: TIM3enable
 * Description		: ---
 * Author			: Lynx@sia 84693469@qq.com.
 *
 * History
 * ----------------------
 * Rev	: 0.00
 * Date	: 2013.11.03
 *
 * create.
 * ----------------------
 */
// static int TIM3enable(void)
// {
// 	TIM3->CNT = 0;

// 	TIM3->CR1.B.CEN = 1;

//  	return 0;
// }

/*
 * Name				: TIM3disable
 * Description		: ---
 * Author			: Lynx@sia 84693469@qq.com.
 *
 * History
 * ----------------------
 * Rev	: 0.00
 * Date	: 2013.11.03
 *
 * create.
 * ----------------------
 */
// static int TIM3disable(void)
// {
// 	TIM3->CR1.B.CEN = 0;

// 	return 0;
// }

// void TIM3_IRQHandler(void)
// {
// 	int TMPStickMoveLR;
// 	int TMPStickPower;
// 	int TMPStickMoveBF;
// 	int TMPStickTurnLR;
// 	int TMPSwitchEnable;
// 	OSIntEnter();
// 	
// 	if(FoundSthThisTime){
// 		TMPStickMoveLR = CH1HTimer;  //��ԭ�ĸ�ҡ�˵�λ��
// 		TMPStickPower = CH2HTimer;
// 		TMPStickMoveBF = CH3HTimer;
// 		TMPStickTurnLR = CH4HTimer;
// 		TMPSwitchEnable = CH5HTimer;
// 		
// 		if(TMPStickMoveLR>STICK_ERR_INF && TMPStickMoveLR<STICK_ERR_SUP){
// 			TMPStickMoveLR = (TMPStickMoveLR-STICK_M_LR_MID)*100/(STICK_M_LR_SUP-STICK_M_LR_MID);
// 			if(TMPStickMoveLR>99){
// 				receiver.StickMoveLR = 99;
// 			}else if(TMPStickMoveLR<-99){
// 				receiver.StickMoveLR = -99;
// 			}else{
// 				receiver.StickMoveLR = TMPStickMoveLR;
// 			}
// 		}else{
// 			receiver.ReceiverIntErr++;
// 		}
// 		
// 		if(TMPStickMoveBF>STICK_ERR_INF && TMPStickMoveBF<STICK_ERR_SUP){
// 			TMPStickMoveBF = (TMPStickMoveBF-STICK_M_BF_MID)*100/(STICK_M_BF_SUP-STICK_M_BF_MID);
// 			if(TMPStickMoveBF>99){
// 				receiver.StickMoveBF = 99;
// 			}else if(TMPStickMoveBF<-99){
// 				receiver.StickMoveBF = -99;
// 			}else{
// 				receiver.StickMoveBF = TMPStickMoveBF;
// 			}
// 		}else{
// 			receiver.ReceiverIntErr++;
// 		}
// 		
// 		if(TMPStickTurnLR>STICK_ERR_INF && TMPStickTurnLR<STICK_ERR_SUP){
// 			TMPStickTurnLR = (TMPStickTurnLR-STICK_T_LR_MID)*100/(STICK_T_LR_SUP-STICK_T_LR_MID);
// 			if(TMPStickTurnLR>99){
// 				receiver.StickTurnLR = 99;
// 			}else if(TMPStickTurnLR<-99){
// 				receiver.StickTurnLR = -99;
// 			}else{
// 				receiver.StickTurnLR = TMPStickTurnLR;
// 			}
// 		}else{
// 			receiver.ReceiverIntErr++;
// 		}
// 		
// 		if(TMPStickPower>STICK_ERR_INF && TMPStickPower<STICK_ERR_SUP){
// 			TMPStickPower = (TMPStickPower-STICK_P_INF)*100/(STICK_P_SUP-STICK_P_INF);   //ע��������ǰ��Ĳ�������ǿ�����
// 			if(TMPStickPower>99){
// 				receiver.StickPower = 99;
// 			}else if(TMPStickPower<0){
// 				receiver.StickPower = 0;
// 			}else{
// 				receiver.StickPower = TMPStickPower;
// 			}
// 		}else{
// 			receiver.ReceiverIntErr++;
// 		}
// 		
// 		if(TMPSwitchEnable>STICK_ERR_INF && TMPSwitchEnable<STICK_ERR_SUP){
// 			//ע��������ǰ��Ĳ��
// 			if(TMPSwitchEnable>SWITCH_ENABLE_MID_UP){  //�����ֹ�����м�λ��������
// 				receiver.SwitchEnable = 1;
// 			}else if(TMPSwitchEnable<SWITCH_ENABLE_MID_DOWN){
// 				receiver.SwitchEnable = 0;
// 			}
// 		}else{
// 			//receiver.SwitchEnable = 0;    ��䱻��ע�͵��ˣ���Ȼ�������£��������»�������صĶ������Ƶ����⣬�����Ѱ�
// 			receiver.ReceiverIntErr++;
// 		}
// 		
// 		receiver.ReceiverOK = 1;  //���ջ�״̬����
// 		FoundSthThisTime = 0; //clear
// 	}else{
// 		receiver.ReceiverOK = 0;
// 		receiver.SwitchEnable = 0;
// 		FoundSthThisTime = 0; //clear
// 	}

// 	TIM3->SR.B.UIF = 0;
// 	TIM3->SR.B.TIF = 0;
// 	OSIntExit();
// }

