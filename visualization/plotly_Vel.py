import plotly.graph_objects as go
import numpy as np
import math
import time
import sys
import csv
import copy
import plotly 
import datetime 
from numpy import pi, cos, sin, sqrt
from plotly.subplots import make_subplots

filename = 'demo_CurveV.html'

def readCSV (fileName , color = ""):

    start_time = 0
    v = []
    t = []
    a = []

    with open(fileName, newline='') as csvfile:
        rows = csv.reader(csvfile)
        for row in rows:
            v.append(float(row[0]))
            dt = datetime.datetime.strptime(str(row[1]),"%H:%M:%S.%f")
            if start_time == 0 :
                start_time = dt
            diff = dt - start_time
            elapsed_time = float((diff.seconds * 1000) + (diff.microseconds / 1000)) / 1000
            print(float(row[0]) , elapsed_time)
            t.append(elapsed_time)
            
    return go.Scatter(
        x=t, y=v,
        name="curvature of Path",
        line=dict(
            color = color
        ),
        mode='lines'
    )
        
data_origin= readCSV("D:/coding/oran/output/blackbox_vel.csv" , 'blue')

fig = go.Figure()

fig.add_trace(
    data_origin
)

fig.show()

plotly.offline.plot(fig, filename=filename, auto_open=False)
