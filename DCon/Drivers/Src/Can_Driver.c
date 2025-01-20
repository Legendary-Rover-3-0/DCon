/**
  ******************************************************************************
  * @file    Can_Driver.c
  * @brief   Driver dla układu trancievera SN65HVD230 
  ******************************************************************************
  * @author Mateusz Kudyba
  ******************************************************************************
  */
/**
 * @brief SN65HVD230QDR.
 * 
 * | Pin | Nazwa | Kierunek     | Opis                                                                               |
 * |-----|-------|--------------|------------------------------------------------------------------------------------|
 * | 1   | D     | Wejście      | Pin wejściowy odbierający dane z kontrolera CAN (połączenie z TX kontrolera CAN).  |
 * | 2   | R     | Wyjście      | Pin wyjściowy przesyłający dane do kontrolera CAN (połączenie z RX kontrolera CAN).|
 * | 3   | Vcc   | Zasilanie    | Pin zasilający transceiver (typowe napięcie: 3.3 V, maksymalne: 6 V).              |
 * | 4   | GND   | Masa         | Masa referencyjna transceivera, połączona z masą płytki PCB.                       |
 * | 5   | Rs    | Wejście      | Pin sterujący trybem pracy:                                                        |
 * |     |       |              | - Niska rezystancja (<10 kΩ): Tryb normalny (wysoka prędkość).                     |
 * |     |       |              | - Średnia rezystancja (10 kΩ - 100 kΩ): Kontrola nachylenia zbocza.                |
 * |     |       |              | - Wysoka rezystancja (>100 kΩ): Tryb standby.                                      |
 * | 6   | CANL  | Dwukierunkowy| Linia CAN niskiego poziomu (podłączenie do linii CANL magistrali CAN).             |
 * | 7   | CANH  | Dwukierunkowy| Linia CAN wysokiego poziomu (podłączenie do linii CANH magistrali CAN).            |
 * | 8   | NC    | Niepodłączony| Pin niepodłączony (No Connect).                                                    |
 */

#include "main.h"
#include "can.h"
#include "Can_Driver.h"

#ifdef DLT_ENABLE
    #include "DLTuc.h"
    #define DLT_DEBUG(...) LOGL(DL_INFO, __VA_ARGS__)
#else
    #define DLT_DEBUG(...)
#endif

#define CAN_WAKEUP_DELAY_MS ((uint32_t) 1u)
#define CAN_ACTIVE          ((uint8_t)  0u)
#define CAN_STANDBY         ((uint8_t)  1u)
#define CAN_IDLE_TIMEOUT    ((uint32_t) 1000u)

typedef enum
{
    CAN_STATE_ACTIVE = 0,
    CAN_STATE_GO_STANDBY,
    CAN_STATE_STANDBY,
}CanDriver_StateType;

static CanDriver_StateType state = CAN_STATE_ACTIVE; /* Zminna odpowiadająca za stan maszyny */
static volatile uint8_t canFlag = 0u; /* Flaga ustawiana w przerwaniu HAL informująca o ruchu na magistrali */
static volatile uint32_t lastRxTime = 0u;

static void setStandbyMode(void)
{
    /* Jeśli na pinie RS jest stan wysoki tranciver pracuje w stanie standby */
    HAL_GPIO_WritePin(CAN_RS_GPIO_Port, CAN_RS_Pin, GPIO_PIN_SET);
}

static void setActiveMode(void)
{
    /* Jeśli na pinie RS jest stan niski tranciver pracuje w stanie active */
    HAL_GPIO_WritePin(CAN_RS_GPIO_Port, CAN_RS_Pin, GPIO_PIN_RESET);
}

static void switchState(const CanDriver_StateType newState)
{
    /* Setter dla zmiennej state */
    state = newState;
}

static CanDriver_StateType getState(void)
{
    /* Getter dla zmiennej state */
    return state;
}

static void canWakeUp(void)
{
    HAL_CAN_WakeUp(CAN_INSTANCE);
}

void HAL_CAN_SleepCallback(CAN_HandleTypeDef *hcan)
{
    if (hcan == CAN_INSTANCE)
    {
        /* Callback od wykrycia braku ruchu na magistrali CAN, ustawinie flagi wymaganej do przejścia w standby */
        canFlag = CAN_STANDBY;
    }
}

void HAL_CAN_WakeUpFromRxMsgCallback(CAN_HandleTypeDef *hcan)
{
    if (hcan == CAN_INSTANCE)
    {
        /* Callback od wykrycia ruchu na magistrali CAN, ustawinie flagi wymaganej do przejścia w active */
        canFlag = CAN_ACTIVE;
    }
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    if (hcan == CAN_INSTANCE)
    {
        lastRxTime = HAL_GetTick(); /* Zapisz czas ostatniej odebranej ramki */
    }
}

