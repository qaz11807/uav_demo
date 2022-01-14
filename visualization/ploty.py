import plotly.graph_objects as go
import numpy as np
import math
import time
import sys
import csv
import copy
import plotly 
from numpy import pi, cos, sin, sqrt

filename = 'demo_5.html'
field = {
    'src' : (1,1,0),
    'dest' : (99,99,25),
    'obstacles' : [
        [[10,0,0],[30, 50,50]] ,
        [[10,50,0],[30, 100,20]],
        [[50,30,0],[70, 100,50]],
        [[50,0,0],[70, 30,10]],
        [[50,0,20],[70,30,30]],
        [[50,0,40],[70, 30,50]],
        [[70,30,0],[100, 60,30]],
        [[70,65,15],[100, 90,35]]
    ],
    'basestaions' : [
        (0,0,25),
        (0, 20 ,25),
        (0, 60 ,25),
        (0, 100 ,25),
        (20, 80 ,25),
        (60, 0 ,25)
    ]
}

SIGNALPLOT = False

path_dict = [
    # dict(
    #     label = "Greedy + DE" , 
    #     color = "blue" ,
    #     show = True ,
    #     fileName = "/demo/outputGWO_Greeding+DE3",
    #     data = None,
    #     avgOmega = [],
    #     visted = []
    # ),
    # dict(
    #     label = "Origin" , 
    #     color = "#a64dff" ,
    #     show = True ,
    #     fileName = "outputGWO_Greeding+DE_0",
    #     data = None, 
    #     avgOmega = [],
    #     visted = []
    # ),
    # dict(
    #     label = "Greedy" , 
    #     color = "#ffa366" ,
    #     show = True ,
    #     fileName = "/demo/outputGWO_Greeding",
    #     data = None, 
    #     avgOmega = [],
    #     visted = []
    # ),
    dict(
        label = "Origin" , 
        color = "blue" ,
        show = True ,
        fileName = "outputGWO_Global_curve_back",
        data = None, 
        avgOmega = [],
        visted = []
    ),
    # dict(
    #     label = "Origin" , 
    #     color = "blue" ,
    #     show = True,
    #     fileName = "demo/outputGWO_Greeding+DE3",
    #     data = None, 
    #     avgOmega = [],
    #     visted = []
    # ),
    # dict(
    #     label = "Optimized" , 
    #     color = "red" ,
    #     show = True,
    #     fileName = "demo/outputGWO_Optimize3",
    #     data = None, 
    #     avgOmega = [],
    #     visted = []
    # ),
    dict(
        label = "Optimize" , 
        color = "red" ,
        show = True ,
        fileName = "outputGWO_Local_curve_back",
        data = None, 
        avgOmega = [],
        visted = []
    ),
    # dict(
    #     label = "Astar" , 
    #     color = "red" ,
    #     show = True ,
    #     fileName = "outputAstar_F1",
    #     data = None
    # ),
    # dict(
    #     label = "Dijkstra" , 
    #     color = "yellow" ,
    #     show = True ,
    #     fileName = "outputDijkstra_F1",
    #     data = None
    # ),
    # dict(
    #     label = "VG" , 
    #     color = "green" ,
    #     show = True ,
    #     fileName = "outputVG",
    #     data = None
    # ),
    # dict(
    #     label = "Proposed method" , 
    #     color = "blue" ,
    #     show = True ,
    #     fileName = "/demo/outputGWO_Optimize3",
    #     data = None
    # ),
]

data = []

def plotyPoint( coordinate ) :
    return [coordinate[0]] , [coordinate[1]] , [coordinate[2]] 

src_x,src_y,src_z = plotyPoint(field['src'])

for _ in range(0,20):
    data.append(
        go.Scatter3d(
            x=[], y=[], z=[],
            mode='markers'
        )
    )

data.append(
    go.Scatter3d(
        x=src_x, y=src_y, z=src_z,
        mode='markers',
        name='source',      
        marker=dict(
            size=5,
            color="green"               
        )
    )
)

