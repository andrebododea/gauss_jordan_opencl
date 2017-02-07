////////////////////////////////////////////////////////////////////////////////

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <CL/cl.h>
#include <stdbool.h>

////////////////////////////////////////////////////////////////////////////////
#define WA 20
#define HA 20
#define WB 20

#define HB WA
#define WC WB
#define HC HA
////////////////////////////////////////////////////////////////////////////////

 
// Allocates a matrix with random float entries.
void randomMemInit(float* data, int size)
{
   int i;

   for (i = 0; i < size; ++i)
   	data[i] = rand() / (float)RAND_MAX;
}

long LoadOpenCLKernel(char const* path, char **buf)
{
    FILE  *fp;
    size_t fsz;
    long   off_end;
    int    rc;

    /* Open the file */
    fp = fopen(path, "r");
    if( NULL == fp ) {
        return -1L;
    }

    /* Seek to the end of the file */
    rc = fseek(fp, 0L, SEEK_END);
    if( 0 != rc ) {
        return -1L;
    }

    /* Byte offset to the end of the file (size) */
    if( 0 > (off_end = ftell(fp)) ) {
        return -1L;
    }
    fsz = (size_t)off_end;

    /* Allocate a buffer to hold the whole file */
    *buf = (char *) malloc( fsz+1);
    if( NULL == *buf ) {
        return -1L;
    }

    /* Rewind file pointer to start of file */
    rewind(fp);

    /* Slurp file into buffer */
    if( fsz != fread(*buf, 1, fsz, fp) ) {
        free(*buf);
        return -1L;
    }

    /* Close the file */
    if( EOF == fclose(fp) ) {
        free(*buf);
        return -1L;
    }


    /* Make sure the buffer is NUL-terminated, just in case */
    (*buf)[fsz] = '\0';

    /* Return the file size */
    return (long)fsz;
}

int main(int argc, char** argv)
{
   int err;                            // error code returned from api calls

   cl_device_id device_id;             // compute device id 
   cl_context context;                 // compute context
   cl_command_queue commands;          // compute command queue
   cl_program program;                 // compute program
   cl_kernel kernel1;                   // compute kernel
   cl_kernel kernel2;                   // compute kernel

    // OpenCL device memory for matrices
   cl_mem d_A;
   cl_mem d_C;

   // set seed for rand()
   srand(2014);
 
   //Allocate host memory for matrices A and B
   unsigned int size_A = WA * HA;
   unsigned int mem_size_A = sizeof(float) * size_A;
   float* h_A = (float*) malloc(mem_size_A);

   //Initialize host memory
   randomMemInit(h_A, size_A);
 
   //Allocate host memory for the result C
   unsigned int size_C = WC * HC;
   unsigned int mem_size_C = sizeof(float) * size_C;
   float* h_C = (float*) malloc(mem_size_C);
  
   printf("Initializing OpenCL device...\n"); 

   cl_uint dev_cnt = 0;
   clGetPlatformIDs(0, 0, &dev_cnt);
	
   cl_platform_id platform_ids[100];
   clGetPlatformIDs(dev_cnt, platform_ids, NULL);
	
   // Connect to a compute device
   int gpu = 1;
   err = clGetDeviceIDs(platform_ids[0], gpu ? CL_DEVICE_TYPE_GPU : CL_DEVICE_TYPE_CPU, 1, &device_id, NULL);
   if (err != CL_SUCCESS)
   {
       printf("Error: Failed to create a device group!\n");
       return EXIT_FAILURE;
   }
  
   // Create a compute context 
   context = clCreateContext(0, 1, &device_id, NULL, NULL, &err);
   if (!context)
   {
       printf("Error: Failed to create a compute context!\n");
       return EXIT_FAILURE;
   }

   // Create a command commands
   commands = clCreateCommandQueue(context, device_id, 0, &err);
   if (!commands)
   {
       printf("Error: Failed to create a command commands!\n");
       return EXIT_FAILURE;
   }

   // Create the compute program from the source file
   char *KernelSource;
   long lFileSize;

   lFileSize = LoadOpenCLKernel("gaussian_elim_kernel.cl", &KernelSource);
   if( lFileSize < 0L ) {
       perror("File read failed");
       return 1;
   }

   program = clCreateProgramWithSource(context, 1, (const char **) & KernelSource, NULL, &err);
   if (!program)
   {
       printf("Error: Failed to create compute program!\n");
       return EXIT_FAILURE;
   }

   // Build the program executable
   err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
   if (err != CL_SUCCESS)
   {
       size_t len;
       char buffer[2048];
       printf("Error: Failed to build program executable!\n");
       clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
       printf("%s\n", buffer);
       exit(1);
   }


   // Create the input and output arrays in device memory for our calculation
   d_C = clCreateBuffer(context, CL_MEM_READ_WRITE, mem_size_A, NULL, &err);
   d_A = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, mem_size_A, h_A, &err);

   if (!d_A || !d_C)
   {
       printf("Error: Failed to allocate device memory!\n");
       exit(1);
   }    


   //Launch OpenCL kernel
   size_t localWorkSize[2], globalWorkSize[2];
 
   int wA = WA;
   int wC = WC;

   // Set local and global work sizes
   localWorkSize[0] = 5;
   localWorkSize[1] = 5;
   globalWorkSize[0] = 20;
   globalWorkSize[1] = 20;
 
  
 
   //print out the results
