#include "Pololu.h"
#include "stdio.h"
/*
Bien faire attention à ranger les pin dans pn[]
dans l'ordre définit dans la fonction Pololu_init
*/
void Pololu_init(Pololu_TypeDef *Pololu, GPIO_TypeDef * port, uint8_t *pn){

	Pololu->gpioReset = port;
	Pololu->gpioSleep = port;
	Pololu->gpioEnable = port;
	Pololu->gpioStep = port;
	Pololu->gpioDir = port;
	Pololu->gpioMs1 = port;
	Pololu->gpioMs2 = port;
	Pololu->gpioMs3 = port;
	// ATTENTION ! Le pin PWM (Step) de la carte doit être un pin compatible PWM (cf PinMapping.pdf)
	Pololu->pinR = pn[0];
	Pololu->pinSl = pn[1];
	Pololu->pinE = pn[2];
	Pololu->pinSt = pn[3];
	Pololu->pinD = pn[4];
	Pololu->pinM1 = pn[5];
	Pololu->pinM2 = pn[6];
	Pololu->pinM3 = pn[7];
	


	//déterminer le numéro du port 0--> GPIOA, 1-->GPIOB, etc.
	uint8_t nb_port;
	nb_port=(uint32_t)((uint32_t *)port - GPIOA_BASE)/ (uint32_t)0x400;
	//activation de l'hologe
	RCC->APB2ENR|=(1<<(nb_port+2));

	//configuration de la pin en sortie

	// RESET 
	if(	Pololu->pinR<8){
		Pololu->gpioReset->CRL &=~(3 << (4 * pn[0]+2));	//CNF1 CNF0 = 00
		Pololu->gpioReset->CRL &=~(3 << 4 * pn[0]);
		Pololu->gpioReset->CRL |= (3 << 4 * pn[0]);
		//Pololu->gpioReset->CRL |= (2 << (4 * pn[0] + 2)); // On laisse à 00 (General output push pull) le CNF 
	}
	else if (Pololu->pinSl >= 8) {
		Pololu->gpioReset->CRH &= ~(3 << (4 * pn[0] +2));	//CNF1 CNF0 = 00
		Pololu->gpioReset->CRH &= ~(3 << 4 * pn[0]);
		Pololu->gpioReset->CRH |= (3 << 4 * pn[0]);
		//Pololu->gpioReset->CRH |= (2 << (4 * pn[0] + 2));
	}

	// SLEEP 
	if (Pololu->pinSl < 8) {
		Pololu->gpioSleep->CRL &= ~(3 << (4 * pn[1] + 2));	//CNF1 CNF0 = 00
		Pololu->gpioSleep->CRL &= ~(3 << 4 * pn[1]);
		Pololu->gpioSleep->CRL |= (3 << 4 * pn[1]);
		//Pololu->gpioSleep->CRL |= (2 << (4 * pn[1] + 2));
	}
	else if (Pololu->pinSl >= 8) {
		Pololu->gpioSleep->CRH &= ~(3 << (4 * pn[1] + 2));	//CNF1 CNF0 = 00
		Pololu->gpioSleep->CRH &= ~(3 << 4 * pn[1]);
		Pololu->gpioSleep->CRH |= (3 << 4 * pn[1]);
		//Pololu->gpioSleep->CRH |= (2 << (4 * pn[1] + 2));
	}

	// ENABLE
	if (Pololu->pinE < 8) {
		Pololu->gpioEnable->CRL &= ~(3 << (4 * pn[2] + 2));	//CNF1 CNF0 = 00
		Pololu->gpioEnable->CRL &= ~(3 << 4 * pn[2]);
		Pololu->gpioEnable->CRL |= (3 << 4 * pn[2]);
		//Pololu->gpioEnable->CRL |= (2 << (4 * pn[2] + 2));
	}
	else if (Pololu->pinE >= 8) {
		Pololu->gpioEnable->CRH &= ~(3 << (4 * pn[2] + 2));	//CNF1 CNF0 = 00
		Pololu->gpioEnable->CRH &= ~(3 << 4 * pn[2]);
		Pololu->gpioEnable->CRH |= (3 << 4 * pn[2]);
		//Pololu->gpioEnable->CRH |= (2 << (4 * pn[2] + 2));
	}

	// STEP 
	if (Pololu->pinSt < 8) {
		Pololu->gpioStep->CRL &= ~(3 << (4 * pn[3] + 2));	//CNF1 CNF0 = 00
		Pololu->gpioStep->CRL &= ~(3 << 4 * pn[3]);
		Pololu->gpioStep->CRL |= (3 << 4 * pn[3]);
		Pololu->gpioStep->CRL |= (2 << (4 * pn[3] + 2)); // CNF = 10(2) Mode Alternate fuction output Push-Pull
	}
	else if (Pololu->pinSt >= 8) {
		Pololu->gpioStep->CRH &= ~(3 << (4 * pn[3] + 2));	//CNF1 CNF0 = 00
		Pololu->gpioStep->CRH &= ~(3 << 4 * pn[3]);
		Pololu->gpioStep->CRH |= (3 << 4 * pn[3]); 
		Pololu->gpioStep->CRH |= (2 << (4 * pn[3] + 2)); // CNF = 10(2) Mode Alternate fuction output Push-Pull
	}

	// DIR
	if (Pololu->pinD < 8) {
		Pololu->gpioDir->CRL &= ~(3 << (4 * pn[4] + 2));	//CNF1 CNF0 = 00
		Pololu->gpioDir->CRL &= ~(3 << 4 * pn[4]);
		Pololu->gpioDir->CRL |= (3 << 4 * pn[4]);
		//Pololu->gpioDir->CRL |= (2 << (4 * pn[4] + 2));
	}
	else if (Pololu->pinD >= 8) {
		Pololu->gpioDir->CRH &= ~(3 << (4 * pn[4] + 2));	//CNF1 CNF0 = 00
		Pololu->gpioDir->CRH &= ~(3 << 4 * pn[4]);
		Pololu->gpioDir->CRH |= (3 << 4 * pn[4]);
		//Pololu->gpioDir->CRH |= (2 << (4 * pn[4] + 2));
	}

	// MS1
	if (Pololu->pinM1 < 8) {
		Pololu->gpioMs1->CRL &= ~(3 << (4 * pn[5] + 2));	//CNF1 CNF0 = 00
		Pololu->gpioMs1->CRL &= ~(3 << 4 * pn[5]);
		Pololu->gpioMs1->CRL |= (3 << 4 * pn[5]);
		//Pololu->gpioMs1->CRL |= (2 << (4 * pn[5] + 2));
	}
	else if (Pololu->pinM1 >= 8) {
		Pololu->gpioMs1->CRH &= ~(3 << (4 * pn[5] + 2));	//CNF1 CNF0 = 00
		Pololu->gpioMs1->CRH &= ~(3 << 4 * pn[5]);
		Pololu->gpioMs1->CRH |= (3 << 4 * pn[5]);
		//Pololu->gpioMs1->CRH |= (2 << (4 * pn[5] + 2));
	}

	// MS2
	if (Pololu->pinM2 < 8) {
		Pololu->gpioMs2->CRL &= ~(3 << (4 * pn[6] + 2));	//CNF1 CNF0 = 00
		Pololu->gpioMs2->CRL &= ~(3 << 4 * pn[6]);
		Pololu->gpioMs2->CRL |= (3 << 4 * pn[6]);
		//Pololu->gpioMs2->CRL |= (2 << (4 * pn[6] + 2));
	}
	else if (Pololu->pinM2 >= 8) {
		Pololu->gpioMs2->CRH &= ~(3 << (4 * pn[6] + 2));	//CNF1 CNF0 = 00
		Pololu->gpioMs2->CRH &= ~(3 << 4 * pn[6]);
		Pololu->gpioMs2->CRH |= (3 << 4 * pn[6]);
		//Pololu->gpioMs2->CRH |= (2 << (4 * pn[6] + 2));
	}

	// MS3
	if (Pololu->pinM3 < 8) {
		Pololu->gpioMs3->CRL &= ~(3 << (4 * pn[7] + 2));	//CNF1 CNF0 = 00
		Pololu->gpioMs3->CRL &= ~(3 << 4 * pn[7]);
		Pololu->gpioMs3->CRL |= (3 << 4 * pn[7]);
		//Pololu->gpioMs3->CRL |= (2 << (4 * pn[7] + 2));
	}
	else if (Pololu->pinM3 >= 8) {
		Pololu->gpioMs3->CRH &= ~(3 << (4 * pn[7] + 2));	//CNF1 CNF0 = 00
		Pololu->gpioMs3->CRH &= ~(3 << 4 * pn[7]);
		Pololu->gpioMs3->CRH |= (3 << 4 * pn[7]);
		//Pololu->gpioMs3->CRH |= (2 << (4 * pn[7] + 2));
	}
	
		// On active la carte Pololu
		Pololu_turnOff(Pololu);
}


