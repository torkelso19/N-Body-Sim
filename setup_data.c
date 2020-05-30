#include <stdlib.h>
#include <CL/cl.h>

void setup_data(int NUM_BODIES,
    cl_double* mass_data,
    cl_double3* pos_data,
    cl_double3* vel_data)
{

    const double M = 1.98847e30;
    const double AU = 1.495978707e11;

    for (int i = 0; i < NUM_BODIES; i++) {
        mass_data[i] = 0;
        pos_data[i].x = 0;
        pos_data[i].y = 0;
        pos_data[i].z = 0;
        vel_data[i].x = 0;
        vel_data[i].y = 0;
        vel_data[i].z = 0;
    }

    //Sun
    mass_data[0] = 1;

    //Mercury
    mass_data[1] = (3.3011e23) / M;
    pos_data[1].x = (-57316149e3) / AU;
    pos_data[1].y = (5014513e3) / AU;
    vel_data[1].x = (-4201.0) * 3600.0 / AU;
    vel_data[1].y = (-48017.0) * 3600.0 / AU;
    
    //Venus
    mass_data[2] = (4.8675e24) / M;
    pos_data[2].x = (-47556114e3) / AU;
    pos_data[2].y = (-97504484e3) / AU;
    vel_data[2].x = (31368.0) * 3600.0 / AU;
    vel_data[2].y = (-15299.0) * 3600.0 / AU;
    
    //Earth
    mass_data[3] = (5.9724e24)/M;
    pos_data[3].x = (-59246036e3) / AU;
    pos_data[3].y = (-139574913e3) / AU;
    vel_data[3].x = (27063.0) * 3600.0 / AU;
    vel_data[3].y = (-11487.0) * 3600.0 / AU;
    
    //Mars
    mass_data[4] = (6.4171e23)/M;
    pos_data[4].x = (89258838e3) / AU;
    pos_data[4].y = (-191416197e3) / AU;
    vel_data[4].x = (23564.0) * 3600.0 / AU;
    vel_data[4].y = (10988.0) * 3600.0 / AU;
    
    //Jupiter
    mass_data[5] = (1.89819e27)/M;
    pos_data[5].x = (239138144e3) / AU;
    pos_data[5].y = (-735991528e3) / AU;
    vel_data[5].x = (12458.0) * 3600.0 / AU;
    vel_data[5].y = (4048.0) * 3600.0 / AU;

    //Saturn
    mass_data[6] = (5.6834e26)/M;
    pos_data[6].x = (680357985e3) / AU;
    pos_data[6].y = (-1335277729e3) / AU;
    vel_data[6].x = (8197.0) * 3600.0 / AU;
    vel_data[6].y = (4177.0) * 3600.0 / AU;

    //Uranus
    mass_data[7] = (8.6813e25)/M;
    pos_data[7].x = (2365598985e3) / AU;
    pos_data[7].y = (1782606696e3) / AU;
    vel_data[7].x = (-3972.0) * 3600.0 / AU;
    vel_data[7].y = (5270.0) * 3600.0 / AU;

    //Neptune
    mass_data[8] = (1.02413e26)/M;
    pos_data[8].x = (4395276526e3) / AU;
    pos_data[8].y = (-854355209e3) / AU;
    vel_data[8].x = (1049.0) * 3600.0 / AU;
    vel_data[8].y = (5399.0) * 3600.0 / AU;
    

}