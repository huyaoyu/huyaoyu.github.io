---
layout: post
title:  "The Convolution Neural Network with PyTorch"
date:   2017-11-15 12:00:00 -0500
categories: technical
cover_image: "Resources/Posts/ANN/CNN/LeNet.png"
---

# Let's look back first

In one of my [previous post][PrePost], at the last section, I claimed that I would give a try to the Convolution Neural Network (CNN). Here I am! I know it has been a while since I make that promise. People would say, better late than never.

[PrePost]: {% post_url 2017-07-11-fully-connected-artificial-neural-network %}

CNN is more complex than fully connected neural network (FCNN). At the beginning, I was trying to grasp the fundamentals and, like what I did for FCNN, was trying to develop a code in plain python. I found some concepts of CNN hard to fully understand and they all make the modeling of the back-propagation even more complex. These concepts include the notions of the kernel, channel, padding, stride, pooling (or subsampling) and "drop-out". And back at this August and September, I was so busy and lots of other stuff need my time. I did not succeed in developing my own CNN code. However, later I learned a fascinating tool with a fast-growing user community, the [PyTorch][PyTorchWebSite]. This tool is so good that it is basically designed for CNN application and research. [PyTorch][PyTorchWebSite] even supports GPU acceleration, and it is REALLY easy to shift between GPU and CPU computation.

So let's rock.

[PyTorchWebSite]: http://pytorch.org/

# Handwritten number recognition by the Convolution Neural Network

I take the same task, handwritten number recognition, in my [previous post][PrePost] as the objective for this CNN. The task is to train a CNN on an image set of handwritten numbers and then test the obtained CNN on another set of test images. The training set is a bunch of labeled grayscale images which are similar to the following figure.

<p><img src="{{site.baseurl}}/Resources/Posts/ANN/FCANN/all-ten-numbers.png" alt="<img>All ten numbers."></p>

The training set and testing set are the same as my [previous post][PrePost]. In fact, it is [The MNIST Database][MNISTWebSite].

[MNISTWebSite]: http://yann.lecun.com/exdb/mnist/

I built a CNN with the following features:

* Two 2D convolution layers. The first layer has 1 channel, 24 feature maps, and 5x5 kernel. The second layer has 1 channel, 64 feature maps, and 5x5 kernel. 
* No padding for the convolution layers.
* Two pooling layers. The 2D max subsampling with 2x2 sample is adopted.
* Three fully connected linear layers. The first layer has 240 neural nodes and 168 for the second. For the last layer, there must be 10 nodes since we are trying to recognize the ten handwritten numbers.
* The ReLU function is embraced as the activation function for the first two linear layers.
* The cross-entropy is used as the loss function.
* The updating process is optimized by the Adam method with a learning rate set to be 0.001
* A mini-batch of 100 is used to further stabilize the learning process.
* The learning runs 10 epochs in total.
* GPU acceleration enabled.

The source codes for learning and testing are available on my [GitHub page](https://github.com/huyaoyu/NN).

OK, it sounds good enough, let's see how about the outcomings.

# Training and Testing 

The training is performed on my laptop with has a mobile version of nVIDIA GeForce 740 graphics card. 2GB of GPU memory is available. The training takes about 200MB of that memory.

After the training was done. A test is conducted on the testing set which has 10000 images. The result is 98.86% of correct recognition. Not as superb as expected, but fairly good enough.

# Again, at the Last

I'll try to learn more and experience more on CNN and machine learning. See you guys later.

__Special thanks: My mentor of artificial intelligence and machine learning, [Wenshan Wang](http://wangwenshan.com).__

__The cover image of this post is directly borrowed from the [PyTorch site](http://pytorch.org/tutorials/beginner/blitz/neural_networks_tutorial.html#sphx-glr-beginner-blitz-neural-networks-tutorial-py).__

__This work is conducted when I am at the Robotics Institute of Carnegie Mellon University.__