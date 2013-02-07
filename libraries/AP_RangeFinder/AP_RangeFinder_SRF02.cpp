// -*- tab-width: 4; Mode: C++; c-basic-offset: 3; indent-tabs-mode: t -*-
/*
 *       AP_RangeFinder_MaxsonarI2CXL.cpp - Arduino Library for MaxBotix I2C XL sonar
 *       Code by Randy Mackay. DIYDrones.com
 *
 *       This library is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU Lesser General Public
 *   License as published by the Free Software Foundation; either
 *   version 2.1 of the License, or (at your option) any later version.
 *
 *       datasheet: http://www.maxbotix.com/documents/I2CXL-MaxSonar-EZ_Datasheet.pdf
 *
 *       Sensor should be connected to the I2C port
 *
 *       Variables:
 *               bool healthy : indicates whether last communication with sensor was successful
 *
 *       Methods:
 *               take_reading(): ask the sonar to take a new distance measurement
 *               read() : read last distance measured (in cm)
 *
 */

// AVR LibC Includes
#include <AP_Common.h>
#include <I2C.h>                // Arduino I2C lib
#include "AP_RangeFinder_SRF02.h"

// Constructor //////////////////////////////////////////////////////////////

AP_RangeFinder_SRF02::AP_RangeFinder_SRF02( FilterInt16 *filter ) :
    RangeFinder(NULL, filter),
    healthy(true),
    _addr(AP_RANGE_FINDER_SRF02_DEFAULT_ADDR)
{
    max_distance = AP_RANGE_FINDER_SRF02_MIN_DISTANCE;
    min_distance = AP_RANGE_FINDER_SRF02_MAX_DISTANCE;
    state = 0;
    a1 = max_distance;
    a2 = max_distance;
    a3 = max_distance;
}

// Public Methods //////////////////////////////////////////////////////////////

// take_reading - ask sensor to make a range reading
bool AP_RangeFinder_SRF02::take_reading()
{
    // take range reading and read back results
    if (I2c.write(_addr, (uint8_t)0x00, (uint8_t)AP_RANGE_FINDER_SRF02_COMMAND_TAKE_RANGE_READING) != 0) {
        healthy = false;
        return false;
    }else{
        healthy = true;
        return true;
    }
}

// read - return last value measured by sensor
int AP_RangeFinder_SRF02::read()
{
    uint8_t buff[2];
    int16_t ret_value = 0;
    // take range reading and read back results
    if (I2c.read(_addr,(uint8_t)0x02, 2, buff) != 0) {
        healthy = false;
    }else{
        // combine results into distance
        ret_value = buff[0] << 8 | buff[1];
        healthy = true;
    }
    if (ret_value < 40) {
	state = 1;
    } else if(state == 1 && ret_value > 250 && ret_value < 400) {
	ret_value = 15;
    } else if(state == 1) {
	state = 0;
    } else {
	state = 0;
    }
    a1 = a2;
    a2 = a3;
    a3 = ret_value;
    if ((a1 <= a2 && a2 <= a3) || (a1 > a2 && a2 > a3)) {return a2;}
    else if((a1 <= a3 && a3 <= a2) || (a1 > a3 && a3 > a2)) {return a3;}
    else {return a1;}
}
