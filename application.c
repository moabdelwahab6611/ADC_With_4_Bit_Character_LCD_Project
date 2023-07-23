/* 
 * File     : application.c
 * Author   : Mohamed Ahmed Abdel Wahab
 * LinkedIn : https://www.linkedin.com/in/mohamed-abdel-wahab-162413253/
 * Github   : https://github.com/moabdelwahab6611
 * Created on May 21, 2023, 5:38 PM
 */

/**************************Includes-Section*****************************/
#include "application.h"
#include "MCAL_Layer/ADC/hal_adc.h"
#include "ECU_Layer/Chr_LCD/ecu_chr_lcd.h"
/***********************************************************************/

/***********************************************************************/
uint16 adc_result_1;
uint16 adc_result_2;
uint16 adc_result_3;
uint16 adc_result_4;
uint8 adc_result_1_txt[6];
uint8 adc_result_2_txt[6];
uint8 adc_result_3_txt[6];
uint8 adc_result_4_txt[6];
uint8 ADC_Req = 0;

   
void ADC_DefaultInterruptHandler(void);

chr_4bit_lcd_t lcd_1 =
{
    .lcd_rs.port = PORTC_INDEX,
    .lcd_rs.pin = GPIO_PIN0,
    .lcd_rs.direction = GPIO_DIRECTION_OUTPUT,
    .lcd_rs.logic = GPIO_LOW,
    .lcd_en.port = PORTC_INDEX,
    .lcd_en.pin = GPIO_PIN1,
    .lcd_en.direction = GPIO_DIRECTION_OUTPUT,
    .lcd_en.logic = GPIO_LOW,
    .lcd_data[0].port = PORTC_INDEX,
    .lcd_data[0].pin = GPIO_PIN2,
    .lcd_data[0].direction = GPIO_DIRECTION_OUTPUT,
    .lcd_data[0].logic = GPIO_LOW,
    .lcd_data[1].port = PORTC_INDEX,
    .lcd_data[1].pin = GPIO_PIN3,
    .lcd_data[1].direction = GPIO_DIRECTION_OUTPUT,
    .lcd_data[1].logic = GPIO_LOW,
    .lcd_data[2].port = PORTC_INDEX,
    .lcd_data[2].pin = GPIO_PIN4,
    .lcd_data[2].direction = GPIO_DIRECTION_OUTPUT,
    .lcd_data[2].logic = GPIO_LOW,
    .lcd_data[3].port = PORTC_INDEX,
    .lcd_data[3].pin = GPIO_PIN5,
    .lcd_data[3].direction = GPIO_DIRECTION_OUTPUT,
    .lcd_data[3].logic = GPIO_LOW
};

adc_config_t adc_1 =
{
    .ADC_InterruptHandler = ADC_DefaultInterruptHandler,
    .acquisition_time = ADC_12_TAD,
    .adc_channel = ADC_CHANNEL_AN0,
    .conversion_clock = ADC_CONVERSION_CLOCK_FOSC_DIV_16,
    .result_format = ADC_RESULT_RIGHT,
    .voltage_reference = ADC_VOLTAGE_REFERENCE_DISABLED
};
/***********************************************************************/

/***********************Main Function-Section***************************/
int main() 
{ 
    Std_ReturnType ret = E_NOT_OK;
    application_intialize();
    ret =  ADC_Init(&adc_1);
    ret = lcd_4bit_intialize(&lcd_1);
    
    
    ret = lcd_4bit_send_string_pos(&lcd_1, 1, 7, "ADC Test");
    __delay_ms(2000);
    ret = lcd_4bit_send_command(&lcd_1, _LCD_CLEAR);
    
    ret = lcd_4bit_send_string_pos(&lcd_1, 1, 1, "Port0: ");
    ret = lcd_4bit_send_string_pos(&lcd_1, 2, 1, "Port1: ");
    ret = lcd_4bit_send_string_pos(&lcd_1, 3, 1, "Port2: ");
    ret = lcd_4bit_send_string_pos(&lcd_1, 4, 1, "Port3: ");
    
    while(1)
    {   
        if(0 == ADC_Req)
        {
            ret = ADC_StartConversion_Interrupt(&adc_1, ADC_CHANNEL_AN0);
        }
        else if(1 == ADC_Req)
        {
            ret = ADC_StartConversion_Interrupt(&adc_1, ADC_CHANNEL_AN1);
        }
        else if(2 == ADC_Req)
        {
            ret = ADC_StartConversion_Interrupt(&adc_1, ADC_CHANNEL_AN2);
        }
        else if(3 == ADC_Req)
        {
            ret = ADC_StartConversion_Interrupt(&adc_1, ADC_CHANNEL_AN3);
        }
        else{/*****Nothing*****/}
        
        ret = convert_uint16_to_string(adc_result_1, adc_result_1_txt);
        ret = convert_uint16_to_string(adc_result_2, adc_result_2_txt);
        ret = convert_uint16_to_string(adc_result_3, adc_result_3_txt);
        ret = convert_uint16_to_string(adc_result_4, adc_result_4_txt);
        
        ret = lcd_4bit_send_string_pos(&lcd_1, 1, 7, adc_result_1_txt);
        ret = lcd_4bit_send_string_pos(&lcd_1, 2, 7, adc_result_2_txt);
        ret = lcd_4bit_send_string_pos(&lcd_1, 3, 7, adc_result_3_txt);
        ret = lcd_4bit_send_string_pos(&lcd_1, 4, 7, adc_result_4_txt);
    }
    return (EXIT_SUCCESS);
}
/***********************************************************************/

/*************************Functions-Section*****************************/
void application_intialize(void)
{
    Std_ReturnType ret = E_NOT_OK;   
    ecu_layer_intialize();
    ret = ADC_Init(&adc_1);
}

void ADC_DefaultInterruptHandler(void)
{
    Std_ReturnType ret = E_NOT_OK; 
    if(0 == ADC_Req)
        {
            ret = ADC_GetConversionResult(&adc_1, adc_result_1);
            ADC_Req = 1;
        }
        else if(1 == ADC_Req)
        {
            ret = ADC_GetConversionResult(&adc_1, adc_result_2);
            ADC_Req = 2;
        }
        else if(2 == ADC_Req)
        {
            ret = ADC_GetConversionResult(&adc_1, adc_result_3);
            ADC_Req = 3;
        }
        else if(3 == ADC_Req)
        {
            ret = ADC_GetConversionResult(&adc_1, adc_result_4);
            ADC_Req = 0;
        }
        else{/*****Nothing*****/}
}
/***********************************************************************/