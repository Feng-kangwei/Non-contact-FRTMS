import sensor, image, time,pyb


RED_LED_PIN=1
BLUE_LED_PIN=3

sensor.reset()
sensor.set_pixformat(sensor.GRAYSCALE)
sensor.set_framesize(sensor.B128X128)
sensor.set_windowing((92,112))
sensor.skip_frames(10)
sensor.skip_frames(time = 5000)


#图像库一共是三个人 每个库中20张照片
NUM_SUBJECTS=4
NUM_SUBJECTS_IMGS=20

img = sensor.snapshot()


name=["fkw","wxy","zzl","ADDER"]

img=None
pmin=999999
num=0
clock = time.clock()

def min(pmin,a,s):
    global num
    if a<pmin:
        pmin=a
        num=s
    return pmin

while(True):
    clock.tick()
    img = sensor.snapshot()
    d0=img.find_lbp((0,0,img.width(),img.height()))
    for s in range(1,NUM_SUBJECTS+1):
        dist=0
        for i in range(2,NUM_SUBJECTS_IMGS+1):
            img1=image.Image("TEST1/s%d/%d.pgm"%(s,i))
            d1=img1.find_lbp((0,0,img1.width(),img1.height()))
            dist += image.match_descriptor(d0,d1)
        print("Averge dist for subject %d: %d"%(s,dist/NUM_SUBJECTS_IMGS))
        pmin= min(pmin,dist/NUM_SUBJECTS_IMGS,s)
        print(pmin)
    print(name[num-1])
    pmin=999999 #将pmin重新初始化
    img.draw_string(img1.width()-20,img1.height()-20, "%s"%name[num-1], color = (255, 255, 255), scale = 1, mono_space = False,
                char_rotation = 0, char_hmirror = False, char_vflip = False,
                string_rotation = 0, string_hmirror = False, string_vflip = False)
