---
layout: post
title:  "Simple optical flow based on depth and pose information"
date:   2018-08-05 23:21:05 -0400
categories: technical
cover_image: "Resources/Posts/Robotics/OpticalFlow/image_warped.png"
tag: mechanical
---

Recently, I was helping to implement a simple program that performs optical flow calculation between frames of video images with the help of known depth and pose information associated with the images.

Let us define the first frame as Frame 0, and the second to be Frame 1. And for my current task, the camera is under constant movement. For optical flow, it is usually represented by a color image with HSV color space. In this image, Channel H (Hue) is used to represent the flow direction of the individual pixel of the Frame 0. That is if the camera is moving, every pixel of Frame 0 seems to move with respect to Frame 1 and exhibiting a representative color regarding the flow direction. Channel V (Value or Lightness) tells the magnitude of the movement of a pixel. The larger the movement, the brighter it should be. We leave the S (Saturation) Channel to be its highest value and never touch it.

The thing is, once you know the camera pose and depth information of each frame, you know everything. No fancy algorithm is needed to perform optical flow calculation since you can calculate the movement of the individual pixel. The overall process looks very much like the following.

<p><img src="{{site.baseurl}}/Resources/Posts/Robotics/OpticalFlow/flow_chart.png" alt="<img>Flow chart." width="600px"><br>
Figure 1 The flow chart. &uarr;
</p>

As shown in figure 1, the overall process is really straightforward. If the camera is rectified and has no distortion, all of the transformations could be expressed by simple linear algebra expressions.

Actually, the images are extracted from video frames recorded in the [Unreal Engine](https://www.unrealengine.com/en-US/what-is-unreal-engine-4?lang=en-US). We are simulating a UAV in Unreal with the help of [AirSim](https://github.com/Microsoft/AirSim), an open-source project implementing drone simulators. AirSim is maintained by Microsoft. The video stream comes from the virtual camera mounted on the simulated UAV in Unreal. Individual frame images are extracted from the video stream, with associated depth and camera pose information. Two sample image frames are shown in figure 2.

<p>
    <img src="{{site.baseurl}}/Resources/Posts/Robotics/OpticalFlow/image_0.png" alt="<img>Image 0." ><br>
    <img src="{{site.baseurl}}/Resources/Posts/Robotics/OpticalFlow/image_1.png" alt="<img>Image 1." ><br>
Figure 2 Original frame images. Top: first frame, bottom: second frame. &uarr;
</p>

As one can easily perceive, the camera has a yaw movement from left to right direction. The global coordinate system is defined in such a way that the z-axis is pointing downwards. Then this yaw movement is represented by a rotation with positive angle value along the z-axis. The resulting optical flow image may look like figure 3. The values for the H and V channels of the resulting HSV image are calculated in a way similar to [one of the OpenCV tutorials](https://docs.opencv.org/master/d7/d8b/tutorial_py_lucas_kanade.html).

<p><img src="{{site.baseurl}}/Resources/Posts/Robotics/OpticalFlow/optical_flow.jpg" alt="<img>The optical flow." ><br>
Figure 3 The optical flow. &uarr;
</p>

The color in figure 3 shows that all the pixels of Image 0 are moving in roughly the same direction. In this case, the direction is from left to right. The brightness of figure 3 shows the magnitude of pixel movements. For pure yaw movement, the close a pixel locates to the vertical center line of the image, the less it moves in magnitude. 

Since we have these data, we could do something else like computing the 3D point clouds based on Image 0 and 1. We could import these point clouds into MeshLab and check the consistency between the depth and cameras poses. The point clouds may look like figure 4 and figure 5.

<p><img src="{{site.baseurl}}/Resources/Posts/Robotics/OpticalFlow/single_point_cloud_scaled.png" alt="<img>Single point cloud." ><br>
Figure 4 Point cloud of Image 0. &uarr;
</p>

<p><img src="{{site.baseurl}}/Resources/Posts/Robotics/OpticalFlow/double_point_clouds_scaled.png" alt="<img>Double point clouds." ><br>
Figure 5 Point clouds of Image 0 and 1. White: Image 0. Red: Image 1. &uarr;
</p>

In figure 4, I use color to indicate the distance from the camera. Blue means points are near the camera and red means far away. I generate my own color mapping method with the help of [this post](https://bsou.io/posts/color-gradients-with-python). In figure 5, the two point clouds get aligned, meaning that the depths and camera poses are consistent with each other. Good job.

One other thing one may do with these data is warping Image 0 to Image 1. After obtaining the optical flow between the two images, we could easily warp Image 0 and see how it will look like from the perspective of Image 1. Figure 6 shows the result of warping.

<p><img src="{{site.baseurl}}/Resources/Posts/Robotics/OpticalFlow/image_warped.png" alt="<img>The warped image." ><br>
Figure 6 Warped image. &uarr;
</p>

In figure 6, all the regions that could be seen originally in Image 0 but could not be perceived in Image 1 are rendered black. The pixels of Image 0 move to their new locations in Image 1. Since the camera is rotating from left to right, objects in the camera view will move to the left. The black ripples in figure 6 are the result of transformation with discretized pixels, pixels originally located at the left of Image 0 simply could not cover a relatively larger area in Image 1, leaving holes after warping.

This work is done with the help of [Wenshan Wang](http://wangwenshan.com).

