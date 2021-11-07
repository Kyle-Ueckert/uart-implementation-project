/*
 * uart.c
 *
 *  Created on: 3 Nov 2020
 *      Author: Mohamed Yaqoob
 */

#include "uart.h"
#include "rcc.h"
#include "main.h"

/**
 * @brief UART1 GPIO config
 */
void uart_UART1_GPIO_config(void)
{
  //Enable Port A clock
  RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
  RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
  //Mode to AF and Fast speed
  GPIOA->CRH &= ~GPIO_CRH_CNF9;
  GPIOA->CRH |= GPIO_CRH_CNF9_1;
  GPIOA->CRH &= ~GPIO_CRH_CNF10;
  GPIOA->CRH |= GPIO_CRH_CNF10_0;//Input Floating
  //Output max 10Mhz
  GPIOA->CRH &= ~GPIO_CRH_MODE9;
  GPIOA->CRH |= GPIO_CRH_MODE9_0;
  GPIOA->CRH &= ~GPIO_CRH_MODE10;
  //Map to PA9, PA10
  AFIO->MAPR &= ~AFIO_MAPR_USART1_REMAP;
}

/**
 * @brief UART1 configuration
 */
void uart_UART1_config(void)
{
  //UART clock enable
  RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

  //Transmit Enable
  USART1->CR1 |= USART_CR1_TE;
  //Receive Enable
  USART1->CR1 |= USART_CR1_RE;
  //Parity - Even
  USART1->CR1 &= ~(USART_CR1_PS);
  //Parity Control Enable
  USART1->CR1 &= ~(USART_CR1_PCE);
  //Word length = 8bit
  USART1->CR1 &= ~(USART_CR1_M);
  //Stop bits = 1
  USART1->CR2 &= ~(USART_CR2_STOP);
  //Disable Hardware flow control (RTS, CTS)
  USART1->CR3 &= ~(USART_CR3_CTSE);
  USART1->CR3 &= ~(USART_CR3_RTSE);
  //Set Baud rate to 115200 (72MHz = 39.0625 -> 39 / 1)
  //BRR = 72MHz/115200/16 = 39.0625
  //Mantissa = 39
  //Fraction = .0625*16 = 1
  USART1->BRR =0;
  USART1->BRR |= (39UL << 4);
  USART1->BRR |= (1UL << 0);
  //Clear LINEN and CLKEN in CR2
  USART1->CR2 &= ~(USART_CR2_LINEN | USART_CR2_CLKEN);
  //Clear SCEN, HDSEL and IREN in CR3
  USART1->CR3 &= ~(USART_CR3_SCEN | USART_CR3_HDSEL | USART_CR3_IREN);
  //Enable UART
  USART1->CR1 |= USART_CR1_UE;
}

/**
 * @brief UART1 transmit
 */
bool uart_UART1_transmit(uint8_t *data, uint8_t len, uint32_t timeout)
{
  //Wait on TXE to start transmit
  //Write to DR as TXE flag is HIGH (Tx buffer Empty)
  uint8_t dataIdx = 0;
  uint32_t startTick = rcc_msGetTicks();
  while(dataIdx<len)
  {
    if(USART1->SR & USART_SR_TXE) //Tx buffer empty
    {
      USART1->DR = data[dataIdx];
      dataIdx++;
    }
    else //Manage timeout
    {
      if((rcc_msGetTicks() - startTick)>= timeout) return false;
    }
  }
  //Wait for busy flag
  while(USART1->SR & USART_SR_TC)
  {
    if((rcc_msGetTicks() - startTick)>= timeout) return false;
  }
  return true;
}
