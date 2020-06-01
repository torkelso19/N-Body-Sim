#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <CL/cl.h>

#define MAX_SOURCE_SIZE (0x100000)

int main(void) {
    // Create data needed
    const int PRINT_CONSOLE = 0;
    const int PRINT_FILE = 1;

    const int NUM_BODIES = 11;
    const int DAYS = 10; // sim for a year per batch
    const double dt = 1; //Days computed at once
    const int UPDATE_FREQ = 1; //Every how many dt to output data
    const int NUM_BATCHES = 3; //Number Iterations

    const int SIM_FRAMES_EST = (int)round((double)DAYS/dt);
    const int alignment = SIM_FRAMES_EST % UPDATE_FREQ;
    const int SIM_FRAMES = (alignment == 0) ? SIM_FRAMES_EST : SIM_FRAMES_EST + UPDATE_FREQ - alignment; //Aligns frames to include more if necessary

    const int OUTPUT_FRAMES = SIM_FRAMES / UPDATE_FREQ;

    cl_double* mass_data = (cl_double*)malloc(sizeof(cl_double) * NUM_BODIES);
    cl_double3* pos_data = (cl_double3*)malloc(sizeof(cl_double3) * NUM_BODIES);
    cl_double3* vel_data = (cl_double3*)malloc(sizeof(cl_double3) * NUM_BODIES);

    cl_double3* init_pos_data = (cl_double3*)malloc(sizeof(cl_double3) * NUM_BODIES);
    cl_double3* output_pos_data = (cl_double3*)malloc(sizeof(cl_double3) * NUM_BODIES * OUTPUT_FRAMES);
    cl_double3* output_vel_data = (cl_double3*)malloc(sizeof(cl_double3) * NUM_BODIES);

    cl_double3* graphics_data = (cl_double3*)malloc(sizeof(cl_double3) * NUM_BODIES * OUTPUT_FRAMES * NUM_BATCHES);

    setup_data(NUM_BODIES, mass_data, pos_data, vel_data);
    
    //Copy init position data
    for (int z = 0; z < NUM_BODIES; z++) {
        init_pos_data[z] = pos_data[z];
    }

    // Load the kernel source code into the array source_str
    FILE* fp;
    char* source_str;
    size_t source_size;

    fp = fopen("kernel.cl", "r");
    if (!fp) {
        fprintf(stderr, "Failed to load kernel.\n");
        exit(1);
    }
    source_str = (char*)malloc(MAX_SOURCE_SIZE);
    source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);
    fclose(fp);

    // Get platform and device information
    cl_platform_id platform_id = NULL;
    cl_device_id device_id = NULL;
    cl_uint ret_num_devices;
    cl_uint ret_num_platforms;
    cl_int ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
    ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1,
        &device_id, &ret_num_devices);

    // Create an OpenCL context
    cl_context context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);

    // Create a command queue
    cl_command_queue command_queue = clCreateCommandQueue(context, device_id, 0, &ret);

    // Create a program from the kernel source
    cl_program program = clCreateProgramWithSource(context, 1,
        (const char**)&source_str, (const size_t*)&source_size, &ret);

    // Build the program
    ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);

    // Create the OpenCL kernel
    cl_kernel kernel = clCreateKernel(program, "n_body", &ret);

    // Create memory buffers on the device for each vector 
    cl_mem mass_mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY,
        NUM_BODIES * sizeof(cl_double), NULL, &ret);
    cl_mem pos_mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE,
        NUM_BODIES * sizeof(cl_double3), NULL, &ret);
    cl_mem vel_mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY,
        NUM_BODIES * sizeof(cl_double3), NULL, &ret);
    cl_mem output_pos_mem_obj = clCreateBuffer(context, CL_MEM_WRITE_ONLY,
        NUM_BODIES * OUTPUT_FRAMES * sizeof(cl_double3), NULL, &ret);
    cl_mem output_vel_mem_obj = clCreateBuffer(context, CL_MEM_WRITE_ONLY,
        NUM_BODIES * sizeof(cl_double3), NULL, &ret);
    ret = clFinish(command_queue);

    //Start timer
    size_t before = clock();

    //Execute each batch
    for (int u = 0; u < NUM_BATCHES; u++) {
        const int INIT_BATCH = u == 0 ? 1 : 0;

        // Copy data to respective memory buffers
        ret = clEnqueueWriteBuffer(command_queue, mass_mem_obj, CL_TRUE, 0,
            NUM_BODIES * sizeof(cl_double), mass_data, 0, NULL, NULL);
        ret = clEnqueueWriteBuffer(command_queue, pos_mem_obj, CL_TRUE, 0,
            NUM_BODIES * sizeof(cl_double3), pos_data, 0, NULL, NULL);
        ret = clEnqueueWriteBuffer(command_queue, vel_mem_obj, CL_TRUE, 0,
            NUM_BODIES * sizeof(cl_double3), vel_data, 0, NULL, NULL);
        ret = clFinish(command_queue);

        // Set the arguments of the kernel
        ret = clSetKernelArg(kernel, 0, sizeof(int), (void*)&INIT_BATCH);
        ret = clSetKernelArg(kernel, 1, sizeof(int), (void*)&SIM_FRAMES);
        ret = clSetKernelArg(kernel, 2, sizeof(int), (void*)&UPDATE_FREQ);
        ret = clSetKernelArg(kernel, 3, sizeof(int), (void*)&NUM_BODIES);
        ret = clSetKernelArg(kernel, 4, sizeof(double), (void*)&dt);
        ret = clSetKernelArg(kernel, 5, sizeof(cl_mem), (void*)&mass_mem_obj);
        ret = clSetKernelArg(kernel, 6, sizeof(cl_mem), (void*)&pos_mem_obj);
        ret = clSetKernelArg(kernel, 7, sizeof(cl_mem), (void*)&vel_mem_obj);
        ret = clSetKernelArg(kernel, 8, sizeof(cl_mem), (void*)&output_pos_mem_obj);
        ret = clSetKernelArg(kernel, 9, sizeof(cl_mem), (void*)&output_vel_mem_obj);

        // Execute the OpenCL kernel on the list
        size_t global_item_size = NUM_BODIES; // Process the entire lists
        ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL,
            &global_item_size, NULL, 0, NULL, NULL);

        //Block until computation completed
        ret = clFinish(command_queue);

        //Read the memory buffer C on the device to the local variable C
        
        ret = clEnqueueReadBuffer(command_queue, output_pos_mem_obj, CL_TRUE, 0,
            sizeof(cl_double3) * NUM_BODIES * OUTPUT_FRAMES, output_pos_data, 0, NULL, NULL);
        ret = clEnqueueReadBuffer(command_queue, output_vel_mem_obj, CL_TRUE, 0,
            sizeof(cl_double3) * NUM_BODIES, output_vel_data, 0, NULL, NULL);
        //Block
        ret = clFinish(command_queue);

        //Write data back for next batch
        memcpy(pos_data,output_pos_data+((long)OUTPUT_FRAMES-1)*NUM_BODIES, sizeof(cl_double3)*NUM_BODIES);
        memcpy(vel_data, output_vel_data,sizeof(cl_double3)*NUM_BODIES);
        memcpy(graphics_data+((long)u*(long)OUTPUT_FRAMES*(long)NUM_BODIES),output_pos_data,sizeof(cl_double3)*OUTPUT_FRAMES*NUM_BODIES);
    }

    //Finish Timer
    size_t diff = clock() - before;
    int time_milli = diff * 1000 / CLOCKS_PER_SEC;

    if (PRINT_CONSOLE) {
        printf("Frame Init:\n\n");
        for (int j = 0; j < NUM_BODIES; j++) {
            printf("Body = %d\nX=%lf\nY=%lf\nZ=%lf\n\n",
                j,
                init_pos_data[j].x,
                init_pos_data[j].y,
                init_pos_data[j].z);
        }
        printf("----------\n\n");
        for (int i = 0; i < OUTPUT_FRAMES*NUM_BATCHES; i++) {
            printf("Frame %d:\n\n", i);
            for (int j = 0; j < NUM_BODIES; j++) {
                printf("Body = %d\nX=%lf\nY=%lf\nZ=%lf\n\n",
                    j,
                    graphics_data[i * NUM_BODIES + j].x,
                    graphics_data[i * NUM_BODIES + j].y,
                    graphics_data[i * NUM_BODIES + j].z);
            }
            printf("----------\n\n");
        }
        printf("Final Velocities:\n\n");
        for (int j = 0; j < NUM_BODIES; j++) {
            printf("Body = %d\nX=%lf\nY=%lf\nZ=%lf\n\n",
                j,
                output_vel_data[j].x,
                output_vel_data[j].y,
                output_vel_data[j].z);
        }
    }

    if (PRINT_FILE) {
        //Write data to file
        FILE* fp2;
        fp2 = fopen("output_data.txt", "w");
        if (!fp2) {
            fprintf(stderr, "\n-----\nFailed to create output data file.\n-----\n");
            exit(1);
        }
        for (int j = 0; j < NUM_BODIES; j++) {
            fprintf(fp2, "%lf,%lf,%lf",
                init_pos_data[j].x,
                init_pos_data[j].y,
                init_pos_data[j].z);
            if (j != NUM_BODIES - 1) fprintf(fp2, ":");
        }
        fprintf(fp2, "\n");
        for (int i = 0; i < OUTPUT_FRAMES*NUM_BATCHES; i++) {
            for (int j = 0; j < NUM_BODIES; j++) {
                fprintf(fp2, "%lf,%lf,%lf",
                    graphics_data[i * NUM_BODIES + j].x,
                    graphics_data[i * NUM_BODIES + j].y,
                    graphics_data[i * NUM_BODIES + j].z);
                if (j != NUM_BODIES - 1) fprintf(fp2, ":");
            }
            fprintf(fp2, "\n");
        }
        fclose(fp2);
    }

    printf("\nTime for computation = %dms\n\n", time_milli);

    // Clean up
    ret = clFlush(command_queue);
    ret = clFinish(command_queue);
    ret = clReleaseKernel(kernel);
    ret = clReleaseProgram(program);

    ret = clReleaseMemObject(mass_mem_obj);
    ret = clReleaseMemObject(pos_mem_obj);
    ret = clReleaseMemObject(vel_mem_obj);
    ret = clReleaseMemObject(output_pos_mem_obj);
    ret = clReleaseMemObject(output_vel_mem_obj);

    ret = clReleaseCommandQueue(command_queue);
    ret = clReleaseContext(context);

    free(mass_data);
    free(pos_data);
    free(vel_data);
    free(init_pos_data);
    free(output_pos_data);
    free(output_vel_data);
    free(graphics_data);

    return 0;
}