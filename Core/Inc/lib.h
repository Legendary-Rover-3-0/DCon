/*
 * lib.h
 *
 *  Created on: Dec 20, 2024
 *      Author: Andrzej Ozga
 */

#ifndef INC_LIB_H_
#define INC_LIB_H_
#include <math.h> //to bibliteka do funkcji sqrt
//w tym pliku bede umieszczal struktury i funkcje
const uint8_t Lenght=90;  //tworze zmienną określajaca długość łazika od 0-255cm
const uint8_t Width=40;   //tworzę zmienną określajaca szerokosc łazika od 0_255cm

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
void InitFunction(WHEEL_Type* FR)          //,WHEEL_Type* FL,WHEEL_Type* BR,WHEEL_Type* BL)//funkcja incalizujaca 4 obiekty
{
	FR->MOTOR_STATE=DRIVE;
	FR->MOVE_STATE=FORWARD;                            //domyślny tryb to jazda do przodu
	FR->set_velocity=0;
	FR->encoder_data.CurrentVelocity=0;
	FR->encoder_data.LastValue=0;

	FR->timer_data._htim_PWM=&htim4;
	FR->timer_data._channel_PWM=TIM_CHANNEL_1;

	FR->timer_data._htim_ENCODER=&htim3;
	FR->timer_data._channel_ENCODER=TIM_CHANNEL_ALL;

	FR->pinout_data.DirectionPort=Kierunek_GPIO_Port;
	FR->pinout_data.DirectionPin=Kierunek_Pin;

	FR->pinout_data.WorkPort=Praca_GPIO_Port;
	FR->pinout_data.WorkPin=Praca_Pin;

	FR->pinout_data.BrakePort=Stop_GPIO_Port;
	FR->pinout_data.BrakePin=Stop_Pin;

	HAL_TIM_Encoder_Start(FR->timer_data._htim_ENCODER,FR->timer_data._channel_ENCODER); //to start timera od enkodera
	HAL_TIM_PWM_Start(FR->timer_data._htim_PWM, FR->timer_data._channel_PWM);			 //to start timera od PWM na kanale1

	FR->ramp_data.RisingTime=10;
	FR->ramp_data.FinalSpeed=10;
	FR->ramp_data.StartSpeed=0;
	FR->ramp_data.LastFinalSpeed=70;
	//FR->encoder_data.LastValue=0;
	//FR->encoder_data.CurrentVelocity=0;
	/*memcpy(FR->limit_velocity,lim_vel,sizeof(lim_vel)); //kopiuje zawartosc tabicy tymczasowej do docelowej

	FL->MOTOR_STATE=DRIVE;
	FL->MOVE_STATE=FORWARD;
	FL->set_velocity=0;
	FL->current_velocity=0;
	memcpy(FL->limit_velocity,lim_vel,sizeof(lim_vel)); //kopiuje zawartosc tabicy tymczasowej do docelowej

	BR->MOTOR_STATE=DRIVE;
	BR->MOVE_STATE=FORWARD;
	BR->set_velocity=0;
	BR->current_velocity=0;
	memcpy(BR->limit_velocity,lim_vel,sizeof(lim_vel)); //kopiuje zawartosc tabicy tymczasowej do docelowej

	BL->MOTOR_STATE=DRIVE;
	BL->MOVE_STATE=FORWARD;
	BL->set_velocity=0;
	BL->current_velocity=0;
	memcpy(BL->limit_velocity,lim_vel,sizeof(lim_vel)); //kopiuje zawartosc tabicy tymczasowej do docelowej*/
}
void VelocityCalculation(WHEEL_Type* FR,WHEEL_Type* FL,WHEEL_Type* BR,WHEEL_Type* BL,int speed, int radius) //ta funkcja oblicza jaką predkosc ma mieć kazde kolo
{
	if((FR->MOTOR_STATE==DRIVE)&&(FL->MOTOR_STATE==DRIVE)&&(BR->MOTOR_STATE==DRIVE)&&(BL->MOTOR_STATE==DRIVE))//sprawdzam czy kazde kolo jest w stanie drive
	{
		if((FR->MOVE_STATE==FL->MOVE_STATE)&&(BR->MOVE_STATE==BL->MOVE_STATE)&&(FL->MOVE_STATE==BR->MOVE_STATE))//sprawdzam czy kazde kolo jest w tym samym trybie ruchu
		{
			switch(FR->MOVE_STATE)                    //a teraz sprawdzam w którym dokładnie trybie ruchu jestem
			{
			case IDLE_2:
				FR->set_velocity=0;                  //w trybie nieustalonym predkosc wynosi 0
				FL->set_velocity=0;
				BR->set_velocity=0;
				BL->set_velocity=0;
				break;
			case FORWARD:
				FR->set_velocity=speed;              //w trybie jazdy do przodu każde koło jedzie z ta sama predkosc
				FL->set_velocity=speed;
				BR->set_velocity=speed;
				BL->set_velocity=speed;
				break;
			case AROUND:
				FR->set_velocity=speed;              //w trybie jazdy wokol wlasnej osi prawe koła jada do przodu a lewe do tylu
				BR->set_velocity=speed;
				FL->set_velocity=(-1)*speed;
				BL->set_velocity=(-1)*speed;
				break;
			case ROUND:                                    //w trybie jazdy po okregu jedna strona jedzie szybciej niż druga
				if(radius>0)                               //gdy promien wynosi >0 to łazik jedzie w prawą strone
				{
					FR->set_velocity=speed*((sqrt((Lenght^2)+((radius-Width/2)^2)))/radius);    //obliczam predkosc koła z prawej strony z przodu i przypisuje kołu z tyłu
					BR->set_velocity=FR->set_velocity;
					FL->set_velocity=speed*((sqrt((Lenght^2)+((radius+Width/2)^2)))/radius);    //obliczam predkosc koła z lewej strony z przodu i przypisuje kołu z tyłu
					BL->set_velocity=FL->set_velocity;
				}
				else if(radius<0)                          //gdy promien wynosi <0 to łazik jedzie w lewą strone
				{
					FR->set_velocity=speed*((sqrt((Lenght^2)+((radius+Width/2)^2)))/radius);    //obliczam predkosc koła z prawej strony z przodu i przypisuje kołu z tyłu
					BR->set_velocity=FR->set_velocity;
					FL->set_velocity=speed*((sqrt((Lenght^2)+((radius-Width/2)^2)))/radius);    //obliczam predkosc koła z lewej strony z przodu i przypisuje kołu z tyłu
					BL->set_velocity=FL->set_velocity;
				}
				else                                        //gdy promien wynosi 0 to łazik jedzie do przodu
				{
					FR->set_velocity=speed;
					FL->set_velocity=speed;
					BR->set_velocity=speed;
					BL->set_velocity=speed;
				}
				break;
			}
		}
	}
	else //w przypadku gdy nie jesteśmy w trybie DRIVE to predkosc każdego koła wynosi zero
	{
		FR->set_velocity=0;
		FL->set_velocity=0;
		BR->set_velocity=0;
		BL->set_velocity=0;
	}
}
void PID(WHEEL_Type* key)
{
	float P=0.267;     //definiuje współczynnik P,I,D
	float I=14.1;
	float D=0.0;
	key->pid_data.Deviation=key->set_velocity-key->encoder_data.CurrentVelocity;    //obliczam uchyb regulacji
	if(key->pid_data.PidSignal>-1000&&key->pid_data.PidSignal<1000)     //to zabezpieczenie przed windup
	{
		key->pid_data.Integral+=key->pid_data.Deviation*0.2;            //obliczam całkę z uchybu 0.2 ponieważ bede tą funkcję wykonywał co 0.2s
	}
	else
	{
		key->pid_data.Integral=0;
	}
	key->pid_data.PidSignal = key->pid_data.Deviation*P + key->pid_data.Integral*I + key->pid_data.Derivative*D;  //to standardowy regulator PID
	key->motor_siganl=key->pid_data.PidSignal;
}



}
void EncoderVelocity(uint32_t EncoderSiganl,WHEEL_Type* key)      //tworzę funkcje która oblicza aktualną prędkość(przyjumje wartosci z timera i kolo)
{
	int x=EncoderSiganl-(key->encoder_data.LastValue);//tworze zmienną pomocniczą x do naszej funkcji
	if(x<-50000)                  //te if-y odpowiadają za obliczanie predkosci gdy przekreci sie timer
	{
		x=x+65535;
	}
	if(x>50000)
	{
		x=65535-x;
	}
	key->encoder_data.LastValue=EncoderSiganl;      //przypisuje aktualną wartosć do poprzedniej
	key->encoder_data.TotalTurns+=abs(x);           //uaktualniam liczbę wszytkich obrotów które zrobił silnik
	float alpha;
	if(x!=0)
	{
		alpha = 0.01*abs(x);                      //to zrobienie dynamicznego filtra - poprzez dynamiczną zmianę współczynnika wygładzającego
		key->encoder_data.CurrentVelocity=x*alpha+(1-alpha)*key->encoder_data.V1;  //Obliczam prędkosć z filtra ekspotencjalnego
		key->encoder_data.V1=key->encoder_data.CurrentVelocity;                    //cofam próbki o jeden
	}
	else
	{
		key->encoder_data.V1=0;
		key->encoder_data.CurrentVelocity=0;
	}
}

