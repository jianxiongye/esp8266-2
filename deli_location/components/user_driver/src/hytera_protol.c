

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "esp_log.h"
#include "user_uart.h"
#include "hytera_protol.h"


tSendUartData tUartSendData = {0};
static tHyteraProReq tHyteraProData = {0};
uint16_t gHy_pn = 1;

//CONFIG_WIFI_SSID="Hytera-Guest-PK"
//CONFIG_WIFI_PASSWORD="hyt616616"

uint16_t checksum(uint16_t * buffer, int size)
{
    volatile uint16_t temp = 0;
    volatile uint16_t answer = 0;
    volatile unsigned long cksum = 0;

    while (size > 1)
    {
        answer = *buffer;
        temp = ~answer;
        cksum += temp;
        buffer++;
        size = size - sizeof(uint16_t);
    }

    if (size)
    {
        answer = *buffer;
        temp = ~answer;
        cksum += temp;
    }

    while(cksum >> 16)
    {
        cksum = (cksum >> 16) + (cksum & 0xffff);
    }

    answer = (uint16_t)cksum;

    return answer;
}

uint8_t checksum01(tSendUartData data)
{
    uint8_t i = 0;
    uint8_t sum = 0;
    uint8_t *buf = (uint8_t *)&data;

    for (i = 1; i < sizeof (tSendUartData) - 2; i ++)
    {
        sum += buf[i];
    }
    sum = ~sum + 0x33;
    return sum;
}

void connect_hytera(void)
{
    uint8_t len = sizeof (tHyteraProUnit);
    memset(&tHyteraProData, 0, sizeof(tHyteraProData));
    tHyteraProData.hy_header.hy_headerId = HYTERA_HEADER_ID;
    tHyteraProData.hy_header.hy_version = HYTERA_VERSION;
    tHyteraProData.hy_header.hy_block = HYTERA_BLOCK;
    tHyteraProData.hy_header.hy_opCode = HYTERA_CONNECT;
    tHyteraProData.hy_header.hy_srcId = HYTERA_SCRID;
    tHyteraProData.hy_header.hy_desId = HYTERA_DESID;
    tHyteraProData.hy_header.hy_pn = 0x00;
    tHyteraProData.hy_header.hy_length = BigtoLittle16(len);
    tHyteraProData.hy_header.hy_checkSum = checksum((uint16_t *)&tHyteraProData, len);

    char *buf = (char *)&tHyteraProData;
#if OPEN_PRINTF
    printf("-------connect_hytera start---------\r\n");
    for (int i = 0; i < len; i ++)
    {
        printf("%02X ", buf[i]);
    }
    printf("\r\n");
    printf("-------connect_hytera end---------\r\n");
#endif
    uart_send_data(buf, len);
    uart_send_data("\r\n", 2);
}

void send_data_ack_hytera(uint16_t thy_pn)
{
    //char buf[] = {0x7E, 0x04, 0x00, 0x10, 0x10, 0x20, 0x00, 0x01, 0x00, 0x0C, 0x71, 0xBE};

    uint8_t len = sizeof (tHyteraProUnit);
    memset(&tHyteraProData, 0, sizeof(tHyteraProData));
    tHyteraProData.hy_header.hy_headerId = HYTERA_HEADER_ID;
    tHyteraProData.hy_header.hy_version = HYTERA_VERSION;
    tHyteraProData.hy_header.hy_block = HYTERA_BLOCK;
    tHyteraProData.hy_header.hy_opCode = HYTERA_DATA_ACK;
    tHyteraProData.hy_header.hy_srcId = HYTERA_SCRID;
    tHyteraProData.hy_header.hy_desId = HYTERA_DESID;
    tHyteraProData.hy_header.hy_pn = BigtoLittle16(thy_pn);
    tHyteraProData.hy_header.hy_length = BigtoLittle16(len);
    tHyteraProData.hy_header.hy_checkSum = checksum((uint16_t *)&tHyteraProData, len);

    char *buf = (char *)&tHyteraProData;
#if OPEN_PRINTF
    printf("-------connect_hytera start---------\r\n");
    for (int i = 0; i < len; i ++)
    {
        printf("%02X ", buf[i]);
    }
    printf("\r\n");
    printf("-------connect_hytera end---------\r\n");
#endif
    uart_send_data(buf, len);
    uart_send_data("\r\n", 2);
}

void send_data_hytera(char *data, uint8_t length)
{
    uint8_t len = sizeof (tHyteraProData);
    memset(&tHyteraProData, 0, sizeof(tHyteraProData));
    tHyteraProData.hy_header.hy_headerId = HYTERA_HEADER_ID;
    tHyteraProData.hy_header.hy_version = HYTERA_VERSION;
    tHyteraProData.hy_header.hy_block = HYTERA_BLOCK;
    tHyteraProData.hy_header.hy_opCode = HYTERA_DATA;
    tHyteraProData.hy_header.hy_srcId = HYTERA_SCRID;
    tHyteraProData.hy_header.hy_desId = HYTERA_DESID;
    tHyteraProData.hy_header.hy_pn = BigtoLittle16(gHy_pn);
    tHyteraProData.hy_header.hy_length = BigtoLittle16(len);
    //tHyteraProData.hy_header.hy_checkSum = BigtoLittle16(checksum((uint16_t *)&tHyteraProData, len));
    memcpy(&tHyteraProData.hy_payload, data, length);
    tHyteraProData.hy_header.hy_checkSum = checksum((uint16_t *)&tHyteraProData, len);
    char *buf = (char *)&tHyteraProData;
#if OPEN_PRINTF
    printf("-------send_data_hytera start---------\r\n");
    for (int i = 0; i < len; i ++)
    {
        printf("%02X ", buf[i]);
    }
    printf("\r\n");
    printf("-------send_data_hytera end---------\r\n");
#endif
    uart_send_data(buf, len);
    uart_send_data("\r\n", 2);
}

void close_hytera(void)
{
    uint8_t len = sizeof (tHyteraProUnit);
    memset(&tHyteraProData, 0, sizeof(tHyteraProData));
    tHyteraProData.hy_header.hy_headerId = HYTERA_HEADER_ID;
    tHyteraProData.hy_header.hy_version = HYTERA_VERSION;
    tHyteraProData.hy_header.hy_block = HYTERA_BLOCK;
    tHyteraProData.hy_header.hy_opCode = HYTERA_CLOSE;
    tHyteraProData.hy_header.hy_srcId = HYTERA_SCRID;
    tHyteraProData.hy_header.hy_desId = HYTERA_DESID;
    tHyteraProData.hy_header.hy_pn = 0x00;
    tHyteraProData.hy_header.hy_length = BigtoLittle16(len);
    tHyteraProData.hy_header.hy_checkSum = checksum((uint16_t *)&tHyteraProData, len);
    char *buf = (char *)&tHyteraProData;
#if OPEN_PRINTF
    printf("-------close_hytera start---------\r\n");
    for (int i = 0; i < len; i ++)
    {
        printf("%02X ", buf[i]);
    }
    printf("\r\n");
    printf("-------close_hytera end---------\r\n");
#endif
    uart_send_data(buf, len);
    uart_send_data("\r\n", 2);
}


bool data_check(char *dataBuffer, int len, uint16_t crc)
{
    dataBuffer[10] = 0x00;
    dataBuffer[11] = 0x00;
    uint16_t result = checksum((uint16_t *)dataBuffer, len);
    ESP_LOGI("hytera_protol", "--result: %04x---crc: %04x-\r\n", result, crc);
    if (crc == result)
    {
        return true;
    }
    return false;
}
