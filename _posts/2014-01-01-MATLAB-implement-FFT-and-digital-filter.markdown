---
layout: post
title:  "MATLAB: Implement Digital Filter and FFT"
date:   2016-08-24 12:00:00 +0800
categories: technical
cover_image: "Resources/Posts/Matlab/filter-and-FFT/FFT.png"
---

Signal filtering and FFT are the two most used techniques in data processing, especially for engineering experiments. For me I played with these two for lots of times for my own tests. Here, as a demonstration, I provide a piece of Matlab code to implement a lower pass filter and FFT on a sample data.

The sample data is borrowed from a real world vibration test and the data is stored as a Microsoft Excel file. The Matlab script, `RunFilterData.m`, reads in the file then performs filter and FFT operations on the specified column in the xlsx file. The user can open and check the comments in `RunFilterData.m` and see what happens inside this demonstration.

All the files used could be obtained [here]({{site.baseurl}}/Resources/Posts/Matlab/filter-and-FFT/filter-and-FFT.zip).

The following are the results of the code.

* The time history. Time domain. &darr;

<p><img src="{{site.baseurl}}/Resources/Posts/Matlab/filter-and-FFT/time-history.png" alt="<img>The time history of the data."></p>

* The results of FFT. Frequency domain. &darr;

<p><img src="{{site.baseurl}}/Resources/Posts/Matlab/filter-and-FFT/FFT.png" alt="<img>The time history of the data."></p>
