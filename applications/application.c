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
* Description   : RT-Thread �û���ʼ���߳�
*
*				
* Date          : 2013.XX.XX
*******************************************************************************/
/*
 * File      : application.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2009-01-05     Bernard      the first version
 */

/**
 * @addtogroup STM32
 */
/*@{*/

#include <stdio.h>
#include <board.h>
#include <rtthread.h>

#include <battery.h>   //���ͷ�ļ�
#include <pwmRotor4.h>   //�������ͷ�ļ�
#include <stm32f4_i2c_dma.h>   //DMA��ʽ��I2Cͷ�ļ�

//�û��̵߳��������
#include "..\LynxFly\global_define.h"
#include "..\LynxFly\TaskNotify.h"
#include "..\LynxFly\TaskComm.h"
#include "..\LynxFly\TaskControl.h"
#include "..\LynxFly\TaskIMU.h"

#include "cpuusage.h"  //����CPUʹ���������

#ifdef RT_USING_LWIP
#include <lwip/sys.h>
#include <lwip/api.h>
#include <netif/ethernetif.h>
#endif

#ifdef RT_USING_FINSH
#include <finsh.h>
#include <shell.h>
#endif

#ifdef RT_USING_DFS
#include <dfs_init.h>
#include <dfs_fs.h>
#include <dfs_elm.h>
#endif

//һ��ѳ������õ����� ���⾯��������ô�Ķ���
void rt_platform_init(void);
int rt_wlan_init(void);
void lwip_sys_init(void);
int WlanDirectConnect(void);
int ConnectSSID(char *connssid);
int wlan_pm_exit(void);

//��ص�����ʾ�ļ�����ز���
int BatteryTimeToSleep = 0;   //����ʱ��Ϊһ��0~20����
float BatteryVTop = 4.1f;   //�������ĵ�ѹ
float BatteryVButtom = 3.0f;   //�����ȫ�յ�ĵ�ѹ
float BatteryVNow = 0;     //��ص�ǰ�ĵ�ѹ

extern char    *Default_SSID;   //����wlan_main�е�Ĭ��SSID�����޸ĸ�Ĭ��ֵ

//���Ƶ�Դ������ȫ�ֱ�����ͨ���Ժ�ɻ����������򿪳���̣߳��յ���������ʱ�ſ�ʼ�������߳��벿��
int PowerOnSign = 0;

