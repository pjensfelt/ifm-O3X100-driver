# ifm-O3X100-driver

Driver for connecting to an ifm O3X100 time of flight 3D camera over ethernet for the purpose of triggered image acquisition. The 3D data is stored in a point cloud, but for debugging purposes a scaled gray level image is supplied as well. 

## Prerequisites

From the website

https://www.ifm.com

the windows software ifm vision assistant can be downloaded. Using that program it is possible to control the settings of the camera and to set the camera in triggered mode.

The default ip-address of the camera is 192.168.0.69. It may be necessary to change the ip-settings in ubuntu to manual if the default subnet is one (192.168.1.xxx) and set the ip-address of the host computer to 192.168.0.xxx. Another solution may be to change the ip-address of the camera by using the vision assistant software. The ip-address setting must then be changed in the driver.

OpenCV must be installed to run the simpletest.cpp

## How to run

Clone the repository

```
cd ifm-O3X100-driver
mkdir build
cd build
cmake ..
make
```

There is a testapp that can be run

`./simpletest`

A small OpenCV window should appear in the upper left corner.