void MotorState(WHEEL_Type *key)
{
	switch(key->MOTOR_STATE)                    //a teraz sprawdzam w którym dokładnie trybie ruchu jestem
	{
		case IDLE_1: //w tym stanie nie hamujemy i nie mamy zezwolenia na prace i predkosc wynosi 0
			HAL_GPIO_WritePin(key->pinout_data.BrakePort,key->pinout_data.BrakePin, SET);       //gdy reset to silnik sie nie obraca a gdy set to sie obraca - pin stopu
			HAL_GPIO_WritePin(key->pinout_data.WorkPort,key->pinout_data.WorkPin, SET);         //gdy reset to silnik sie obraca a gdy set to sie nie obraca - pin zezwolenia na prace
		 	__HAL_TIM_SET_COMPARE(key->timer_data._htim_PWM,key->timer_data._channel_PWM,0);    //ustawiam sygnał PWM 0%
			break; //zapobiega wykonywaniu kolejnych przypadków.
		 case DRIVE: //w tym stanie nie hamujemy i mamy zezwolnie na prace i ustawaim zadaną prędkość
		 	HAL_GPIO_WritePin(key->pinout_data.BrakePort,key->pinout_data.BrakePin, SET);   //gdy reset to silnik sie nie obraca a gdy set to sie obraca - pin stopu
		 	HAL_GPIO_WritePin(key->pinout_data.WorkPort,key->pinout_data.WorkPin, RESET);   //gdy reset to silnik sie obraca a gdy set to sie nie obraca - pin zezwolenia na prace
		 	if(key->motor_siganl<0)              //gdy predkosc jest mniejsza od zera to obracam silnk w lewo a gdy wieksza to w prawo
		 	{
			  	HAL_GPIO_WritePin(key->pinout_data.DirectionPort,key->pinout_data.DirectionPin, RESET);                    //gdy set to obroty w lewo
		 		__HAL_TIM_SET_COMPARE(key->timer_data._htim_PWM,key->timer_data._channel_PWM,abs(key->motor_siganl));    //ustawiam żądany sygnał na odpowiednim timerze i kanale ale sygnał bez minusa
		 	}
		 	else if(key->motor_siganl>=0)
		 	{
		 		HAL_GPIO_WritePin(key->pinout_data.DirectionPort,key->pinout_data.DirectionPin, SET);                  //gdy reset to obroty w prawo
		 		__HAL_TIM_SET_COMPARE(key->timer_data._htim_PWM,key->timer_data._channel_PWM,key->motor_siganl);         //ustawiam żądany sygnał na odpowiednim timerze i kanale		 	}
		 	}
		 	break;
		 case BRAKE: //w tym stanie hamujemy i nie mamy zezwolenia na prace i predkosc wynosi 0
			HAL_GPIO_WritePin(key->pinout_data.BrakePort,key->pinout_data.BrakePin, RESET);   //gdy reset to silnik sie nie obraca a gdy set to sie obraca - pin stopu
			HAL_GPIO_WritePin(key->pinout_data.WorkPort,key->pinout_data.WorkPin, RESET);   //gdy reset to silnik sie obraca a gdy set to sie nie obraca - pin zezwolenia na prace
		 	__HAL_TIM_SET_COMPARE(key->timer_data._htim_PWM,key->timer_data._channel_PWM,0);        //ustawiam sygnał PWM 0%
		 	break;
	}
}
void SpeedRamp(WHEEL_Type *key)
{
	if(key->ramp_data.LastFinalSpeed!=key->ramp_data.FinalSpeed)
	{
		key->ramp_data.LastFinalSpeed=key->ramp_data.FinalSpeed;     //przypisuje starej predkosci nową prędkość
		key->ramp_data.StartSpeed=key->encoder_data.CurrentVelocity; //predkosc startująca to aktualna predkość
	}
	if(key->ramp_data.LastFinalSpeed==key->ramp_data.FinalSpeed)
	{
		key->ramp_data.Delta=(key->ramp_data.FinalSpeed-key->ramp_data.StartSpeed)/(key->ramp_data.RisingTime*5);//Obliczam o ile mam zwiększać predkosc z kolejnym krokiem + plus kontrola przed złym znakiem
		if(key->ramp_data.Delta<0)//dla rampy mniejszej od zera czyli zmniejszanie predkosci są inne warunki w if poniżej
		{
			if((key->set_velocity<(key->ramp_data.FinalSpeed-key->ramp_data.Delta))&&(key->set_velocity>(key->ramp_data.FinalSpeed+key->ramp_data.Delta)))
			{
				key->set_velocity=key->ramp_data.FinalSpeed;
			}
			else
			{
				key->set_velocity+=key->ramp_data.Delta;
			}
		}
		else if(key->ramp_data.Delta>0)
		{
			if((key->set_velocity>(key->ramp_data.FinalSpeed-key->ramp_data.Delta))&&(key->set_velocity<(key->ramp_data.FinalSpeed+key->ramp_data.Delta)))
			{
				key->set_velocity=key->ramp_data.FinalSpeed;
			}
			else
			{
				key->set_velocity+=key->ramp_data.Delta;
			}
		}
	}
}
#endif /* INC_LIB_H_ */
