PROJECT(PCF)

cmake_minimum_required(VERSION 2.8)

# Set variables.

SET(T_NAME PerturbedClearanceFlow)
SET(NAME_TEST ${T_NAME}_Test)

SET(CMAKE_C_FLAGS "-fPIC -Wall -Wwrite-strings -Wno-strict-aliasing -Wno-unknown-pragmas -g3 -O0")
SET(CMAKE_CXX_FLAGS "-fPIC -Wall -Wwrite-strings -Wno-strict-aliasing -Wno-unknown-pragmas -g3 -O0")

# Search for the source files.

FILE(GLOB_RECURSE SRC_LIST *.cpp)

IF (SRC_LIST)
	FOREACH (SFILE ${SRC_LIST})
		MESSAGE(STATUS "CPP: ${SFILE}")
	ENDFOREACH (SFILE)
ENDIF (SRC_LIST)

# Set include paths and link paths.

SET(PETSC_ROOT $ENV{PETSC_DIR})
SET(PETSC_ROOT_ARCH $ENV{PETSC_DIR}/$ENV{PETSC_ARCH})
SET(MPI_ROOT $ENV{MPI_DIR})
SET(GCC_ROOT $ENV{SYS_GCC_DIR})
SET(GTEST_ROOT $ENV{GTEST_DIR})

INCLUDE_DIRECTORIES(${PETSC_ROOT}/include ${PETSC_ROOT_ARCH}/include ${MPI_ROOT}/include ${GTEST_ROOT}/include)

LINK_DIRECTORIES(${PETSC_ROOT}/lib ${PETSC_ROOT_ARCH}/lib ${MPI_ROOT}/lib ${GCC_ROOT} ${GTEST_ROOT}/builddir /usr/local/lib)

# Set executable target.

ADD_EXECUTABLE(${T_NAME} ${SRC_LIST})

# Check MPI type.
IF ("$ENV{MPI_NAME}" MATCHES "OpenMPI")
	MESSAGE(STATUS "The mpi type is OpenMPI")
	TARGET_LINK_LIBRARIES(${T_NAME} flapack fblas petsc mpi_usempi mpi_mpifh mpi_cxx mpi gtest X11 pthread ssl crypto m gfortran quadmath stdc++ dl rt)
ELSEIF ("$ENV{MPI_NAME}" MATCHES "local_lapack")
	MESSAGE(STATUS "The mpi type is OpenMPI_local_lapack")
	TARGET_LINK_LIBRARIES(${T_NAME} lapack blas petsc mpi_usempi mpi_mpifh mpi_cxx mpi gtest X11 pthread ssl crypto m gfortran quadmath stdc++ dl rt)
ELSEIF ("$ENV{MPI_NAME}" MATCHES "mgmt")
	MESSAGE(STATUS "The mpi type is OpenMPI_mgmt")
	TARGET_LINK_LIBRARIES(${T_NAME} flapack fblas petsc mpi_usempif08 mpi_usempi_ignore_tkr mpi_mpifh mpi_cxx mpi gtest X11 pthread ssl crypto m gfortran stdc++ dl rt)
ELSEIF ("$ENV{MPI_NAME}" MATCHES "MPICH2")
	MESSAGE(STATUS "The mpi type is MPICH2")
	TARGET_LINK_LIBRARIES(${T_NAME} flapack fblas petsc mpifort mpicxx mpi gtest X11 pthread ssl crypto m gfortran quadmath stdc++ dl rt)
ENDIF()

# Add test target.

ADD_EXECUTABLE(${NAME_TEST} ${SRC_LIST})

TARGET_COMPILE_DEFINITIONS(${NAME_TEST} PRIVATE "-DPCF_TEST")

# Check MPI type.
IF ("$ENV{MPI_NAME}" MATCHES "OpenMPI")
	MESSAGE(STATUS "The mpi type is OpenMPI")
	TARGET_LINK_LIBRARIES(${NAME_TEST} flapack fblas petsc mpi_usempi mpi_mpifh mpi_cxx mpi gtest X11 pthread ssl crypto m gfortran quadmath stdc++ dl rt)
ELSEIF ("$ENV{MPI_NAME}" MATCHES "local_lapack")
	MESSAGE(STATUS "The mpi type is OpenMPI_local_lapack")
	TARGET_LINK_LIBRARIES(${NAME_TEST} lapack blas petsc mpi_usempi mpi_mpifh mpi_cxx mpi gtest X11 pthread ssl crypto m gfortran quadmath stdc++ dl rt)
ELSEIF ("$ENV{MPI_NAME}" MATCHES "mgmt")
	MESSAGE(STATUS "The mpi type is OpenMPI_mgmt")
	TARGET_LINK_LIBRARIES(${NAME_TEST} flapack fblas petsc mpi_usempif08 mpi_usempi_ignore_tkr mpi_mpifh mpi_cxx mpi gtest gtest_main X11 pthread ssl crypto m gfortran stdc++ dl rt)
ELSEIF ("$ENV{MPI_NAME}" MATCHES "MPICH2")
	MESSAGE(STATUS "The mpi type is MPICH2")
	TARGET_LINK_LIBRARIES(${NAME_TEST} flapack fblas petsc mpifort mpicxx mpi gtest X11 pthread ssl crypto m gfortran quadmath stdc++ dl rt)
ENDIF()
