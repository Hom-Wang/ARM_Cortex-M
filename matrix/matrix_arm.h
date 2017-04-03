/**
  *      __            ____
  *     / /__ _  __   / __/                      __  
  *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
  *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
  *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
  *                    /_/   github.com/KitSprout    
  * 
  * @file    matrix_arm.h
  * @author  KitSprout
  * @date    3-Apr-2017
  * @brief   
  * 
  */

/* Define to prevent recursive inclusion ---------------------------------------------------*/
#ifndef __MATRIX_ARM_H
#define __MATRIX_ARM_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes --------------------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "algorithms\mathUnit.h"

/* Exported types --------------------------------------------------------------------------*/
typedef arm_matrix_instance_f32 matrix_t;
typedef arm_matrix_instance_f64 matrix64_t;

/* Exported constants ----------------------------------------------------------------------*/
#define USE_MATRIX_BUFFER

/* Exported functions ----------------------------------------------------------------------*/  
void        Matrix_Clear( matrix_t *pMat );
void        Matrix_Init( matrix_t *pMat, float32_t *pArr, uint16_t rows, uint16_t cols );

arm_status  Matrix_Copy( matrix_t *pMat, const matrix_t *pMatC );
arm_status  Matrix_SetData( matrix_t *pMat, uint16_t rows, uint16_t cols, float32_t data );
arm_status  Matrix_GetData( const matrix_t *pMat, uint16_t rows, uint16_t cols, float32_t *data );
arm_status  Matrix_SetDiag( matrix_t *pMat, float32_t data );
arm_status  Matrix_GetDiag( matrix_t *pMat, matrix_t *pMatD );
arm_status  Matrix_SetMatrix( matrix_t *pMat, matrix_t *pMatS, uint16_t rowsPos, uint16_t colsPos );
arm_status  Matrix_GetMatrix( matrix_t *pMatG, matrix_t *pMat, uint16_t rowsPos, uint16_t colsPos );

arm_status  Matrix_Add( matrix_t *pMat, const matrix_t *pMatA1, const matrix_t *pMatA2 );
arm_status  Matrix_Sub( matrix_t *pMat, const matrix_t *pMatS1, const matrix_t *pMatS2 );
arm_status  Matrix_Mult( matrix_t *pMat, const matrix_t *pMatM1, const matrix_t *pMatM2 );
arm_status  Matrix_MultScale( matrix_t *pMat, const matrix_t *pMatM, float32_t scale );
arm_status  Matrix_Trans( matrix_t *pMat, const matrix_t *pMatT );
arm_status  Matrix_Inv( matrix_t *pMat, const matrix_t *pMatI );
arm_status  Matrix_Inv64( matrix_t *pMat, const matrix_t *pMatI );

void        Matrix_Print( matrix_t *pMat );
void        Matrix_Print64( matrix64_t *pMat );
void        Matrix_PrintInfo( matrix_t *pMat );

#ifdef __cplusplus
}
#endif

#endif

/*************************************** END OF FILE ****************************************/
