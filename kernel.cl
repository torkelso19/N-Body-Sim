__kernel void n_body(
    const int SIM_FRAMES,
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

    const double G = 3.96413586e-14; // AU / (Solar Mass * sec^2)
    const double softening = 0.000001;

    __private double3 r = pos[i];
    __private double3 v = vel[i];
    __private double3 a = 0;

    __private double mag;
    __private double3 normal;
    __private double square;
    __private double3 len;

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

        v += a * dt;
        r += v * dt;

        //Synch
        barrier(CLK_GLOBAL_MEM_FENCE);

        pos[i] = r;

        //Write position to global memory output
        //(maybe write to local mem and global at end)
        output_pos[j * NUM_BODIES + i] = r;
    }
    output_vel[i] = v;
}