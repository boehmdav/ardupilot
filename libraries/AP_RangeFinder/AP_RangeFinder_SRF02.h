// -*- tab-width: 4; Mode: C++; c-basic-offset: 3; indent-tabs-mode: t -*-

#ifndef __AP_RANGEFINDER_SRF02_H__
#define __AP_RANGEFINDER_SRF02_H__

#include "RangeFinder.h"

#define AP_RANGE_FINDER_SRF02_DEFAULT_ADDR   0x74

#define AP_RANGEFINDER_SRF02                5
#define AP_RANGE_FINDER_SRF02_SCALER        1.0
#define AP_RANGE_FINDER_SRF02_MIN_DISTANCE  15
#define AP_RANGE_FINDER_SRF02_MAX_DISTANCE  600

#define AP_RANGE_FINDER_SRF02_COMMAND_TAKE_RANGE_READING 0x51

class AP_RangeFinder_SRF02 : public RangeFinder
{

public:

    // constructor
    AP_RangeFinder_SRF02(FilterInt16 *filter);

    // init - simply sets the i2c address
    void init(uint8_t address = AP_RANGE_FINDER_SRF02_DEFAULT_ADDR) { _addr = address; }

    // take_reading - ask sensor to make a range reading
    bool            take_reading();

    // read value from sensor and return distance in cm
    int             read();

    // heath
    bool            healthy;



protected:
    uint8_t _addr;
    uint8_t state;
    int a1,a2,a3;
};
#endif  // __AP_RANGEFINDER_SRF02_H__
