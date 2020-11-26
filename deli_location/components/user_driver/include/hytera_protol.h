
#ifndef __HYTERA_PROTOL_
#define __HYTERA_PROTOL_

#include "esp_system.h"
#include "user_main.h"

#define SET_CMD     (0x0242)
#define REPLY_CMD   (0x8242)

#define OP_GET      (0x00)
#define OP_SET      (0x01)

#define R_SUCCESS   (0x0000)
#define R_FAILURE   (0x0001)
#define R_INVALID   (0x8000)

#define SUCCESS     (0x00)
#define FAILURE     (0x01)

#define NORTH       'N' // "+"
#define SOUTH       'S' // "-"
#define EAST        'E' // "+"
#define WEST        'W' // "-"

#define MSG_HDR     (0x02)
#define MSG_END     (0x03)
#define MSG_LEN     (36)

/*----------------typedef Hytera protocol define start------------------*/
#define HYTERA_HEADER_ID    0x7E
#define HYTERA_VERSION      0x04
#define HYTERA_BLOCK        0x00
// opcode start
#define HYTERA_CONNECT      0xFE
#define HYTERA_ACCEPT       0xFD
#define HYTERA_REJECT       0xFC
#define HYTERA_CLOSE        0xFB
#define HYTERA_CLOSE_ACK    0xFA
#define HYTERA_DATA         0x00
#define HYTERA_DATA_ACK     0x10

#define HYTERA_SCRID        0x20
#define HYTERA_DESID        0x10


#define BigtoLittle16(A) (( ((uint16_t)(A) & 0xff00) >> 8) | (( (uint16_t)(A) & 0x00ff) << 8))
#define BigtoLittle32(A) ((( (uint32_t)(A) & 0xff000000) >> 24) | (( (uint32_t)(A) & 0x00ff0000) >> 8) | (( (uint32_t)(A) & 0x0000ff00) << 8) | (( (uint32_t)(A) & 0x000000ff) << 24))


/*----------------typedef Hytera protocol define end------------------*/

#pragma pack(push)
#pragma pack(1)

typedef struct _send_data_
{
    uint8_t msgHdr;
    uint16_t cmd;    // 命令码
    uint16_t length;
    uint8_t operat;  // 操作符
    uint16_t result; // 结果
    uint8_t time[6]; // 时分秒
    uint8_t data[6]; // 年月日
    uint8_t NSFlag;  // 
    uint8_t latitude[9]; // 
    uint8_t EWFlag;
    uint8_t longitude[10];
    uint8_t checksum;
    uint8_t msgEnd;
}tSendUartData;


/*----------------typedef Hytera protocol start------------------*/

typedef struct _hytera_pro_unit_
{
    uint8_t hy_headerId;
    uint8_t hy_version;
    uint8_t hy_block;
    uint8_t hy_opCode;
    uint8_t hy_srcId;
    uint8_t hy_desId;
    uint16_t hy_pn;
    uint16_t hy_length;
    uint16_t hy_checkSum;
}tHyteraProUnit;

typedef struct _hytera_pro_req_
{
    tHyteraProUnit hy_header;
    tSendUartData hy_payload;
}tHyteraProReq;

typedef struct _hytera_pro_rep_
{
    tHyteraProUnit hy_header;
    uint8_t msgHdr;
    uint16_t cmd;
    uint16_t length;
    uint8_t result;
    uint8_t check;
    uint8_t msgEnd;
}tHyteraProRep;

/*----------------typedef Hytera protocol end------------------*/



#pragma pack(pop)

void connect_hytera(void);
void send_data_hytera(char *data, uint8_t length);
void send_data_ack_hytera(uint16_t thy_pn);
void close_hytera(void);
bool data_check(char *data, int len, uint16_t crc);
extern tSendUartData tUartSendData;
extern uint16_t gHy_pn;
extern uint8_t checksum01(tSendUartData data);
#endif


