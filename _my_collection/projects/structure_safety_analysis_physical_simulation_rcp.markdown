---
layout: post
title:  "Structure Safety Analysis of a Canned Motor Reactor Coolant Pump"
date:   2011-11-14 8:00:00 +0800
categories: projects
---

# Overview

Safety is the top priority in the design of a nuclear power plant. For one of the 3rd-Generation nuclear reactor systems, the AP series systems have very high level of safety requirements. AP system takes canned motor reactor coolant pump (RCP) as its main pump of the primary coolant loop. The safety characteristics of this canned motor RCP is crucial to the reactor system. 

In this project we have conducted numerical analysis on several aspects of a canned motor RCP concerning its safety. The content includes:

* Static structure analysis
* Strength and integrity of high inertia fly wheel
* Cracks in fly wheel
* Response of the static components under seismic load
* Response of the rotating components under seismic load in running down condition

The task of analysis the response of the rotating components under seismic load in running down condition was assigned to me. 

# Motivation

The RCP should shift from normal condition to running down condition as soon as a certain level of earthquake strikes to the nuclear power plant and the pump's electric power supply is cut off. The nuclear reactor follows a emergency shut down procedure. The RCP **MUST** keep rotating for a certain period of time, continue delivering reactor coolant to the reactor core, removing heat from the still active core. In the entire running down process of RCP during an earthquake, no rub or contact between the rotor and stator is permitted. This analysis was aiming at performing simulation of a running down process of canned motor RCP under earthquake condition. 

# Methodology

In the simulation we had to calculate the deflection of both the pump rotor and stator under seismic load in a running down. Then we checked the relative displacements of the rotor and stator during the whole running down process. We utilized the following tool chain to perform a transient simulation with rigid-flexible bodies.

* Generate mesh with Altar Hypermesh.
* Modal analysis of rotor and stator with MCS Nastran. Mode expansion.
* Dynamic/Transient analysis with MCS Adams.

In MCS Adams, the models of rotor and stator are imported as MCS Nastran results after mode expansion. The rotor and stator are both flexible and deformable. They may appear like the following figures. A dummy rigid body is inserted into the workspace to play as the supporting structure of the canned motor RCP. The earthquake movement is applied on this dummy rigid body. The stator is attached to the rigid body and shaking together with it. The rotor is connected to the stator by two journal bearings and one thrust bearing. An initial rotating speed is set up at the beginning of the transient simulation. The seismic movement takes place on the rigid body through out the simulation. 

<p><img src="{{site.baseurl}}/Resources/Projects/RCP_Adams/Adams_Rotor.png" alt="<img>Rotor model in Adams" width="400px"></p>

<p><img src="{{site.baseurl}}/Resources/Projects/RCP_Adams/Adams_Stator.png" alt="<img>Stator model in Adams"></p>

# Results

The displacement of some pre-selected monitor points are sampled during the simulation. These points are arranged in pairs. For each pair of monitor points, one is on the rotor and the other is the nearest point on the stator. The distance between two points in a pair is calculated. A time history of such a distance could be drawn during the simulation process. A typical result could be shown in the following figure.

<p><img src="{{site.baseurl}}/Resources/Projects/RCP_Adams/RunningDown_Cutted.png" alt="<img>Running down process"></p>
