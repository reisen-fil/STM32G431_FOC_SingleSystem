#include "Global_Variable.h"

/* AS5600 */
float angle_prev=0; 
int full_rotations=0; // full rotation tracking;
float Zero_Angle=0;			//AS5600_Origin_Angle

/* SlaveDevice_State */
Device_State SlaveDevice_State;

/* FOC */
struct Three_Phase Current_abc;		/* Origin_abc */
struct AlphaBeta_Aix Clark_AB;		/* Clark_AB */
struct DQ_Aix Park_DQ;					/* Park_DQ */
struct SVPWM_Control FOC_ctrl;
struct SVPWM_Duty FOC_Duty;

float Target_Uq;            /* ?????? */

/* debug */
uint8_t rx_buffer[BUFF_SIZE];           /* Ω” ’ª∫≥Â«¯ */
uint8_t UART_RX_Flag;


