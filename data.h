/*----------------------------------------------------------------------------
 * data.h
 *----------------------------------------------------------------------------*/

#ifndef DATA_H_
#define DATA_H_

/*----------------------------------------------------------------------------
 * Description:
 * 		Move structure that contains the distance, speed and movement type
 *----------------------------------------------------------------------------*/
typedef struct
{
	double distance;
	double speed;
	int type;
} Move;

/*----------------------------------------------------------------------------
 * Description:
 * 		Data structure that contains the temperature array and Move object
 *----------------------------------------------------------------------------*/
typedef struct
{
	uint8_t temperatures[9];
	Move move;
} Data;

#endif
