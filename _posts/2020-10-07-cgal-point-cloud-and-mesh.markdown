---
layout: post
title:  "Some usefull CGAL functionalities for working with point clouds"
date:   2020-10-07 00:00:00 -0500
categories: technical
cover_image: "Resources/Posts/CGAL/cgal_front_page_2013.png"
---

# Introduction

Recent work requires lots of processing on point clouds, including sampling, filtering, and surface reconstruction. Most of the codes are composed by using PCL at the beginning. Later, more and more processing involves mesh manipulation. Then I came across [CGAL][CGAL_site]. Never used but I knew its existence. CGAL turns out to be extremely powerful and versatile, however at the same time, hard to understand due to its heavy use of C++ template meta-programming. It causes some headache, and possibly reduces my hair coverage, while I am trying to learn and use CGAL. This blog shows some of my own experiences with CGAL, particularly on aspects related to point cloud processing.

[CGAL_site]: https://www.cgal.org/

The content includes:

- Read and write point cloud with PLY format with point properties
- Read and write surface mesh.
- Surface mesh property map and normal estimation.
- Mesh check and repair.
- Advancing front surface reconstruction.
- Isotropic remeshing.
- Surface mesh hole identification and filling.
- Ray shooting to a surface mesh.
- Shape detection from a point cloud.

CGAL's official site provides detailed documentations including tutorials and user references on every package. I will borrow a lot of content from the official documentation, implementing the code, and adding my own comments on the usage.

I want to acknowledge that the [CGAL mailing][CGAL_mail] list is extremely helpful. The experts there are responsive and active, once you've found yourself in a corner of code and could not make out any sense of it, then the ultimate resort is the CGAL mailing list.

[CGAL_mail]: https://www.cgal.org/mailing_list.html

# Source of sample data

