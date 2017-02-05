
/* kernel.cl 
 * Matrix multiplication: C = A * B.
 * Device code.
 */
 
// OpenCL Kernel
__kernel void step_1_row_operation(__global float* C, 
          __global float* A, 
          __global float* B, 
          int wA, int wB, int currentRow)
{
  
   int tx = get_global_id(0); 
   int ty = get_global_id(1);

   int a = orig_mat[currentRow][currentRow];
    
    /* KERNEL #1 */
    // Do step 1 for each element of the current row: R_i = R_i/a_ii
    for(int col = 0; col < n+1; col++){
      A[currentRow][tx] = A[currentRow][tx]/ a;
    }

   
   // value stores the element that is 
   // computed by the thread
   float value = 0;
   for (int k = 0; k < wA; ++k)
   {
      float elementA = A[ty * wA + k];
      float elementB = B[k * wB + tx];
      value += elementA * elementB;
   }
 
   // Write the matrix to device memory each 
   // thread writes one element
   C[ty * wA + tx] = value;
}



// Just testing functionality of a second kernel: hello_world
__kernel void step_2_col_operation(int a, int b)
{
  a += 1; 
}
