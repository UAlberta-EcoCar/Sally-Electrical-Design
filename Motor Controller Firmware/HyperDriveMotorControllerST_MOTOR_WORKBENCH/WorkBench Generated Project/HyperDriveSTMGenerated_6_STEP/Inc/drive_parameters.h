
/**
  ******************************************************************************
  * @file    drive_parameters.h
  * @author  Motor Control SDK Team, ST Microelectronics
  * @brief   This file contains the parameters needed for the Motor Control SDK
  *          in order to configure a motor drive.
  *
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef DRIVE_PARAMETERS_H
#define DRIVE_PARAMETERS_H

/************************
 *** Motor Parameters ***
 ************************/

/******** MAIN AND AUXILIARY SPEED/POSITION SENSOR(S) SETTINGS SECTION ********/

/*** Speed measurement settings ***/
#define MAX_APPLICATION_SPEED_RPM          5000 /*!< rpm, mechanical */
#define MIN_APPLICATION_SPEED_RPM          (0 + U_RPM / SPEED_UNIT) /*!< rpm, mechanical, absolute value */
#define M1_SS_MEAS_ERRORS_BEFORE_FAULTS    3 /*!< Number of speed measurement errors before main sensor goes in fault */

/****** Hall sensors ************/
#define HALL_AVERAGING_FIFO_DEPTH          16 /*!< depth of the FIFO used to average  mechanical speed in 0.1Hz resolution */
#define HALL_MTPA  true

/**************************    DRIVE SETTINGS SECTION   **********************/
/* PWM generation and current reading */
#define PWM_FREQUENCY                      27000
#define PWM_FREQ_SCALING                   1
#define PWM_FREQUENCY_REF                  60000
#define LOW_SIDE_SIGNALS_ENABLING          LS_PWM_TIMER
#define SW_DEADTIME_NS                     10 /*!< Dead-time to be inserted by FW, only if low side signals are enabled */

/* High frequency task regulation loop */
#define REGULATION_EXECUTION_RATE          2 /*!< Execution rate in number of PWM  cycles */

/* Speed control loop */
#define SPEED_LOOP_FREQUENCY_HZ            (uint16_t)1000 /*!<Execution rate of speed regulation loop (Hz) */
#define PID_SPEED_KP_DEFAULT          3108/(SPEED_UNIT/10) /* Workbench compute the gain for 01Hz unit*/
#define PID_SPEED_KI_DEFAULT          943/(SPEED_UNIT/10) /* Workbench compute the gain for 01Hz unit*/
#define PID_SPEED_KD_DEFAULT          0/(SPEED_UNIT/10) /* Workbench compute the gain for 01Hz unit*/

/* Speed PID parameter dividers */
#define SP_KPDIV                           128
#define SP_KIDIV                           16384
#define SP_KDDIV                           16
#define SP_KPDIV_LOG                       LOG2((128))
#define SP_KIDIV_LOG                       LOG2((16384))
#define SP_KDDIV_LOG                       LOG2((16))

/* USER CODE BEGIN PID_SPEED_INTEGRAL_INIT_DIV */
#define PID_SPEED_INTEGRAL_INIT_DIV 1 /*  */
/* USER CODE END PID_SPEED_INTEGRAL_INIT_DIV */

#define SPD_DIFFERENTIAL_TERM_ENABLING     DISABLE
#define PERIODMAX                          (uint16_t)((PWM_PERIOD_CYCLES * 100 / 100) + 1)
#define LF_TIMER_ARR                       65535
#define LF_TIMER_PSC                       100

/* Default settings */
#define DEFAULT_CONTROL_MODE               MCM_SPEED_MODE
#define DEFAULT_DRIVE_MODE                 VM /*!< VOLTAGE_MODE (VM) or CURRENT_MODE (CM) */
#define DEFAULT_TARGET_SPEED_RPM           1500
#define DEFAULT_TARGET_SPEED_UNIT          (DEFAULT_TARGET_SPEED_RPM*SPEED_UNIT/U_RPM)