/*
   printf("\n\nMatrix C (Results)\n");
   int i;
   for(i = 0; i < size_C; i++)
   {
      printf("%f ", h_C[i]);
      if(((i + 1) % WC) == 0)
      printf("\n");
   }
   printf("\n");
*/

   // BEGIN GAUSSIAN ELIMINATION

   // Create the compute kernel in the program we wish to run
   kernel1 = clCreateKernel(program, "step_1_row_operation", &err);
   if (!kernel1 || err != CL_SUCCESS)
   {
       printf("Error: Failed to create compute kernel!\n");
       exit(1);
   }

    kernel2 = clCreateKernel(program, "step_2_col_operation", &err);
   if (!kernel2 || err != CL_SUCCESS)
   {
       printf("Error: Failed to create compute kernel!\n");
       exit(1);
   }


   //Launch OpenCL kernel

  for(int currentRow = 0; currentRow < HA; currentRow++){

      /************/
      /* Kernel 1 */
      /************/

      // Setting arguments for the kernel
     
       err = clSetKernelArg(kernel1, 0, sizeof(cl_mem), (void *)&d_C);
       err |= clSetKernelArg(kernel1, 1, sizeof(cl_mem), (void *)&d_A);
       err |= clSetKernelArg(kernel1, 2, sizeof(int), (void *)&wA);
       err |= clSetKernelArg(kernel1, 3, sizeof(int), (void *)&currentRow);

       if (err != CL_SUCCESS)
       {
           printf("Error: Failed to set kernel arguments! %d\n", err);
           exit(1);
       }

       // This is where execution of the kernel is actually done
       err = clEnqueueNDRangeKernel(commands, kernel1, 2, NULL, globalWorkSize, localWorkSize, 0, NULL, NULL);

       if (err != CL_SUCCESS)
       {
           printf("Error: Failed to execute kernel1! %d\n", err);
           exit(1);
       }
     
       //Retrieve result from device
       err = clEnqueueReadBuffer(commands, d_A, CL_TRUE, 0, mem_size_A, h_A, 0, NULL, NULL);

       if (err != CL_SUCCESS)
       {
           printf("Error: Failed to read output array! %d\n", err);
           exit(1);
       }


        /************/
        /* Kernel 2 */
        /************/
       /*
       err = clSetKernelArg(kernel2, 0, sizeof(cl_mem), (void *)&d_C);
       err |= clSetKernelArg(kernel2, 1, sizeof(cl_mem), (void *)&d_A);
       err |= clSetKernelArg(kernel2, 2, sizeof(int), (void *)&wA);
       err |= clSetKernelArg(kernel2, 3, sizeof(int), (void *)&currentRow);

       if (err != CL_SUCCESS)
       {
           printf("Error: Failed to set kernel arguments! %d\n", err);
           exit(1);
       }

       // This is where execution of the kernel is actually done
       err = clEnqueueNDRangeKernel(commands, kernel2, 2, NULL, globalWorkSize, localWorkSize, 0, NULL, NULL);

       if (err != CL_SUCCESS)
       {
           printf("Error: Failed to execute kernel2! %d\n", err);
           exit(1);
       }
     
       //Retrieve result from device
       err = clEnqueueReadBuffer(commands, d_A, CL_TRUE, 0, mem_size_A, h_A, 0, NULL, NULL);

       if (err != CL_SUCCESS)
       {
           printf("Error: Failed to read output array! %d\n", err);
           exit(1);
       }
       */
   }


   //print out the results

   printf("\n\nMatrix A (Results)\n");
   int i;
   for(i = 0; i < size_A; i++)
   {
      printf("%f ", h_A[i]);
      if(((i + 1) % WA) == 0)
      printf("\n");
   }
   printf("\n");




   //Shutdown and cleanup

   free(h_A);
   free(h_C);
 
   clReleaseMemObject(d_A);
   clReleaseMemObject(d_C);

   clReleaseProgram(program);
   clReleaseKernel(kernel1);
   clReleaseKernel(kernel2);
   clReleaseCommandQueue(commands);
   clReleaseContext(context);

   return 0;
}
