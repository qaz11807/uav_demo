import numpy as np
import json
import time
import datetime
import csv
from numpy import pi, cos, sin, sqrt , arcsin , linalg

# x1,x2,x3為定位點 x4為轉移點
def transfer(x1,x2,x3,x4,xx1,xx2,xx3):

    def caculate_four(x4,old,new):
            
        mid=new-old
        A_=np.array([[1,0,x4[0],-x4[1]],
                        [0,1,x4[1],x4[0]]])

        old_=np.array([[x4[0]],
                        [x4[1]]])

        xxx=linalg.lstsq(A,mid,rcond=-1)

        v=np.dot(A_,xxx[0])
        new_=old_+v
        return new_

    A=np.array([[1.0,0,x1[0],-x1[1]],
                [0,1.0,x1[1], x1[0]],
                [1.0,0,x2[0],-x2[1]],
                [0,1.0,x2[1], x2[0]],
                [1.0,0,x3[0],-x3[1]],
                [0,1.0,x3[1], x3[0]]])

    old=np.array([[x1[0]],
                [x1[1]],
                [x2[0]],
                [x2[1]],
                [x3[0]],
                [x3[1]]])
    
    new=np.array([[xx1[0]],
                    [xx1[1]],
                    [xx2[0]],
                    [xx2[1]],
                    [xx3[0]],
                    [xx3[1]]])
    
    return caculate_four(x4,old,new)

_np = [25.012195907407403, 121.54163157407409]
_wp = [25.01170942063492, 121.54104515079362]
_ep = [25.01208321111112, 121.54175562222221]
_sp = [25.011606166666663, 121.54115716666666]

x1 = _sp  
xx1 = [-806,163]
_xx1 = [806,163]
x2 = _np
xx2 = [0,0]
x3 = _ep
xx3 = [0,163]

fileName = "blackbox"

f = open('./input/20210808_1500/circle/record_0144/'+fileName+'.json',)
  
data = json.load(f)

with open('./output/'+fileName+'.csv', 'w', newline='') as csvfile:

    writer = csv.writer(csvfile)

    hPa_offset = 0

    xrecord = []
    yrecord = []
    zrecord = []

    speed = []
    times = []

    hPa_offset = 1002.5702305652511 #用最大氣壓值當基準點

    for i,record in enumerate(data['GPS']):
        state = record['state']
        if hPa_offset == 0 :
            hPa_offset = data['BME'][i]['pression'] 
            #hPa_offset = record['alt']
        if state == 'OK' :
            position = record['position']
            xrecord.append(position[0])
            yrecord.append(position[1])
            convert = np.array(transfer(x1,x2,x3,position,xx1,xx2,xx3)).flatten()
            convert[0] = abs(convert[0]) 
            convert[1] = abs(convert[1]) + 0
            hPa = data['BME'][i]['pression'] - hPa_offset 
            zrecord.append(data['BME'][i]['pression'])
            z  = -( ( hPa * 100 ) * 9 / 10 ) 
            #z = (record['alt'] - hPa_offset )*10
            convert = np.append(convert, z)
            convert = list(map(lambda coord : int(coord) , convert))
            print(position ," => " ,  convert )
            writer.writerow(convert)

    print("Max X : ", max(xrecord),"Min X : ", min(xrecord))
    print("Max Y : ", max(yrecord),"Max Y : ", min(yrecord))
    print("Max Z : ", max(zrecord),"Min Z : ", min(zrecord))


# diff = end_time - start_time
# elapsed_time = int((diff.seconds * 1000) + (diff.microseconds / 1000))

with open('./output/'+fileName+'_vel.csv', 'w', newline='') as csvfile:

    writer = csv.writer(csvfile)

    rows = []

    for i,record in enumerate(data['GPS']):
        state = record['state']
        if state == 'OK' :
            time_string = data['TIME'][i]['time']
            #dt = datetime.datetime.strptime(time_string,"%H:%M:%S.%f")
            #print(dt)
            rows = [record['speed'],time_string]
            print(rows)  
            writer.writerow(rows)

with open('./output/outputGWO_GPS.csv', 'w', newline='') as csvfile:

    writer = csv.writer(csvfile)
    writer.writerow(['latitude','longitude'])

    with open('./output/outputGWO_Local_0.csv', newline='') as csvfile:
        print(fileName)
        rows = csv.reader(csvfile)
        
        for row in rows:
            convert = np.array(transfer(xx1,xx2,xx3,[-float(row[0]) , float(row[1])],x1,x2,x3)).flatten()
            writer.writerow(convert)

with open('./output/blackbox_GPS.csv', 'w', newline='') as csvfile:

    writer = csv.writer(csvfile)
    writer.writerow(['latitude','longitude'])

    for i,record in enumerate(data['GPS']):
        state = record['state']
        if state == 'OK' :
            writer.writerow(record['position'])
            
            