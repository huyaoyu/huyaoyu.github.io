---
layout: post
title:  "Study of Physical Simulation Technology for a Pump Rotor"
date:   2011-6-1 8:00:00 +0800
categories: phd
cover_image: "Resources/Projects/PhysicalSimulation/VirtualLab.png"
---

# Overview

Let's be honest, this work was not excited as you may expect.

At beginning, I was designated to develop a program that could some how perform simulation just like MCS Adams dose. 

Well, it was mission impossible.

To demonstrate the developed program, it is designed to simulate a rotating rotor with unbalanced mass.

Did I mention that the project only have 3 month available work time? And I was the only team member?

# Tools

Here we go, I decide it has to be done with the help of open source software packages. I chose ODE (Open Dynamic Engine) as the dynamics solver and OSG (Open Scene Graph) as the 3D geometry renderer. The program was developed as object-oriented codes by C++ with some help of XML and it was running on Windows. The UI was designed based on Xtreme Toolkit Pro. I use doxygen to generate my documentation and visual leak detector to facilitate the debugging process. Back to those days, I setup a private SVN server to do the version control.

# Codes

It was pretty simple, actually. 

The main components were arranged as the following figure. The relationship among these components are:

* When the program is launched, a global OSG object and a global ODE object are created.
* A new CVLGroup object is created together with a new CVLSim object. Here, the components with the prefix "CVL" are the c++ classes that designed by myself. CVLSim holds the simulated objects and simulation settings. It performs dynamic simulation. CVLGroup manages the objects and their connections. CVLObject is the object which CVLSim and CVLGroup operate on.
* Moving constraints and external forces or loads are inserted to CVLSim.
* Let CVLSim do the simulation based on ODE.
* Use VLDB to write the model into file system or read a previous simulation back.
* Data and information are exchanged in formate defined by CVLData.

<p><img src="{{site.baseurl}}/Resources/Projects/PhysicalSimulation/Components.png" alt="<img>Main components" width="400px"></p>

Data are exchanged and represented by CVLData. Based on CVLData, a inheritance tree could be designed as the following figure.

<p><img src="{{site.baseurl}}/Resources/Projects/PhysicalSimulation/DataExchangeClasses.png" alt="<img>Data exchange classes"></p>

I defined a series of external force classes. They serve as an extension of the original ODE functionalities. They are as follows.

<p><img src="{{site.baseurl}}/Resources/Projects/PhysicalSimulation/ForceClasses.png" alt="<img>Force classes"></p>

When the program launched and a case is read back from file system. It looks like the following figure.

<p><img src="{{site.baseurl}}/Resources/Projects/PhysicalSimulation/VirtualLab.png" alt="<img>Force classes"></p>

