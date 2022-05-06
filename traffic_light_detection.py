
import warnings
warnings.filterwarnings("ignore")
from IPython.display import clear_output
import datetime
from time import time
import pandas as pd
import os
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns
from tqdm import tqdm
import cv2
from sklearn.model_selection import train_test_split, GroupKFold, StratifiedKFold
import albumentations as A
from albumentations.pytorch.transforms import ToTensorV2
import torch
import torchvision
from torchvision import transforms
from torchvision.models.detection.faster_rcnn import FastRCNNPredictor
from torchvision.models.detection import FasterRCNN
from torchvision.models.detection.rpn import AnchorGenerator
from torchvision.ops import nms
plt.style.use('fivethirtyeight')
from torch.utils.data import DataLoader, Dataset
from torch.utils.data.sampler import SequentialSampler


DATA_PATH = './Videos'

cap = cv2.VideoCapture(os.path.join(DATA_PATH,'traffic_and_sign_recognition_low.mp4'))

ret, frame = cap.read()

frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
    
plt.axis("off")

device = torch.device('cuda') if torch.cuda.is_available() else torch.device('cpu')

model = torchvision.models.detection.fasterrcnn_resnet50_fpn(pretrained=True)

N_CLASS = 4  

INP_FEATURES = model.roi_heads.box_predictor.cls_score.in_features

model.roi_heads.box_predictor = FastRCNNPredictor(INP_FEATURES, N_CLASS)

model.to(device)

model.load_state_dict(torch.load(os.path.join('./', 'fasterrcnn_resnet50_fpn.pth'),
                                 map_location=device))

def filterBoxes(output,nms_th=0.3,score_threshold=0.5):
    
    boxes = output['boxes']
    scores = output['scores']
    labels = output['labels']
    
    mask = nms(boxes,scores,nms_th)
    
    boxes = boxes[mask]
    scores = scores[mask]
    labels = labels[mask]
    
    boxes = boxes.data.cpu().numpy().astype(np.int32)
    scores = scores.data.cpu().numpy()
    labels = labels.data.cpu().numpy()
    
    mask = scores >= score_threshold
    boxes = boxes[mask]
    scores = scores[mask]
    labels = labels[mask]
    
    return boxes, scores, labels


def displayPredictions(image,output,nms_th=0.3,score_threshold=0.5):
    
    boxes,scores,labels = filterBoxes(output,nms_th,score_threshold)
    
    fig, ax = plt.subplots(1, 1, figsize=(16, 8))

    colors = {1:(0,255,0), 2:(255,255,0), 3:(255,0,0)}
    
    for box,label in zip(boxes,labels):
        image = cv2.rectangle(image,
                      (box[0], box[1]),
                      (box[2], box[3]),
                      colors[label], 2)
      
    return image

def preprocessImage(frame):
    
    frame = cv2.resize(frame,(512,512))
    
    frame_ = frame.astype(np.float32)/255.0
    frame_ = torch.as_tensor([frame_]).to(device).permute(0,3,1,2)
    
    return frame_, frame

def detectTrafficLight(frame,nms_th=0.2,score_th=0.5,model=model):
    
    model.eval()
    
    frame_, frame = preprocessImage(frame)
    
    output = model(frame_)[0]
    
    pred = displayPredictions(frame,output,nms_th,score_th)
    
    return pred


#Model evaluation 
pred = detectTrafficLight(frame,0.2,0.0)

plt.axis("off")
plt.imshow(pred)

plt.show()
frame_width = int(cap.get(3))
frame_height = int(cap.get(4))
fourcc = cv2.VideoWriter_fourcc(*'MP4V')


preprocess = transforms.Compose([
    transforms.ToTensor()
])

result = cv2.VideoWriter(os.path.join(DATA_PATH,'result-1.mp4'),
                         cv2.VideoWriter_fourcc(*"FMP4"), 10, (512,512), True)

cap = cv2.VideoCapture(os.path.join(DATA_PATH,'traffic_and_sign_recognition_low.mp4'))
c = 10
while c > 0:#cap.isOpened():
    
    ret, frame = cap.read()

    if not ret:
      break
    frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
    
    pred = detectTrafficLight(frame,0.2,0.0)
    pred = pred.astype(np.uint8)
    pred = cv2.cvtColor(pred, cv2.COLOR_BGR2RGB)
    print(c)
    c -= 1
    result.write(pred)
    
result.release()
cap.release()
    
    
    # ret, frame = cap.read()

    # if not ret:
    #   break
    # frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
    
    # pred = detectTrafficLight(frame,0.2,0.0)
    # pred = pred.astype(np.uint8)
    # nframe = cv2.cvtColor(pred, cv2.COLOR_BGR2RGB)
    # pred = cv2.cvtColor(pred, cv2.COLOR_BGR2RGB)
    # cv2.imshow('frame', nframe)
    # key = cv2.waitKey(100)
    
    # if key == ord('q'):
    #     break
    
    # result.write(pred)
    
# result.release()
# cap.release()
    
  

