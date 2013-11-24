/*********************************************************************************
*                                ɽè�ɿأ�Lynx��
*                             for LynxFly under GPLv2
*
* COPYRIGHT (C) 2012 - 2013, Lynx 84693469@qq.com
*
* Version   	: V1.0
* By        	: Lynx@sia 84693469@qq.com
*
* For       	: Stm32f405RGT6
* Mode      	: Thumb2
* Description   : PID�㷨��װ
*
*				
* Date          : 2013.XX.XX
*******************************************************************************/
#include "PIDNormal.h"

// ����PID��ϵ��
// #define F_PID_PHI_KP 0 //4   //*57.29578   //Kp*���ȽǶȱ��� �����
// #define F_PID_PHI_KI 0 //0.3 //8.1     //*57.29578   //Ki
// #define F_PID_PHI_KD 0 //17     //*57.29578   //Kd

// #define F_PID_THETA_KP 4   //*57.29578   //Kp*���ȽǶȱ��� ������
// #define F_PID_THETA_KI 0.3 //8.1     //*57.29578   //Ki
// #define F_PID_THETA_KD 17     //*57.29578   //Kd

// #define F_PID_PSI_KP 0 //8.5   //*57.29578   //Kp*���ȽǶȱ��� ƫ����
// #define F_PID_PSI_KI 0     //*57.29578   //Ki
// #define F_PID_PSI_KD 8.5 //0     //*57.29578   //Kd


// //����Kp 1/Ti Td���ض��壨���ձ�׼PID��ʽ��
// //ע�����ﶨ�����Ti�ĵ���������Ti��Ϊ�˵��������������
// #define F_PID_PHI_KP 5.5 //5 //Kp �����
// #define F_PID_PHI_TII (1/0.8) //(1/0.09) //1/Ti ע����Ti�ĵ���
// #define F_PID_PHI_TD 1.1 //0.8 //Td

// #define F_PID_THETA_KP 3.3  //5.5 //5 //4   //Kp ������  //�����10Hz����ʱ�Ĳ���������Ч����ѷɫ��25Hzʱ
// #define F_PID_THETA_TII (1/0.25)  //(1/0.8) //(1/0.12) //(1/0.53) //1/Ti ע����Ti�ĵ���
// #define F_PID_THETA_TD  0.5625  //1.1 //0.73 //0.17  //Td

// // #define F_PID_PSI_KP 15 //0.1   //Kp ƫ����
// // #define F_PID_PSI_TII (1/0.45)   //1/Ti ע����Ti�ĵ���
// // #define F_PID_PSI_TD 0 //Td
// //Ϊ������Ӧ������10Hzʱ�Ŀ�������
// #define F_PID_PSI_KP 1 //3 //0.1   //Kp ƫ����
// #define F_PID_PSI_TII (1/0.45)   //1/Ti ע����Ti�ĵ���
// #define F_PID_PSI_TD 0 //Td

//-----------------------------------------------------------
//����Kp 1/Ti Td���ض��壨���ձ�׼PID��ʽ��
//ע�����ﶨ�����Ti�ĵ���������Ti��Ϊ�˵��������������
//ע�⣬ʹ��PD�͵Ŀ���������ظ�Сҡ�˿��Ƶ���������ʹ��PID����ʱ��Ŵ�ÿ�����
#define F_PID_PHI_KP (24.2393f) //(8.82f) //Kp �����
#define F_PID_PHI_TII (1/2.0f) //(1/0.25f) //(1/2.6475f)) // 1/Ti ע����Ti�ĵ���
#define F_PID_PHI_TD (0.17004f) //(0.66187f) //Td

#define F_PID_THETA_KP (24.2393f) //(8.82f)   //Kp ������  //�����10Hz����ʱ�Ĳ���������Ч����ѷɫ��25Hzʱ
#define F_PID_THETA_TII (1/2.0f) //(1/0.25f) // 1/Ti ע����Ti�ĵ���
#define F_PID_THETA_TD (0.17004f) //(1.3f)   //Td

//Ϊ������Ӧ������10Hzʱ�Ŀ�������
#define F_PID_PSI_KP (20.3175f) //3 //0.1   //Kp ƫ����
#define F_PID_PSI_TII (1/0.19651f) //(1/0.45)   //1/Ti ע����Ti�ĵ���
#define F_PID_PSI_TD 0 //Td
//-----------------------------------------------------------

#define F_PID_BOUNDARY_I_PHI (50/(F_PID_PHI_KP*F_PID_PHI_TII))        //���û������ޣ���ֹ����̫��ز�����Ҳ������ת���ĽǶ�
#define F_PID_BOUNDARY_I_THETA (50/(F_PID_THETA_KP*F_PID_THETA_TII))        //���û������ޣ���ֹ����̫��ز�����Ҳ������ת���ĽǶ�
#define F_PID_BOUNDARY_I_PSI (50/(F_PID_PSI_KP*F_PID_PSI_TII))        //���û������ޣ���ֹ����̫��ز�����Ҳ������ת���ĽǶ�

//PID�Ļ���ȫ�ֱ���
static float EIPhi = 0;
static float EITheta = 0;
static float EIPsi = 0;

void PIDNormalClear(void) {
	EIPhi = 0;
	EITheta = 0;
	EIPsi = 0;
}

