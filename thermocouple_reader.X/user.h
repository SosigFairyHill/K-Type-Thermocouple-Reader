/******************************************************************************/
/* User Level #define Macros                                                  */
/******************************************************************************/
#define SEGMENT_A PORTCbits.RC6
#define SEGMENT_B PORTCbits.RC7
#define SEGMENT_C PORTBbits.RB5
#define SEGMENT_D PORTBbits.RB4
#define SEGMENT_E PORTBbits.RB3
#define SEGMENT_F PORTBbits.RB2
#define SEGMENT_G PORTBbits.RB1
#define SEGMENT_DP PORTBbits.RB0
/* TODO Application specific user parameters used in user.c may go here */

/******************************************************************************/
/* User Function Prototypes                                                   */
/******************************************************************************/

/* TODO User level functions prototypes (i.e. InitApp) go here */

void InitApp(void);         /* I/O and Peripheral Initialization */

char* display_number(float);

void select_chip(int);

void buf_wait();

void seven_segment(float, int);

int binary_decimal(int n);

float temp_voltage(float);

float voltage_temp(float);