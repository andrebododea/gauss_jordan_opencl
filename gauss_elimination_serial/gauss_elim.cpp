#include <iostream>
#include <algorithm>
#include <iterator>

int main()
{


	int n = 3;

    /* Allocate memory for the resulting augmented matrix  //
    // This will be an n*2 x n matrix                      */ 
	float mat_with_identity[n][n+1];


	// Test matrix which will be inverted
	float orig_mat[3][4] = {
	  {2.0, 1.0, -1.0, 0.0},
	  {-3.0, -1.0, 2.0, 0.0},
	  {-2.0, 1.0, 2.0, 0.0}
	};

	// Create vector
	float identity_mat[3] = {8.0, -11.0, -3.0};

              // Conjoin the matrix with vector
	    for (int i = 0; i < n; i++) {
	            orig_mat[i][n]= identity_mat[i];
	    }




	// Print the starting matrix
	for (int i = 0; i < n; i++)
	{
	    for (int j = 0; j < n+1; j++)
	    {
	        std::cout << orig_mat[i][j] << " _ " <<' ';
	    }
	    std::cout << std::endl;
	}
	std::cout << std::endl;
	std::cout << std::endl;



    /********************************************/
	/***        Gaussian elimination          ***/
	/********************************************/
	float a;
	bool a_is_zero;
	int nonzero_row;

	// Loop through rows for Step 1
	for (int currentRow = 0; currentRow < n; currentRow++)
	{
		a = orig_mat[currentRow][currentRow];
		
		/** KERNEL #1 **/
		// Do step 1 for each element of the current row: R_i = R_i/a_ii
		for(int col = 0; col < n+1; col++){
			orig_mat[currentRow][col] = orig_mat[currentRow][col]/ a;
		}

		/** KERNEL #2 **/
		// n^2 loop for Step 2
		for (int col = 0; col < n+1; col++)
		{
			// Skip the row if step 1 has already been applied to it
		
		
			for (int row = 0; row < n; row++)
		    {
	    		if(row != currentRow)
				{	
						float R_j = orig_mat[row][currentRow];
			        // std::cout << orig_mat[row][col] << " - " << R_j << " * " << orig_mat[currentRow][col] << std::endl;
			        orig_mat[row][col]  = orig_mat[row][col] - R_j * orig_mat[currentRow][col];
		   		}
			}
		}
	}


	float solution_vector[n];


	// store solution vector as an array
	    for (int i = 0; i < n; i++) {
	            solution_vector[i] = orig_mat[i][n];
	    }	


	// Print the end matrix
	for (int i = 0; i < n; i++)
	{
	    for (int j = 0; j < n+1; j++)
	    {
	        std::cout << orig_mat[i][j] << " _ " <<' ';
	    }
	    std::cout << std::endl;
	}
	std::cout << std::endl;
	std::cout << std::endl;


	//Print the solution vector
	std::cout <<  "Solution vector: "<< std::endl;
	 for (int i = 0; i < n; i++) {
	              std::cout <<  orig_mat[i][n] << ", "<< ' ';
	    }	
	std::cout << std::endl;


   return 0;
}
