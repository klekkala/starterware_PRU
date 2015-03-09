/**********************************************************************************
* Author: Kiran Kumar Lekkala                                                     *
* Filename: pot_adc.c                                                             *
* Description: This is a C code to output the voltage as a scale from 0 to 1 when *
* a Potentiometer is connected to the P9_27 (GPIO3_19) across the 1.8V and the ground pin.   *
* It uses the Starterware library for PRU in Beaglebone black. This example is    *
* presented to demonstrate the usage of Starterware library for the Beaglebone.	  *
* This was originaly developed for the am335x main processor and can also be      *
* modified for accessing the main core's peripheral unit by PRU                   *
* PRU-Compiler version: 2.0.0B1                                                   *
**********************************************************************************/



#include <stdio.h>
#include <stdlib.h>
#include <consoleUtils.h>
#include <soc_AM335x.h>
#include <hw_types>
#include <tsc_adc.h>

/* Internal macro definitions */
#define RESOL_X_MILLION            (439u)


/* Varibles for sampling data */
volatile unsigned int flag = 1;
unsigned int sample;
unsigned int value;


/* Functions declared */
static void ADCConfigure(void)
static void CleanUpInterrupts(void)
static void ADC_Read(void)


int main(){

    /* Initialize interrupt controller */
    SetupIntc();

    /* Initialize ADC */
    ADCConfigure();

    while(flag);

    val1 = (sample1 * RESOL_X_MILLION) / 1000;

    ConsoleUtilsPrintf("Potentiometer voltage scaled from 0 to 1");

    ConsoleUtilsPrintf("%d", val1);

    ConsoleUtilsPrintf("mV\r\n");

    while(1);

}

/* ADC is configured */
static void ADCConfigure(void)
{
    /* Enable the clock for touch screen */
    TSCADCModuleClkConfig();

    TSCADCPinMuxSetUp();

    /* Configures ADC to 3Mhz */
    TSCADCConfigureAFEClock(SOC_ADC_TSC_0_REGS, 24000000, 3000000);

    /* Enable Transistor bias */
    TSCADCTSTransistorConfig(SOC_ADC_TSC_0_REGS, TSCADC_TRANSISTOR_ENABLE);

    TSCADCStepIDTagConfig(SOC_ADC_TSC_0_REGS, 1);

    /* Disable Write Protection of Step Configuration regs*/
    TSCADCStepConfigProtectionDisable(SOC_ADC_TSC_0_REGS);

    /* Configure step 1 for channel 1(AN0)*/
    StepConfigure(0, TSCADC_FIFO_0, TSCADC_POSITIVE_INP_CHANNEL1);

    /* Configure step 2 for channel 2(AN1)*/
    StepConfigure(1, TSCADC_FIFO_1, TSCADC_POSITIVE_INP_CHANNEL2);

    /* General purpose inputs */
    TSCADCTSModeConfig(SOC_ADC_TSC_0_REGS, TSCADC_GENERAL_PURPOSE_MODE);

    /* Enable step 1 */
    TSCADCConfigureStepEnable(SOC_ADC_TSC_0_REGS, 1, 1);

    /* Enable step 2 */
    TSCADCConfigureStepEnable(SOC_ADC_TSC_0_REGS, 2, 1);

    /* Clear the status of all interrupts */
    CleanUpInterrupts();

    /* End of sequence interrupt is enable */
    TSCADCEventInterruptEnable(SOC_ADC_TSC_0_REGS, TSCADC_END_OF_SEQUENCE_INT);

    /* Enable the TSC_ADC_SS module*/
    TSCADCModuleStateSet(SOC_ADC_TSC_0_REGS, TSCADC_MODULE_ENABLE);
}


void StepConfigure(unsigned int stepSel, unsigned int fifo,
                   unsigned int positiveInpChannel)
{
    /* Configure ADC to Single ended operation mode */
    TSCADCTSStepOperationModeControl(SOC_ADC_TSC_0_REGS,
                                  TSCADC_SINGLE_ENDED_OPER_MODE, stepSel);

    /* Configure step to select Channel, refernce voltages */
    TSCADCTSStepConfig(SOC_ADC_TSC_0_REGS, stepSel, TSCADC_NEGATIVE_REF_VSSA,
                    positiveInpChannel, TSCADC_NEGATIVE_INP_CHANNEL1, TSCADC_POSITIVE_REF_VDDA);

    /* XPPSW Pin is on, Which pull up the AN0 line*/
    TSCADCTSStepAnalogSupplyConfig(SOC_ADC_TSC_0_REGS, TSCADC_XPPSW_PIN_ON, TSCADC_XNPSW_PIN_OFF,
                                TSCADC_YPPSW_PIN_OFF, stepSel);

    /* XNNSW Pin is on, Which pull down the AN1 line*/
    TSCADCTSStepAnalogGroundConfig(SOC_ADC_TSC_0_REGS, TSCADC_XNNSW_PIN_ON, TSCADC_YPNSW_PIN_OFF,
                                TSCADC_YNNSW_PIN_OFF,  TSCADC_WPNSW_PIN_OFF, stepSel);

    /* select fifo 0 or 1*/
    TSCADCTSStepFIFOSelConfig(SOC_ADC_TSC_0_REGS, stepSel, fifo);

    /* Configure ADC to one short mode */
    TSCADCTSStepModeConfig(SOC_ADC_TSC_0_REGS, stepSel,  TSCADC_ONE_SHOT_SOFTWARE_ENABLED);
}

/* Clear status for the PRU0 interrupt */
static void CleanUpInterrupts(void)
{
    
}


/* Reads the data from FIFO 0 and FIFO 1 Buffers */
static void ADC_Read()
{
    volatile unsigned int status;

    status = TSCADCIntStatus(SOC_ADC_TSC_0_REGS);

    TSCADCIntStatusClear(SOC_ADC_TSC_0_REGS, status);

    if(status & TSCADC_END_OF_SEQUENCE_INT)
    {
         /* Read data from fifo 0 */
         sample1 = TSCADCFIFOADCDataRead(SOC_ADC_TSC_0_REGS, TSCADC_FIFO_0);

         /* Read data from fif 1*/
         sample2 = TSCADCFIFOADCDataRead(SOC_ADC_TSC_0_REGS, TSCADC_FIFO_1);

         flag = 0;
    }
}


