---
layout: post
title:  "Deep-Assisted High Resolution Binocular Stereo Depth Reconstruction"
date:   2019-09-08 15:10:15 -0400
categories: technical
cover_image: "Resources/Posts/Robotics/DeepAssistedStereo/CoverFigure_Scaled.png"
tag: mechanical
---

# Deep-Assisted High Resolution Binocular Stereo Depth Reconstruction #

Authors: Yaoyu Hu, Weikun Zhen, and Sebastian Scherer.

<p><img src="{{site.baseurl}}/Resources/Posts/Robotics/DeepAssistedStereo/CoverFigure_Scaled.png" alt="<img>Cover figure." width="500px"><br>
</p>

This is the project page of __Deep-Assisted High Resolution Binocular Stereo Depth Reconstruction__.

Here we list the comparisons between PSMNU, SGBM, SGBMUR, and SGBMP on Middleburry Evaluation V3 with the training cases in their full resolution.

## Middlebury dataset
In the following materials, the abbreviation GTC means Ground Truth Coverage, it is the percentage of pixels which have un-occluded true disparities. Middlebury dataset does not provide disparities for all of the pixels in the left image. Some pixels are occluded and some pixels have Inf. disparities. GTCs are different across the cases. All pixels that are NOT covered by the ground truth will NOT participate in the calculation of _Bad1.0_, _invalid_, _avgErr_, and _stdErr_.

The disparity ranges are determined by looking at the true disparity first. Then a total number of disparity dividable by 8 is chosen. Disparity ranges are converted to _minDisparity_ and _numDisparity_ for SGBM, SGBMUR, and SGBMP. All of the true disparities are confined by the disparity ranges.

SGBM has the lowest _Bad1.0_ among all the cases. This is because SGBM invalidates lots of pixels and only keeps the pixels which it has 'confident' about. The same reason leads to SGMB's high _invalid_ value.

To compare the performance, we have to consider all the metrics together rather than comparing only one of them. The goal of SGBMP is achieving low _avgErr_ and low _invalid_ at the same time and do its job on our 4K stereo images. Due to the intrinsic difficultis in the hard regions for stereo reconstruction, disparity predictions by SGBMP in SGBM's _invalid_ regions contains additional noise, making the _Bad1.0_ value higher than SGBM.  The increased _Bad1.0_ is the cost SGBMP paied to have lower _invalid_. However, SGBMP manages to achieve lower _avgErr_, lower _invalid_, and lower _stdErr_ for most of the cases, meaning it does perform better than SGBM.

The 12 cases on which SGBMP has lowest _stdErr_ are: Adirondack, ArtL, Jadeplant\*, Motorcycle\*, MotorcycleE, PianoL, Pipes, Playroom, Playtable, PlaytableP, Shelves, and Teddy. The cases marked by * are those SGBMP did not get the lowest _avgErr_.

### Adirondack, GTC 91%, disparity range 1-304 pixels

| Method | Bad1.0 | invalid | avgErr | stdErr |
|--------|------------|---------|--------|--------|
| PSMNU  | __20.92__  | (7.52)  | 1.18   | 4.81   |
| SGBM   | 21.59      | 31.60   | 9.54   | 30.54  |
| SGBMUR | 32.40      | 18.85   | 14.17  | 37.71  |
| SGBMP  | 24.82      | __8.47__| __1.10__| __3.61__ |

<p><img src="{{site.baseurl}}/Resources/Posts/Robotics/DeepAssistedStereo/M00_Adirondack.png" alt="<img>Adirondack." width="750px"><br>
Adirondack, true disparity, PSMNU (up-sampled), SGBM, SGBMUR, SGBMP.
</p>

### ArtL, GTC 76%, disparity range 1-256 pixels

| Method | Bad1.0  | invalid | avgErr | stdErr |
|--------|------------|---------|--------|--------|
| PSMNU  | 14.77      | (9.25)  | 2.94   | 10.34  |
| SGBM   | __9.05__   | 25.00   | 4.50   | 17.89  |
| SGBMUR | 12.87      | 18.96   | 6.92   | 24.18  |
| SGBMP  | 11.52      | __13.44__ | __1.78__ | __7.63__ |

<p><img src="{{site.baseurl}}/Resources/Posts/Robotics/DeepAssistedStereo/M01_ArtL.png" alt="<img>ArtL." width="750px"><br>
ArtL, true disparity, PSMNU (up-sampled), SGBM, SGBMUR, SGBMP.
</p>

### Jadeplant, GTC 74.3%, disparity range 1-640 pixels

| Method | Bad1.0  | invalid | avgErr | stdErr |
|--------|------------|---------|--------|--------|
| PSMNU  | 35.09      | (19.35) | 18.29  | 54.90  |
| SGBM   | __13.90__  | 34.26   | __7.54__ | 37.45  |
| SGBMUR | 19.61      | __27.29__ | 11.62  | 46.53  |
| SGBMP  | 26.11      | 28.23   | 7.84   | __35.74__ |