The point clouds and meshes in this blog are copied from the sample files of [Meshlab](https://sourceforge.net/projects/meshlab/). All rights reserved to the original authors.

# Sample code

I prepared a [Github repo][GithubRepo] that holds all the sample codes discussed in this blog. I am working on Ubuntu 18.04 and CGAL 5.1. To compile the code, a system environment variable should be defined as `CGAL_DIR` showing where CGAL is located in the filesystem.

The sample data should be placed in the `Data` sub-directory of the source code. All the data could be downloaded from here.  __Remember to upload the files!__

[GithubRepo]: https://github.com/huyaoyu/CGAL_samples

# Read and write point cloud with PLY format with point properties

The first task is how to get points in and out of CGAL. For me, I usually use the PLY format as my primary point cloud file format since it has the ability to store various per-point properties and we also can save mesh into PLY files. So doing IO with PLY files is the first thing I'd like to discuss.

Here I provide a [sample code][ReadWritePointCloudSample] showing the process.

[ReadWritePointCloudSample]: https://github.com/huyaoyu/CGAL_samples/blob/master/ReadWritePLYPointCloud.cpp

It seems to me that, unlike PCL, CGAL leaves the job of defining the type for 3D point representation to the user and try to be flexible and at the same time. A point cloud in CGAL is nothing but a collection of points, however, we have to define the type for "point". CGAL kernel already provides users the basic point representation being `Kernel_t::Point_3` is the sample code. So we could represent a point cloud as a `std::vector<Kernel_t::Point_3>`. Normally, a point cloud also has additional properties other than the coordinates of points, e.g. point normal and color. For those additional properties, CGAL expects us to just define a `std::tuple` and put any property we want in the tuple, then make a container of this tuple type as the point cloud.

As for normal vectors, we could use `Kernel_t::Vecotr_3`. For color, however, depending on how many color channels you want, you could use `std::array< unsigned char, N >` where `N` could be 3 or 4 (an additional alpha channel).

For 3D points and normals, CGAL provides helper functions `CGAL::make_ply_point_reader()` and `CGAL::make_ply_normal_reader()` for reading and similar helpers for writing. For a `std::array` represented color, however, we have to be more specific on how CGAL is supposed to read and write data. Specifically, during the call to `CGAL::read_ply_points_with_properties()`, we will use helper function `CGAL::Construct_array()`. For writing the color property, we have to implement `CGAL::Output_rep` class so that CGAL knows how to deal with the color type. Please refer to the [sample code][ReadWritePointCloudSample] for more details.

The sample code reads a point cloud in PLY format and writes it back to the filesystem as it is. So we could load the files into point cloud visualization software such as [CloudCompare](https://www.danielgm.net/cc/) to check if we are reading and writing the point clouds properly. I noticed that the size of the output PLY file is larger than the input. This might have something to do with the underlying CGAL kernel we are using and the kernel is using a double-precision floating-point type. The following is a sample point cloud, `Data/PointClouds/TrimStarInit0.5_NormalColor.ply`.

<img src="{{site.baseurl}}/Resources/Posts/CGAL/ReadWritePointCloud/output_point_cloud.png" alt="The point cloud to be read and written. " width="400px"/>

To test with the sample point cloud shown above, we could use the following commands.

{% highlight shell %}
<path to executable>/ReadWritePLYPointCloud \
	<path to repo>/Data/PointClouds/TrimStarInit0.5_NormalColor.ply \
	<output filename>
{% endhighlight %}

# Read and write surface mesh in PLY format

Read and write surface meshes in PLY format is made really simple in CGAL, the [Surface Mesh](https://doc.cgal.org/latest/Surface_mesh/index.html) package provides the dedicated APIs for these purposes, namely, the `CGAL::read_ply()` and `CGAL::write_ply()` functions.

The [sample code][ReadWritePLYMeshSample] for using these two functions is [here][ReadWritePLYMeshSample].

[ReadWritePLYMeshSample]: https://github.com/huyaoyu/CGAL_samples/blob/master/ReadWritePLYMesh.cpp

If we put a sample mesh, `T_d.ply`, in `Data/Meshes`, then we can invoke the sample program by 

{% highlight shell %}
<path to executable>/ReadWritePLYMesh \
	<path to repo>/Data/Meshes/T_d.ply \
	<output filename>
{% endhighlight %}

This mesh is one of the sample meshes of Meshlab. It is basically a set of blocks in a T-shape layout. To illustrate some interesting usage of CGAL, I modified the original mesh by adding a new vertex property called "d" with `float` property type. During the reading process, CGAL handles this new vertex property automatically for us and a property map with the name `v:d` will be created. In the [sample code][ReadWritePLYMeshSample], we try to get access to property map `v:d` and print out all the values. The values start from 100 and end at 119 (20 vertices in total).

CGAL handle entities such as points, vertex normals, face normals as property maps. The actual 3D points are saved as a vertex property map called `v:point`. CGAL has conventional names for frequently used properties such as `v:normal` and `f:normal` with `f:` being a face property map. So as been said, if we want to access the point of a vertex, it is normally done through the `v:point` property map. From my experience, CGAL provides no method to directly modify the coordinates of a point (the `Kernel_t::Point_3` in the sample code). If you really want to do that, make a new `Kernel_t::Point_3` and overwrite the original object in the `v:point` property map. I add 6 to all the y-coordinates of the original mesh vertices.

The [sample code][ReadWritePLYMeshSample] writes the modified mesh to the filesystem in both ASCII and binary format. If we load the mesh into Meshlab, we will see something like the following, with the green mesh being the modified mesh (the color is assigned in Meshlab). To write a binary PLY file, it seems that we also have to use the `CGAL::set_binary_mode()` method. Please refer to the [sample code][ReadWritePLYMeshSample] for details.

<img src="{{site.baseurl}}/Resources/Posts/CGAL/ReadWritePLYMesh/shifted-blocks.png" alt="The original and modified (green) meshes. " width="400px"/>

One additional thing we could observe from the result ASCII PLY file is that CGAL does use double-precision floating-point numbers.