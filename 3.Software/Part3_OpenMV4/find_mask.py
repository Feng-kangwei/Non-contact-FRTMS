# Single Color RGB565 Blob Tracking Example
#
# This example shows off single color RGB565 tracking using the OpenMV Cam.

import sensor, image, time, math,pyb

threshold_index = 2 # 0 for red, 1 for green, 2 for blue

# Color Tracking Thresholds (L Min, L Max, A Min, A Max, B Min, B Max)
# The below thresholds track in general red/green/blue things. You may wish to tune them...
thresholds = [(30, 100, 15, 127, 15, 127), # generic_red_thresholds
              (30, 100, -64, -8, -32, 32), # generic_green_thresholds
          (4, 80, -96, -1, -83, -3)] # generic_blue_thresholds(0, 30, 0, 64, -128, 0)

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time = 2000)
sensor.set_auto_gain(False) # must be turned off for color tracking
sensor.set_auto_whitebal(False) # must be turned off for color tracking
Signal_Mask= pyb.Pin('P1', pyb.Pin.OUT_PP)
Signal_Mask.init(Signal_Mask.OUT_PP, pull=Signal_Mask.PULL_DOWN, af=-1)
Signal_Mask.value(False)
clock = time.clock()
flag=0

# Only blobs that with more pixels than "pixel_threshold" and more area than "area_threshold" are
# returned by "find_blobs" below. Change "pixels_threshold" and "area_threshold" if you change the
# camera resolution. "merge=True" merges all overlapping blobs in the image.

while(True):
    clock.tick()
    img = sensor.snapshot()
    for blob in img.find_blobs([thresholds[threshold_index]],roi=(91,3,130,238) ,pixels_threshold=1792, area_threshold=1800, merge=True):
        flag=1

            # These values depend on the blob not being circular - otherwise they will be shaky.
        img.draw_rectangle(blob.rect(),color=(0,255,0),thickness=3,)
        img.draw_string(blob.x(),blob.y()-20, "MASK", color = (255, 0, 0), scale = 2, mono_space = False,
                                                   char_rotation = 0, char_hmirror = False, char_vflip = False,
                                                   string_rotation = 0, string_hmirror = False, string_vflip = False)
        Signal_Mask.value(True)
        img.draw_cross(blob.cx(), blob.cy())
            # Note - the blob rotation is unique to 0-180 only.
        img.draw_keypoints([(blob.cx(), blob.cy(), int(math.degrees(blob.rotation())))], size=20)
    if(flag==1):
        Signal_Mask.value(True)
    else:
        Signal_Mask.value(False)

    print("fps:%f     Pin_value:%d"%(clock.fps(),Signal_Mask.value()))
    flag=0

