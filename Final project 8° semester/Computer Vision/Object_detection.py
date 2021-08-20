# -*- coding: utf-8 -*-
"""
Created on Wed Apr 14 19:33:57 2021

@author: rodri
"""

import cv2 #Import opencv
import time

thres = 0.45

#cap = cv2.VideoCapture(0)
cap = cv2.VideoCapture('http://192.168.0.6:8080/videofeed')
cap.set(3,1280)
cap.set(4,720)
cap.set(10,70)


#Import label names
classNames = []
classFile = 'coco.names'
with open(classFile,'rt') as f:
    classNames = f.read().rstrip('\n').split('\n')

    
#Set configuration
configPath = "ssd_mobilenet_v3_large_coco_2020_01_14.pbtxt"
weightsPath = "frozen_inference_graph.pb"

#Load the neural network
net = cv2.dnn_DetectionModel(weightsPath,configPath)
net.setInputSize(320,320).setInputScale(1.0/ 127.5).setInputMean((127.5, 127.5, 127.5)).setInputSwapRB(True)

#Loop img
while True:
    t_start = time.time()#process_time()
    left = []
    center = []
    right = []
    success,img = cap.read()
    cv2.line(img, (240,0), (240,480), color=(100,100,0), thickness=1)
    cv2.line(img, (480,0), (480,480), color=(100,100,0), thickness=1)
    classIds, confs, bbox = net.detect(img,confThreshold=thres)
#     print('Class id: {}\n Bbox: {}'.format(classIds,bbox))
    if len(classIds) != 0:
        for classId, confidence,box in zip(classIds.flatten(),confs.flatten(),bbox):
            cv2.rectangle(img,box,color=(0,255,0),thickness=1)
            cv2.putText(img,(classNames[classId-1].upper()+' '+str(round(confidence*100,2))),(box[0]+10,box[1]+30),
                        cv2.FONT_HERSHEY_COMPLEX,1,(255,100,0),2)
            if (box[0]<240):
                left.append(box[0])
            elif (240<=box[0]<480):
                center.append(box[0])
            else:
                right.append(box[0])
#             print('--Objects in left: {} \n Objects in center: {} \n Objects in right: {}'.format(len(left),len(center),len(right)))
    if img is not None:
        cv2.imshow("Output",img)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break
    t_end = time.time()#process_time()
    print('{} FPS'.format(1/(t_end-t_start)))
#     cv2.putText(img,str())
cap.release()

cv2.destroyAllWindows()