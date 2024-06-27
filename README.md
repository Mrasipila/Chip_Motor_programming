# Chip_Motor_programming

![image](https://github.com/Mrasipila/Chip_Motor_programming/assets/30113273/e47b8905-bb04-4784-bb0d-02c195256d07)
![image](https://github.com/Mrasipila/Chip_Motor_programming/assets/30113273/9b1314e3-fc79-496d-a84e-13c61a878795)



## Introduction :

Le moteur pas à pas contrairement aux servomoteurs permet le contrôle de la rotation avec un couple fort et pour une faible de vitesse. C’est donc un moteur qui sera accès sur le contrôle de la position. On retrouve ce type de moteurs dans différentes technologies tel que les actionneurs linéaires. Qui eux sont utilisés par exemples dans le contrôle du plateau dans certains modèles d’imprimantes 3D. 
   
Il existe différents types de moteur pas à pas, principalement caractérisés par leurs nombres de pôle (nombre de dents de son stator). Pour travailler avec ce genre de moteur il est plus utile d’utiliser un pilote qui va permettre d’éviter les surchauffes et de gérer l’alimentation des pôles du stator d’un moteur pas à pas. On pourra ainsi maîtriser plus aisément la position de la rotation du moteur. 
   
Nous utiliserons dans ce projet le pilote Pololu A4988 qui est construit pour gérer les moteurs bipolaires sur différents modes (moitié-, quart-,huitième- et seizième-step mode). Nous nous intéresserons dans ce projet à la maîtrise de ce genre de moteur. Et essaierons de nous étendre sur les fonctionnalités les plus importantes du contrôle de ce genre de moteur via un pilote.



















## Objectif du Projet :

Ce projet a pour objectif le contrôle d’un moteur pas à pas dans son ensemble via deux boutons. On aura donc connecté à notre microcontrôleur deux boutons configurés en pull-down et les 7 pins d’entrée du pilote Pololu A4988.
L’appuie du premier bouton activera la carte A4988 et enclenchera une rotation dans sens. Tandis que le deuxième bouton activera la carte A4988 en enclenchera une rotation dans le sens opposé à la première. Lorsque un des boutons est relâché, on désactive le pilote A4988. Appuyer sur les deux boutons en même temps désactivera la carte.  
   
Afin de réaliser ces fonctionnalités, on gérera l’événement d’appuie de bouton via le contrôleur d’interruption EXTI connecté au NVIC. 
    
Et pour générer le mouvement du moteur pas à pas on engendrera un signal PWM sur le pin STEP de la carte Pololu. Les moteurs pas à pas ont typiquement un pas d’un degré défini (e.g. un moteur pas à pas de pas 1.8° aura besoin de 200 pas pour faire une révolution complète).
La fréquence du signal PWM envoyé sur STEP aura donc un effet sur la vitesse du moteur. 
   
Dans notre cas, nous nous contenterons d’une fréquence PWM de 500Hz et d’un full-step mode. Le moteur avancera donc à une vitesse de 500*1,8 degrés par secondes pour un moteur de pas 1,8°.
















## Montage électronique :


![image](https://github.com/Mrasipila/Chip_Motor_programming/assets/30113273/9f4c1887-89df-4b9f-828b-ab8fed31c711)
   
 

Ci-dessus le microcontrôleur STM32F103RBTx avec ses deux boutons configurés en Pull-down pour éviter le flottement de la tension transmise par celui-ci. Alimenté d’une tension 2-3.6V comme référencé par le constructeur. 

Les deux boutons sont alimentés par une alimentation de 15V partagée entre les deux boutons et la carte du pilote Pololu A4988. Chaque boutons nécessitant 5V et le pilote une tension comprise entre 3 et 5.5V.

On observe aussi que tous les pins sortants du microcontroleur sont situés sur les broches du port GPIOC. On a fait ce choix afin de réduire la consommation d’énergie, ainsi seul l’horloge GPIOC est active.


![image](https://github.com/Mrasipila/Chip_Motor_programming/assets/30113273/e8e791a0-b4fa-4e3a-bce1-bef6dc32b04d)






 
Ci-dessus on peut voir que les 8 pins sortants du microcontrôleur sont branchés en entrée au pilote Pololu A4988. Gérant ainsi les modes de fonctionnement du pilotes via les pins branché sur MS1, MS2,MS3. Ci-dessous une table montrant la valeur des bits à rentrer dans MSx pour obtenir le mode de résolution voulu.
On rappelle que dans notre cas ces trois bits sont fixés à zéro.

![image](https://github.com/Mrasipila/Chip_Motor_programming/assets/30113273/b8b8a2f2-3b9e-40bf-a1a8-dd11b1a74d0a)

Le pilote est aussi alimenté à 8-35V car il gère l’alimentation du moteur. 
On branche le moteur bipolaire au pilote. Et en fonction de ce qui est entré sur ses pins en entré, dans notre cas RESET, SLEEP, ENABLE, MS1, MS2, MS3, STEP et DIR. Il gèrera ce qu’il attribuera en sortie au moteur.  
 
![image](https://github.com/Mrasipila/Chip_Motor_programming/assets/30113273/af9a358f-b1a7-4ef2-b62b-6fca1d657478)


## Programme et Fonctionnalités : 

 ![image](https://github.com/Mrasipila/Chip_Motor_programming/assets/30113273/d87d073a-cc21-4033-b4a6-235fc6c01845)


Les drivers de mes composants sont ‘Pololu.c’ et ‘Pololu.h’ qui gèrent toutes les fonctionnalités tournant autour de la carte Pololu A4988. 
Ensuite, ‘button.c’ et ‘button.h’ qui gèrent toutes les fonctionnalités tournant autour de la configuration du contrôleur d’interruption sur les pins des ports concerné des boutons (ATTACHE AU VECTEUR D’INTERRUPTION). Mais aussi la configuration des modes d’entrée sortie pour tous ces pins. 

Ensuite dans le ‘main.c’ on initialise toutes nos composantes. Met en place la fonction initialisant les deux boutons, les huit pins du pilote Pololu et le PWM sur un timer et un canal souhaité. On met aussi en place les actions a effectuer lorsqu’un vecteur d’interruption est enclenché.  




















1)	Pololu A4988

![image](https://github.com/Mrasipila/Chip_Motor_programming/assets/30113273/27d7639f-fa35-45dd-84ea-6aca27aa417b)

Afin de gérer ce pilote on a créé une structure regroupant les informations sur les 8 pins et ports reliant le microcontrôleur à la carte. 




















Ci-dessous les différentes fonctions liées au fonctionnement du pilote Pololu A4899. 

La fonction Pololu_init() : 

Elle va initialiser tous les pins de la variable de type Pololu_TypeDef en sortie pour la carte du microcontrôleur. En fonction du port et du tableau de pin passé en paramètre. 
Elle va aussi mettre la carte Pololu dans l’état d’arrêt. 

La fonction Pololu_turnOn() :

Elle mettre la carte Pololu dans l’état de marche. C’est-à-dire mettre les pin Enable à 0, Sleep à 1 et Reset à 1. 

La fonction Pololu_turnOff() : 
 
Elle mettre la carte Pololu dans l’état d’arrêt. C’est-à-dire mettre les pin Enable à 1, Sleep à 0 et Reset à 0. 

La fonction Pololu_Dir1() : 
 
Elle met le bit du pin pinD de la broche DIR du pilote A4899 à 0. Le moteur tourne dans un sens.  

La fonction Pololu_Dir2() : 
 
Elle met le bit du pin pinD de la broche DIR du pilote A4899 à 1. Le moteur tourne dans l’autre sens.  

La fonction Pololu_ChoseStep() :

Elle définit le step mode du moteur en fonction de la variable mode. 0 pour Full-step. Elle agit sur les pin pinMs1, pinMs2, pinMs3. 

![image](https://github.com/Mrasipila/Chip_Motor_programming/assets/30113273/48f93a34-ee1c-431f-8a12-42b0925e7624)











2)	Les boutons 


Afin de gérer les boutons on a créé une structure regroupant les informations sur le pin, le port et le type d’appui reliant le microcontrôleur au bouton. 

![image](https://github.com/Mrasipila/Chip_Motor_programming/assets/30113273/2593506e-effd-4d4f-ae77-1cc03b9f80a0)


 



La fonction Button_init() :

Elle va initialiser le pin du bouton en paramètre en entrée pour la carte du microcontrôleur. Elle va aussi définir le type d’appui sur le bouton (Pull-up, pull-down, no-pull).  

La fonction Button_State() :

Retourne l’état actuel du bouton 1 ou 0 respectivement un appui et un relâchement dans le cas d’un bouton configuré en pull-down. 

La fonction Button_enableIRQ() :

Elle va activer l’interruption sur le pin et le port voulu du bouton passé en paramètre. En définissant la condition d’interruption, appui du bouton (front montant), relâchement du bouton (front descendant) ou les deux. 

![image](https://github.com/Mrasipila/Chip_Motor_programming/assets/30113273/d2ec336a-4d74-48bf-bbae-53aa89e59c0b)

 
 


3)	Le main.c


 ![image](https://github.com/Mrasipila/Chip_Motor_programming/assets/30113273/15462a08-aee4-43d6-9a2d-4dd76922c694)


On inclut nos deux pilotes dans le main.c

 ![image](https://github.com/Mrasipila/Chip_Motor_programming/assets/30113273/e69afa67-0892-45f4-af9a-d18f567e2157)


On crée nos 3 structure : les deux boutons et le Pololu. 

 ![image](https://github.com/Mrasipila/Chip_Motor_programming/assets/30113273/2876a7b1-e2bb-4292-aa72-5ac62b1808ea)


On déclare nos fonctions PWM qui prend en paramètre le canal et le timer lié au pin compatible PWM connecté à la broche STEP du pilote Pololu A4988. Dans notre cas le pin 7 du port GPIOC compatible PWM est connecté
à la carte A4988. Le canal relié à ce pin est le canal 2 du timer 3 lorsque le registre AFIO_MAPR est initialisé en FULLREMAP. 

 ![image](https://github.com/Mrasipila/Chip_Motor_programming/assets/30113273/fef0752a-d90e-4474-a78c-61407b171022)


On a écrit ensuite les prototypes des vecteurs d’interruption NVIC liés au contrôleur d’interruption des pins des boutons.




Main() :

 ![image](https://github.com/Mrasipila/Chip_Motor_programming/assets/30113273/cde2387e-a134-465a-a2c9-446042cc8025)


On initialise nos deux boutons, et on remplie notre tableau avec les pins voulu pour la fonction Pololu_init(). 

 ![image](https://github.com/Mrasipila/Chip_Motor_programming/assets/30113273/9bd13061-81ae-463a-8a48-a203d5d98536)


On définit le mode Full-step pour le pilote du moteur. 
On pose la condition d’interruption comme étant front montant et front descendant des boutons. 
On initialise le canal 2 du timer 3 à une fréquence de 500 Hz et un duty cycle de 50%. On veut 50% en duty cycle pour que le moteur réalise le pas sur une même durée que le temps d’attente entre deux pas.


On a maintenant notre PWM qui est fonctionnel sur le pin 7 du port GPIOC, il ne reste plus qu’à configurer le comportement des deux boutons pour allumer et éteindre la carte en fonction de si il y a un appuie ou pas. 

Le PWM est toujours actif sur le pin PC7. On éteint et allume la carte pour arrêter le fonctionnement du moteur. 


![image](https://github.com/Mrasipila/Chip_Motor_programming/assets/30113273/360408fe-ee85-4e97-ac8b-e5eb82a45b9f)


Lorsqu’une interruption est détectée sur les pins 6 et 4, on vérifie que seul un appuie sur les boutons est effectué. Et on allume le pilote Pololu A4988 avec la direction 1 pour le bouton. Et la direction 2 pour le bouton 1. 


Notre système fonctionne. 

Le PWM tourne en boucle sur le pin PC7 relié à la broche STEP du pilote Pololu. Et les boutons gèrent l’état de cette même carte par interruption.  

