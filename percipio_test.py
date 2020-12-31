#!/usr/bin/python
# -*- coding: UTF-8 -*-
import platform
from ctypes import *
import ctypes
#import cv2
#import numpy as np



if platform.system().lower() == 'windows':
  lib = cdll.LoadLibrary('percipio_cam.dll')
else:
  lib = cdll.LoadLibrary('libpercipio_cam.so')
trigger = c_bool(0)
depth = c_bool(1)
color = c_bool(1)
#INIT DEVICE
ret=lib.ty_camera_init(trigger, depth, color)
print("sdk init ret = %d" % ret)
#GET DEVICE CNT
cnt=lib.ty_cameraGetCnt()
print("percipio device cnt : %d" % cnt)

RGB_BUFFER_LEN = 10000000 #10MB
DEPTH_BUFFER_LEN=10000000 #10MB
DEPTH_BUFFER_LEN2=10000000 #10MB

color_width = lib.ty_cameraGetRGBImageWidth()
color_height = lib.ty_cameraGetRGBImageHeight()
print("RGB image size :{} {}" .format(color_width, color_height));
depth_width = lib.ty_cameraGetDepthImageWidth()
depth_height = lib.ty_cameraGetDepthImageHeight()
print("Depth image size :{} {}" .format(depth_width, depth_height));
data_rgb = (c_ubyte * RGB_BUFFER_LEN)()
data_rgb2 = (c_ubyte * RGB_BUFFER_LEN)()
data_depth = (c_ushort * DEPTH_BUFFER_LEN)()
data_depth2 = (c_ushort * DEPTH_BUFFER_LEN2)()

FrameIdx=0

#img = np.ones((960, 1280), dtype=np.uint8)

while(True):
    idx = c_int(0)
    lib.ty_camera_fetch_all_image(data_rgb, data_depth, idx)
    lib.ty_cameraMapDepthImage2RGBCoordinate(data_depth, depth_width, depth_height, color_width, color_height, data_depth2, idx)
    lib.ty_cameraGetUndistortRGBImage(data_rgb, color_width, color_height, idx, data_rgb2);

    offset_center = color_width * color_height / 2 + color_width / 2;
    print("RGBD image center info:{} {} {} {}".format(data_rgb2[3*offset_center + 2], data_rgb2[3*offset_center + 1], data_rgb2[3*offset_center + 0], data_depth2[offset_center])); 
    
    FrameIdx+=1

lib.ty_camera_close()
print("Main done!");