dest_x,dest_y,dest_z = plotyPoint(field['dest'])

data.append(
    go.Scatter3d(
        x=dest_x, y=dest_y, z=dest_z,
        mode='markers', 
        name='dest',     
        marker=dict(
            size=5,
            color="red"
        )
    )
)

for idx,obstacle in enumerate(field['obstacles']):
    src = obstacle[0]
    dest = obstacle[1]
    obstacle_x = []
    obstacle_y = []
    obstacle_z = []

    obstacle_x.append(src[0])
    obstacle_y.append(src[1])
    obstacle_z.append(src[2])

    obstacle_x.append(src[0])
    obstacle_y.append(dest[1])
    obstacle_z.append(src[2])

    obstacle_x.append(dest[0])
    obstacle_y.append(dest[1])
    obstacle_z.append(src[2])

    obstacle_x.append(dest[0])
    obstacle_y.append(src[1])
    obstacle_z.append(src[2])

    obstacle_x.append(src[0])
    obstacle_y.append(src[1])
    obstacle_z.append(dest[2])

    obstacle_x.append(src[0])
    obstacle_y.append(dest[1])
    obstacle_z.append(dest[2])

    obstacle_x.append(dest[0])
    obstacle_y.append(dest[1])
    obstacle_z.append(dest[2])

    obstacle_x.append(dest[0])
    obstacle_y.append(src[1])
    obstacle_z.append(dest[2])

    data.append(
        go.Mesh3d(
            x=obstacle_x, 
            y=obstacle_y, 
            z=obstacle_z, 
            i = [7, 0, 0, 0, 4, 4, 6, 6, 4, 0, 3, 2],
            j = [3, 4, 1, 2, 5, 6, 5, 2, 0, 1, 6, 3],
            k = [0, 7, 2, 3, 6, 7, 1, 1, 5, 5, 7, 6],
            color='black', 
            name='Obstacle ' + str(idx) , 
            opacity=.3,
            flatshading = True,
            hoverinfo='skip'
        )
    )

r = 100 
c = 100 
h = 50 

weights = [[[0 for _ in range(h)] for _ in range(c)] for _ in range(r)]

with open('D:/coding/oran/output/outputGWO_Weight.csv', newline='') as csvfile:
    rows = csv.reader(csvfile)
    for row in rows:
        _x = int(row[0])
        _y = int(row[1])
        _z = int(row[2])
        rssi = int(row[3])
        weights[_x][_y][_z] = rssi

weight_z = [[0 for _ in range(c)] for _ in range(r)]
for i in range(0,r) :
    for j in range(0,c) :
        w = -1*(-100 - weights[i][j][0]) * (5/4)  
        weight_z[j][i] = w

if SIGNALPLOT :
    data.append(
        go.Surface(
            z=weight_z , showscale=False, opacity=0.3
        )
    )

def readCSV (fileName):
    Line_x = []
    Line_y = []
    Line_z = []
    colors = []

    with open(fileName, newline='') as csvfile:
        rows = csv.reader(csvfile)
        for row in rows:
            Line_x.append(int(row[0]))
            Line_y.append(int(row[1]))
            Line_z.append(int(row[2]))
            if 'VG' not in fileName :
                colors.append(weights[int(row[0])][int(row[1])][int(row[2])])
    
    return Line_x , Line_y , Line_z , colors

x_eye, y_eye, z_eye = -1.25, -1.25, 0.1
def rotate_z(x, y, z, theta):
    w = x+1j*y
    return np.real(np.exp(1j*theta)*w), np.imag(np.exp(1j*theta)*w), z

def loaded_frames (path_dict) : 
    lens = []
    for _ in path_dict :
        x,y,z,colors= readCSV("D:/coding/oran/output/"+_['fileName']+".csv")
        _['data'] = dict(x=x,y=y,z=z,colors=colors)
        lens.append(len(x)+1)
    return max(lens)

