---
layout: post
title:  "Research on the Effects of Annular Flow on the Stability of the Rotordynamic System in a Canned Motor RCP"
date:   2016-1-1 8:00:00 +0800
categories: phd
cover_image: "Resources/Projects/AnnularFlow/geometry-and-coordinate-transformation.png"
---

It is a government funded research project. The subject is the annular flow in a canned motor reactor coolant pump (RCP). Half of the content is the basis of my own Ph.D. thesis. The extent of this project includes:

* Flow characteristics of an annular flow.
* Its rotordynamic effects on the rotor.
* Major affecting factors of the rotordynamic effects.
* Heat transfer inside the annular flow. And 
* Coupled annular flow rotordynamics.

The tasks that I was working on are the rotordynamic effects of the annular flow and its affecting factors. Generally, the work consists of theoretical analysis, CFD programming, numerical analysis and experimental study. 

In all the tasks, an annular flow field is studied. The cross-section of the annular flow field is illustrated in the following figure. The fluid inside the annular clearance is disturbed by the whirling motion of the rotating rotor. The flow in the clearance is axial asymmetric and periodic. There are flow induced forces on the rotor and these forces in return, affect the dynamics of the rotor.

<p><img src="{{site.baseurl}}/Resources/Projects/AnnularFlow/geometry-and-coordinate-transformation.png" alt="<img>Cross-section view of the annular flow" width="400px"></p>

Numerical analysis was conducted to study the annular flow. A parallel CFD code was developed using C++ to solve the incompressible N-S equations with curvilinear coordinate system plus coordinate transformation. Hexahedral unstructured collocated grid is generated automatically by a python script. The SST *k*-*&omega;* turbulence model is adopted. The numerical solver implements the finite volume method in 3D space, in cooperating with the TVD scheme and least square gradient reconstruction method. Considering the difficulties that brought by the curvilinear coordinate system and coordinate transformation, modified SIMPLE algorithm is utilized to handle the pressure-velocity coupling. The program runs on a Linux cluster and the result data is processed by Matlab scripts.

Experimental study was also performed. A new experiment rig was designed and built. The design model is illustrated in the following figure. This experiment rig has a vertical rotor supported by two set of bearing components. The bearing components are designed to make the rotor moving in a whirling fashion with controlled eccentricity. The rotor is enclosed by a stator and a annular clearance is formed between the rotor and stator. Experiments were taken out with various rotating speeds, whirl frequencies, eccentric ratios and axial flow rates. The fluid induced forces of the annular clearance flow were measured by the load sensors installed at the supporting structure of the bearing components. The rotor displacements, axial pressure drop across the annular clearance, the fluid temperature were also measured. 

<p><img src="{{site.baseurl}}/Resources/Projects/AnnularFlow/experiment-rig.png" alt="<img>Experiment rig"></p>

In the above figure, the labeled components are:

1-Base supporting structure. 2-Load sensor. 3-Outlet component. 4-Upper bearing component. 5-Flexible coupling. 6-Non-whirling bearing. 7-Synchronous pulley (for rotating only). 8-Rigid coupling. 9-Stator. 10-Displacement and pressure sensor. 11-Rigid coupling. 12-Synchronous pulley (for whirling only). 13-Lower bearing component.


