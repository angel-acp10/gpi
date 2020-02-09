/*
 * file: gpi_debouncer.c
 *
 * A simple library to debounce buttons through software.
 */
//////////////
//////////////
/* includes */
//////////////
//////////////
#include "gpi_debouncer.h"


//////////////////////
//////////////////////
/* private typedefs */
//////////////////////
//////////////////////
enum{
	low,
	high,
	undef
};
typedef uint8_t debState_t;


typedef struct{
	uint16_t max_limit;
	int16_t liveUse_cnt;
}timerStats_t;


///////////////////////
///////////////////////
/* private variables */
///////////////////////
///////////////////////
volatile timerStats_t timStats = {0, 0};


/////////////////////////////////
/////////////////////////////////
/* private function prototypes */
/////////////////////////////////
/////////////////////////////////
static debState_t debounced_GPIO_ReadPin(volatile gpi_t *GPI);

/* inside these functions, specific MCU functions must be included */ 
static void startTimer();
static void stopTimer();
static _Bool readPin(GPIO_TypeDef* port, uint16_t pin);

///////////////////////
///////////////////////
/* public functions */
///////////////////////
///////////////////////
/* init_GPI
 *
 * @brief: initializes a general purpose input
 * @param: port: input port
 * @param: pin: input pin
 * @param: threshold: number of repetitions of a same state (HIGH or LOW
 * pin states) to consider that the state is stabilized
 */
void init_gpi(volatile gpi_t* GPI, GPIO_TypeDef* port, uint16_t pin, uint16_t threshold)
{
	GPI->init.port = port;
	GPI->init.pin = pin;
	GPI->init.threshold = threshold;

	GPI->process.high_cnt = 0;
	GPI->process.low_cnt = 0;
	GPI->process.debouncing = 0;

	GPI->edge_flag = none;
	GPI->process.state = readPin(GPI->init.port, GPI->init.pin);
	GPI->process.prev_state = GPI->process.state;
	GPI->deb_state = GPI->process.state;

	timStats.max_limit++;

}


/* gpi_startDebouncingProcess
 *
 * @brief:  Function called from the corresponding external interrupt. It starts the debouncing process
 * 			and the timer if necessary.
 * @param: *GPI: pointer to the corresponding gpi_t structure
 */
void gpi_startDebouncingProcess(volatile gpi_t* GPI)
{

	GPI->process.prev_state = GPI->process.state;
	GPI->process.state = readPin(GPI->init.port, GPI->init.pin);

	// if an edge is detected
	if(GPI->process.prev_state != GPI->process.state)
	{
		/* if timer is not being used, start it */
		if(timStats.liveUse_cnt <= 0)
			startTimer();

		/* update the timer stats only if this GPI is not already being debounced */
		if(!GPI->process.debouncing)
		{
			timStats.liveUse_cnt++;

			if(timStats.liveUse_cnt > timStats.max_limit)
				timStats.liveUse_cnt = timStats.max_limit;
		}

		GPI->process.debouncing = 1;
	}

}

/* gpi_debouncingProcess
 *
 * @brief: It process the debouncing process
 * @param: *GPI: pointer to the corresponding gpi_t structure
 */
void gpi_debouncingProcess(volatile gpi_t* GPI)
{
	if(GPI->process.debouncing)
	{
		debState_t pinState = debounced_GPIO_ReadPin(GPI);

		if(pinState == high)
		{
			GPI->deb_state = 1;
			GPI->edge_flag = rising;
			GPI->process.debouncing = 0;

			/* update the timer statistics, if it is not used anymore, stop it */
			timStats.liveUse_cnt--;
			if(timStats.liveUse_cnt <=0){
				timStats.liveUse_cnt = 0;
				stopTimer();
			}

		}
		else if(pinState == low)
		{
			GPI->deb_state = 0;
			GPI->edge_flag = falling;
			GPI->process.debouncing = 0;

			/* update the timer statistics, if it is not used anymore, stop it */
			timStats.liveUse_cnt--;
			if(timStats.liveUse_cnt <=0){
				timStats.liveUse_cnt = 0;
				stopTimer();
			}


		}
	}
}

///////////////////////
///////////////////////
/* private functions */
///////////////////////
///////////////////////

/* debounced_GPIO_ReadPin
 *
 * @brief  It must be called a certain number of times until it returns a debounced high or low.
 * 			A state must be repeated "CONFIDENCE_NUMBER" times before it returns the state itself.
 *
 * @param  *GPI: pointer to the corresponding gpi_t structure
 * @retval The debounced pin state, where means that the function needs to be called again.
 */
static debState_t debounced_GPIO_ReadPin(volatile gpi_t *GPI)
{
	if( readPin(GPI->init.port, GPI->init.pin) )
	{
		GPI->process.high_cnt++;
		GPI->process.low_cnt = 0;
		if(GPI->process.high_cnt >= GPI->init.threshold)
		{
			GPI->process.high_cnt = 0;
			return high;
		}
	}
	else
	{
		GPI->process.high_cnt = 0;
		GPI->process.low_cnt++;
		if(GPI->process.low_cnt >= GPI->init.threshold)
		{
			GPI->process.low_cnt = 0;
			return low;
		}
	}

	return undef;
}

/////////////////////////
/////////////////////////
/* USER IMPLEMENTATION */
/////////////////////////
/////////////////////////

static void startTimer()
{
	
}

static void stopTimer()
{
	
}

static _Bool readPin(GPIO_TypeDef* port, uint16_t pin)
{
	
}

