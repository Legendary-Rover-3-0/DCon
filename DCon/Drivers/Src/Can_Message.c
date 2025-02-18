#include "main.h"
#include "can.h"
#include "Can_Driver.h"

#ifdef DLT_ENABLE
    #include "DLTuc.h"
    #define DLT_DEBUG(...) LOGL(DL_INFO, __VA_ARGS__)
#else
    #define DLT_DEBUG(...)
#endif
#include "Can_Message.h"

/* ID ramki dla parametrów silnika krokowego */
#define ID_RIGHT_FRONT_STEPPERMOTOR  ((uint32_t) 100u)
#define ID_LEFT_FRONT_STEPPERMOTOR   ((uint32_t) 101u)
#define ID_RIGHT_REAR_STEPPERMOTOR   ((uint32_t) 102u)
#define ID_LEFT_REAR_STEPPERMOTOR    ((uint32_t) 103u)

/* ID ramki dla parametrów silnika bldc */
#define ID_RIGHT_FRONT_BLDC          ((uint32_t) 200u)
#define ID_LEFT_FRONT_BLDC           ((uint32_t) 201u)
#define ID_RIGHT_REAR_BLDC           ((uint32_t) 202u)
#define ID_LEFT_REAR_BLDC            ((uint32_t) 203u)

/* ID ramki dla parametrów endodera inkrementalnego */
#define ID_RIGHT_FRONT_INC_ENCODER   ((uint32_t) 300u)
#define ID_LEFT_FRONT_INC_ENCODER    ((uint32_t) 301u)
#define ID_RIGHT_REAR_INC_ENCODER    ((uint32_t) 302u)
#define ID_LEFT_REAR_INC_ENCODER    ((uint32_t) 303u)

/* ID ramki dla napięcia na baterii */
#define ID_RIGHT_FRONT_BATTERY_VOLTAGE   ((uint32_t) 400u)
#define ID_LEFT_FRONT_BATTERY_VOLTAGE   ((uint32_t) 401u)
#define ID_RIGHT_REAR_BATTERY_VOLTAGE   ((uint32_t) 402u)
#define ID_LEFT_REAR_BATTERY_VOLTAGE    ((uint32_t) 403u)


#define DATA_LENGTH_8_BYTES          ((uint32_t) 8u)
#define DATA_LENGTH_4_BYTES          ((uint32_t) 4u)
#define DATA_LENGTH_2_BYTES          ((uint32_t) 2u)
#define AMOUT_OF_WHEELS              (4u)
#define AMOUT_OF_PARMAS              (4u)

typedef enum
{
    STEPPER_MOTOR = 0,
    BLDC,
    INC_ENCODER,
    BATTERY_VOLTAGE
}Objectparam_Type;

const uint32_t messageIdArray[AMOUT_OF_WHEELS][AMOUT_OF_PARMAS] =
{
    [RIGHT_FRONT] = {ID_RIGHT_FRONT_STEPPERMOTOR, ID_RIGHT_FRONT_BLDC, ID_RIGHT_FRONT_INC_ENCODER, ID_RIGHT_FRONT_BATTERY_VOLTAGE},
    [LEFT_FRONT]  = {ID_LEFT_FRONT_STEPPERMOTOR,  ID_LEFT_FRONT_BLDC,  ID_LEFT_FRONT_INC_ENCODER,  ID_LEFT_FRONT_BATTERY_VOLTAGE },
    [RIGHT_REAR]  = {ID_RIGHT_REAR_STEPPERMOTOR,  ID_RIGHT_REAR_BLDC,  ID_RIGHT_REAR_INC_ENCODER,  ID_RIGHT_REAR_BATTERY_VOLTAGE },
    [LEFT_REAR]   = {ID_LEFT_REAR_STEPPERMOTOR,   ID_LEFT_REAR_BLDC,   ID_LEFT_REAR_INC_ENCODER,   ID_LEFT_REAR_BATTERY_VOLTAGE  },
};

static uint32_t getMessageId(const uint8_t wheel, Objectparam_Type param)
{
    uint32_t retId = 0xFFFFFFFF;

    if ((wheel < AMOUT_OF_WHEELS) && (param < AMOUT_OF_PARMAS))
    {
        retId = messageIdArray[wheel][param];
    }

    return retId;
}

