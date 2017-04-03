/**
  *      __            ____
  *     / /__ _  __   / __/                      __  
  *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
  *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
  *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
  *                    /_/   github.com/KitSprout    
  * 
  * @file    main.c
  * @author  KitSprout
  * @date    3-Apr-2017
  * @brief   
  * 
  */

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\stm32f4_system.h"
#include "modules\serial.h"
#include "algorithms\matrix_arm.h"
#include "stm32f4xx_bsp.h"

/** @addtogroup STM32_Program
  * @{
  */

/* Private typedef -------------------------------------------------------------------------*/
/* Private define --------------------------------------------------------------------------*/
#define ROW 6
#define COL 6

/* Private macro ---------------------------------------------------------------------------*/
/* Private variables -----------------------------------------------------------------------*/
arm_matrix_instance_f32 matrixA;
float32_t arrayA[ROW * COL] = {
   1,  2, 53,  4,  3,  4,
   5, 46,  7,  8,  7,  8,
   9, 10, 22, 12, 22, 12,
  22, 45, 12, 32, 12, 32,
  34, 67, 33, 32, 63, 44,
  95, 46, 34,  8, 67,  8,
};

arm_matrix_instance_f32 matrixB;
float32_t arrayB[ROW * COL] = {0};

arm_matrix_instance_f32 matrixR;
float32_t arrayR[ROW * COL] = {0};

/* Private function prototypes -------------------------------------------------------------*/
/* Private functions -----------------------------------------------------------------------*/

void printMatrix( void )
{
  printf(" [  matrixA  ]\r\n");
  Matrix_Print(&matrixA);
  printf(" [  matrixB  ]\r\n");
  Matrix_Print(&matrixB);
  printf(" [  matrixR  ]\r\n");
  Matrix_Print(&matrixR);
  printf("\r\n");
}

int main( void )
{
  arm_status status;

  HAL_Init();
  BSP_GPIO_Config();
  BSP_UART_SERIAL_Config(NULL);

  Matrix_Init(&matrixA, arrayA, ROW, COL);
  Matrix_Init(&matrixB, arrayB, ROW, COL);
  Matrix_Init(&matrixR, arrayR, ROW, COL);
//  printMatrix();

  printf("\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n");
  printf(" ==== TEST MATRIX_ARM.C ======================\r\n");
  printf("\r\n");


  /* matrixB = matrixA */
  status = Matrix_Copy(&matrixB, &matrixA);
  printf(" ---- matrixB = matrixA ---------------------- [%2d]\r\n", status);
//  printMatrix();


  /* matrixA = inv(matrixA) */
//  status = Matrix_Inv(&matrixA, &matrixA);
  status = Matrix_Inv64(&matrixA, &matrixA);
  printf(" ---- matrixA = inv(matrixA) ----------------- [%2d]\r\n", status);
//  printMatrix();


  /* matrixR = matrixA * matrixB */
  status = Matrix_Mult(&matrixR, &matrixA, &matrixB);
  printf(" ---- matrixR = matrixA * matrixB ------------ [%2d]\r\n", status);
  printMatrix();


  /* matrixR = diag(matrixA) */
  status = Matrix_GetDiag(&matrixR, &matrixA);
  printf(" ---- matrixR = diag(matrixA) ---------------- [%2d]\r\n", status);
  printMatrix();


  /* matrixA = matrixA * 10 */
  status = Matrix_MultScale(&matrixA, &matrixA, 100.0f);
  printf(" ---- matrixA = matrixA * 100 ---------------- [%2d]\r\n", status);
  printMatrix();


  /* matrixB = matrixB' */
  status = Matrix_Trans(&matrixB, &matrixB);
  printf(" ---- matrixB = matrixB' --------------------- [%2d]\r\n", status);
  printMatrix();

  /* matrixA = matrixA + matrixB */
  status = Matrix_Add(&matrixA, &matrixA, &matrixB);
  printf(" ---- matrixA = matrixA + matrixB ------------ [%2d]\r\n", status);
  printMatrix();

  /* matrixB = matrixA - matrixB */
  status = Matrix_Sub(&matrixB, &matrixA, &matrixB);
  printf(" ---- matrixB = matrixA - matrixB ------------ [%2d]\r\n", status);
  printMatrix();

  while (1) {
    LED_G_Toggle();
    delay_ms(100);
  }
}

/*************************************** END OF FILE ****************************************/
