---
layout: post
title:  "Apply Velocity Profile Boundary Condition with UDF in ANSYS FLUENT"
date:   2013-12-04 12:00:00 +0800
categories: technical
cover_image: "Resources/Posts/Fluent/UDF/velocity-profiles.png"
---

I have spent a long time studying the flow in 3D annular regions, e.g. the clearances or the seals in pumps. In order to save computation resources, only the annular flow region is modeled in most of the cases, and a simplified simulation is achieved. An issue for such simplified simulation is that the inlet flow condition could not be well defined, leading to the ignorance of the inlet pre-swirl effect of the annular flow.

In order to deal with the above issue, I have to find a way to apply a velocity profile at the inlet of the annular flow region. In the context of Ansys FLUENT, this could be realized by using the UDF (user-defined function). 

The target is to get a velocity profile along the radial direction at the inlet of a annular shaped flow region. The following figure shows the sketch of the cross-sectional geometry of the annular flow.

<p><img src="{{site.baseurl}}/Resources/Posts/Fluent/UDF/cross-section-annular-flow.png" alt="<img>The cross-sectional geometry." width="300px"></p>

The above figure is identical with that used in one of my [conference paper]({{site.baseurl}}/Resources/PDFPublications/ICON_22_Numerical Analysis of Rotordynamic Coefficients of Annular Flow in Canned Motor RCP.pdf). Detailed description and definitions could be found there.

The profile that I would like to apply is illustrated in the following figure.

<p><img src="{{site.baseurl}}/Resources/Posts/Fluent/UDF/velocity-profiles.png" alt="<img>The cross-sectional geometry." width="400px"></p>

The UDF which generates such a velocity profile is provided in [*velocity-profile.c*]({{site.baseurl}}/Resources/Posts/Fluent/UDF/velocity-profile.c).

In *velocity-profile.c* the velocity boundary conditions are implemented by using the UDF macro `DEFINE_PROFILE`. The velocity vector is decomposed into velocity components. It is assumed that the axis of the annular flow is the global y-axis. Then on the inlet face of the flow region, the *u* and *w* velocity components are defined by the UDF, namely `pre_swirl_velocity_u` and `pre_swirl_velocity_w`. The axial velocity at the inlet is also specified by `DEFINE_PROFILE` macro, however it is simplified to be the average axial flow velocity. Since the rotor may be in an eccentric position as shown in the first figure, the velocity components of the rotor wall are also specified by the UDF through `wirl_v_u` and `wirl_v_w`.

The UDFs in *velocity-profile.c* are all designed to have the ability to handle both the serial and parallel execution, with debug features. The major IO operation, if necessary, is performed by "node zero" which is defined by Fluent itself.
