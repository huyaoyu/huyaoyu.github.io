---
layout: post
title:  "Safety Analysis of a Main Feedwater Pump in the Secondary Loop of a Nuclear Power Plant"
date:   2015-3-1 8:00:00 +0800
categories: projects
---

# Overview

The feedwater pump under question is the pump unit of the secondary loop/circuit for a pressurized water nuclear reactor system (PWR). In a typical PWR system the feedwater pump is used to deliver water to the steam generator as illustrated in the following figure\[1\] and enclosed with a red circle. The water transforms into steam and then drive the steam turbine to generate power in terms of electricity. 

<p><img src="{{site.baseurl}}/Resources/Projects/FeedwaterPump/PWR.png" alt="<img>Pressurized water nuclear reactor system" width="600px"></p>

The feedwater pump is a single stage double suction centrifugal pump and is very similar to the figure below[2]. Our team were required to do a series of analysis focusing on the safety of the pump. The analysis included thermal stress analysis, seismic response analysis and rotor dynamic analysis.

<p><img src="{{site.baseurl}}/Resources/Projects/FeedwaterPump/feedwater-pump.jpg" alt="<img>Feedwater pump"></p>

# Tasks and Results

Again I was designated to conduct the rotordynamic analysis. For rotordynamic characteristics of the feedwater pump is affected by the fluid in the running clearances between the pump rotor and stator. Those running clearances are designed to be helically grooved annular seals. Lots of efforts were taken to evaluate the resistance characteristics of those seals, in order to calculate their supporting stiffness and damping effects. The above mentioned evaluation were done by using ANSYS CFX. The geometry and some boundary conditions are shown in the following figures.

<p><img src="{{site.baseurl}}/Resources/Projects/FeedwaterPump/grooves.png" alt="<img>Grooves" width="400px"></p>

<p><img src="{{site.baseurl}}/Resources/Projects/FeedwaterPump/boundary-1.png" alt="<img>Boundary, part 1" width="400px"></p>

<p><img src="{{site.baseurl}}/Resources/Projects/FeedwaterPump/boundary-2.png" alt="<img>Boundary, part 2" width="400px"></p>

The grid was created by ICEM, all hexahedra.

<p><img src="{{site.baseurl}}/Resources/Projects/FeedwaterPump/grid.png" alt="<img>Grid" width="400px"></p>

A typical velocity vector field inside the clearance is shown in the following figure.

<p><img src="{{site.baseurl}}/Resources/Projects/FeedwaterPump/velocity-field.png" alt="<img>Velocity field" width="400px"></p>

The major outcome of the CFD simulations is the resistance coefficients associated to different clearance widths and rotating speeds. These results are summarized in the following figure. From this figure we can see that, if the rotating speed is fixed, the resistance of the helically grooved annular seal is hardly affected when the clearance width is larger than 1 mm. Generally, higher speed means larger resistance.

<p><img src="{{site.baseurl}}/Resources/Projects/FeedwaterPump/sigma.png" alt="<img>Resistance coefficients"></p>

# References

[\[1\] Pressurized water nuclear reactor system](https://www.clpgroup.com/nuclearenergy/Eng/power/power4_1_2.aspx)

[\[2\] Feedwater pump](https://www.ksb.com/centrifugal-pump-lexicon/boiler-feed-pump/191374/)
