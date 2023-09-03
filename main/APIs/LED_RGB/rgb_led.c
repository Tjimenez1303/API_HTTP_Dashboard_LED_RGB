/*
 * rgb_led.c
 *
 *  Created on: Oct 11, 2021
 *      Author: kjagu
 */

#include <stdbool.h>

#include <stdio.h>
#include <string.h>
#include <math.h>
#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"
#include "stdlib.h"

#include "driver/ledc.h"
#include "rgb_led.h"


//Variables del ADC
int raw = 0;
int res_2 = 1000;
float temp = 1.0;
float v_res = 0.0;
float res_ntc = 0.0;
int temp_index = 0;

float valores_res_ntc[] = {3210,2830,2500,2210,1960,1750,1560, 1390, 1240, 1110,1000, 899, 810, 731, 661, 599, 543, 493, 449, 409, 373, 341, 312, 286, 262,241,221,204,188,174,160,148,137,127,118,110,102,95.1,88.6,82.7,79.3};
float valores_temp_ntc[] = {0,2.5,5,7.5,10,12.5,15,17.5,20,22.5,25,27.5,30,32.5,35,37.5,40,42.5,45,47.5,50,52.5,55,57.5,60,62.5,65,67.5,70,72.5,75,77.5,80,82.5,85,87.5,90,92.5,95,97.5,99};


// RGB LED Configuration Array
ledc_info_t ledc_ch[RGB_LED_CHANNEL_NUM];

// handle for rgb_led_pwm_init
bool g_pwm_init_handle = false;


// Función de inicialización del ADC
esp_err_t init_ADC(void){
    adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_11); //GPIO Vp
    adc1_config_width(ADC_WIDTH_BIT_12); //4096
    return ESP_OK;
}

/**
 * Initializes the RGB LED settings per channel, including
 * the GPIO for each color, mode and timer configuration.
 */
void rgb_led_pwm_init(void)
{
	int rgb_ch;

	// Red
	ledc_ch[0].channel		= LEDC_CHANNEL_0;
	ledc_ch[0].gpio			= RGB_LED_RED_GPIO;
	ledc_ch[0].mode			= LEDC_HIGH_SPEED_MODE;
	ledc_ch[0].timer_index	= LEDC_TIMER_0;

	// Green
	ledc_ch[1].channel		= LEDC_CHANNEL_1;
	ledc_ch[1].gpio			= RGB_LED_GREEN_GPIO;
	ledc_ch[1].mode			= LEDC_HIGH_SPEED_MODE;
	ledc_ch[1].timer_index	= LEDC_TIMER_0;

	// Blue
	ledc_ch[2].channel		= LEDC_CHANNEL_2;
	ledc_ch[2].gpio			= RGB_LED_BLUE_GPIO;
	ledc_ch[2].mode			= LEDC_HIGH_SPEED_MODE;
	ledc_ch[2].timer_index	= LEDC_TIMER_0;

	// Configure timer zero
	ledc_timer_config_t ledc_timer =
	{
		.duty_resolution	= LEDC_TIMER_8_BIT,
		.freq_hz			= 100,
		.speed_mode			= LEDC_HIGH_SPEED_MODE,
		.timer_num			= LEDC_TIMER_0
	};
	ledc_timer_config(&ledc_timer);

	// Configure channels
	for (rgb_ch = 0; rgb_ch < RGB_LED_CHANNEL_NUM; rgb_ch++)
	{
		ledc_channel_config_t ledc_channel =
		{
			.channel	= ledc_ch[rgb_ch].channel,
			.duty		= 0,
			.hpoint		= 0,
			.gpio_num	= ledc_ch[rgb_ch].gpio,
			.intr_type	= LEDC_INTR_DISABLE,
			.speed_mode = ledc_ch[rgb_ch].mode,
			.timer_sel	= ledc_ch[rgb_ch].timer_index,
		};
		ledc_channel_config(&ledc_channel);
	}

	g_pwm_init_handle = true;
}

/**
 * Sets the RGB color.
 */
void rgb_led_set_color(uint8_t red, uint8_t green, uint8_t blue)
{
	// Value should be 0 - 255 for 8 bit number
	printf("%d",g_pwm_init_handle);
	ledc_set_duty(ledc_ch[0].mode, ledc_ch[0].channel, red);
	ledc_update_duty(ledc_ch[0].mode, ledc_ch[0].channel);

	ledc_set_duty(ledc_ch[1].mode, ledc_ch[1].channel, green);
	ledc_update_duty(ledc_ch[1].mode, ledc_ch[1].channel);

	ledc_set_duty(ledc_ch[2].mode, ledc_ch[2].channel, blue);
	ledc_update_duty(ledc_ch[2].mode, ledc_ch[2].channel);
}

void rgb_led_wifi_app_started(void)
{
	if (g_pwm_init_handle == false)
	{
		rgb_led_pwm_init();
	}

	rgb_led_set_color(255, 102, 255);
}

void rgb_led_http_server_started(void)
{
	if (g_pwm_init_handle == false)
	{
		rgb_led_pwm_init();
	}

	rgb_led_set_color(204, 255, 51);
}


void rgb_led_wifi_connected(void)
{
	if (g_pwm_init_handle == false)
	{
		rgb_led_pwm_init();
	}

	rgb_led_set_color(0, 255, 153);
}




float Get_prom_temp(void) //Tarea B
{
    float prom_temp = 0;

    for(int i = 0; i < 10; i++)
    {
        raw = adc1_get_raw(ADC1_CHANNEL_0);
        v_res = (raw * SUPPLY_VOLTAGE) / BITS_ADC_CONFI;
        res_ntc = (SUPPLY_VOLTAGE / v_res - 1)* res_2;

        temp_index = encontrar_posicion_mas_cercana(valores_res_ntc, sizeof(valores_res_ntc) / sizeof(valores_res_ntc[0]), res_ntc);
        temp = valores_temp_ntc[temp_index];

        prom_temp += temp;
    }

    return prom_temp/10;
}


int encontrar_posicion_mas_cercana(float* vector, int longitud, float variable) 
{
    int indice = 0;
    float distancia_minima = abs(vector[0] - variable);
    
    for (int i = 1; i < longitud; i++) {
        float distancia_actual = abs(vector[i] - variable);
        if (distancia_actual < distancia_minima) {
            indice = i;
            distancia_minima = distancia_actual;
        }
    }
    
    return indice;
}