float PIDNormal(float CDataYr[], float CDataYE[], float CDataYG[], int Channel, float SampleTime) {
	
	float EP = 0;
	float EI = 0;
	float ED = 0;
	
	float TempIClc = 0;
	float ControlOut = 0;
	
	EP = CDataYr[0]-CDataYE[0];  //���ε����
	
	ED = -CDataYG[0];   //�������ǵĲ���ֵ����΢��
	
	//���ֵĴ�����Ҫ�����һЩ����
	TempIClc = EP*SampleTime;//�ۼӻ��ֲ����ǲ���ʱ�䣨һ�ײ������֣�
	if(Channel == I_PID_CHANNEL_ROLL){
		if((TempIClc>=0 && EIPhi<F_PID_BOUNDARY_I_PHI)||(TempIClc<0 && EIPhi>(-F_PID_BOUNDARY_I_PHI))){
			EIPhi += TempIClc;
			EI = EIPhi;
		}
		//���ɿ������
		ControlOut = F_PID_PHI_KP*(EP+F_PID_PHI_TII*EI+F_PID_PHI_TD*ED);
	}else if(Channel == I_PID_CHANNEL_PITCH){
		if((TempIClc>=0 && EITheta<F_PID_BOUNDARY_I_THETA)||(TempIClc<0 && EITheta>(-F_PID_BOUNDARY_I_THETA))){
			EITheta += TempIClc;	
			EI = EITheta;
		}
		//���ɿ������
		ControlOut = F_PID_THETA_KP*(EP+F_PID_THETA_TII*EI+F_PID_THETA_TD*ED);
	}else if(Channel == I_PID_CHANNEL_YAW){
		if((TempIClc>=0 && EIPsi<F_PID_BOUNDARY_I_PSI)||(TempIClc<0 && EIPsi>(-F_PID_BOUNDARY_I_PSI))){
			EIPsi += TempIClc;
			EI = EIPsi;
		}
		//���ɿ������
		ControlOut = F_PID_PSI_KP*(EP+F_PID_PSI_TII*EI+F_PID_PSI_TD*ED);
	}else{
		EI = 0;  //����������ϲ����ܳ���
		//���ɿ������
		ControlOut = 0;
	}
	
	return ControlOut;  //���ظ�ͨ����ֵ
}

float PDNormal(float CDataYr[], float CDataYE[], float CDataYG[], int Channel, float SampleTime) {
	
	float EP = 0;
	float ED = 0;
	
	float ControlOut = 0;
	
	EP = CDataYr[0]-CDataYE[0];  //���ε����
	
	ED = -CDataYG[0];   //�������ǵĲ���ֵ����΢��
	
	if(Channel == I_PID_CHANNEL_ROLL){
		//���ɿ������
		ControlOut = F_PID_PHI_KP*(EP+F_PID_PHI_TD*ED);
	}else if(Channel == I_PID_CHANNEL_PITCH){
		//���ɿ������
		ControlOut = F_PID_THETA_KP*(EP+F_PID_THETA_TD*ED);
	}else if(Channel == I_PID_CHANNEL_YAW){
		//���ɿ������
		ControlOut = F_PID_PSI_KP*(EP+F_PID_PSI_TD*ED);
	}else{
		//���ɿ������
		ControlOut = 0;
	}
	
	return ControlOut;  //���ظ�ͨ����ֵ
}

float PINormal(float CDataYr[], float CDataY[], int Channel, float SampleTime) {
	
	float EP = 0;
	float EI = 0;
	
	float TempIClc = 0;
	float ControlOut = 0;
	
	EP = CDataYr[0]-CDataY[0];  //���ε����
	
	//���ֵĴ�����Ҫ�����һЩ����
	TempIClc = EP*SampleTime;//�ۼӻ��ֲ����ǲ���ʱ�䣨һ�ײ������֣�
	if(Channel == I_PID_CHANNEL_ROLL){
		if((TempIClc>=0 && EIPhi<F_PID_BOUNDARY_I_PHI)||(TempIClc<0 && EIPhi>(-F_PID_BOUNDARY_I_PHI))){
			EIPhi += TempIClc;
			EI = EIPhi;
		}
		//���ɿ������
		ControlOut = F_PID_PHI_KP*(EP+F_PID_PHI_TII*EI);
	}else if(Channel == I_PID_CHANNEL_PITCH){
		if((TempIClc>=0 && EITheta<F_PID_BOUNDARY_I_THETA)||(TempIClc<0 && EITheta>(-F_PID_BOUNDARY_I_THETA))){
			EITheta += TempIClc;	
			EI = EITheta;
		}
		//���ɿ������
		ControlOut = F_PID_THETA_KP*(EP+F_PID_THETA_TII*EI);
	}else if(Channel == I_PID_CHANNEL_YAW){
		if((TempIClc>=0 && EIPsi<F_PID_BOUNDARY_I_PSI)||(TempIClc<0 && EIPsi>(-F_PID_BOUNDARY_I_PSI))){
			EIPsi += TempIClc;
			EI = EIPsi;
		}
		//���ɿ������
		ControlOut = F_PID_PSI_KP*(EP+F_PID_PSI_TII*EI);
	}else{
		EI = 0;  //����������ϲ����ܳ���
		//���ɿ������
		ControlOut = 0;
	}
	
	return ControlOut;  //���ظ�ͨ����ֵ
}








