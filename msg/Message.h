/*
 * MessageBase.h
 *
 *  Created on: 2021年5月30日
 *      Author: 电脑
 */

#ifndef APPS_MESSAGE_H_
#define APPS_MESSAGE_H_

extern "C" {
#include <stdint.h>
}
namespace msg
{
class Message
{
public:
    Message(uint8_t dataNum, uint8_t type);
    virtual ~Message();
    
    void PackJson(char* send);
    float* data;

    static void ParseJson(char* strJson);

    uint8_t type;
private:
    uint8_t dataNum;

};

extern Message receivedMotorSpeed;

}
#endif /* APPS_MESSAGE_H_ */
