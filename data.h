/*----------------------------------------------------------------------------
 * data.h
 *----------------------------------------------------------------------------*/

#ifndef DATA_H_
#define DATA_H_

/*----------------------------------------------------------------------------
 * Description:
 * 		Types of movements that can be achieved
 *----------------------------------------------------------------------------*/
typedef enum
{
	STOP,
	FORWARD,
	BACKWARD,
	SPIN_CW,
	SPIN_CCW
} Type;

/*----------------------------------------------------------------------------
 * Description:
 * 		Move structure that contains the distance, speed and movement type
 *----------------------------------------------------------------------------*/
typedef struct
{
	double distance;
	double speed;
	Type type;
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
