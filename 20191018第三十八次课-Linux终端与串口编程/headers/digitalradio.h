/*
 * digitalradio.h
 *
 *  Created on: Oct 19, 2016
 *      Author: wangbo
 */

#ifndef HEADERS_DIGITALRADIO_H_
#define HEADERS_DIGITALRADIO_H_

//#define UART_RADIO 6
#define UART_RADIO "/dev/ttyUSB0"

#define UART_RADIO_BAUD 9600
#define UART_RADIO_DATABITS 8 //8 data bit
#define UART_RADIO_STOPBITS 1 //1 stop bit
#define UART_RADIO_PARITY 0 //no parity

#define RADIO_MAX_WAIT_TIME 6//[s]



int radio_uart_init();

/*
 * Function:read_radio_data
 * Description:ͨ����̨����cmd��waypoint��link��cte��parameter���ɵ���վ���͹���������
 *             ������������ݾ���ÿ������unsigned char�ͣ������Ҫ���Ǵ�С�ˣ�Ȼ����ʵ���н���λ��
 *             ����Ŀǰ�ĺ��������еľ��Ⱥ�γ��
 */
int read_radio_data(unsigned char *buf, unsigned int len);

/*
 * Function:send_radio_data
 * Description:ͨ����̨�������ݣ�Ŀǰ�Ƿ�����ʵʱ���ݣ������˻ش�������������˻ش�����
 */
int send_radio_data(unsigned char *buf, unsigned int len);

int radio_uart_close();



#endif /* HEADERS_DIGITALRADIO_H_ */
