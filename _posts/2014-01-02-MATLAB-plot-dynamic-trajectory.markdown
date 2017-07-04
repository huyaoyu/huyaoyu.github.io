---
layout: post
title:  "MATLAB: Plot Dynamic Trajectory"
date:   2013-01-17 12:00:00 +0800
categories: technical
cover_image: "Resources/Posts/Matlab/trajectory/last-frame.png"
---

Matlab provides a function, `comet`, to dynamically plot a trajectory. But back to the days in 2013, I was not satisfied with this function since it leaves the all the history of a dynamic trajectory on the figure. What I was seeking in mind is a function that plots a fixed mount of data from the trajectory and the user can only see a portion of the trajectory on the screen. And the trajectory moves like a snake.

To do this I checkout the source codes of comet and composed my own version based on the official one. The function is called `newcomet`. And the source code is [here]({{site.baseurl}}/Resources/Posts/Matlab/trajectory/newcomet.m).

The following is the trajectory of its full time history.

<p><img src="{{site.baseurl}}/Resources/Posts/Matlab/trajectory/the-trajectory.png" alt="<img>The trajectory."></p>

The following is the snapshot of newcomet on the fly.

<p><img src="{{site.baseurl}}/Resources/Posts/Matlab/trajectory/last-frame.png" alt="<img>Output of the comet."></p>

The syntax of `newcomet` is the same with `comet`. However, if you supply a third argument, which is supposed to be an integer larger than 1, to `newcomet`, `newcomet` draws the trajectory with the length equals to this argument.

The script producing the above two figures is [here]({{site.baseurl}}/Resources/Posts/Matlab/trajectory/run.m). Enjoy!