<p><img src="{{site.baseurl}}/Resources/Posts/Robotics/DeepAssistedStereo/M02_Jadeplant.png" alt="<img>Jadeplant." width="750px"><br>
Jadeplant, true disparity, PSMNU (up-sampled), SGBM, SGBMUR, SGBMP.
</p>

### Motorcycle, GTC 86%, disparity range 1-288 pixels

| Method | Bad1.0  | invalid | avgErr | stdErr |
|--------|------------|---------|--------|--------|
| PSMNU  | 29.11      | (6.28)    | 2.95   | 11.64  |
| SGBM   | __17.68__  | 13.81   | __1.95__ | 9.91   |
| SGBMUR | 20.21      | 9.24    | 2.35   | 11.30  |
| SGBMP  | 27.65      | __8.44__ | 2.09   | __8.69__ |

<p><img src="{{site.baseurl}}/Resources/Posts/Robotics/DeepAssistedStereo/M03_Motorcycle.png" alt="<img>Motorcycle." width="750px"><br>
Motorcycle, true disparity, PSMNU (up-sampled), SGBM, SGBMUR, SGBMP.
</p>

### MotorcycleE, GTC 86%, disparity range 1-288 pixels

| Method | Bad1.0  | invalid | avgErr | stdErr |
|--------|------------|---------|--------|--------|
| PSMNU  | 30.99      | (6.28)    | 3.20   | 12.21  |
| SGBM   | __17.28__  | 31.93   | 8.38   | 29.30  |
| SGBMUR | 23.83      | 21.40   | 13.16  | 37.46  |
| SGBMP  | 34.59      | __8.93__ | __2.30__ | __8.62__ |

<p><img src="{{site.baseurl}}/Resources/Posts/Robotics/DeepAssistedStereo/M04_MotorcycleE.png" alt="<img>MotorcycleE." width="750px"><br>
MotorcycleE, true disparity, PSMNU (up-sampled), SGBM, SGBMUR, SGBMP.
</p>

### Piano, GTC 86%, disparity range 1-272 pixels

| Method | Bad1.0  | invalid | avgErr | stdErr |
|--------|------------|---------|--------|--------|
| PSMNU  | 30.75      | (4.49)  | 3.09   | 12.99  |
| SGBM   | __25.78__  | 19.59   | 4.09   | __9.51__ |
| SGBMUR | 34.30      | 9.23    | 5.31   | 11.27  |
| SGBMP  | 35.90      | __5.21__ | __3.04__ | 12.66  |

<p><img src="{{site.baseurl}}/Resources/Posts/Robotics/DeepAssistedStereo/M05_Piano.png" alt="<img>Piano." width="750px"><br>
Piano, true disparity, PSMNU (up-sampled), SGBM, SGBMUR, SGBMP.
</p>

### PianoL, GTC 86%, disparity range 1-272 pixels

| Method | Bad1.0  | invalid | avgErr | stdErr |
|--------|------------|---------|--------|--------|
| PSMNU  | 59.32      | (4.49)  | 14.56  | 29.67  |
| SGBM   | __21.44__  | 44.60   | 25.01  | 52.33  |
| SGBMUR | 35.69      | 28.18   | 34.76  | 57.11  |
| SGBMP  | 55.32      | __10.87__ | __11.92__ | __26.49__ |

<p><img src="{{site.baseurl}}/Resources/Posts/Robotics/DeepAssistedStereo/M06_PianoL.png" alt="<img>PianoL." width="750px"><br>
PianoL, true disparity, PSMNU (up-sampled), SGBM, SGBMUR, SGBMP.
</p>

### Pipes, GTC 78.6%, disparity range 1-304 pixels

| Method | Bad1.0  | invalid | avgErr | stdErr |
|--------|------------|---------|--------|--------|
| PSMNU  | 27.92      | (5.23)  | 3.75   | 13.81  |
| SGBM   | __13.46__  | 13.94   | 3.43   | 16.71  |
| SGBMUR | 16.80      | 9.45    | 4.26   | 18.41  |
| SGBMP  | 21.54      | __8.37__ | __2.44__ | __10.84__ |

<p><img src="{{site.baseurl}}/Resources/Posts/Robotics/DeepAssistedStereo/M07_Pipes.png" alt="<img>Pipes." width="750px"><br>
Pipes, true disparity, PSMNU (up-sampled), SGBM, SGBMUR, SGBMP.
</p>

### Playroom, GTC 79.9%, disparity range 1-336 pixels

| Method | Bad1.0  | invalid | avgErr | stdErr |
|--------|------------|---------|--------|--------|
| PSMNU  | 42.08      | (4.55)  | 4.03   | 12.46  |
| SGBM   | __23.62__  | 31.89   | 5.05   | 16.27  |
| SGBMUR | 36.41      | 16.17   | 7.42   | 20.28  |
| SGBMP  | 42.56      | __6.87__ | __3.33__ | __10.48__ |

<p><img src="{{site.baseurl}}/Resources/Posts/Robotics/DeepAssistedStereo/M08_Playroom.png" alt="<img>Pipes." width="750px"><br>
Playroom, true disparity, PSMNU (up-sampled), SGBM, SGBMUR, SGBMP.
</p>

