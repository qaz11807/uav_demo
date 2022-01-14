import plotly.graph_objects as go
import numpy as np
import math
import time
import sys
import csv
import copy
import plotly 
from numpy import pi, cos, sin, sqrt
from plotly.subplots import make_subplots

filename = 'demo_CurveO.html'

def readCSV (fileName , color = ""):

    r = []
    dis = []

    with open(fileName, newline='') as csvfile:
        rows = csv.reader(csvfile)
        for row in rows:
            r.append(float(row[0]))
            dis.append(float(row[1]))

    s = ''

    if 'Origin' in fileName :
        s = "Original"
    elif 'N' in fileName :
        s = "Original"
    else :
        s = "Optimized"

    return go.Scatter(
        x=dis, y=r,
        name="curvature of Path",
        line=dict(
            color = color
        ),
        mode='lines'
    )
        
data_origin= readCSV("D:/coding/oran/output/outputGWO_Origin_Curve_0.csv" , 'blue')
data_optimize = readCSV("D:/coding/oran/output/outputGWO_Curve_0.csv" , 'red' )

fig = go.Figure(layout_yaxis_range=[0,1])

fig.add_trace(
    data_origin,
)

fig.add_trace(
    data_optimize,
)

fig.show()

plotly.offline.plot(fig, filename=filename, auto_open=False)