void Pololu_turnOff(Pololu_TypeDef * Pololu){
	//led->gpioPort->ODR |= (1<<led->pin);
	// reset et sleep à 0
	// enable à 1
	Pololu->gpioEnable->ODR|=(1<<Pololu->pinE);
	Pololu->gpioReset->ODR&=~(1<<Pololu->pinR);
	Pololu->gpioSleep->ODR&=~(1<<Pololu->pinSl);
}

void Pololu_Switch(Pololu_TypeDef * Pololu){
	//led->gpioPort->ODR |= (1<<led->pin);
	// reset et sleep à 0
	// enable à 1
		Pololu->gpioEnable->ODR^=(1<<Pololu->pinE);
		Pololu->gpioReset->ODR^=(1<<Pololu->pinR);
		Pololu->gpioSleep->ODR^=(1<<Pololu->pinSl);
}


void Pololu_turnOn(Pololu_TypeDef * Pololu){
	//led->gpioPort->ODR &=~ (1<<led->pin);
	// reset et sleep à 1
	// enable à 0
	Pololu->gpioEnable->ODR&= ~(1<<Pololu->pinE);
	Pololu->gpioReset->ODR|=(1<<Pololu->pinR);
	Pololu->gpioSleep->ODR|=(1<<Pololu->pinSl);

}

