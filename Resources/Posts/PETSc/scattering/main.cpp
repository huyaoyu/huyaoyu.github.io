/*
 * main.cpp
 *
 *  Created on: Jul 5, 2017
 *      Author: Yaoyu Hu <huyaoyu@sjtu.edu.cn>
 */

#include <iostream>
#include <petscksp.h>

static char help[] = "Example of vector scattering.\n";

int main(int argc,char **args)
{
	Vec            x;
	PetscInt       m = 16;
	PetscMPIInt    rank;
	PetscErrorCode ierr;

	PetscInitialize(&argc,&args,(char*)0,help);
	ierr = PetscOptionsGetInt(NULL,NULL,"-m",&m,NULL);CHKERRQ(ierr);

	ierr = MPI_Comm_rank(PETSC_COMM_WORLD, &rank); CHKERRQ(ierr);

	/*
	 Create parallel vectors.
	*/

	ierr = VecCreate(PETSC_COMM_WORLD,&x);CHKERRQ(ierr);
	ierr = VecSetSizes(x,PETSC_DECIDE,m);CHKERRQ(ierr);
	ierr = VecSetFromOptions(x);CHKERRQ(ierr);

	/*
	  Fill values in the vector
	*/
	PetscInt ownRangeLow = 0, ownRangeHigh = 0;

	ierr = VecGetOwnershipRange(x, &ownRangeLow, &ownRangeHigh); CHKERRQ(ierr);

	PetscScalar base = 100 * rank;
	PetscInt    len  = ownRangeHigh - ownRangeLow; // This may not work.

	PetscScalar* values   = new PetscScalar[len];
	PetscInt*    idxArray = new PetscInt[len];

	int idx = 0;
	for ( int i=ownRangeLow; i<ownRangeHigh; ++i )
	{
	  values[idx] = base + idx;
	  idxArray[idx] = i;
	  idx++;
	}

	ierr = VecSetValues(x, len, idxArray, values, INSERT_VALUES); CHKERRQ(ierr);

	delete [] idxArray; idxArray = PETSC_NULL;
	delete [] values;     values = PETSC_NULL;

	/*
	 Show the content of each processor.
	*/

	PetscScalar* p = PETSC_NULL;

	ierr = PetscBarrier((PetscObject)x); CHKERRQ(ierr); // This is bad. Avoid using it in production code.
	ierr = PetscPrintf(PETSC_COMM_WORLD, "\nShow the content of each processor.\n\n"); CHKERRQ(ierr);
	ierr = PetscBarrier((PetscObject)x); CHKERRQ(ierr); // This is bad. Avoid using it in production code.

	// Show the content by using raw pointer.
	ierr = VecGetArray(x, &p); CHKERRQ(ierr);

	for ( int i = 0; i < len; ++i )
	{
	  ierr = PetscPrintf(PETSC_COMM_SELF, "Rank %d: idx = %d, value = %f.\n", rank, i, p[i]); CHKERRQ(ierr);
	}

	ierr = VecRestoreArray(x, &p); CHKERRQ(ierr);

	/*
	* Vector scattering.
	*/

	ierr = PetscBarrier((PetscObject)x); CHKERRQ(ierr); // This is bad. Avoid using it in production code.
	ierr = PetscPrintf(PETSC_COMM_WORLD, "\nVector scattering, forward.\n\n"); CHKERRQ(ierr);
	ierr = PetscBarrier((PetscObject)x); CHKERRQ(ierr); // This is bad. Avoid using it in production code.

	PetscInt    idx0 = 1, idx1 = m - 2;
	VecScatter  scatter;
	IS          isFrom, isTo;
	PetscInt    fromIdxArray[2] = { idx0, idx1 };
	PetscInt    toIdxArray[2] = { 0, 1 };
	Vec         localVec; // The serial local vector.
	PetscScalar scatteredValues[2] = {-1.0, -2.0};

	ierr = VecCreateSeq(PETSC_COMM_SELF, 2, &localVec); CHKERRQ(ierr);

	ierr = ISCreateGeneral(PETSC_COMM_SELF, 2, fromIdxArray, PETSC_COPY_VALUES, &isFrom); CHKERRQ(ierr);
	ierr = ISCreateGeneral(PETSC_COMM_SELF, 2,   toIdxArray, PETSC_COPY_VALUES,   &isTo); CHKERRQ(ierr);
	ierr = VecScatterCreate(x, isFrom, localVec, isTo, &scatter); CHKERRQ(ierr);

	// Forward scattering.
	ierr = VecScatterBegin(scatter, x, localVec, INSERT_VALUES, SCATTER_FORWARD); CHKERRQ(ierr);
	ierr = VecScatterEnd(scatter,   x, localVec, INSERT_VALUES, SCATTER_FORWARD); CHKERRQ(ierr);

	// Show the result.
	ierr = VecGetArray(localVec, &p); CHKERRQ(ierr);

	for (int i=0; i < 2; ++i)
	{
	  ierr = PetscPrintf(PETSC_COMM_SELF, "Rank %d: localVec[%d] = %f.\n",
			  rank, i, p[i]);
	}

	ierr = VecRestoreArray(localVec, &p); CHKERRQ(ierr);

	ierr = PetscBarrier((PetscObject)x); CHKERRQ(ierr); // This is bad. Avoid using it in production code.
	ierr = PetscPrintf(PETSC_COMM_WORLD, "\nVector scattering, reverse.\n\n"); CHKERRQ(ierr);
	ierr = PetscBarrier((PetscObject)x); CHKERRQ(ierr); // This is bad. Avoid using it in production code.

	// Prepare the values.
	ierr = VecSetValues(localVec, 2, toIdxArray, scatteredValues, INSERT_VALUES); CHKERRQ(ierr);

	// Reverse scattering.
	ierr = VecScatterBegin(scatter, localVec, x, INSERT_VALUES, SCATTER_REVERSE); CHKERRQ(ierr);
	ierr = VecScatterEnd(scatter,   localVec, x, INSERT_VALUES, SCATTER_REVERSE); CHKERRQ(ierr);

	// Show the result.
	ierr = VecGetArray(x, &p); CHKERRQ(ierr);

	for ( int i = 0; i < len; ++i )
	{
	  ierr = PetscPrintf(PETSC_COMM_SELF, "Rank %d: idx = %d, value = %f.\n", rank, i, p[i]); CHKERRQ(ierr);
	}

	ierr = VecRestoreArray(x, &p); CHKERRQ(ierr);

	// Clean up.
	ierr = VecScatterDestroy(&scatter); CHKERRQ(ierr);
	ierr = ISDestroy(&isFrom);          CHKERRQ(ierr);
	ierr = ISDestroy(&isTo);            CHKERRQ(ierr);
	ierr = VecDestroy(&localVec);       CHKERRQ(ierr);
	ierr = VecDestroy(&x);              CHKERRQ(ierr);

	ierr = PetscFinalize();
	return 0;
}

