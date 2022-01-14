import numpy as np
from tkinter import *
from tkinter import messagebox
from tkinter import ttk
from mpl_toolkits.mplot3d import Axes3D
import matplotlib
import matplotlib.pyplot as plt

import matplotlib.patches as patches
import numpy as np
import math
import time
import sys
import csv

from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from matplotlib import animation

import os
import glob
import pathlib
import json

filed1 = {
    'src' : (1,1,0),
    'dest' : (99,99,25),
    'bound' : [100,100,50],
    'obstacles' : [
        [[10,0,0],[30, 50,50]] ,
        [[10,50,0],[30, 100,20]],
        [[50,30,0],[70, 100,50]],
        [[50,0,0],[70, 30,10]],
        [[50,0,20],[70,30,30]],
        [[50,0,40],[70, 30,50]],
        [[70,30,0],[100, 60,30]],
        [[70,65,15],[100, 90,35]]
    ]
}

filed2 = {
    'src' : (1,1,0),
    'dest' : (99,99,25),
    'bound' : [806,160,50],
    'obstacles' : [
        [[50,0,0],[80, 20,16]] ,  
        [[80,0,0],[140, 40,30]],
        [[95,25,0],[150, 40,30]],
        [[175,0,0],[195, 20,35]],
        [[80,80,0],[105, 100,35]],
        [[230,75,0],[250, 95,15]],
        [[440,80,0],[460, 100,35]],
        [[495,0,0],[600, 75,30]],
        [[520,80,0],[590, 135,16]],
        [[520,140,0],[540, 160,6]],
        [[415,160,0],[550, 170,16]],
        [[750,10,0],[775, 35,16]],
        [[760,85,0],[780, 105,35]],
    ],
    'obstacles_rotated' : [
        [[170,50,0],[30,20,15,45]],
        [[235,50,0],[30, 20,15,45]],
        [[105,160,0],[25, 15,16,45]],
        [[230,120,0],[120, 20,15,45]],
        [[290,120,0],[120, 20,15,45]],
        [[350,120,0],[120, 20,15,45]],
        [[625,100,0],[40, 25,15,51]],
        [[685,125,0],[80, 15,15,51]],
        [[620,60,0],[35, 15,15,51]],
        [[685,55,0],[50, 35,15,-31]],
    ]
}

filed3 = {
    'src' : (10,0,0),
    'dest' : (90,90,25),
    'bound' : [100,100,50],
    'obstacles' : [
        [[20,0,0],[100,80,50]] 
    ],
    'obstacles_rotated' : [
    ]
}

filed4 = {
    'src' : (10,0,0),
    'dest' : (90,90,25),
    'bound' : [100,100,50],
    'obstacles' : [
    ],
    'obstacles_rotated' : [
    ]
}

currentFiled =  filed1

matplotlib.use("TkAgg")

root = Tk()
root.title("tkinter")
root.geometry('700x750')

frame1 = Frame(root, bg="#ffffff")
frame1.place( width=700, height=700)
plt.rcParams['font.sans-serif'] = ['SimHei']  
fig = plt.figure(figsize=(6.5, 7), edgecolor='blue')

length = 0

isDraging = False

def getAllfileName():
    path = str(pathlib.Path().absolute()) + '/output/'
    extension = 'csv'
    os.chdir(path)
    filesNames = glob.glob('*.{}'.format(extension))
    global length 
    filesNames = list(filter(lambda s: "Weight" not in s and "Barrier" not in s, filesNames))
    filesNames = list(filter(lambda s: "LOCAL"  in s, filesNames))
    length = len(filesNames)
    return filesNames

def onSelect (event) :
    global index
    index = fileSelector.current()
    readCSV(fileSelector.get())

index = 0

fileSelector = ttk.Combobox(root, width = 40 ,height = 10) 
fileSelector['values'] = (getAllfileName()) 
fileSelector.grid(column = 0, row = 0) 
fileSelector.current(index) 
fileSelector.bind('<<ComboboxSelected>>', onSelect)    

def onNext():
    global index
    if index >= (length - 1) :
        return
    index+=1
    fileSelector.current(index) 
    readCSV(fileSelector.get())

def onPrev():
    global index
    if index <= 0 :
        return
    index-=1
    fileSelector.current(index) 
    readCSV(fileSelector.get())

prevButton = ttk.Button(root, text ="←", command = onPrev , width = 20  ) 
prevButton.grid(column = 1, row = 0)

nextButton = ttk.Button(root, text ="→", command = onNext , width = 20 )
nextButton.grid(column = 2, row = 0) 

def onCheck():
    readCSV(fileSelector.get())
    
chkValue_Weight = BooleanVar() 
chkValue_Weight.set(False)
 
chk_Weight = ttk.Checkbutton(root, text='Weight', var=chkValue_Weight , command=onCheck) 
chk_Weight.grid(column=0, row=1)

