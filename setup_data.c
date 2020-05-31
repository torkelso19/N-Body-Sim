#include <stdlib.h>
#include <CL/cl.h>

void setup_data(int NUM_BODIES,
    cl_double* mass_data,
    cl_double3* pos_data,
    cl_double3* vel_data)
{
    const double M = 1.98847e30;

    //Sun
    mass_data[0] = 1;
    pos_data[0].x = -5.023815526961201E-03;
    pos_data[0].y = 7.034169085936035E-03;
    pos_data[0].z = 5.731575217419630E-05;
    vel_data[0].x = -8.116530158260999E-06 / 24.0;
    vel_data[0].y = -3.542524347294409E-06 / 24.0;
    vel_data[0].z = 2.328538477240252E-07 / 24.0;

    //Mercury
    mass_data[1] = (3.3011e23) / M;
    pos_data[1].x = -3.885983994049520E-01;
    pos_data[1].y = 1.786387205767142E-02;
    pos_data[1].z = 3.612910596371826E-02;
    vel_data[1].x = -6.654279990434935E-03 / 24.0;
    vel_data[1].y = -2.691816507723168E-02 / 24.0;
    vel_data[1].z = -1.589408574859103E-03 / 24.0;
    
    //Venus
    mass_data[2] = (4.8675e24) / M;
    pos_data[2].x = -3.206388146488250E-01;
    pos_data[2].y = -6.458031331272464E-01;
    pos_data[2].z = 9.312117470243282E-03;
    vel_data[2].x = 1.806301439760956E-02 / 24.0;
    vel_data[2].y = -8.896659217746190E-03 / 24.0;
    vel_data[2].z = -1.164634893187325E-03 / 24.0;
    
    //Earth
    mass_data[3] = (5.9724e24)/M;
    pos_data[3].x = -3.876708318285312E-01;
    pos_data[3].y = -9.315577255384649E-01;
    pos_data[3].z = 9.940356641087374E-05;
    vel_data[3].x = 1.564682820469637E-02 / 24.0;
    vel_data[3].y = -6.557443594716502E-03 /24.0;
    vel_data[3].z = 1.346396573061984E-07 / 24.0;
    
    //Mars
    mass_data[4] = (6.4171e23)/M;
    pos_data[4].x = 5.817306160565072E-01;
    pos_data[4].y = -1.276274807392623E+00;
    pos_data[4].z = -4.122749331199866E-02;
    vel_data[4].x = 1.324781341884570E-02 / 24.0;
    vel_data[4].y = 7.016922715278679E-03 / 24.0;
    vel_data[4].z = -1.778647871801350E-04 / 24.0;
    
    //Jupiter
    mass_data[5] = (1.89819e27)/M;
    pos_data[5].x = 1.606581007474169E+00;
    pos_data[5].y = -4.908413137704151E+00;
    pos_data[5].z = -1.558398639027597E-02;
    vel_data[5].x = 7.079357911484605E-03 / 24.0;
    vel_data[5].y = 2.706494062581339E-03 / 24.0;
    vel_data[5].z = -1.695745005823816E-04 / 24.0;

    //Saturn
    mass_data[6] = (5.6834e26)/M;
    pos_data[6].x = 4.503628545440582E+00;
    pos_data[6].y = -8.938582945040473E+00;
    pos_data[6].z = -2.386962644143594E-02;
    vel_data[6].x = 4.671604644725415E-03 / 24.0;
    vel_data[6].y = 2.494639577588684E-03 / 24.0;
    vel_data[6].z = -2.292281910128984E-04 / 24.0;

    //Uranus
    mass_data[7] = (8.6813e25)/M;
    pos_data[7].x = 1.587389597158051E+01;
    pos_data[7].y = 1.183395017953625E+01;
    pos_data[7].z = -1.616965879314545E-01;
    vel_data[7].x = -2.379592555358805E-03 / 24.0;
    vel_data[7].y = 2.969984148832247E-03 / 24.0;
    vel_data[7].z = 4.181215874092596E-05 / 24.0;

    //Neptune
    mass_data[8] = (1.02413e26)/M;
    pos_data[8].x = 2.933172629412627E+01;
    pos_data[8].y = -5.899162960200655E+00;
    pos_data[8].z = -5.544967800368118E-01;
    vel_data[8].x = 5.978987902862180E-04 / 24.0;
    vel_data[8].y = 3.096410532911095E-03 / 24.0;
    vel_data[8].z = -7.713357423857181E-05 / 24.0;

    //Pluto
    mass_data[9] = (1.02413e26) / M;
    pos_data[9].x = 1.341465665222103E+01;
    pos_data[9].y = -3.128085514390688E+01;
    pos_data[9].z = -5.330622589855416E-01;
    vel_data[9].x = 2.967460055851011E-03 / 24.0;
    vel_data[9].y = 5.741468463838569E-04 / 24.0;
    vel_data[9].z = -9.224163263758033E-04 / 24.0;
}