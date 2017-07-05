---
layout: post
title:  "PETSc: Vector Scattering"
date:   2016-08-02 12:00:00 +0800
categories: technical
cover_image: "Resources/Posts/PETSc/scattering/Cover.png"
---

In PETSc, vector scattering is the functionality to gather and distribute values from and to the global vector. When the program is running in parallel, every processor (or thread or rank) possesses a certain portion of the global vector. Vector scattering comes into play when one processor need to get access to the values stored on the other processors. 

In order to perform a vector scattering operation, the global indices of the desired values must be determined beforehand. Then every processor should create a `VecScatter` object. Each processor should create a local vector larger enough to hold the scattered values. Then EVERY processor should call `VecScatterBegin()` and `VecScatterEnd()` no matter whether a processor needs vector scattering or not. After performing relevant operations with the scattered values, the resources occupied by the VecScatter object and the local vector should be released.

A sample code is provided [here]({{site.baseurl}}/Resources/Posts/PETSc/scattering/main.cpp).

In the sample code a global vector __x__ of length 16 is created across 4 processors (ranks). The content of the original __x__ is illustrated in the following figure.

<p><img src="{{site.baseurl}}/Resources/Posts/PETSc/scattering/original-vector.png" alt="<img>The original vector."></p>

A vector scattering operation is performed on every processor to obtain values at the global indices 1 and 14. And every processor makes a copy to those values and stores them into its local vector `localVec`. &darr;

<p><img src="{{site.baseurl}}/Resources/Posts/PETSc/scattering/local-vector-after-forward-scattering.png" alt="<img>The local vector after forward scattering."></p>

Every processor changes the values in `localVec`. &darr;

<p><img src="{{site.baseurl}}/Resources/Posts/PETSc/scattering/local-vector-before-reverse-scattering.png" alt="<img>The local vector before reverse scattering."></p>

A reverse scattering operation is performed by all the processors to write values back to the specified global indices. &darr;

<p><img src="{{site.baseurl}}/Resources/Posts/PETSc/scattering/global-vector-after-reverse-scattering.png" alt="<img>The global vector after reverse scattering."></p>

The executable of this sample code should be run by using the commands `mpiexec --np 4 <executable name>`. The output of this sample code is as follows.

<p><img src="{{site.baseurl}}/Resources/Posts/PETSc/scattering/output.png" alt="<img>The output."></p>

This sample code is extremely simplified that all the processors are accessing the values with the same set of global indices. And all the processors require to alter the values with the same global indices. In reality, each processor may require values with different sets of global indices. The important thing to remember is that all the processors must perform the scattering operation. For the processors which do not actually need vector scattering, just perform a scattering operation with zero inquiries of global values.