HAL_StatusTypeDef Can_Driver_init(void)
{
    HAL_StatusTypeDef retStatus = HAL_OK;

    setActiveMode(); /* Ustawienie stanu active przy starcie systemu */
    if ((retStatus = HAL_CAN_ActivateNotification(CAN_INSTANCE, CAN_IT_WAKEUP | CAN_IT_SLEEP_ACK | CAN_IT_RX_FIFO0_MSG_PENDING)) != HAL_OK) /* Włączenie przerwań */
    {
        DLT_DEBUG("Can notify error!");
        return retStatus;
    }

    if ((retStatus = HAL_CAN_Start(CAN_INSTANCE)) != HAL_OK) /* Start magistarli */
    {
        DLT_DEBUG("Can start error!");
        return retStatus;
    }

    DLT_DEBUG("Can driver init! Can mode:ACTIVE");
    return retStatus;

    return retStatus;
}

void Can_Driver_wakeup(void)
{
    const CanDriver_StateType currState = getState();
    uint32_t startTime = 0u;

    if (currState != CAN_STATE_ACTIVE)
    {
        setActiveMode();
        canWakeUp();
        startTime = HAL_GetTick();
        while ((HAL_GetTick() - startTime) < CAN_WAKEUP_DELAY_MS)
        {
            __NOP();
        }
        switchState(CAN_STATE_ACTIVE);
        canFlag = CAN_ACTIVE;
        DLT_DEBUG("Can mode:WAKEUP");
    }
}

static uint32_t checkAllMailBoxes(void)
{
    uint32_t isPending = 0u;
    uint8_t mailboxIdx = 1u;

    for (; (mailboxIdx < (CAN_TX_MAILBOX2 << 1)); mailboxIdx <<= 1)
    {
        /* Iteracja po CAN_TX_MAILBOX0, CAN_TX_MAILBOX1, CAN_TX_MAILBOX2 */
        isPending |= HAL_CAN_IsTxMessagePending(CAN_INSTANCE, mailboxIdx);
    }

    return isPending;
}

static uint32_t checkCanIdle(void)
{
    uint32_t isIdle = 0u;
    uint32_t currTime = HAL_GetTick();

    if ((currTime - lastRxTime) >= CAN_IDLE_TIMEOUT)
    {
        lastRxTime = 0u;
    }
    else
    {
        isIdle = 1u;
    }

    return isIdle;
}

void Can_Driver_stateMenager(void)
{
    switch (state)
    {
        case CAN_STATE_ACTIVE:
            /* Tranciver jest w stanie aktywnym i sprawdza czy wartość flagi została zminiona */
            /* Jeśli kontroler wykrył brak ruchu na CAN wystawia odpowiednie przerwanie w którym ustawiana jest flaga */
            if ((checkCanIdle() == 0u) && (checkAllMailBoxes() == 0u))
            {
                if (HAL_CAN_RequestSleep(CAN_INSTANCE) != HAL_OK)
                {
                    DLT_DEBUG("Failed sleep request!");
                    return;
                }
            }

            if (canFlag == CAN_STANDBY)
            {
                /* Jeśli flaga ustawiona jest na CAN_STANDBY, należy przejść do stanu CAN_STATE_GO_STANDBY */
                switchState(CAN_STATE_GO_STANDBY);
            }
            break;

        case CAN_STATE_GO_STANDBY:
            /* Jeśli flaga ustawiona jest na CAN_STANDBY, należy przejść do wywołać funkcję ustawijącą pin RS dla trancivera i przejść do
            stanu CAN_STATE_STANDBY */
            setStandbyMode();
            switchState(CAN_STATE_STANDBY);
            DLT_DEBUG("Can mode:STANDBY");
            break;

        case CAN_STATE_STANDBY:
            /* Tranciver jest w stanie standby i sprawdza czy wartość flagi została zminiona */
            if (canFlag == CAN_ACTIVE)
            {
                /* Jeśli flaga ustawiona jest na CAN_STATE_ACTIVE, należy przejść do stanu CAN_STATE_ACTIVE */
                setActiveMode();
                canWakeUp();
                switchState(CAN_STATE_ACTIVE);
                DLT_DEBUG("Can mode:ACTIVE");
            }
            break;
        
        default:
            DLT_DEBUG("Can mode error!");
            break;
    }
}