void Can_Message_sendStepperMotorParams(const uint8_t wheel, const uint16_t data1, const uint16_t data2, const uint16_t data3, const uint16_t data4)
{
    CAN_TxHeaderTypeDef header =
    {
        .ExtId = 0u,
        .IDE = CAN_ID_STD,
        .RTR = CAN_RTR_DATA,
        .DLC = DATA_LENGTH_8_BYTES,
        .TransmitGlobalTime = DISABLE
    };
    header.StdId = getMessageId(wheel, STEPPER_MOTOR);
    uint8_t data[DATA_LENGTH_8_BYTES] = {0};
    data[0] = (data1 >> 8);  
    data[1] = (data1 & 0xFF);
    data[2] = (data2 >> 8);  
    data[3] = (data2 & 0xFF);
    data[4] = (data3 >> 8);  
    data[5] = (data3 & 0xFF);
    data[6] = (data4 >> 8);  
    data[7] = (data4 & 0xFF);
    uint32_t pTxMailbox = 0u;

    Can_Driver_wakeup();

    if (HAL_OK != HAL_CAN_AddTxMessage(CAN_INSTANCE, &header, data, &pTxMailbox))
    {
        DLT_DEBUG("Can message error!");
    }
}

void Can_Message_sendBldcMotorParams(const uint8_t wheel, const uint16_t data1, const uint16_t data2, const uint16_t data3, const uint16_t data4)
{
    CAN_TxHeaderTypeDef header =
    {
        .ExtId = 0u,
        .IDE = CAN_ID_STD,
        .RTR = CAN_RTR_DATA,
        .DLC = DATA_LENGTH_8_BYTES,
        .TransmitGlobalTime = DISABLE
    };
    header.StdId = getMessageId(wheel, BLDC);
    uint8_t data[DATA_LENGTH_8_BYTES] = {0};
    data[0] = (data1 >> 8);  
    data[1] = (data1 & 0xFF);
    data[2] = (data2 >> 8);  
    data[3] = (data2 & 0xFF);
    data[4] = (data3 >> 8);  
    data[5] = (data3 & 0xFF);
    data[6] = (data4 >> 8);  
    data[7] = (data4 & 0xFF);
    uint32_t pTxMailbox = 0u;

    Can_Driver_wakeup();

    if (HAL_OK != HAL_CAN_AddTxMessage(CAN_INSTANCE, &header, data, &pTxMailbox))
    {
        DLT_DEBUG("Can message error!");
    }
}

void Can_Message_sendIncrementalEncoderInfo(const uint8_t wheel, const uint16_t dataChannelA, const uint16_t dataChannelB)
{
    CAN_TxHeaderTypeDef header =
    {
        .ExtId = 0u,
        .IDE = CAN_ID_STD,
        .RTR = CAN_RTR_DATA,
        .DLC = DATA_LENGTH_4_BYTES,
        .TransmitGlobalTime = DISABLE
    };
    header.StdId = getMessageId(wheel, INC_ENCODER);
    uint8_t data[DATA_LENGTH_4_BYTES] = {0};
    data[0] = (dataChannelA >> 8);  
    data[1] = (dataChannelA & 0xFF);
    data[2] = (dataChannelB >> 8);  
    data[3] = (dataChannelB & 0xFF);
    uint32_t pTxMailbox = 0u;

    Can_Driver_wakeup();

    if (HAL_OK != HAL_CAN_AddTxMessage(CAN_INSTANCE, &header, data, &pTxMailbox))
    {
        DLT_DEBUG("Can message error!");
    }
}

void Can_Message_sendBatteryVoltage(const uint8_t wheel, const uint16_t voltage)
{
    CAN_TxHeaderTypeDef header =
    {
        .ExtId = 0u,
        .IDE = CAN_ID_STD,
        .RTR = CAN_RTR_DATA,
        .DLC = DATA_LENGTH_2_BYTES,
        .TransmitGlobalTime = DISABLE 
    };
    header.StdId = getMessageId(wheel, BATTERY_VOLTAGE);
    uint8_t data[DATA_LENGTH_2_BYTES] = {0};
    data[0] = (voltage >> 8);  
    data[1] = (voltage & 0xFF);
    uint32_t pTxMailbox = 0u;

    Can_Driver_wakeup();

    if (HAL_OK != HAL_CAN_AddTxMessage(CAN_INSTANCE, &header, data, &pTxMailbox))
    {
        DLT_DEBUG("Can message error!");
    }
}
