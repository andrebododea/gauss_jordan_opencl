#include <iostream>
#include <algorithm>
#include <iterator>

int main()
{


	int n = 4;

    /* Allocate memory for the resulting augmented matrix  //
    // This will be an n*2 x n matrix                      */ 
	float mat_with_identity[n][n*2];


	// Test matrix which will be inverted
	float orig_mat[4][4] = {
	  {1.0, 3.0, 3.0, 4.0},
	  {5.0, 6.0, 9.0, 8.0},
	  {9.0, 10.0, 11.0, 12.0},
	  {13.0, 14.0, 15.0, 16.0}
	};

	// Create identity matrix
	float identity_mat[4][4] = {
	  {1.0, 0.0, 0.0, 0.0},
	  {0.0, 1.0, 0.0, 0.0},
	  {0.0, 0.0, 1.0, 0.0},
	  {0.0, 0.0, 0.0, 1.0}
	};

// Conjoin the matrix with the identity matrix: mat_with_identity = [orig_mat | identity_mat]
	for (int i = 0; i < n; i++) {
	    for (int j = 0; j < n*2; j++) {
	        if (j < n) {
	            mat_with_identity[i][j] = orig_mat[i][j];
	        }
	        else {
	            mat_with_identity[i][j] = identity_mat[i][j - n];
	        }
	    }
	}



	// Print the original matrix
	for (int i = 0; i < n; ++i)
	{
	    for (int j = 0; j < n*2; ++j)
	    {
	        std::cout << mat_with_identity[i][j] << " _ " <<' ';
	    }
	    std::cout << std::endl;
	}
	std::cout << std::endl;
	std::cout << std::endl;



    /*************************************/
	/*** Gauss-Jordan matrix inversion ***/
	/*************************************/
	float a;
	bool a_is_zero;
	int nonzero_row;

	// Loop through rows for Step 1
	for (int currentRow = 0; currentRow < n; ++currentRow)
	{

		a = mat_with_identity[currentRow][currentRow];
		
		/*
		a_is_zero = false;
		if(a == 0.0){

			std::cout << "BANG! A IS ZERO: " << std::endl;

			a_is_zero = true;
			for(int row = 0; row < n; ++row){
				if(mat_with_identity[row][currentRow] != 0.0){
					nonzero_row = row;
					std::cout << "Nonzero row is: " << nonzero_row << std::endl;
					a = mat_with_identity[currentRow][currentRow] + mat_with_identity[nonzero_row][currentRow];
					break;
				}
			}
		}
		*/



		// Do step 1 for each element of the current row: R_i = R_i/a_ii
		for(int col = 0; col < n*2; ++col){
			/*
			if(a_is_zero){
				mat_with_identity[currentRow][col] += mat_with_identity[nonzero_row][col];
			}
			*/
			std::cout << mat_with_identity[currentRow][col] << " / " << a << std::endl;
			mat_with_identity[currentRow][col] = mat_with_identity[currentRow][col]/ a;
		}


		// n^2 loop for Step 2
		for (int row = 0; row < n; ++row)
		{
			// Skip the row if step 1 has already been applied to it
			if(row != currentRow)
			{	

				float R_j = mat_with_identity[row][currentRow];
				for (int col = 0; col < n*2; ++col)
			    {
			    	std::cout << mat_with_identity[row][col] << " - " << R_j << " * " << mat_with_identity[currentRow][col] << std::endl;
			        mat_with_identity[row][col]  = mat_with_identity[row][col] - R_j * mat_with_identity[currentRow][col];
			    }
			}
		}



	// Print the matrix
	std::cout << std::endl;
	std::cout << "NEXT PHASE: " << std::endl;
	for (int i = 0; i < n; ++i)
	{
	    for (int j = 0; j < n*2; ++j)
	    {
	        std::cout << mat_with_identity[i][j] << " _ " <<' ';
	    }
	    std::cout << std::endl;
	}
	std::cout << std::endl;
	std::cout << std::endl;


	}



	// Print the matrix
	for (int i = 0; i < n; ++i)
	{
	    for (int j = 0; j < n*2; ++j)
	    {
	        std::cout << mat_with_identity[i][j] << " _ " <<' ';
	    }
	    std::cout << std::endl;
	}
	std::cout << std::endl;
	std::cout << std::endl;


   return 0;
}
