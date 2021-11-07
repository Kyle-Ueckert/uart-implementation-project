/*
 * uart.h
 *
 *  Created on: Nov 6, 2021
 *      Author: Kyle Ueckert
 */

#ifndef INC_UART_H_
#define INC_UART_H_

/*
 * uart.h
 *
 *  Created on: 3 Nov 2020
 *      Author: Mohamed Yaqoob
 */



#include "main.h"

/**
 * @brief UART1 GPIO configuration
 */
void uart_UART1_GPIO_config(void);

/**
 * @brief UART1 configuration
 */
void uart_UART1_config(void);

/**
 * @brief UART1 transmit
 */
bool uart_UART1_transmit(uint8_t *data, uint8_t len, uint32_t timeout);

#endif /* INC_UART_H_ */
