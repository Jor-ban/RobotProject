import cv2 
import numpy as np
import imutils
import matplotlib.pyplot as plt
import time 
import sys
import math
    

class LaneDetection:
    
    def __init__(self, width, height):
        self.w = width
        self.h = height
    
    def calculate(self):
        global direction 
        cap = cv2.VideoCapture("./Videos/lane_detection.mp4") 
        fps = cap.get(cv2.CAP_PROP_FPS) 
        
        while cap.isOpened(): 
            start_time = time.time()
            ret, frame = cap.read()

            if ret:
                try: 
                    frame = cv2.resize(frame, (self.w, self.h))
                    frame = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
                    frame[:,:,2] -= 20
                    frame = cv2.cvtColor(frame, cv2.COLOR_HSV2BGR)
                    extract = extract_color(frame)
                    
                    noise = denoise_image(extract)
                    
                    segment = ROI(noise)
                    
                    hough = hough_line_detect(segment)
                    lines = measure_line(frame, hough)
    
                    frame = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
                    frame[:,:,2] += 20
                    frame = cv2.cvtColor(frame, cv2.COLOR_HSV2BGR)

                    direction = get_direction(lines)                
                    
                    lines_visualize = visualize_lines(frame, lines)
    
                    output = cv2.addWeighted(frame, 1, lines_visualize, 1, 1)
                except:
                    output = frame
            else:            
                break
        
            end_time = time.time()
            text = "Radius of curvature {} m".format(direction)
            
            org = (10, 25)                                   
            output = cv2.putText(output, text, org, cv2.FONT_HERSHEY_PLAIN, 1, 
                                (0, 0, 0), 2)
            
            text = "FPS: {}".format(int(1/(end_time - start_time+0.001)))
            org = (10, 50)
            
            output = cv2.putText(output, text, org, cv2.FONT_HERSHEY_PLAIN, 1, 
                                (0, 0, 0), 2)
    
            cv2.imshow("result1.mp4", output)
            if cv2.waitKey(1) & 0xFF == ord('q'):
                break


def hough_line_detect(segment):
    hough = cv2.HoughLinesP(segment, 1, np.pi / 180, 25, np.array([]), 
                    minLineLength = 25, maxLineGap = 50)
    print(hough)              
    return hough
    

def extract_color(frame):    
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    lower_white = np.array([0, 0, 100], dtype=np.uint8) 
    upper_white = np.array([255, 255, 255], dtype=np.uint8) 
    mask_white = cv2.inRange(hsv, lower_white, upper_white)
    
    lower_yellow = np.array([20, 100, 100], dtype=np.uint8) 
    upper_yellow = np.array([30, 255, 255], dtype=np.uint8)     
    mask_yellow = cv2.inRange(hsv, lower_yellow, upper_yellow)
    
    mask = cv2.bitwise_or(mask_white, mask_yellow)
    frame = cv2.bitwise_and(frame, frame, mask=mask)
    cv2.imshow("colors extracted", frame)
    print(frame)
    return frame


def denoise_image(frame):
    gray = cv2.cvtColor(frame, cv2.COLOR_RGB2GRAY)
    blur = cv2.GaussianBlur(gray, (3, 3), 0)       
    
    cv2.imshow('Blurred', blur)
    print(blur)
    return blur


def ROI(frame):
    
    height = frame.shape[0] 
    width = frame.shape[1] 
    
    polygons = np.array([
                        [(5 * width // 16, height), 
                         (6 * width // 16, 7 * height // 9),  
                         (10 * width // 16, 7 * height // 9), 
                         (14 * width // 16, height)] 
                        ])
    
    mask = np.zeros_like(frame)
    cv2.fillPoly(mask, polygons, 255)
    segment = cv2.bitwise_and(frame, mask)

    cv2.imshow("Roi segment", segment)
    print(segment)
    return segment

def measure_line(frame, lines):
    global prev_left_avg, prev_right_avg 
    if lines is None:
        return frame
    
    left, right = [], []   

    for line in lines:
        x1, y1, x2, y2 = line.reshape(4)
        parameters = np.polyfit((x1, x2), (y1, y2), 1)        
        slope = parameters[0]
        y_intercept = parameters[1]
        if slope < 0:
            left.append((slope, y_intercept)) 
        else:
            right.append((slope, y_intercept))
            
    left_avg = np.average(left, axis = 0) 
    right_avg = np.average(right, axis = 0) 

    if type(left_avg) == np.ndarray:
        prev_left_avg = left_avg
    else: 
        left_avg = prev_left_avg

    if type(right_avg) == np.ndarray:
        prev_right_avg = right_avg
    else: 
        right_avg = prev_right_avg
        
    left_line = coordinates(frame, left_avg)
    right_line = coordinates(frame, right_avg)

    return np.array([left_line, right_line])

def coordinates(frame, line_params):
    try:
        slope, intercept = line_params
    except:
        slope, intercept = 0, 0
    if slope == 0:
        y1 = frame.shape[0]
        y2 = frame.shape[0]
        x1 = frame.shape[1] / 2
        x2 = frame.shape[1] / 2
    else: 
        y1 = frame.shape[0]
        y2 = int(y1 - frame.shape[0]*0.2)
        x1 = int((y1 - intercept) / slope)
        x2 = int((y2 - intercept) / slope)
    
    return np.array([x1, y1, x2, y2])


def visualize_lines(frame, lines):
    lines_visualize = np.zeros_like(frame)
    f_points = []
        
    mask = np.zeros_like(frame)
    
    if lines is None:
        return frame
    
    for x1, y1, x2, y2 in lines:
        cv2.line(lines_visualize, (x1, y1), (x2, y2), (0, 255, 0), 5)        
        f_points.append((x2, y2)) 
        f_points.append((x1, y1))

    points = []
    points.append(f_points[0]) 
    points.append(f_points[1])
    points.append(f_points[3])
    points.append(f_points[2])

    cv2.fillPoly(mask, np.array([points]), (0, 0, 255))
    
    return cv2.bitwise_or(lines_visualize, mask)

def get_direction(lines):
    global direction 
    radius = 0
    slopes = []

    if lines is None:
        return direction
    if len(lines) > 2:
        return direction
    for x1, y1, x2, y2 in lines.reshape(2, 4):
        slopes.append(np.arctan((y2-y1) / (x2-x1)) * 180 / np.pi)

    if slopes[0] < -55 and slopes[1] < 48:
        direction = "left {:.2f}".format(abs(slopes[0]))
        
    elif slopes[0] > -50 and slopes[1] > 48:
        direction = "right {:.2f}".format(abs(slopes[1]))
        
    else:        
        direction = "{:.2f}".format(abs(abs(slopes[0]) - abs(slopes[1])))
    print(direction)
    return direction

lane = LaneDetection(width=640, height=480)
lane.calculate()