### Playtable, GTC 86%, disparity range 1-304 pixels

| Method | Bad1.0  | invalid | avgErr | stdErr |
|--------|------------|---------|--------|--------|
| PSMNU  | 50.96      | (6.54)  | 13.72  | 31.52  |
| SGBM   | __31.39__  | 28.35   | 10.52  | 28.23  |
| SGBMUR | 41.49      | 15.61   | 10.85  | 27.61  |
| SGBMP  | 42.41      | __10.85__ | __7.13__ | __19.99__ |

<p><img src="{{site.baseurl}}/Resources/Posts/Robotics/DeepAssistedStereo/M09_Playtable.png" alt="<img>Playtable." width="750px"><br>
Playtable, true disparity, PSMNU (up-sampled), SGBM, SGBMUR, SGBMP.
</p>

### PlaytableP, GTC 86.2, disparity range 1-304 pixels

| Method | Bad1.0  | invalid | avgErr | stdErr |
|--------|------------|---------|--------|--------|
| PSMNU  | 38.00      | (6.53)  | 7.83   | 22.77  |
| SGBM   | __17.46__  | 20.79   | 2.15   | 7.30   |
| SGBMUR | 25.57      | 10.75   | 2.78   | 8.31   |
| SGBMP  | 25.01      | __8.65__ | __1.68__ | __5.19__ |

<p><img src="{{site.baseurl}}/Resources/Posts/Robotics/DeepAssistedStereo/M10_PlaytableP.png" alt="<img>PlaytableP." width="750px"><br>
PlaytableP, true disparity, PSMNU (up-sampled), SGBM, SGBMUR, SGBMP.
</p>

### Recycle, GTC 90.4%, disparity range 1-272 pixels

| Method | Bad1.0  | invalid | avgErr | stdErr |
|--------|------------|---------|--------|--------|
| PSMNU  | 29.38      | (7.49)  | 2.51   | 10.01  |
| SGBM   | __26.97__  | 25.73   | __2.31__ | __8.33__ |
| SGBMUR | 36.58      | 13.40   | 3.54   | 11.73  |
| SGBMP  | 37.72      | __8.57__ | 2.43   | 9.06   |

<p><img src="{{site.baseurl}}/Resources/Posts/Robotics/DeepAssistedStereo/M11_Recycle.png" alt="<img>Recycle." width="750px"><br>
Recycle, true disparity, PSMNU (up-sampled), SGBM, SGBMUR, SGBMP.
</p>

### Shelves, GTC 82.8%, disparity range 1-240 pixels

| Method | Bad1.0  | invalid | avgErr | stdErr |
|--------|------------|---------|--------|--------|
| PSMNU  | 61.72      | (5.94)  | 4.86   | 12.34  |
| SGBM   | __25.96__  | 35.95   | 5.80   | 14.97  |
| SGBMUR | 42.90      | 17.69   | 8.31   | 17.01  |
| SGBMP  | 50.12      | __6.91__ | __4.00__ | __9.20__ |

<p><img src="{{site.baseurl}}/Resources/Posts/Robotics/DeepAssistedStereo/M12_Shelves.png" alt="<img>Shelves." width="750px"><br>
Shelves, true disparity, PSMNU (up-sampled), SGBM, SGBMUR, SGBMP.
</p>

### Teddy, GTC 86.8%, disparity range 1-256 pixels

| Method | Bad1.0  | invalid | avgErr | stdErr |
|--------|------------|---------|--------|--------|
| PSMNU  | 9.78       | (7.97)  | 1.19   | 4.17   |
| SGBM   | __7.94__   | 13.51   | 1.38   | 8.37   |
| SGBMUR | 9.92       | 10.51   | 1.82   | 10.83  |
| SGBMP  | 10.85      | __8.74__ | __1.12__ | __3.49__ |

<p><img src="{{site.baseurl}}/Resources/Posts/Robotics/DeepAssistedStereo/M13_Teddy.png" alt="<img>Teddy." width="750px"><br>
Teddy, true disparity, PSMNU (up-sampled), SGBM, SGBMUR, SGBMP.
</p>

### Vintage, GTC 88.2%, disparity range 1-768 pixels

| Method | Bad1.0  | invalid | avgErr | stdErr |
|--------|------------|---------|--------|--------|
| PSMNU  | 37.54      | (23.09) | 8.33   | 27.43  |
| SGBM   | __23.57__  | 45.02   | __5.72__ | __14.16__ |
| SGBMUR | 36.36      | 29.49   | 7.91   | 21.57  |
| SGBMP  | 33.88      | __24.90__ | 6.48   | 20.49  |

<p><img src="{{site.baseurl}}/Resources/Posts/Robotics/DeepAssistedStereo/M14_Vintage.png" alt="<img>Vintage." width="750px"><br>
Vintage, true disparity, PSMNU (up-sampled), SGBM, SGBMUR, SGBMP.
</p>
