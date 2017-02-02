#include <iostream>
#include <algorithm>
#include <iterator>
#include <math.h>

#define minvalue 0.0005

int main(){




	int n = 4;

    /* Allocate memory for the resulting augmented matrix  //
    // This will be an n*2 x n matrix                      */ 
	float **mat_with_identity; //two * are needed because it is a pointer to a pointer
	mat_with_identity=new float*[8]; //creates a new array of pointers to int objects 
	for(int i=0; i<8; ++i)
	mat_with_identity[i]=new float[8];


	// Test matrix which will be inverted
	float orig_mat[4][4] = {
	  {100.0, 300.0, 300.0, 400.0},
	  {500.0, 600.0, 900.0, 800.0},
	  {900.0, 1000.0, 1100.0, 1200.0},
	  {1300.0, 1400.0, 1500.0, 1600.0}
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







	/**************************/
	/**************************/
	/* CHINESE CODE BEGINS ****/
	/**************************/
	/**************************/


	float temporary, r;
	int temp;


	for (int j = 0; j<n; j++){
		temp = j;

		/* finding maximum jth column element in last (n-j) rows */

		for (int i = j + 1; i<n; i++)
		if (mat_with_identity[i][j]>mat_with_identity[temp][j])
			temp = i;

		if (fabs(mat_with_identity[temp][j]) < minvalue){
			std::cout << "Elements are too small to deal with !!!" << std::endl; 
			break;
		}

		/* swapping row which has maximum jth column element */

		if (temp != j)
		for (int k = 0; k<2 * n; k++){
			temporary = mat_with_identity[j][k];
			mat_with_identity[j][k] = mat_with_identity[temp][k];
			mat_with_identity[temp][k] = temporary;
		}

		/* performing row operations to form required identity matrix out of the input matrix */

		for (int i = 0; i<n; i++)
		if (i != j){
			r = mat_with_identity[i][j];
			for (int k = 0; k<2 * n; k++)
				mat_with_identity[i][k] -= (mat_with_identity[j][k] / mat_with_identity[j][j])*r;
		}
		else{
			r = mat_with_identity[i][j];
			for (int k = 0; k<2 * n; k++)
				mat_with_identity[i][k] /= r;
		}

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


	delete[] mat_with_identity;


   return 0;





}