/**************************    FIRMWARE PROTECTIONS SECTION   *****************/
#define OV_VOLTAGE_THRESHOLD_V             50 /*!< Over-voltage threshold */
#define UD_VOLTAGE_THRESHOLD_V             8 /*!< Under-voltage threshold */
#ifdef NOT_IMPLEMENTED
#define ON_OVER_VOLTAGE                    false /*!< TURN_OFF_PWM, TURN_ON_R_BRAKE or TURN_ON_LOW_SIDES */
#endif /* NOT_IMPLEMENTED */
#define OV_TEMPERATURE_THRESHOLD_C         70 /*!< Celsius degrees */
#define OV_TEMPERATURE_HYSTERESIS_C        10 /*!< Celsius degrees */
#define HW_OV_CURRENT_PROT_BYPASS          DISABLE /*!< In case ON_OVER_VOLTAGE is set to TURN_ON_LOW_SIDES this feature
                                                        may be used to bypass HW over-current protection (if supported
                                                        by power stage) */
#define OVP_INVERTINGINPUT_MODE            INT_MODE
#define OVP_INVERTINGINPUT_MODE2           INT_MODE
#define OVP_SELECTION                      COMP_Selection_COMP1
#define OVP_SELECTION2                     COMP_Selection_COMP1

/******************************   START-UP PARAMETERS   **********************/

#define TRANSITION_DURATION                25 /* Switch over duration, ms */

/******************************   Current sensing Motor 1   **********************/
#define ADC_SAMPLING_CYCLES                (19 + SAMPLING_CYCLE_CORRECTION)

/******************************   ADDITIONAL FEATURES   **********************/

/* **** Potentiometer parameters **** */
/** @brief Sampling time set to the ADC channel used by the potentiometer component */
#define POTENTIOMETER_ADC_SAMPLING_TIME_M1 LL_ADC_SAMPLING_CYCLE(47)

/**
 * @brief Speed reference set to Motor 1 when the potentiometer is at its maximum
 *
 * This value is expressed in #SPEED_UNIT.
 *
 * Default value is #MAX_APPLICATION_SPEED_UNIT.
 *
 * @sa POTENTIOMETER_MIN_SPEED_M1
 */
#define POTENTIOMETER_MAX_SPEED_M1         MAX_APPLICATION_SPEED_UNIT

/**
 * @brief Speed reference set to Motor 1 when the potentiometer is at its minimum
 *
 * This value is expressed in #SPEED_UNIT.
 *
 * Default value is 10 % of #MAX_APPLICATION_SPEED_UNIT.
 *
 * @sa POTENTIOMETER_MAX_SPEED_M1
 */
#define POTENTIOMETER_MIN_SPEED_M1         ((MAX_APPLICATION_SPEED_UNIT)/10)

/**
 * @brief Potentiometer change threshold to trigger speed reference update for Motor 1
 *
 * When the potentiometer value differs from the current speed reference by more than this
 * threshold, the speed reference set to the motor is adjusted to match the potentiometer value.
 *
 * The threshold is expressed in u16digits. Its default value is set to 13% of the potentiometer
 * aquisition range
 *
 */
 #define POTENTIOMETER_SPEED_ADJUSTMENT_RANGE_M1 (655)

/**
 * @brief Acceleration used to compute ramp duration when setting speed reference to Motor 1
 *
 * This acceleration is expressed in #SPEED_UNIT/s. Its default value is 100 Hz/s (provided
 * that #SPEED_UNIT is #U_01HZ).
 *
 */
 #define POTENTIOMETER_RAMP_SLOPE_M1       1000

/**
 * @brief Bandwith of the low pass filter applied on the potentiometer values
 *
 * @see SpeedPotentiometer_Handle_t::LPFilterBandwidthPOW2
 */
#define POTENTIOMETER_LPF_BANDWIDTH_POW2_M1 4

/*** On the fly start-up ***/

/**************************
 *** Control Parameters ***
 **************************/

/* ##@@_USER_CODE_START_##@@ */
/* ##@@_USER_CODE_END_##@@ */

#endif /*DRIVE_PARAMETERS_H*/
/******************* (C) COPYRIGHT 2024 STMicroelectronics *****END OF FILE****/
