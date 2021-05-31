/*
 * MessageBase.cpp
 *
 *  Created on: 2021年5月30日
 *      Author: 电脑
 */
extern "C" {
#include <stdlib.h>
#include "cJSON.h"
#include <string.h>
}
#include <Apps/Message.h>

namespace msg
{

Message receivedMotorSpeed(2, 10);
Message receivedMotorDistance(2, 11);
Message receivedServoAngle(2, 12);

Message sentMotorSpeed(2, 20);
Message sentMotorDistance(2, 21);
Message sentIMUAngle(3, 22);
Message sentChart(4, 23);

const uint8_t rcvMsgNum = 3;
Message* receivedMessages[rcvMsgNum] = {&receivedMotorSpeed,&receivedMotorDistance,&receivedServoAngle};
const uint8_t sentMsgNum = 4;
Message* sentMessages[sentMsgNum] = {&sentMotorSpeed,&sentMotorDistance,&sentIMUAngle,&sentChart};


Message::Message(uint8_t dataNum, uint8_t type)
{
    this->dataNum = dataNum;
    data = new float[dataNum]();
    this->type = type;
}

Message::~Message()
{
    delete[] data;
}

void Message::ParseJson(char* strJson)
{
    cJSON* root = cJSON_Parse(strJson);
    if(root != 0) //转换成功
    {
        cJSON* typeJson = cJSON_GetObjectItem(root,"t");//获得消息类型
        if(typeJson != 0)
        {
            for(uint8_t i = 0; i < rcvMsgNum; ++i) //寻找消息类型对应的Message对象
            {
                if(typeJson->valueint == receivedMessages[i]->type)
                {
                    cJSON* dataJson = cJSON_GetObjectItem(root,"d");
                    if(cJSON_GetArraySize(dataJson) == receivedMessages[i]->dataNum) //判断数组长度是否相等
                    {
                        for(uint8_t j = 0; j < receivedMessages[i]->dataNum; ++j) //获取data数组
                        {
                            receivedMessages[i]->data[j] = cJSON_GetArrayItem(dataJson,j)->valuedouble;
                        }
                    }
                }
            }
        }
        cJSON_Delete(root);
    }
}

void Message::PackJson(char* send)
{
    cJSON* root = cJSON_CreateObject();

    cJSON_AddNumberToObject(root, "t", type);
    cJSON_AddItemToObject(root, "d", cJSON_CreateFloatArray(data,dataNum));

    char* msg = cJSON_PrintUnformatted(root);
    strcpy(send,msg);

    free(msg);
    cJSON_Delete(root);
}
}
