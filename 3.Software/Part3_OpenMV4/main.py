import sensor, image, time, math,pyb
threshold_index = 2
thresholds = [(30, 100, 15, 127, 15, 127),
			  (30, 100, -64, -8, -32, 32),
		  (4, 80, -96, -1, -83, -3)]
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time = 2000)
sensor.set_auto_gain(False)
sensor.set_auto_whitebal(False)
Signal_Mask= pyb.Pin('P1', pyb.Pin.OUT_PP)
Signal_Mask.init(Signal_Mask.OUT_PP, pull=Signal_Mask.PULL_DOWN, af=-1)
Signal_Mask.value(False)
clock = time.clock()
flag=0
while(True):
	clock.tick()
	img = sensor.snapshot()
	for blob in img.find_blobs([thresholds[threshold_index]],roi=(91,3,130,238) ,pixels_threshold=1692, area_threshold=1800, merge=True):
		flag=1
		img.draw_rectangle(blob.rect(),color=(0,255,0),thickness=3,)
		img.draw_string(blob.x(),blob.y()-20, "MASK", color = (255, 0, 0), scale = 2, mono_space = False,
												   char_rotation = 0, char_hmirror = False, char_vflip = False,
												  string_rotation = 0, string_hmirror = False, string_vflip = False)
		img.draw_cross(blob.cx(), blob.cy())
		img.draw_keypoints([(blob.cx(), blob.cy(), int(math.degrees(blob.rotation())))], size=20)
	if(flag==1):
		Signal_Mask.value(True)
		pyb.LED(1).on()
	else:
		Signal_Mask.value(False)
		pyb.LED(1).off()
	print("fps:%f	 Pin_value:%d"%(clock.fps(),Signal_Mask.value()))
	flag=0