/*
 * file: gpi_debouncer.c
 *
 * A simple library to debounce buttons through software.
 */

#ifndef GPI_DEBOUNCER_H
#define GPI_DEBOUNCER_H

//////////////
//////////////
/* includes */
//////////////
//////////////
#include "main.h"

//////////////////////
//////////////////////
/* public typedefs */
//////////////////////
//////////////////////
enum{
	rising,
	falling,
	none
};
typedef uint8_t edge_t;

typedef struct{
	GPIO_TypeDef* port;
	uint16_t pin;
	uint16_t threshold;

}gpi_init_t;

typedef struct{
	_Bool state, prev_state;
	uint16_t high_cnt, low_cnt;
	_Bool debouncing;

}gpi_process_t;


/* gpi_t
 *
 * Struct with the parameters
 */
typedef struct{
	gpi_init_t init;
	gpi_process_t process;
	_Bool deb_state;
	edge_t edge_flag;

}gpi_t;


////////////////////////
////////////////////////
/* external variables */
////////////////////////
////////////////////////
extern TIM_HandleTypeDef htim6;


/////////////////////////////////
/////////////////////////////////
/* public function prototypes */
/////////////////////////////////
/////////////////////////////////
/* init_GPI
 *
 * @brief: initializes a general purpose input
 * @param: port: input port
 * @param: pin: input pin
 * @param: threshold: number of repetitions of a same state (HIGH or LOW
 * pin states) to consider that the state is stabilized
 */
void init_gpi(volatile gpi_t* GPI, GPIO_TypeDef* port, uint16_t pin, uint16_t threshold);

/* gpi_startDebouncingProcess
 *
 * @brief:  Function called from the corresponding external interrupt. It starts the debouncing process
 * 			and the timer if necessary.
 * @param: *GPI: pointer to the corresponding gpi_t structure
 */
void gpi_startDebouncingProcess(volatile gpi_t* GPI);

/* gpi_debouncingProcess
 *
 * @brief: It process the debouncing process
 * @param: *GPI: pointer to the corresponding gpi_t structure
 */
void gpi_debouncingProcess(volatile gpi_t* GPI);

#endif
