#ifndef CAN_MESSAGE_H
#define CAN_MESSAGE_H

#define RIGHT_FRONT ((uint8_t) 0u)
#define LEFT_FRONT  ((uint8_t) 1u)
#define RIGHT_REAR  ((uint8_t) 2u)
#define LEFT_REAR   ((uint8_t) 3u)

/* 8 byte na dane, do ustalenia */
void Can_Message_sendStepperMotorParams(const uint8_t wheel, const uint16_t data1, const uint16_t data2, const uint16_t data3, const uint16_t data4);

void Can_Message_sendBldcMotorParams(const uint8_t wheel, const uint16_t data1, const uint16_t data2, const uint16_t data3, const uint16_t data4);

void Can_Message_sendIncrementalEncoderInfo(const uint8_t wheel, const uint16_t dataChannelA, const uint16_t dataChannelB);

#endif /* CAN_MESSAGE_H */