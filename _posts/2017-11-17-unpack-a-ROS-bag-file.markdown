---
layout: post
title:  "Read a ROS bag file"
date:   2017-11-17 16:00:00 -0500
categories: technical
cover_image: "Resources/Posts/ReadARosBagFile/roslogo.png"
---

# The opportunity

Recently I am at Carnegie Mellon University. This is a really good academic institute and I really enjoy the time while I am being here.

Thing is I got a wonderful opportunity to work with a team of the Robotics Institute, specifically, the Field Robotics Center, FRC. 

I offered my help on developing codes which work on the bag file produced by the ROS ([Robot Operating System][ROSWebSite]) system. So today, I am gonna write a simple post on the unpacking and reading of a ROS bag file.

[ROSWebSite]: http://www.ros.org/

# The objective

A bag file could be thought as a sort of log file that ROS produces when different `nodes` of various `packages` are exchanging `messages` through `topics`. A bag file can contain various types of data, ranging from raw bytes to video streams. ROS provides utilities to record or re-play a bag file, along with many other operations. The objective of this task is to extract images from a video stream that packed inside a bag file.

Due to confidential reason, I could not upload the actual bag file of their robot. I prepared a sample bag file holding a segment of a video stream and to demonstrate the procedures of reading information back from a bag file.

The sample bag file is uploaded to my [Google Drive directory][GoogleDrive]. The video is me in front of my laptop and waving my body stupidly.

<p><img src="{{site.baseurl}}/Resources/Posts/ReadARosBagFile/0.jpg" alt="<img>Snapshot of the sample video."></p>

The version of ROS I am working on is lunar.

[GoogleDrive]: https://drive.google.com/open?id=1P-Ve6QPEvLd49j-76mj1AlvhBUTbmBNR

# The procedure

To work on a ROS bag file, you have to install ROS in your system. It is pretty easy and straightforward. To process video and image stuff, I choose to use [OpenCV][OpenCVWebSite]. In the context of Python, this means installing the cv2 package via pip. Furthermore, we need cv_bridge to transfer data from the bag file into formatted data which OpenCV could operate.

[OpenCVWebSite]: https://docs.opencv.org/2.4/index.html

I wrote a sample code to extract individual images from the video stream packed inside the ROS bag file, in Python. Of course, you could use C++ if you prefer. The sample code looks like this.

{% highlight python %}

import pprint

import subprocess, yaml
import rosbag
from cv_bridge import CvBridge
import cv2

testInputBagFile = 'ccBag.bag'

readTopic = '/cc'

outputPath = './SingleImages'

FX = 1.0
FY = 1.0

if __name__ == '__main__':
	info_dict = yaml.load(subprocess.Popen(['rosbag', 'info', '--yaml', testInputBagFile], stdout=subprocess.PIPE).communicate()[0])

	# pp = pprint.PrettyPrinter(indent = 4)
	# pp.pprint( info_dict )

	bag = rosbag.Bag(testInputBagFile)

	print(bag)

	genBag = bag.read_messages(readTopic)

	for k,b in enumerate(genBag):
		print("OK, %d / %d" % (k, info_dict['messages']))
		cb = CvBridge()
		cv_image = cb.imgmsg_to_cv2( b.message, b.message.encoding )

		cv2.imwrite(outputPath + '/' + str(k) + '.jpg', cv_image)

		cv_image_r = cv2.resize(cv_image, dsize = (0,0), fx = FX, fy = FY)
		cv2.imshow('topic', cv_image_r)
		key = cv2.waitKey(0)

		if 113 == key:
			print("q pressed. Abort.")
			break

	cv2.destroyAllWindows()

	bag.close()

{% endhighlight %}

It opens the bag file, exams the overview of its content and extracts every single image from the video stream with the help of OpenCV packages. The user should select the window with her/his mouse pointer, and hit any button other than 'q' on the keyboard to process a single frame of image. When all the frames are processed, the code terminates. The user could also push 'q' key anytime to quit from the program. The extracted images are stored as jpg files in the folder specified in the python script. There is also some output in the console.

<p><img src="{{site.baseurl}}/Resources/Posts/ReadARosBagFile/PythonOutput.png" alt="<img>The output in ipython command line."></p>

Some quick tips are:
* rosbag.Bag.read_messages() returns a generator.
* The number of messages can be found in the dictionary `info_dict`.

You may want to get access to the documentation of the python API for the ROS bag file. You may find that the URL of this documentation on the ROS Wiki website is broken. I put it [here][ROSBagAPIPython] in case anyone wants to refer to it.

[ROSBagAPIPython]: http://docs.ros.org/lunar/api/rosbag/html/python/

Hope you enjoy it!

__Special thanks: My mentor of ROS, [Wenshan Wang](http://wangwenshan.com).__

__The cover image of this post is copied form the [ROS official website][ROSWebSite].__

