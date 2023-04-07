/*
 * Author: Lucas de Jesus B. Gonçalves
 *
 * Last modified: 02/04/2023
 * Description: SPI library for the RM3100 sensor using stm32.
 */

#include "stm32l0xx_hal.h"
#include <limits.h>
#include <rm3100_spi.h>

/* Endereço hexadecimal escravo para o RM3100.
 * 5 primeiros bits são marcados como 0b01000 e os outros 2 bits pelos pinos 3 e 28.
 * Assim para 3 High e 28 Low, temos 0b0100001 = 0x21
 */

/*Definição de pino Data Ready*/
#define DR_PIN GPIO_PIN_3 //Set pin D8 to be the Data Ready Pin

/*Define pino de CS para o sensor*/
#define CS_PIN

/* Endereços dos registradores internos sem o bit de R/W (vide datasheet) */
#define REVID_REG 0x36 	/* Endereço hexadecimal para o registrador Revid */
#define POLL_REG 0x00  	/* Endereço hexadecimal para o registrador Poll */
#define CMM_REG 0x01	/* Endereço hexadecimal para o registrador CMM */
#define STATUS_REG 0x34 /* Endereço hexadecimal para o registrador Status */
#define CCX1_REG 0x04 	/* Endereço hexadecimal para o registrador Cycle Count X1 */
#define CCX0_REG 0x05	/* Endereço hexadecimal para o registrador Cycle Count X0 */

/* Opções*/
#define INITIAL_CC  200 /* configura  cycle count*/
#define SINGLE_MODE 0 	/* 0 = modo de medida continuo; 1 = single mode */
#define USE_DR_PIN 1 	/* 0 = não usa pino de DR ; 1 = usa pino de DR */
#define UART_DBG 0		/* 0 = não printa mensagem de debug via UART, 1 = emite*/


/*
 * Estruct com as leituras nos 3 eixos em counts.
 * Para obter os dados em uT, basta dividir os valores por gain
 */
typedef struct
{
	long x;
	long y;
	long z;
	float gain;
} RM3100_DATA;

/*variáveis externas (criadas no main do código do stm32)*/
extern UART_HandleTypeDef huart1;
extern I2C_HandleTypeDef hi2c1;