void rt_init_thread_entry(void* parameter)
{
	finsh_system_init();
	finsh_set_device(RT_CONSOLE_DEVICE_NAME);
	/* initialize the device file system */
	dfs_init();
	/* initialize the elm chan FatFS file systam*/
	elm_init();

	rt_platform_init();
	
	cpu_usage_init();   //���ó�ʼ������������CPUʹ�õĹ��ӡ�

	/* Filesystem Initialization */
#ifdef RT_USING_DFS
	{
#ifdef RT_USING_DFS_ELMFAT
			/* mount sd card fat partition 1 as root directory */
			if (dfs_mount("sd0", "/", "elm", 0, 0) == 0)
			{
					rt_kprintf("File System initialized!\n");
			}
			else
					rt_kprintf("File System initialzation failed!\n");
#endif
	}
#endif
	
	pwmRotor4.initialize();   //��ʼ�����
	battery.initialize();  //��ʼ����صĲ���
	I2C_Config();//����I2C
	
	
	//δ����������ǰ����ʼ��ͣ�����������Ŀǰ��һ��ͨ���ζ����Ӳ鿴��ص����ķ�������Ȥ�� �λο��Ƶ�ռ�ձ�
	while(KEY == 0 && PowerOnSign == 0)
	{
		LED_R_ON();
		
		BatteryVNow = battery.read();
		BatteryTimeToSleep = (int)((BatteryVNow-BatteryVButtom)/(BatteryVTop-BatteryVButtom)*20);   //�����ص���0~20
		
		if(BatteryTimeToSleep>20)  //Խ��ʱ��һЩ�淶��
			BatteryTimeToSleep = 20;
		else if(BatteryTimeToSleep<0)
			BatteryTimeToSleep = 0;
		
		rt_thread_delay(rt_tick_from_millisecond(BatteryTimeToSleep));   //��ʱ
		
		if(BatteryTimeToSleep>0)  //���������������ʾ���
		{
			LED_R_OFF();
			rt_thread_delay(rt_tick_from_millisecond(20-BatteryTimeToSleep));   //��ʱ
			
		}
	}
	//Ĭ�������ֻ�е�������������ʱ����ʽ�������������ʾ���״̬
	PowerOnSign = 1;
	
// // 	pwmRotor4.setMotor1I(100);  //�Ų��Ե���õ�
// // 	pwmRotor4.setMotor2I(100);
// // 	pwmRotor4.setMotor3I(100);
// // 	pwmRotor4.setMotor4I(100);
	
#ifdef RT_USING_LWIP
	{
		/* register ethernetif device */
		eth_system_device_init();

		rt_wlan_init();
		rt_kprintf("wlan initialized\n");		
		
		//Default_SSID="LynxGround"; 
		//ConnectSSID(Default_SSID);  //��������SSID���ӵķ��� ͬ����Ҫָ����ȷ��IP��ַ
		//WlanDirectConnect();  //ԭ�ȵ�SSID��Mac��ֱַ������ ��Ҫָ����ȷ��IP��ַ
		
		Default_SSID="rtthread"; 
		StateFlag |= STATE_COMM_CONNECTING;  //��������״̬
		//if(WlanDirectConnect())    //ԭ�ȵ�SSID��Mac��ֱַ������ ��Ҫָ����ȷ��IP��ַ
		if(ConnectSSID(Default_SSID))  //��������SSID���ӵķ��� ͬ����Ҫָ����ȷ��IP��ַ
		{
			StateFlag |= STATE_COMM_FAIL;  //����ʧ�ܣ����ı�־λ
		}
		else
		{
			StateFlag &= ~STATE_COMM_FAIL;  //����������ȡ��ͨ��ʧЧ��־
		}
		StateFlag &= ~STATE_COMM_CONNECTING;  //������Ӳ���
		
		wlan_pm_exit();
		
		// init lwip system ע��һ��Ҫ��wlan����֮�󣬷�����wlan did not connect����
		lwip_sys_init();
		rt_kprintf("TCP/IP initialized!\n");
	}
#endif
}