refreshButton = ttk.Button(root, text ="Refresh", command = onCheck , width = 10 )
refreshButton.grid(column = 3, row = 0) 

chkValue_ShowPoint = BooleanVar() 
chkValue_ShowPoint.set(True)
 
chk_ShowPoint = ttk.Checkbutton(root, text='ShowPoint', var=chkValue_ShowPoint , command=onCheck) 
chk_ShowPoint.grid(column=0, row=2)

chkValue_Point = BooleanVar() 
chkValue_Point.set(False)
 
chk_Point = ttk.Checkbutton(root, text='ShoeDetailedPoint', var=chkValue_Point , command=onCheck) 
chk_Point.grid(column=1, row=2)

def onSave():
    plt.savefig("./output/image")

saveButton = ttk.Button(root, text ="ScreenShot", command = onSave , width = 20  ) 
saveButton.grid(column = 0, row = 2)

afterHandler = None
i = 1

def onCheckAnimation():
    if chkValue_Animation.get() :
        readCSV(fileSelector.get())   
    else:
        global i
        i=1
        global afterHandler
        #root.after_cancel(afterHandler)  
    
chkValue_Animation = BooleanVar() 
chkValue_Animation.set(False)
 
# chk_Animation = ttk.Checkbutton(root, text='Animation', var=chkValue_Animation , command=onCheckAnimation) 
# chk_Animation.grid(column=1, row=1)

chkValue_Barrier = BooleanVar() 
chkValue_Barrier.set(True)
 
chk_Barrier = ttk.Checkbutton(root, text='Barrier', var=chkValue_Barrier , command=onCheck) 
chk_Barrier.grid(column=1, row=1)

chkValue_OP = BooleanVar() 
chkValue_OP.set(False)
 
chk_OP = ttk.Checkbutton(root, text='Original Path', var=chkValue_OP , command=onCheck) 
chk_OP.grid(column=3, row=1)

chkValue_P = BooleanVar() 
chkValue_P.set(True)
 
chk_P = ttk.Checkbutton(root, text='Path', var=chkValue_P , command=onCheck) 
chk_P.grid(column=2, row=1)

ax = Axes3D(fig)

canvas = FigureCanvasTkAgg(fig, master=frame1)
canvas.draw()
canvas.get_tk_widget().place(x=0, y=0)

weight_rssi=[]
weight_x = []
weight_y = []
weight_z = []

Ori_x = []
Ori_y = []
Ori_z = []

Line_x = []
Line_y = []
Line_z = []

with open(os.path.abspath('outputGWO_Weight.csv'), newline='') as csvfile:
    rows = csv.reader(csvfile)
    for row in rows:
        if int(row[3]) > -100:
            weight_x.append(int(row[0]))
            weight_y.append(int(row[1]))
            weight_z.append(int(row[2]))
            weight_rssi.append(int(row[3]))

line = []

def readCSV (fileName):
    
    global Line_x
    global Line_y
    global Line_z

    Line_x = []
    Line_y = []
    Line_z = []

    global Ori_x
    global Ori_y
    global Ori_z

    Ori_x = []
    Ori_y = []
    Ori_z = []

    with open(fileName, newline='') as csvfile:
        rows = csv.reader(csvfile)
        for row in rows:
            Line_x.append(int(row[0]))
            Line_y.append(int(row[1]))
            Line_z.append(int(row[2]))
    
    if "LOCAL" in fileName:
        with open(os.path.abspath("outputGWO_GLOBAL_"+ str(fileName[len(fileName)-5]) + ".csv"), newline='') as csvfile:
            rows = csv.reader(csvfile)
            for row in rows:
                Ori_x.append(int(row[0]))
                Ori_y.append(int(row[1]))
                Ori_z.append(int(row[2]))
    drawImg()
    
def plot_linear_cube(x, y, z, x2, y2, z2, color='black'):
    xx = [x, x, x2, x2, x]
    yy = [y, y2, y2, y, y]
    kwargs = {'alpha': 1, 'color': color}
    ax.plot3D(xx, yy, [z]*5, **kwargs)
    ax.plot3D(xx, yy, [z2]*5, **kwargs)
    ax.plot3D([x, x], [y, y], [z, z2], **kwargs)
    ax.plot3D([x, x], [y2, y2], [z, z2], **kwargs)
    ax.plot3D([x2,x2], [y2, y2], [z, z2], **kwargs)
    ax.plot3D([x2,x2], [y, y], [z, z2], **kwargs)

def plot_cyclinder(cx,cy,cz):
    u = np.linspace(0,2*np.pi,50)  # 把圆分按角度为50等分
    z = np.linspace(0,50,2)        # 把高度1均分为20份
    h = np.outer(np.sin(u),np.ones(len(h)))  # x值重复20次
    y = np.outer(np.cos(u),np.ones(len(h)))  # y值重复20次
    z = np.outer(np.ones(len(u)),h)   # x，y 对应的高度
    x =  np.cos(u) 
    y =  np.sin(u) 
    ax.plot_surface(x, y, z) 

