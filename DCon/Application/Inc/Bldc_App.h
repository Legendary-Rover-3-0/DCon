#ifndef BLDC_APP_H
#define BLDC_APP_H

#include "main.h"

typedef enum //tworzę nowy typ zmiennej enumerycznej-MOTOR_STATE_Type - czyli stan w jakim znajduje sie dane koło
{
	IDLE_1=0,              //to stan jałowy
	DRIVE,                 //to stan jazdy
	BRAKE                  //to stan hamowania
 }MOTOR_STATE_Type;

 typedef enum //tworzę nowy typ zmiennej-MOVE_STATE_Type czyli sposob w jaki auto sie porusza
 {
 	IDLE_2= 0,             //to stan nie ustalony
 	FORWARD,               //to stan jazdy do przodu/tyłu
	AROUND,                //to stan jazdy wokół wlasnej osi
	ROUND                  //to stan jazdy po okregu
 }MOVE_STATE_Type;

 typedef struct //toworze strukture-ENCODER_Type sa tutaj wszytkie parametry dla enkodera
 {
	 uint32_t LastValue;         //tworzę zmienną która pamięta ostanią wartość
	 uint32_t TotalTurns;        //tworze zmienną która przechowuje wszytkie obroty które zrobił silnik
	 float CurrentVelocity;      //tworze zmienną ktora przechowuje aktualną prędkosc  znak to kierunek obrotów
	 float V1;                   //to wartości predkosci o 1,2,3 próbki wcześniej będą słuzyć do średniej aby wyelminować skoki prędkosci
	 float V2;
	 float V3;
	 // tutaj bedą jeszce inne parametry które można obliczyć z enkdera
 }ENCODER_Type;

 typedef struct //toworze strukture-TIMER_Type sa tutaj wszytkie numery timerów i kanały
  {
	 TIM_HandleTypeDef     *_htim_PWM;           //to wskaźnik na timer od pwma
	 uint32_t              _channel_PWM;         //to wskaźnik na kanał od pwma

	 TIM_HandleTypeDef     *_htim_ENCODER;       //to wskaźnik na timer od pwma
	 uint32_t              _channel_ENCODER;     //to wskaźnik na kanał od pwma
  }TIMER_Type;

 typedef struct //toworze strukture-PINOUT_Type sa tutaj wszytkie numery pinów i portów dla jednego koła
 {
  	 GPIO_TypeDef*       DirectionPort;     //to wskaźnik na port od kierunku jazdy
  	 uint16_t            DirectionPin;      //to pin od kierunku jazdy

  	 GPIO_TypeDef*       WorkPort;          //to wskaźnik na port od zezwolenia na pracę
  	 uint16_t            WorkPin;           //to pin od zezwolenia na pracę

  	 GPIO_TypeDef*       BrakePort;         //to wskaźnik na port od hamowania
  	 uint16_t            BrakePin;          //to pin od hamowania
 }PINOUT_Type;

  typedef struct //tworzę strukture PID_Type są tutaj wszystkie dane potrzebne do reg. PID
    {
    float 				PidSignal;     //to syganł obliczony przez nas regulator PID
    float               Deviation;     //to uchyb regulacji - wartość zadana - wartość bieżąca
    float               Integral;      //to całka z uchybu
    float               Derivative;    //to pochodna z uchybu
    }PID_Type;

  typedef struct //tworze strukture RAMP_Type są tutaj wszystkie dane potrzebne do obliczania rampy
  {
	  int          StartSpeed;         //to prędkość początkowa
	  int  		   FinalSpeed;         //to predkosć do której chcę dojść
	  int		   LastFinalSpeed;     //to ostatnia predkosc do której chcieliśmy dojść
	  float        Delta;              //to przyrost predkosci z kolejnym krokiem
	  float 	   RisingTime;         //to czas narastania w sekundach
  }RAMP_Type;
typedef struct //toworze strukture-WHEEL_Type sa tutaj wszytkie parametry kola
{
	MOTOR_STATE_Type      MOTOR_STATE;      //to stan silnika
	MOVE_STATE_Type       MOVE_STATE;       //to stan poruszania sie lazik
	ENCODER_Type          encoder_data;     //to struktura z parametrami odczytanymi z enkodera
	TIMER_Type            timer_data;       //to struktura z numerami wszystkich timerów i kanałaów
	PINOUT_Type           pinout_data;      //to struktura z wszystkimi pinami i portami używanymi w obsługi jednego koła
	float                 set_velocity;     //to zadana prędkość
	int                   motor_siganl;     //to sygnal jaki jest ustawiony na silniku
	PID_Type   			  pid_data;         //to struktura z wszystkimi parametrami regulatora PID
	RAMP_Type  			  ramp_data;        //to struktura z wszystkimi parametrami do rampy narastania predkosci
}WHEEL_Type;

void InitFunction(WHEEL_Type* FR);
void VelocityCalculation(WHEEL_Type* FR,WHEEL_Type* FL,WHEEL_Type* BR,WHEEL_Type* BL,int speed, int radius);
void PID(WHEEL_Type* key);
void EncoderVelocity(uint32_t EncoderSiganl,WHEEL_Type* key);
void MotorState(WHEEL_Type *key);
void SpeedRamp(WHEEL_Type *key);

#endif