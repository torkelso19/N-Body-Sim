__kernel void n_body(
    const int INIT_BATCH,
    const int SIM_FRAMES,
    const int UPDATE_FREQ,
    const int NUM_BODIES,
    const double dt,
    __global double* mass,
    __global double3* pos,
    __global double3* vel,
    __global double3* output_pos,
    __global double3* output_vel
)
{
    // Get the index of the current element
    uint i = get_global_id(0);

    const double G = 0.000295921156297; //AU^3 / (Solar Mass * day^2)
    const double softening = 0.000001;

    __private double3 r = pos[i];
    __private double3 v = vel[i];
    __private double3 a = 0;

    __private double mag;
    __private double3 normal;
    __private double square;
    __private double3 len;

    __private int output_count = 0;

    for (int j = 0; j < SIM_FRAMES; j++) {
        //Compute acceleration due to each other body
        a.x = 0;
        a.y = 0;
        a.z = 0;

        for (int k = 0; k < NUM_BODIES; k++) {
            if (k == i) continue;
            len = pos[k] - r;
            mag = distance(r, pos[k]);
            square = pown(mag, 2);
            a = a + (mass[k] * len) / pow(square + softening, 1.5);
        }

        a *= G;

        if (j == 0 && INIT_BATCH == 1) v -= 0.5 * a * dt; //Correction of velocity for leap frog alg

        v += a * dt;
        r += v * dt;

        //Synch
        barrier(CLK_GLOBAL_MEM_FENCE);

        pos[i] = r;

        //Write position to global memory output
        //(maybe write to local mem and global with asynch copy at end)
        if ((j+1) % UPDATE_FREQ == 0) {
            output_pos[output_count*NUM_BODIES + i] = r;
            output_count++;
        }
    }
    output_vel[i] = v;
}