def drawImg():
    global ax
    global i
    ax.clear()
   
    obstacles = currentFiled['obstacles']
    
    if chkValue_Barrier.get() :
        for obstacle in obstacles:
            src = obstacle[0]
            dest = obstacle[1]
            
            plot_linear_cube(
                src[0],src[1],src[2],
                dest[0],dest[1],dest[2]
            )

        def RotatedA(x0 , y0 , x1 , y1 , angle ) :
            x2 = (x1 - x0) * np.cos(angle) - (y1 - y0) * np.sin(angle) + x0
            y2 = (y1 - y0) * np.cos(angle) + (x1 - x0) * np.sin(angle) + y0
            return x2 , y2
        
        # for obstacle in currentFiled['obstacles_rotated']:
        #     vex = obstacle[0]
        #     params = obstacle[1]

        #     l = params[0]
        #     h = params[1]
        #     d = params[2]
        #     angle = params[3]

        #     vexs = [
        #         [vex[0],vex[1]-l,vex[2]],
        #         [vex[0],vex[1],vex[2]],
        #         [vex[0]+h,vex[1],vex[2]],
        #         [vex[0]+h,vex[1]-l,vex[2]],
        #         [vex[0],vex[1]-l,vex[2]+d],
        #         [vex[0],vex[1],vex[2]+d],
        #         [vex[0]+h,vex[1],vex[2]+d],
        #         [vex[0]+h,vex[1]-l,vex[2]+d],
        #     ]

        #     for v in vexs :   
        #         x2 , y2 =RotatedA(vex[0],vex[1],v[0],v[1],angle)
            
        #     kwargs = {'alpha': 1, 'color': 'black'}
                  
        #     # x2 , y2 =RotatedA(vex[0],vex[1],vex[0],vex[1]-l,angle)
        #     # x3 , y3 =RotatedA(vex[0],vex[1],vex[0]+h,vex[1],angle)
        #     # plot_linear_cube(
        #     #     x2,y2,vex[2],
        #     #     x3,y3,dest[2]+d
        #     # )
            
    
    if chkValue_Weight.get() :
        ax.scatter(weight_x,weight_y, weight_z , alpha = .01 , c=weight_rssi)
    
    if len(Ori_x) > 0:
        if  chkValue_OP.get():
            ax.plot(Ori_x,Ori_y,Ori_z,"o--",ms = 5 , mec = '#4CAF50', mfc = '#4CAF50' , color='#484891')
        elif chkValue_ShowPoint.get() :
            ax.plot(Ori_x,Ori_y,Ori_z,"o",ms = 5 , mec = '#4CAF50', mfc = '#4CAF50')
    
    if chkValue_Animation.get() :  
        ax.plot(Line_x[:i],Line_y[:i],Line_z[:i],c = 'b')
    else :
        if chkValue_Point.get() :  
            line = ax.plot(Line_x,Line_y,Line_z,"o-",ms =3,c = 'b' , mec = '#750000', mfc = '#750000' ,)
        elif chkValue_P.get() == True:
            line = ax.plot(Line_x,Line_y,Line_z,c = 'b')
    
    src = currentFiled['src']
    dest = currentFiled['dest']
    ax.plot(src[0],src[1],src[2] ,'o' ,color='black')
    ax.plot(dest[0],dest[1],dest[2] ,'o' ,color='red')
    
    plt.xlabel("X", fontsize=16)
    plt.ylabel("Y", fontsize=16)
    
    ax.set_xlim(0,currentFiled['bound'][0]) 
    ax.set_ylim(0,currentFiled['bound'][1]) 
    ax.set_zlim3d(0,currentFiled['bound'][2]) 

    print(currentFiled['bound'][1])

    canvas.draw()
        
readCSV(fileSelector.get())

def on_closing():
    plt.close('all')
    root.destroy()

def on_press(event):
    if chkValue_Animation.get() == False:
        ax._button_press(event)

def on_release(event):
    if chkValue_Animation.get() == False:
        ax._button_release(event)

def on_move(event):
    if chkValue_Animation.get() == False:
        ax._on_move(event)

root.protocol("WM_DELETE_WINDOW", on_closing)
canvas.mpl_connect('button_press_event', on_press)
canvas.mpl_connect('button_release_event', on_release)
canvas.mpl_connect('motion_notify_event', on_move)

root.mainloop()

def update(num,data,line):
    print(num,data,line)
    
ani = animation.FuncAnimation(fig, update, frames= len(Line_x), interval=500, blit=False , fargs=([Line_x,Line_y,Line_z], line))