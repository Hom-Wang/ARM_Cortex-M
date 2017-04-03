/**
  *      __            ____
  *     / /__ _  __   / __/                      __  
  *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
  *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
  *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
  *                    /_/   github.com/KitSprout    
  * 
  * @file    matrix_arm.c
  * @author  KitSprout
  * @date    3-Apr-2017
  * @brief   
  * 
  */

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\stm32f4_system.h"
#include "algorithms\matrix_arm.h"
#include <string.h>

/** @addtogroup STM32_Algorithm
  * @{
  */

/* Private typedef -------------------------------------------------------------------------*/
/* Private define --------------------------------------------------------------------------*/
#define MATRIX_MAX_DIMENSION    ((uint32_t)12)
#define MATRIX_MAX_SIZE         ((uint32_t)144)   /* MATRIX_MAX_DIMENSION ^ 2 */
#define MATRIX_BUF_SIZE         ((uint32_t)MATRIX_MAX_SIZE << 1)

/* Private macro ---------------------------------------------------------------------------*/
/* Private variables -----------------------------------------------------------------------*/
#ifdef USE_MATRIX_BUFFER
static float64_t matrixBuf[MATRIX_BUF_SIZE] = {0};
#endif

/* Private function prototypes -------------------------------------------------------------*/
/* Private functions -----------------------------------------------------------------------*/

/**
  * @brief  Matrix_Clear
  */
void Matrix_Clear( matrix_t *pMat )
{
  memset(pMat->pData, 0, sizeof(float32_t) * pMat->numRows * pMat->numCols);
}

/**
  * @brief  Matrix_Init
  */
void Matrix_Init( matrix_t *pMat, float32_t *pArr, uint16_t rows, uint16_t cols )
{
  pMat->numRows = rows;
  pMat->numCols = cols;
  pMat->pData   = pArr;
}

/**
  * @brief  Matrix_Copy
  */
arm_status Matrix_Copy( matrix_t *pMat, const matrix_t *pMatC )
{
  uint32_t i;

  if ((pMat->numRows != pMatC->numRows) ||
      (pMat->numCols != pMatC->numCols)) {
    return ARM_MATH_SIZE_MISMATCH;
  }

  for (i = 0; i < pMat->numRows * pMat->numCols; i++) {
    pMat->pData[i] = pMatC->pData[i];
  }

  return ARM_MATH_SUCCESS;
}

/**
  * @brief  Matrix_SetData
  */
arm_status Matrix_SetData( matrix_t *pMat, uint16_t rows, uint16_t cols, float32_t data )
{
  uint32_t index;

  index = (rows - 1) * pMat->numCols + (cols - 1);

  if ((rows > pMat->numRows) ||
      (cols > pMat->numCols)) {
    return ARM_MATH_LENGTH_ERROR; 
  }

  pMat->pData[index] = data;

  return ARM_MATH_SUCCESS;
}

/**
  * @brief  Matrix_GetData
  */
arm_status Matrix_GetData( const matrix_t *pMat, uint16_t rows, uint16_t cols, float32_t *data )
{
  uint32_t index;

  index = (rows - 1) * pMat->numCols + (cols - 1);

  if ((rows > pMat->numRows) ||
      (cols > pMat->numCols)) {
    return ARM_MATH_LENGTH_ERROR; 
  }

  *data = pMat->pData[index];

  return ARM_MATH_SUCCESS;
}

/**
  * @brief  Matrix_SetDiag
  */
arm_status Matrix_SetDiag( matrix_t *pMat, float32_t data )
{
  uint32_t i;

  if (pMat->numRows != pMat->numCols) {
    return ARM_MATH_SIZE_MISMATCH;
  }

  for (i = 0; i < pMat->numRows; i++) {
    pMat->pData[i * pMat->numRows + i] = data;
  }

  return ARM_MATH_SUCCESS;
}

/**
  * @brief  Matrix_GetDiag
  */
arm_status Matrix_GetDiag( matrix_t *pMat, matrix_t *pMatD )
{
  uint32_t i, j;

  if ((pMat->numRows != pMatD->numRows) ||
      (pMat->numCols != pMatD->numCols) ||
      (pMat->numRows != pMat->numCols)) {
    return ARM_MATH_LENGTH_ERROR; 
  }

  if (pMat != pMatD) {
    Matrix_Clear(pMat);
    for (i = 0; i < pMat->numRows; i++) {
      pMat->pData[i * pMat->numRows + i] = pMatD->pData[i * pMatD->numRows + i];
    }
  }
  else {
    for (i = 0; i < pMat->numRows; i++) {
      for (j = 0; j < pMat->numCols; j++) {
        if (i != j) {
          pMat->pData[i * pMat->numCols + j] = 0.0f;
        }
      }
    }
  }

  return ARM_MATH_SUCCESS;
}

