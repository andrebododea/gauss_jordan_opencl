
/* kernel.cl 
 * Matrix multiplication: C = A * B.
 * Device code.
 */
 
// OpenCL Kernel
__kernel void step_1_row_operation(__global float* C, 
          __global float* A, 
          int wA, int currentRow)
{
  

    /* KERNEL #1 */
    // Do step 1 for each element of the current row: R_i = R_i/a_ii

   int col = get_global_id(0); 
   int ty = get_global_id(1);

   float a = A[currentRow*wA+currentRow];
    
    
    // A[currentRow][tx] = A[currentRow][tx]/ a;
    A[currentRow*wA+col] = A[currentRow*wA+col]/a;
}



// Just testing functionality of a second kernel: hello_world
__kernel void step_2_col_operation(__global float* C, 
          __global float* A, 
          int wA, int currentRow)
{
  
   int col = get_global_id(0); 
   int ty = get_global_id(1);


  /** KERNEL #2 **/
  // n^2 loop for Step 2

  for (int row = 0; row < wA; row++)
  {
    // Skip the row if step 1 has already been applied to it
    if(row != currentRow)
    { 
      float R_j = A[row*wA + currentRow];
      // A[row][col]  = A[row][col] - R_j * A[currentRow][col];
      A[row*wA + col]  = A[row*wA + col] - R_j * A[currentRow*wA + col];
    }
  }
}