N = loaded_frames(path_dict)

def calacuteOemga (vector1 , vector2) :
    AB = vector1[0] * vector2[0] + vector1[1] * vector2[1] +vector1[2] * vector2[2]
    A = math.sqrt( math.pow(vector1[0],2) +  math.pow(vector1[1],2) +  math.pow(vector1[2],2) )
    B = math.sqrt( math.pow(vector2[0],2) +  math.pow(vector2[1],2) +  math.pow(vector2[2],2) )
    _ = 0

    if(A != 0 and B != 0):
        cos = AB / (A*B)
        _ = math.acos(cos) 
      
    if A != 0 :
        return B * math.sin(_) / A  
    else :
        return 0

def make_frames (index) : 
    _ = []
   
    for path in path_dict :
        if path['show'] == False :
            continue
        x , y , z , colors = path['data']['x'] , path['data']['y'] ,path['data']['z'] ,path['data']['colors']
        color = path['color']
        if color == 'gradient' :
            color = colors 
        curr_index = math.ceil((index*len(x) / N)) 
        curr_index2 = math.floor((index*len(x) /N))
        
        _.append(
            go.Scatter3d(
                x=x[:curr_index], 
                y=y[:curr_index], 
                z=z[:curr_index],
                mode="lines",
                line=dict(
                    color=color,
                    width=3
                ),
                name=path['label']
            )
        )
        _.append(
            go.Scatter3d(
                x=[x[curr_index2]], 
                y=[y[curr_index2]], 
                z=[z[curr_index2]],
                mode="markers",
                marker=dict(
                    color=color,
                    symbol = 'square' ,
                    size = 3
                ),
                name='UAV Position'
            )
        )
    return _ 
    
def frame_args(duration):
    return {
        "frame": {"duration": duration },
        "mode": "immediate",
        "fromcurrent": True,
        "transition": {"duration": duration, "easing": "linear"},
    }

frames = []

zs = np.arange(z_eye, 1, (1 - z_eye) / N)
thetas = np.arange(0, 1, 1 / N)

duration = 100

slider_steps = []

for i in range(N):
    xe, ye, ze = rotate_z(x_eye, y_eye, z_eye, thetas[i])
    frames_data  = make_frames(i)
    frames.append(
        go.Frame(
            data = frames_data,
            name='{}'.format(i),
            layout = {
                'scene' : {
                    'camera' : dict(
                        eye=dict(x=xe, y=ye, z=zs[i])
                    )
                }
            }
        )
    )
    slider_steps.append({
        "args": [
            ["{}".format(i)], frame_args(0)
        ],
        "label": "{}".format(i),
        "method": "animate",
    })

sliders = {
    "pad": {"b": 10, "t": 60},
    "len": 0.9,
    "x": 0.1,
    "y": 0,
    "steps": slider_steps
}

layout= go.Layout() 
updatemenus = [{
    "buttons": [
        {
            "args": [None, frame_args(duration)],
            "label": "&#9654;", 
            "method": "animate",
        },
        {
            "args": [[None], frame_args(0)],
            "label": "&#9724;", 
            "method": "animate",
        },
    ],
    "direction": "left",
    "pad": {"r": 10, "t": 70},
    "type": "buttons",
    "x": 0.1,
    "y": 0,
}]

camera = dict(
    eye=dict(x=x_eye, y=y_eye, z=z_eye)
)

layout.update(title='3D UAV Path Planning Demo',updatemenus=updatemenus,scene_camera=camera , sliders = [sliders] ,font=dict(
        size=16,
    ))

fig = go.Figure(data=data, layout=layout, frames=frames)
# fig.update_traces(contours_z=dict(show=True, usecolormap=True,
#                                   highlightcolor="limegreen", project_z=True))
fig.show()

plotly.offline.plot(fig, filename=filename, auto_open=False)
