---
layout: post
title: "TartanAir: A Dataset to Push the Limits of Visual SLAM"
date: 2020-05-03 18:28:07
categories: technical
cover_image: "Resources/Posts/Robotics/TartanAir/web_cover_figure.png"
tag: robotics
---

{% include youtube-player.html id="qDwfHvTbJx4" %}

## This is a joint work with many other collaborators. Please see the Citation section. ##

This work is accomplished with the team of [theairlab](http://theairlab.org) and Microsoft.

The original project page is [here][project_page].

[project_page]: http://theairlab.org/tartanair-dataset/

## Abstract ## 

We present a challenging dataset, the TartanAir, for robot navigation task and more. The data is collected in photo-realistic simulation environments in the presence of various light conditions, weather and moving objects. By collecting data in simulation, we are able to obtain multi-modal sensor data and precise ground truth labels, including the stereo RGB image, depth image, segmentation, optical flow, camera poses, and LiDAR point cloud. We set up a large number of environments with various styles and scenes, covering challenging viewpoints and diverse motion patterns, which are difficult to achieve by using physical data collection platforms. In order to enable data collection in such large scale, we develop an automatic pipeline, including mapping, trajectory sampling, data processing, and data verification. We evaluate the impact of various factors on visual SLAM algorithms using our data. Results of state-of-the-art algorithms reveal that the visual SLAM problem is far from solved, methods that show good performance on established datasets such as KITTI don't perform well in more difficult scenarios. Although we use the simulation, our goal is to push the limits of Visual SLAM algorithms in the real world by providing a challenging benchmark for testing new methods, as well as large diverse training data for learning-based methods. 

## Citation ## 

```
@article{tartanair2020arxiv,
  title =   {TartanAir: A Dataset to Push the Limits of Visual SLAM},
  author =  {Wenshan Wang, Delong Zhu, Xiangwei Wang, Yaoyu Hu, Yuheng Qiu, Chen Wang, Yafei Hu, Ashish Kapoor, Sebastian Scherer},
  journal = {arXiv preprint arXiv:2003.14338},
  year =    {2020}
}
```

## Download

The dataset is published using Azure Open Dataset platform. Please visit the [project page][project_page]

## The mission ##

Simultaneous Localization and Mapping (SLAM) is one of the most fundamental capabilities necessary for robots. Due to the ubiquitous availability of images, Visual SLAM (V-SLAM) has become an important component of many autonomous systems. Impressive progress has been made with both geometric-based methods and learning-based methods. However, developing robust and reliable SLAM methods for real-world applications is still a challenging problem. Real-life environments are full of difficult cases such as light changes or lack of illumination, dynamic objects, and texture-less scenes. Current popular benchmarks such as KITTI, TUM RGB-D SLAM datasets, and EuRoC MAV cover relatively limited scenarios and motion patterns compared to real-world cases. 

We collect a large dataset using photo-realistic simulation environments. We minimize the sim2real gap by utilizing a large number of environments with various styles and diverse scenes. A special goal of our dataset is to focus on the challenging environments with changing light conditions, adverse weather, and dynamic objects. State-of-the-art SLAM algorithms are struggled in tracking the camera pose in our dataset and constantly getting lost on some challenging sequences. We propose a metric to evaluate the robustness of the algorithm. In addition, we develop an automatic data collection pipeline, which allows us to process more environments with minimum human intervention. 

The four most important features of our dataset are: 

- Large size diverse realistic data
- Multimodal ground truth labels
- Diversity of motion patterns
- Challenging Scenes

## Dataset features ##

### Simulated scenes ###
We have adopted more than 50 photo-realistic simulation environments in the [Unreal Engine][UnrealEngine]. The environments provide us a wide range of scenarios that cover many interesting yet challenging situations. The simulation scenes consist of

[UnrealEngine]: https://www.unrealengine.com/

- Indoor and outdoor scenes with detailed 3D objects. We have multi-room, richly decorated indoor environments. For outdoor simulations, there are various kinds of buildings, trees, terrains, and landscapes.
- Special purpose facilities and ordinary household scenes.
- Rural and urban scenes.
- Real-life and sci-fi scenes.

<img src="{{site.baseurl}}/Resources/Posts/Robotics/TartanAir/environments.png" alt="Simulated environments" />
A glance of the simulated environments.

### Challenging visual effects ###
In some simulations, we simulated multiple types of challenging visual effects.

- Hard lighting conditions. Day-night alternating. Low-lighting. Rapidly changing illuminations.
- Weather effects. Clear, raining, snowing, windy, and fog.
- Seasonal change.

<figure>
 <img src="{{site.baseurl}}/Resources/Posts/Robotics/TartanAir/visual_effects.png" alt="The challenging visual effects." />
 <figcaption>
 Challenging visual effects.
 </figcaption>
</figure>

### Diverse ego motions ###

In each simulated environment, we gather data by following multiple routes and making movements with different levels of aggressiveness. The virtual camera can move slowly and smoothly without sudden jittering actions. Or it can have intensive and violent actions mixed with significant rolling and yaw motions.


### Multimodal ground truth labels ###

By unleashing the power of the [Unreal Engine][UnrealEngine] and [AirSim][AirSimSite], we can extract various types of ground truth labels including depth, semantic segmentation tag, and camera pose. From the extracted raw data, we further compute other ground truth labels such as optical flow, stereo disparity, simulated multi-line LiDAR points, and simulated IMU readings.

[AirSimSite]: https://github.com/microsoft/AirSim

<figure>
 <img src="{{site.baseurl}}/Resources/Posts/Robotics/TartanAir/multimodal_data_20200301.png" alt="Data and ground truth labels." />
 <figcaption>
 Data and ground truth labels.
 </figcaption>
</figure>

## Data acquisition pipeline ##

We develop a highly automated pipe-line to facilitate data acquisition. For each environment, we build an occupancy map by incremental mapping. Base on the map, we then sample a bunch of trajectories for the virtual camera to follow. A set of virtual cameras follow the trajectories to capture raw data from Unreal Engine and AirSim. Raw data are processed to generate labels such as optical flow, stereo disparity, simulated LiDAR points, and simualated IMU readings. Data verification. Verify the data synchronization and the accuracy of the derived labels.

<figure>
 <img src="{{site.baseurl}}/Resources/Posts/Robotics/TartanAir/pipeline_20200301.png" alt="Data acquisition pipeline." />
 <figcaption>
 Data acquisition pipeline.
 </figcaption>
</figure>

### Contact

Wenshan Wang - (wenshanw [at] andrew [dot] cmu [dot] edu) 

Sebastian Scherer - (basti [at] cmu [dot] edu)

### Acknowledgments 

This work was supported by Office of Naval Research under award number N0014-19-1-2266. We thank Ratnesh Madaan, Guada Casuso, Rashaud Savage and other team members from Microsoft for the technical support! 

### Term of use

<a rel="license" href="http://creativecommons.org/licenses/by/4.0/"><img alt="Creative Commons License" style="border-width:0" src="https://i.creativecommons.org/l/by/4.0/80x15.png" /></a><br />This work is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by/4.0/">Creative Commons Attribution 4.0 International License</a>.