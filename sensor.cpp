#include "sensor.h"

sensor::sensor()
{
}

// void caculateXORAndUpload(uint8_t *dataBuf, uint8_t bufLen)
// {
//     uint8_t xorValue = 0;
//     dataBuf[2]  = bufLen; // 重新计算一下长度！
//     for (uint32_t i = 0; i < bufLen - 2; i++)
//     {
//         xorValue ^= dataBuf[i];
//     }
//     dataBuf[bufLen - 2] = xorValue;
//     comSendBuf(USART2, dataBuf, bufLen);
// }

#define COMBUFSIZE 128
typedef struct
{
    uint8_t receBuf[COMBUFSIZE];
    uint16_t pReceWrite;
    uint16_t pReceRead;
} _com2DataStr;
_com2DataStr com2DataStr;

uint8_t tmpBuf[256];         // 最大处理报文：256，使用全局变量！
uint32_t bufferValidDataLen; // 串口数据中有效数据
uint8_t frameLen;            // 解析当前帧数据长度
uint8_t checkValue;
uint8_t isDataOK = 0;

void comProcess(void)
{
    do
    { // 第一步找最小的数据长度：55 AA nLen 是否需要检验 命令字 校验 0x5A 也就是长度最少：7
        // nLen：为这一帧的总长度：这样程序方面也比较好操作！
        // 校验字：为0x55开头到校验字之前的异或校验，也就是除了最后2个字节之前的校验！
        bufferValidDataLen =
            ((com2DataStr.pReceWrite + COMBUFSIZE) - com2DataStr.pReceRead) % COMBUFSIZE; // buffer中有效数据长度
        if (bufferValidDataLen >= 7)
        {
            if (((com2DataStr.receBuf[(com2DataStr.pReceRead)]) == 0x55) &&
                ((com2DataStr.receBuf[(com2DataStr.pReceRead + 1) % COMBUFSIZE]) == 0xAA))
            {                                                                             // 符合报文头
                frameLen = com2DataStr.receBuf[(com2DataStr.pReceRead + 2) % COMBUFSIZE]; // 这帧总的长度
                if (frameLen <= bufferValidDataLen)
                { // 长度满足，可以开始解析数据
                    for (uint32_t i = 0; i < frameLen; i++)
                    { // 其实前3个数据可以不需要了，但是为了保持一致性！还是都复制出来！
                        tmpBuf[i] = com2DataStr.receBuf[(com2DataStr.pReceRead + i) % COMBUFSIZE];
                    }
                    {                 // 处理数据
                        isDataOK = 0; // 先假定不符合校验
                        if (tmpBuf[frameLen - 1] == 0x5A)
                        { // 报文尾巴符合
                            if (tmpBuf[3] != 0)
                            { // 需要检验字
                                checkValue = 0;
                                for (uint32_t j = 0; j < frameLen - 2; j++)
                                {
                                    checkValue = checkValue ^ tmpBuf[j];
                                }
                                if (checkValue == tmpBuf[frameLen - 2])
                                {
                                    isDataOK = 1;
                                }
                                else
                                {
                                } // 保持不变就好！
                            }
                            else
                            {
                                isDataOK = 1;
                            }
                            { //
                                if (isDataOK == 1)
                                {     // 可以开始正常处理数据
                                    { // 先把这一帧的数据处理掉
                                        com2DataStr.pReceRead += frameLen;
                                        com2DataStr.pReceRead %= COMBUFSIZE;
                                    }
                                    switch (tmpBuf[4])
                                    {       // 这里就可以开始处理数据了！
                                    case 0: // 验证性消息
                                        break;
                                    case 1: // 串口波特率，设置与查询！
                                        break;
                                    case 2: // 升级相关！
                                        break;
                                    default:
                                        break;
                                    }
                                }
                                else
                                { // 不符合的情况下：去除掉一个会比较稳定，但是去除掉一帧，其实当然也是可以的！
                                    // 去除掉一个，显得特别稳定，当然会存在性能的问题。关于性能可以这样考虑：出错的毕竟少，影响有限。
                                    // 如果不专门分析和提升性能，盲目的优化性能，会带来不稳定，当前情况下，显然是不合适的！
                                    com2DataStr.pReceRead++; // 这里可以直接+2，因为55 aa 移动一个位置也不会是头
                                    com2DataStr.pReceRead %= COMBUFSIZE;
                                }
                            }
                        }
                        else
                        {                            // 报文尾不符合，怎么操作：应该是移动一个位置！
                            com2DataStr.pReceRead++; // 这里可以直接+2，因为55 aa 移动一个位置也不会是头
                            com2DataStr.pReceRead %= COMBUFSIZE;
                        }
                    }
                }
                else
                { // 长度不满足，需要继续接收数据！
                    // 注意：此情况下，里面会一起循环，所以需要结束循环，尤其在电脑上。否则会一直卡循环！
                    // 而作为STM32，可以等待中断数据，在里面处理。然后处理完结。
                    break; // 结束里面的循环，不用等数据全部到齐，可以有序的做其他事情！
                }
            }
            else
            { // 不符合0x55 0xAA开头，此时移动一个位置
                com2DataStr.pReceRead++;
                com2DataStr.pReceRead %= COMBUFSIZE;
            }
        }
        else
        {
        }; // 长度不满足，需要继续接收数据！
    } while ((((com2DataStr.pReceWrite + COMBUFSIZE) - com2DataStr.pReceRead) % COMBUFSIZE) >= 7);
}

/**
 * @brief
 * @param cmd
 * @param payload
 * @param enableCheck
 * @return
 */
std::vector<uint8_t> buildCommand(uint8_t cmd, const std::vector<uint8_t> &payload, bool enableCheck)
{
    std::vector<uint8_t> frame;

    // ===== 帧头 =====
    frame.push_back(0x55);
    frame.push_back(0xAA);

    // 预留 LEN（后面回填）
    frame.push_back(0x00);

    // ===== 协议字段 =====
    frame.push_back(enableCheck ? 0x01 : 0x00);
    frame.push_back(cmd);

    // DATA
    frame.insert(frame.end(), payload.begin(), payload.end());

    // ===== 计算 LEN =====
    // LEN = 从 LEN 本身到 XOR
    uint8_t len = static_cast<uint8_t>(1 +                // LEN 本身
                                       1 +                // CHECK_EN
                                       1 +                // CMD
                                       payload.size() + 1 // XOR
    );
    frame[2]    = len;

    // ===== XOR 校验 =====
    // uint8_t xorVal = calcXor(&frame[2], len - 1); // 不含 XOR 本身
    frame.push_back(0x00);

    // ===== 帧尾 =====
    frame.push_back(0x5A);

    return frame;
}