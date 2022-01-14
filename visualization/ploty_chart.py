import plotly.graph_objects as go
import numpy as np
import math
import time
import sys
import csv
import copy
import plotly 
from numpy import pi, cos, sin, sqrt

filename = 'demo_angle.html'

SIGNALPLOT = False

path_dict = [
    dict(
        label = "Greeding + DE" , 
        color = "blue" ,
        show = True ,
        fileName = "outputGWO_OmegaOptimize",
        data = None,
        avgOmega = [],
        visted = []
    ),
    dict(
        label = "No" , 
        color = "#a64dff" ,
        show = False ,
        fileName = "outputGWO_OmegaOri",
        data = None, 
        avgOmega = [],
        visted = []
    ),
    dict(
        label = "Greeding" , 
        color = "#ffa366" ,
        show = False ,
        fileName = "outputAstar_Omega",
        data = None, 
        avgOmega = [],
        visted = []
    ),
    dict(
        label = "Optimize" , 
        color = "#ed072a" ,
        show = True ,
        fileName = "outputDijiska_Omega",
        data = None, 
        avgOmega = [],
        visted = []
    ),
    # dict(
    #     label = "VG" , 
    #     color = "blue" ,
    #     show = False,
    #     fileName = "outputVG_Omega",
    #     data = None, 
    #     avgOmega = [],
    #     visted = []
    # )
]

data = []

def readCSV (fileName):
    omegas = []

    with open(fileName, newline='') as csvfile:
        rows = csv.reader(csvfile)
        for row in rows:
            omegas.append(float(row[0]))
    
    return omegas
    

def loaded_frames (path_dict) : 
    for index in range(0,len(path_dict)) :
        path = path_dict[index]
        
        y = readCSV("D:/coding/oran/output/"+path['fileName']+".csv")
        x = [i for i in range(0,len(y))]
        
        path['data'] = dict(x=x,y=y)

loaded_frames(path_dict)

for path in path_dict:
    d = path['data']
    if path['show'] == False :
            continue
    data.append(
        go.Scatter(
            x=d['x'], y=d['y'],
            mode='lines'
        )
    )

fig = go.Figure(data=data)
fig.show()

plotly.offline.plot(fig, filename=filename, auto_open=False)
