#ifndef CAN_MESSAGE_H
#define CAN_MESSAGE_H

/* 8 byte na dane, do ustalenia */
void Can_Message_sendStepperMotorParams(const uint8_t direction, const uint8_t enable, const uint8_t ms1, const uint8_t ms2, const uint8_t ms3);

void Can_Message_sendBldcMotorParams(const uint16_t data1, const uint16_t data2, const uint16_t data3, const uint16_t data4);

void Can_Message_sendIncrementalEncoderInfo(const uint16_t dataChannelA, const uint16_t dataChannelB);

void Can_Message_sendBatteryVoltage(const uint16_t voltage);

#endif /* CAN_MESSAGE_H */