void Pololu_Dir1(Pololu_TypeDef * Pololu) {
	//led->gpioPort->ODR &=~ (1<<led->pin);
	// Changer le bit de DIR 
	Pololu->gpioDir->ODR&=~(1<<Pololu->pinD);
}

void Pololu_Dir2(Pololu_TypeDef * Pololu) {
	//led->gpioPort->ODR &=~ (1<<led->pin);
	// Changer le bit de DIR 
	Pololu->gpioDir->ODR|=(1<<Pololu->pinD);
}


void Pololu_ChoseStep(uint8_t mode, Pololu_TypeDef *Pololu){
	// Changer contenu de MS1, MS2, MS3 en fonction de ce qui est mis en paramètre
	switch (mode)
	{
    case 0:
      // Full-Step
			Pololu->gpioMs1->ODR&= ~(1<<Pololu->pinM1);
			Pololu->gpioMs2->ODR&= ~(1<<Pololu->pinM2);
			Pololu->gpioMs3->ODR&= ~(1<<Pololu->pinM3);
      break;

    case 1:
      // Half-Step
			Pololu->gpioMs1->ODR|=(1<<Pololu->pinM1);
			Pololu->gpioMs2->ODR&= ~(1<<Pololu->pinM2);
			Pololu->gpioMs3->ODR&= ~(1<<Pololu->pinM3);		
      break;
	  case 2:
      // Quarter-Step
			Pololu->gpioMs1->ODR&= ~(1<<Pololu->pinM1);
			Pololu->gpioMs2->ODR|=(1<<Pololu->pinM2);
			Pololu->gpioMs3->ODR&= ~(1<<Pololu->pinM3);		
      break;  
		case 3:
      // Eighth-Step
			Pololu->gpioMs1->ODR|=(1<<Pololu->pinM1);
			Pololu->gpioMs2->ODR|=(1<<Pololu->pinM2);
			Pololu->gpioMs3->ODR&= ~(1<<Pololu->pinM3);		
      break;  
		case 4:
      // Sixteenth-Step
			Pololu->gpioMs1->ODR|=(1<<Pololu->pinM1);
			Pololu->gpioMs2->ODR|=(1<<Pololu->pinM2);
			Pololu->gpioMs3->ODR|=(1<<Pololu->pinM3);		
      break;
    default:
			printf("Pololu_ChoseStep Switch...case");
			break;
	}
}