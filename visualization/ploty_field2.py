import plotly.graph_objects as go
import numpy as np
import math
import csv
import plotly 
from numpy import pi, cos, sin, sqrt

filename = 'demo__d.html'

field = {
    'bound' : [806,160,50],
    'basestaions' : [
    ],
    'obstacles' : [
        [[50,0,0],[80, 20,16]] ,  #[[障礙物左下頂點座標][障礙物右上頂點座標]]
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
        [[170,50,0],[30,20,15,45]], #[[障礙物左上頂點座標][障礙物長度(Y軸),障礙物寬度(X軸),障礙物高度,旋轉角度]]
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

path_dict = [
    dict(
        label = "Planning" , 
        color = "blue" ,
        show = True ,
        fileName = "outputGWO_EE",
        data = None
    ),
    dict(
        label = "Reality" , 
        color = "#a64dff" ,
        show = False ,
        fileName = "blackbox",
        data = None 
    ),
    dict(
        label = "Astar" , 
        color = "red" ,
        show = True ,
        fileName = "outputAstar",
        data = None
    ),
    dict(
        label = "Dijkstra" , 
        color = "yellow" ,
        show = True ,
        fileName = "outputDijkstra",
        data = None
    ),
    dict(
        label = "VG" , 
        color = "green" ,
        show = True ,
        fileName = "outputVG",
        data = None
    )
]

data = []

#mock
for _ in range(0,20):
    data.append(
        go.Scatter3d(
            x=[], y=[], z=[],
            mode='markers',
            name='source',    
            marker=dict(
                size=5,
                color="green"               
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

def RotatedA(x0 , y0 , x1 , y1 , angle ) :
    x2 = (x1 - x0) * np.cos(angle) - (y1 - y0) * np.sin(angle) + x0
    y2 = (y1 - y0) * np.cos(angle) + (x1 - x0) * np.sin(angle) + y0
    return x2 , y2

#繪製障礙物
for idx,obstacle in enumerate(field['obstacles_rotated']):
    vex = obstacle[0]

    params = obstacle[1]

    l = params[0]
    h = params[1]
    d = params[2]
    angle = params[3]

    vexs = [
        [vex[0],vex[1]-l,vex[2]],
        [vex[0],vex[1],vex[2]],
        [vex[0]+h,vex[1],vex[2]],
        [vex[0]+h,vex[1]-l,vex[2]],
        [vex[0],vex[1]-l,vex[2]+d],
        [vex[0],vex[1],vex[2]+d],
        [vex[0]+h,vex[1],vex[2]+d],
        [vex[0]+h,vex[1]-l,vex[2]+d],
    ]

    obstacle_x = []
    obstacle_y = []
    obstacle_z = []

    for v in vexs :   
        x2 , y2 =RotatedA(vex[0],vex[1],v[0],v[1],angle)
        obstacle_x.append(x2)
        obstacle_y.append(y2)
        obstacle_z.append(v[2])

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

r = 806 
c = 163 
h = 30 

x_eye, y_eye, z_eye = -1.25, -1.25, 0.1

#視角轉換
def rotate_z(x, y, z, theta):
    w = x+1j*y
    return np.real(np.exp(1j*theta)*w), np.imag(np.exp(1j*theta)*w), z

#讀取資料
def loaded_datas (path_dict) : 
    lens = []

    #讀取CSV
    def readCSV (fileName):
        Line_x = []
        Line_y = []
        Line_z = []
        colors = []

        with open(fileName, newline='') as csvfile:
            print(fileName)
            rows = csv.reader(csvfile)
            for row in rows:
                Line_x.append(int(row[0]))
                Line_y.append(int(row[1]))
                Line_z.append(int(row[2]))
        
        return Line_x , Line_y , Line_z , colors

    for _ in path_dict :
        x,y,z,colors= readCSV("D:/coding/oran/output/"+_['fileName']+".csv")
        _['data'] = dict(x=x,y=y,z=z,colors=colors)
        lens.append(len(x))
    return max(lens)

#回傳最長路線的長度當作Slider step
N = loaded_datas(path_dict)

#繪製起點終點
init = False
for path in path_dict :
    _ = path['data']

    if path['show'] == False :
        continue

    if init == False :
        data.append(
            go.Scatter3d(
                x=[_['x'][0]], y=[_['y'][0]], z=[_['z'][0]],
                mode='markers',
                name='source',      
                marker=dict(
                    size=5,
                    color="green"               
                )
            )
        )

        l = len(_['x'])
        data.append(
            go.Scatter3d(
                x=[_['x'][l-1]], y=[_['y'][l-1]], z=[_['z'][l-1]] ,
                mode='markers', 
                name='dest',     
                marker=dict(
                    size=5,
                    color="red"
                )
            )
        )
        init = True

#繪製影格
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
                name='UAV'
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

duration = 0

slider_steps = []

for i in range(N):
    xe, ye, ze = rotate_z(x_eye, y_eye, z_eye, thetas[i])
    frames_data  = make_frames(i)
    frames.append(
        go.Frame(
            data = frames_data,
            name='{}'.format(i),
            # layout = {
            #     'scene' : {
            #         'camera' : dict(
            #             eye=dict(x=xe, y=ye, z=zs[i]) #視角轉換
            #         )
            #     }
            # }
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

layout.update(title='3D UAV Path Planning Demo',updatemenus=updatemenus,scene_camera=camera , sliders = [sliders])

fig = go.Figure(data=data, layout=layout, frames=frames)

fig.update_layout(scene_aspectmode='data')

fig.show()

plotly.offline.plot(fig, filename=filename, auto_open=False)
