/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef SERIAL_H
#define	SERIAL_H

#include <xc.h> // include processor files - each processor file is guarded.  


#define SERIAL_MODE_16QAM_CODED         0x00
#define SERIAL_MODE_16QAM_UNCODED       0x01
#define SERIAL_MODE_8PSK_UNCODED        0x02
#define SERIAL_START                    0x03
#define SERIAL_STOP                     0x04
#define SERIAL_DATA_UPDATE_REQ          0x05
#define SERIAL_DATA_UPDATE_ACK          0x06
#define SERIAL_DATA_UPDATE_DEC          0x07
#define SERIAL_NAME_REQUEST             0x08
#define SERIAL_NAME_TRANSMITTER         0x09
#define SERIAL_NAME_CHANNEL             0x0A
#define SERIAL_NAME_RECEIVER            0x0B
#define SERIAL_NOISE_UPDATE_REQ         0x0C
#define SERIAL_NOISE_UPDATE_ACK         0x0D
#define SERIAL_ADC_CALIBRATE            0x0E



#endif	