//ɽè�ɿص������߳�
void LynxFly_init(void* parameter)
{
	rt_err_t result;  //��¼�̴߳����Ľ��
	
	rt_kprintf("LynxFly initializating now!\n");
	
	//��ʼ��ʱһֱ�������ֱ����ÿ���ָ���ִ�н������Ĵ���
	while(!PowerOnSign);
	
	rt_thread_delay(rt_tick_from_millisecond(500));  //˯�߰���ȴ�SD����ɳ�ʼ����Ҫ�����ĺ�
	
	//------- ��ʼ��LynxFly���̼߳�ͨ���ź���----------
	result = rt_sem_init(&IMUVarSem , "IMUVarSem", 1, RT_IPC_FLAG_FIFO);
	if (result != RT_EOK)
			rt_kprintf("%7d-LynxFly-->Sem IMUVarSem ERROR\n",rt_tick_get());
	result = rt_sem_init(&ControlJSSem , "ControlJSSem", 1, RT_IPC_FLAG_FIFO);
	if (result != RT_EOK)
			rt_kprintf("%7d-LynxFly-->Sem ControlJSSem ERROR\n",rt_tick_get());
	result = rt_sem_init(&ControlPPRYChannel , "ControlPPRYChannel", 1, RT_IPC_FLAG_FIFO);
	if (result != RT_EOK)
			rt_kprintf("%7d-LynxFly-->Sem ControlPPRYChannel ERROR\n",rt_tick_get());
	
	
	//------- ����LynxFly���߳�----------

	//����ɽè�ɿع��Ե����߳�
	result = rt_thread_init(&Thread_TaskIMU,
								 "TaskIMU",
								 TaskIMU,
								 RT_NULL,
								 &TaskIMU_Stack[0],
								 sizeof(TaskIMU_Stack),PRIORITY_TASK_IMU,rt_tick_from_millisecond(I_SAMPLETIME_CYCLE));
	if (result == RT_EOK)
	{
		rt_thread_startup(&Thread_TaskIMU);
		rt_kprintf("%7d-LynxFly-->TaskIMU Started!\n",rt_tick_get());
	}
	else
	{
		rt_kprintf("%7d-LynxFly-->TaskIMU Failed!\n",rt_tick_get());
	}
	
	//����ɽè�ɿؿ����߳�
	result = rt_thread_init(&Thread_TaskControl,
								 "TaskControl",
								 TaskControl,
								 RT_NULL,
								 &TaskControl_Stack[0],
								 sizeof(TaskControl_Stack),PRIORITY_TASK_CONTROL,rt_tick_from_millisecond(I_CONTROL_CYCLE));
	if (result == RT_EOK)
	{
		rt_thread_startup(&Thread_TaskControl);
		rt_kprintf("%7d-LynxFly-->TaskControl Started!\n",rt_tick_get());
	}
	else
	{
		rt_kprintf("%7d-LynxFly-->TaskControl Failed!\n",rt_tick_get());
	}
	
	//����ɽè�ɿ�֪ͨ�߳�
	result = rt_thread_init(&Thread_TaskNotify,
								 "TaskNotify",
								 TaskNotify,
								 RT_NULL,
								 &TaskNotify_Stack[0],
								 sizeof(TaskNotify_Stack),PRIORITY_TASK_NOTIFY,rt_tick_from_millisecond(I_NOTIFY_CYCLE));
	if (result == RT_EOK)
	{
		rt_thread_startup(&Thread_TaskNotify);
		rt_kprintf("%7d-LynxFly-->TaskNotify Started!\n",rt_tick_get());
	}
	else
	{
		rt_kprintf("%7d-LynxFly-->TaskNotify Failed!\n",rt_tick_get());
	}
	
	//����ɽè�ɿ�ͨ���߳�
	result = rt_thread_init(&Thread_TaskComm,
								 "TaskComm",
								 TaskComm,
								 RT_NULL,
								 &TaskComm_Stack[0],
								 sizeof(TaskComm_Stack),PRIORITY_TASK_COMM,rt_tick_from_millisecond(10));
	if (result == RT_EOK)
	{
		rt_thread_startup(&Thread_TaskComm);
		rt_kprintf("%7d-LynxFly-->TaskComm Started!\n",rt_tick_get());
	}
	else
	{
		rt_kprintf("%7d-LynxFly-->TaskComm Failed!\n",rt_tick_get());
	}
}


int rt_application_init()
{
  rt_thread_t tid;
	
	//����ϵͳ��ʼ״̬
	StateFlag |= STATE_COMM_FAIL;  //����ͨ��ģ���ʼΪʧЧ״̬
	PowerOnSign = 0;  //�����ʼ����·ɻ����򿪸��������ܽ����

  tid = rt_thread_create("init",
	rt_init_thread_entry, RT_NULL,
	2048, RT_THREAD_PRIORITY_MAX/3, 20);

	if (tid != RT_NULL)
			rt_thread_startup(tid);
	
	
	tid = rt_thread_create("LynxFly_init",
	LynxFly_init, RT_NULL,
	1024, RT_THREAD_PRIORITY_MAX/2, 20);

	if (tid != RT_NULL)
			rt_thread_startup(tid);
	
	

	return 0;
}

/*@}*/
