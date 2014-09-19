/*
*********************************************************************************************************
*                                     MICIRUM BOARD SUPPORT PACKAGE
*
*                             (c) Copyright 2012; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used to develop a similar product.
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                        BOARD SUPPORT PACKAGE
*
*                                           CYPRESS PSoC5
*                                              with the
*                                     CY8CKIT-050 Development Kit
*
* Filename      : bsp.h
* Version       : V1.10
* Programmer(s) : DC
*
* V1.10		Modified by YFS (Cypress) to simplify BSP. Removed unused LED and push button APIs.
*			File now contains only the APIs needed to run the OS on a PSoC device. The BSP is
*			provided in the Generated_Source folders. Removed need for bsp_psoc.h file.
*
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                                 MODULE
*
* Note(s) : (1) This header file is protected from multiple pre-processor inclusion through use of the
*               BSP present pre-processor macro definition.
*********************************************************************************************************
*/

#ifndef  BSP_PRESENT
#define  BSP_PRESENT


/*
*********************************************************************************************************
*                                                 EXTERNS
*********************************************************************************************************
*/

#ifdef   BSP_MODULE
#define  BSP_EXT
#else
#define  BSP_EXT  extern
#endif


/*
*********************************************************************************************************
*                                              INCLUDE FILES
*********************************************************************************************************
*/

#include  <cpu.h>
#include  <lib_def.h>

#include  <app_cfg.h>


/* This include provides the definition of PSoC bus clock frequency */
#include  <cyfitter.h>


/*
*********************************************************************************************************
*                                                  DEFINES
*********************************************************************************************************
*/

/* Define for BUS_CLK - used to set tick timer interval */
 #define  BSP_PSOC_CPU_FREQ		BCLK__BUS_CLK__HZ                                                              

/*
*********************************************************************************************************
*                                               DATA TYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            GLOBAL VARIABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                                 MACRO'S
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                           FUNCTION PROTOTYPES
*********************************************************************************************************
*/

void         BSP_PreInit                 (void);

void         BSP_PostInit                (void);

void         BSP_CPU_TickInit            (void);

CPU_INT32U   BSP_CPU_ClkFreq             (void);


/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/


#endif                                                          /* End of module include.                               */
