#include "button.h"


void Button_init(BUTTON_TypeDef *button, GPIO_TypeDef * port, uint8_t pn,
		uint8_t pl) {

	button->gpioPort = port;
	button->pin = pn;
	button->pull = pl;

	//Activation de l'horloge sur le port en question

	//déterminer le numéro du port 0--> GPIOA, 1-->GPIOB, etc.
	uint8_t nb_port;
	nb_port = ((uint32_t) port - GPIOA_BASE) / 0x400;
	//activation de l'hologe
			RCC->APB2ENR|=(1<<(nb_port+2));
	//configuration de la pin en entrée
			if(	button->pin<8)
	button->gpioPort->CRL &= ~(3 << 4 * pn);
else 
		button->gpioPort->CRH &= ~(3 << 4 * pn);
	//configuration du type de pull
	/*button->gpioPort->PUPDR &= ~(0b11 << 2 * pn);
	button->gpioPort->PUPDR |= (pl << 2 * pn);*/
			if (button->pull==LL_GPIO_PULL_NO) //CF1 CNF0=01
			{
					if(	button->pin<8)
						button->gpioPort->CRL |= (1 << (4 * pn+2));
						else 		
						button->gpioPort->CRH |= (1 << (4 * pn+2));
			}	
			if (button->pull==LL_GPIO_PD) //CF1 CNF3=10, ODR=0
			{
					if(	button->pin<8){
						button->gpioPort->CRL &=~ (3 << (4 * pn+2));
						button->gpioPort->CRL |= (2 << (4 * pn+2));
					}
						else {
						button->gpioPort->CRH &=~ (3 << (4 * pn+2));	
						button->gpioPort->CRH |= (2 << (4 * pn+2));
						}
						
						button->gpioPort->ODR&=~(1<<pn);
						
			}	
			
			if (button->pull==LL_GPIO_PU) //CF1 CNF0=10, ODR=1
			{
					if(	button->pin<8){
						button->gpioPort->CRL &=~ (3 << (4 * pn+2));
						button->gpioPort->CRL |= (2 << (4 * pn+2));
					}
						else {
						button->gpioPort->CRH &=~ (3 << (4 * pn+2));	
						button->gpioPort->CRH |= (2 << (4 * pn+2));
						}
						button->gpioPort->ODR|=(1<<pn);					
			}
}


uint8_t Button_State(BUTTON_TypeDef *button) {
		if(button->gpioPort->IDR & (1<<button->pin)){
				return 1;
		} else {
				return 0;
		}
}

// Attention le vecteur d'interruption d'une plage de pin ne traite pas deux pin ayant le même nombre. 
// Exemple PA0, PB0 ou PB12,PC12, ne fonctionnera pas, les pins lié à un vecteur doivent t'être différents.
// cf cours
void Button_enableIRQ(BUTTON_TypeDef *button, uint8_t trigger) {

	//détermination du numéro d'EXTI
	uint8_t nb_EXTI = button->pin /4;
	//déterminer le numéro du port 0--> GPIOA, 1-->GPIOB, etc.
	uint8_t nb_port;
	nb_port = ((uint32_t) button->gpioPort - GPIOA_BASE) / 0x400;

	//activation de l'interruption externe
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
	AFIO->EXTICR[nb_EXTI] &= ~(15 << 4 * (button->pin % 4));
	AFIO->EXTICR[nb_EXTI] |= (nb_port << 4 * (button->pin % 4));

	//activation d'EXTI dans IMR pour qu’elle ne soit pas masquée (sera traité si elle se produit)
	EXTI->IMR |= (1 << button->pin); 

        //activation du type du trigger (front montant/descendant)
	switch (trigger) {
	case LL_EXTI_TRIGGER_RISING:
	//activation de l'interruption sur front montant dans RTSR
		EXTI->RTSR|=(1<< button->pin); 
		break;
	case LL_EXTI_TRIGGER_FALLING:
	//activation de l'interruption sur front descendant dans FTSR
		EXTI->FTSR|=(1<< button->pin);
		break;
	case LL_EXTI_TRIGGER_RISING_FALLING:
		//activation de l'interruption sur front montant
		EXTI->RTSR|=(1<< button->pin); 
		//activation de l'interruption sur front descendant
		EXTI->FTSR|=(1<< button->pin);
		break;
	}

  //activer un vecteur d’interruption EXTI parmi les vecteurs disponibles selon le numéro de pin

		if (button->pin ==0) {
			NVIC_EnableIRQ(EXTI0_IRQn); 
			NVIC_SetPriority(EXTI0_IRQn, 1); 
		} 
		if (button->pin ==1) {
			NVIC_EnableIRQ(EXTI1_IRQn); 
			NVIC_SetPriority(EXTI1_IRQn, 1); 
		} 
		if (button->pin ==2) {
			NVIC_EnableIRQ(EXTI2_IRQn); 
			NVIC_SetPriority(EXTI2_IRQn, 1); 
		}
		if (button->pin ==3) {
			NVIC_EnableIRQ(EXTI3_IRQn); 
			NVIC_SetPriority(EXTI3_IRQn, 1); 
		}
		if (button->pin ==4) {
			NVIC_EnableIRQ(EXTI4_IRQn); 
			NVIC_SetPriority(EXTI4_IRQn, 1); 
		}
		if ((button->pin > 4) && (button->pin < 10)) {
			NVIC_EnableIRQ(EXTI9_5_IRQn); 
			NVIC_SetPriority(EXTI9_5_IRQn, 0); 
		}
		if ((button->pin > 9) && (button->pin < 16)) {
			NVIC_EnableIRQ(EXTI15_10_IRQn); 
			NVIC_SetPriority(EXTI15_10_IRQn, 1); 
		}
}