/**
  * @brief  Matrix_SetMatrix
  */
arm_status Matrix_SetMatrix( matrix_t *pMat, matrix_t *pMatS, uint16_t rowsPos, uint16_t colsPos )
{
  uint32_t i, j, cnt;

  const uint32_t rpos_total = rowsPos + pMatS->numRows;
  const uint32_t cpos_total = colsPos + pMatS->numCols;
  
  if (((rowsPos + pMatS->numRows) > pMat->numRows) ||
      ((colsPos + pMatS->numCols) > pMat->numCols)) {
    return ARM_MATH_LENGTH_ERROR; 
  }

  cnt = 0;
  for (i = rowsPos; i < rpos_total; i++) {
    for (j = colsPos; j < cpos_total; j++) {
      pMat->pData[i * pMat->numCols + j] = pMatS->pData[cnt++];
    }
  }

  return ARM_MATH_SUCCESS;
}

/**
  * @brief  Matrix_GetMatrix
  */
arm_status Matrix_GetMatrix( matrix_t *pMatG, matrix_t *pMat, uint16_t rowsPos, uint16_t colsPos )
{
  uint32_t i, j, cnt;

  const uint32_t rpos_total = rowsPos + pMatG->numRows;
  const uint32_t cpos_total = colsPos + pMatG->numCols;
  
  if (((rowsPos + pMatG->numRows) > pMat->numRows) ||
      ((colsPos + pMatG->numCols) > pMat->numCols)) {
    return ARM_MATH_LENGTH_ERROR; 
  }

  for (i = rowsPos; i < rpos_total; i++) {
    for (j = colsPos; j < cpos_total; j++) {
      pMatG->pData[cnt++] = pMat->pData[i * pMat->numCols + j];
    }
  }

  return ARM_MATH_SUCCESS;
}

/**
  * @brief  Matrix_Add
  */
inline arm_status Matrix_Add( matrix_t *pMat, const matrix_t *pMatA1, const matrix_t *pMatA2 )
{
  return arm_mat_add_f32(pMatA1, pMatA2, pMat);
}

/**
  * @brief  Matrix_Sub
  */
inline arm_status Matrix_Sub( matrix_t *pMat, const matrix_t *pMatS1, const matrix_t *pMatS2 )
{
  return arm_mat_sub_f32(pMatS1, pMatS2, pMat);
}

/**
  * @brief  Matrix_Mult
  */
#ifdef USE_MATRIX_BUFFER
arm_status Matrix_Mult( matrix_t *pMat, const matrix_t *pMatM1, const matrix_t *pMatM2 )
{
  if ((pMat->numRows   != pMatM1->numRows) ||
      (pMat->numCols   != pMatM2->numCols) ||
      (pMatM1->numCols != pMatM2->numRows) ) {
     return ARM_MATH_SIZE_MISMATCH;
  }

  if (pMat == pMatM1) {
    matrix_t matM = {
      .numRows = pMatM1->numRows,
      .numCols = pMatM1->numCols,
      .pData   = (float32_t *)matrixBuf
    };
    for (uint32_t i = 0; i < pMat->numRows * pMat->numCols; i++) {
      matM.pData[i] = pMatM1->pData[i];
    }
    return arm_mat_mult_f32(&matM, pMatM2, pMat);
  }
  else if(pMat == pMatM2) {
    matrix_t matM = {
      .numRows = pMatM2->numRows,
      .numCols = pMatM2->numCols,
      .pData   = (float32_t *)matrixBuf
    };
    for (uint32_t i = 0; i < pMat->numRows * pMat->numCols; i++) {
      matM.pData[i] = pMatM2->pData[i];
    }
    return arm_mat_mult_f32(pMatM1, &matM, pMat);
  }
  else {
    return arm_mat_mult_f32(pMatM1, pMatM2, pMat);
  }
}
#else
inline arm_status Matrix_Mult( matrix_t *pMat, const matrix_t *pMatM1, const matrix_t *pMatM2 )
{
  return arm_mat_mult_f32(pMatM1, pMatM2, pMat);
}
#endif

/**
  * @brief  Matrix_MultScale
  */
inline arm_status Matrix_MultScale( matrix_t *pMat, const matrix_t *pMatM, float32_t scale )
{
  return arm_mat_scale_f32(pMatM, scale, pMat);
}

/**
  * @brief  Matrix_Trans
  */
#ifdef USE_MATRIX_BUFFER
arm_status Matrix_Trans( matrix_t *pMat, const matrix_t *pMatT )
{
  if (pMat == pMatT) {
    matrix_t matTr = {
      .numRows = pMatT->numRows,
      .numCols = pMatT->numCols,
      .pData   = (float32_t *)matrixBuf
    };
    for (uint32_t i = 0; i < pMat->numRows * pMat->numCols; i++) {
      matTr.pData[i] = pMatT->pData[i];
    }

    pMat->numCols = matTr.numRows;
    pMat->numRows = matTr.numCols;

    return arm_mat_trans_f32(&matTr, pMat);
  }
  else {
    return arm_mat_trans_f32(pMatT, pMat);
  }
}
#else
inline arm_status Matrix_Trans( matrix_t *pMat, const matrix_t *pMatT )
{
  return arm_mat_trans_f32(pMatT, pMat);
}
#endif

/**
  * @brief  Matrix_Inv
  */
#ifdef USE_MATRIX_BUFFER
arm_status Matrix_Inv( matrix_t *pMat, const matrix_t *pMatI )
{
  if (pMat == pMatI) {
    matrix_t matInv = {
      .numRows = pMat->numRows,
      .numCols = pMat->numCols,
      .pData   = (float32_t *)matrixBuf
    };

    for (uint32_t i = 0; i < pMat->numRows * pMat->numCols; i++) {
      matInv.pData[i] = pMat->pData[i];
    }
    return arm_mat_inverse_f32(&matInv, pMat);
  }
  else {
    return arm_mat_inverse_f32(pMatI, pMat);
  }
}
#else
inline arm_status Matrix_Inv( matrix_t *pMat, const matrix_t *pMatI )
{
  return arm_mat_inverse_f32(pMatI, pMat);
}
#endif

/**
  * @brief  Matrix_Inv64
  */
#ifdef USE_MATRIX_BUFFER
arm_status Matrix_Inv64( matrix_t *pMat, const matrix_t *pMatI )
{
  arm_status status;

  matrix64_t matCov = {
    .numRows = pMat->numRows,
    .numCols = pMat->numCols,
    .pData   = matrixBuf
  };

  matrix64_t matInv = {
    .numRows = pMatI->numRows,
    .numCols = pMatI->numCols,
    .pData   = &matrixBuf[MATRIX_MAX_SIZE]
  };

  for (uint32_t i = 0; i < pMat->numRows * pMat->numCols; i++) {
    matInv.pData[i] = (float64_t)(pMatI->pData[i]);
  }

  status = arm_mat_inverse_f64(&matInv, &matCov);

  for (uint32_t i = 0; i < pMat->numRows * pMat->numCols; i++) {
    pMat->pData[i] = (float32_t)(matCov.pData[i]);
  }

  return (status);
}
#endif

/**
  * @brief  Matrix_Det
  */
//matrix_float_t Matrix_Det( matrix_t *pMat )
//{
//}

/**
  * @brief  Matrix_Print
  */
void Matrix_Print( matrix_t *pMat )
{
  for (uint32_t i = 0; i < pMat->numRows; i++) {
    printf(" ");
    for (uint32_t j = 0; j < pMat->numCols; j++) {
      if (pMat->pData[i * pMat->numCols + j] < 0) {
        printf("%.5f\t", pMat->pData[i * pMat->numCols + j]);
      }
      else {
        printf("+%.5f\t", pMat->pData[i * pMat->numCols + j]);
      }
    }
    printf("\r\n");
  }
}

/**
  * @brief  Matrix_Print64
  */
void Matrix_Print64( matrix64_t *pMat )
{
  for (uint32_t i = 0; i < pMat->numRows; i++) {
    printf(" ");
    for (uint32_t j = 0; j < pMat->numCols; j++) {
      if (pMat->pData[i * pMat->numCols + j] < 0) {
        printf("%.5f\t", pMat->pData[i * pMat->numCols + j]);
      }
      else {
        printf("+%.5f\t", pMat->pData[i * pMat->numCols + j]);
      }
    }
    printf("\r\n");
  }
}

/**
  * @brief  Matrix_PrintInfo
  */
void Matrix_PrintInfo( matrix_t *pMat )
{
  printf("matrix.rows = %d\r\n", pMat->numRows);
  printf("matrix.cols = %d\r\n", pMat->numCols);
  printf("matrix.data = %d\r\n", (uint32_t)pMat->pData);
  printf("\r\n");
}

/*************************************** END OF FILE ****************************************/
