#!/usr/bin/env python3
# File name   : GUI.py
# Website     : www.adeept.com
# Author      : Adeept
# Date        : 2025/05/15
from socket import *
import sys
import time
import threading as thread
import tkinter as tk
import math
import os
# from PIL import Image, ImageTk
# import numpy as np

radar_data = []  
radar_updated = False
can_scan = None
received_points = 0  
MAX_POINTS = 23


def global_init():
    global DS_stu, TS_stu, DRS_stu, color_bg, color_text, color_btn, color_line, color_can, color_oval, target_color
    global ip_stu, Switch_3, Switch_2, Switch_1, servo_stu, function_stu
    DS_stu = 0
    TS_stu = 0
    DRS_stu = 0

    color_bg='#000000'        #Set background color
    color_text='#E1F5FE'      #Set text color
    color_btn='#0277BD'       #Set button color
    color_line='#01579B'      #Set line color
    color_can='#212121'       #Set canvas color
    color_oval='#2196F3'      #Set oval color
    target_color='#FF6D00'
    ip_stu=1

    Switch_3 = 0
    Switch_2 = 0
    Switch_1 = 0

    servo_stu = 0
    function_stu = 0
    


def replace_num(initial,new_num):   #Call this function to replace data in '.txt' file
    newline=""
    str_num=str(new_num)
    with open("./ip.txt","r") as f:
        for line in f.readlines():
            if(line.find(initial) == 0):
                line = initial+"%s" %(str_num)
            newline += line
    with open("./ip.txt","w") as f:
        f.writelines(newline)    #Call this function to replace data in '.txt' file

def num_import(initial):            #Call this function to import data from '.txt' file
    with open("./ip.txt") as f:
        for line in f.readlines():
            if(line.find(initial) == 0):
                r=line
    begin=len(list(initial))
    snum=r[begin:]
    n=snum
    return n    

def connection_thread():
    global Switch_3, Switch_2, Switch_1, function_stu, radar_data, radar_updated, received_points
    buffer = ""  
    
    while 1:
        try:
            data = tcpClicSock.recv(BUFSIZ).decode()
            if not data:
                continue
            buffer += data
            
            while '.' in buffer:
                index = buffer.index('.')
                data_item = buffer[:index].strip()
                buffer = buffer[index + 1:]
                
                if ',' in data_item:
                    angle_str, dist_str = data_item.split(',')
                    try:
                        angle = int(angle_str)
                        dist = int(dist_str) / 100
                        
                        radar_data.append([angle, dist])
                        received_points += 1
                        print(f"Received data points {received_points}/{MAX_POINTS}: angle={angle}, dist={dist}m")
                        
                        if received_points >= MAX_POINTS:
                            radar_updated = True
                            root.after(0, update_radar_display)
                            received_points = 0
                    except ValueError:
                        print(f"Unable to parse data: {data_item}")
        except Exception as e:
            print(f"Error receiving data: {e}")

def socket_connect():     #Call this function to connect with the server
    global ADDR,tcpClicSock,BUFSIZ,ip_stu,ip_adr
    ip_adr=E1.get()       #Get the IP address from Entry

    if ip_adr == '':      #If no input IP address in Entry,import a default IP
        ip_adr=num_import('IP:')
        l_ip_4.config(text='Connecting')
        l_ip_4.config(bg='#FF8F00')
        l_ip_5.config(text='Default:%s'%ip_adr)
        pass
    
    SERVER_IP = ip_adr
    SERVER_PORT = 4000   #Define port serial 
    BUFSIZ = 1024         #Define buffer size
    ADDR = (SERVER_IP, SERVER_PORT)
    tcpClicSock = socket(AF_INET, SOCK_STREAM) #Set connection value for socket

    for i in range (1,6): #Try 5 times if disconnected
        if ip_stu == 1:
            print("Connecting to server @ %s:%d..." %(SERVER_IP, SERVER_PORT))
            print("Connecting")
            tcpClicSock.connect(ADDR)        #Connection with the server
        
            print("Connected")
        
            l_ip_5.config(text='IP:%s'%ip_adr)
            l_ip_4.config(text='Connected')
            l_ip_4.config(bg='#558B2F')

            replace_num('IP:',ip_adr)
            E1.config(state='disabled')      #Disable the Entry
            Btn14.config(state='disabled')   #Disable the Entry
            
            ip_stu=0                         #'0' means connected

            connection_threading=thread.Thread(target=connection_thread)         
            connection_threading.setDaemon(True)                             
            connection_threading.start()                                      

            break
        else:
            print("Cannot connecting to server,try it latter!")
            l_ip_4.config(text='Try %d/5 time(s)'%i)
            l_ip_4.config(bg='#EF6C00')
            print('Try %d/5 time(s)'%i)
            ip_stu=1
            time.sleep(1)
            continue

    if ip_stu == 1:
        l_ip_4.config(text='Disconnected')
        l_ip_4.config(bg='#F44336')

def connect(event):       #Call this function to connect with the server
    if ip_stu == 1:
        sc=thread.Thread(target=socket_connect) #Define a thread for connection
        sc.setDaemon(True)                      #'True' means it is a front thread,it would close when the mainloop() closes
        sc.start()                              #Thread starts

def servo_buttons(x,y):
    global Btn_SR, Btn_Police, Btn_Rainbow, Btn_Servo1,Btn_Servo2
    def call_home(event):
        global servo_stu
        if servo_stu == 0:
            tcpClicSock.send(('home').encode())
            servo_stu = 1

    def call_lookleft(event):
        global servo_stu
        if servo_stu == 0:
            tcpClicSock.send(('lookleft').encode())
            servo_stu = 1

    def call_lookright(event):
        global servo_stu
        if servo_stu == 0:
            tcpClicSock.send(('lookright').encode())
            servo_stu = 1

    def call_stop(event):
        global servo_stu
        tcpClicSock.send(('RLstop').encode())
        servo_stu = 0

    def call_stop2(event):
        global servo_stu
        tcpClicSock.send(('stop').encode())
        servo_stu = 0


    Btn_0 = tk.Button(root, width=8, text='Left',fg=color_text,bg=color_btn,relief='ridge')
    Btn_0.place(x=x,y=y+35)
    Btn_0.bind('<ButtonPress-1>', call_lookleft)
    Btn_0.bind('<ButtonRelease-1>', call_stop)
    root.bind('<KeyPress-j>', call_lookleft)
    root.bind('<KeyRelease-j>', call_stop)

    Btn_Servo1 = tk.Button(root, width=8, text='Home',fg=color_text,bg=color_btn,relief='ridge')
    Btn_Servo1.place(x=x+70,y=y+35)
    Btn_Servo1.bind('<ButtonPress-1>', call_home)
    Btn_Servo1.bind('<ButtonRelease-1>', call_stop2)
    root.bind('<KeyPress-i>', call_home)
    root.bind('<KeyRelease-i>', call_stop2) 

    Btn_2 = tk.Button(root, width=8, text='Right',fg=color_text,bg=color_btn,relief='ridge')
    Btn_2.place(x=x+140,y=y+35)
    Btn_2.bind('<ButtonPress-1>', call_lookright)
    Btn_2.bind('<ButtonRelease-1>', call_stop)
    root.bind('<KeyPress-l>', call_lookright) 
    root.bind('<KeyRelease-l>', call_stop) 

def motor_buttons(x,y):
    def call_left(event):
        global TS_stu
        if TS_stu == 0:
            tcpClicSock.send(('turn_left').encode())
            TS_stu = 1

    def call_right(event):
        global TS_stu
        if TS_stu == 0:
            tcpClicSock.send(('turn_right').encode())
            TS_stu = 1

    def call_forward(event):
        global DS_stu
        if DS_stu == 0:
            tcpClicSock.send(('forward').encode())
            DS_stu = 1

    def call_backward(event):
        global DS_stu
        if DS_stu == 0:
            tcpClicSock.send(('backward').encode())
            DS_stu = 1

    def call_DS(event):
        global DS_stu
        tcpClicSock.send(('DS').encode())
        DS_stu = 0

    def call_TS(event):
        global TS_stu
        tcpClicSock.send(('TS').encode())
        TS_stu = 0

    #drift
    def call_drift_front_left(event):
        global DRS_stu
        if DRS_stu == 0:
            tcpClicSock.send(('drift_front_left').encode())
            DRS_stu = 1
    def call_drift_front_right(event):
        global DRS_stu
        if DRS_stu == 0:
            tcpClicSock.send(('drift_front_right').encode())
            DRS_stu = 1
    def call_drift_left(event):
        global DRS_stu
        if DRS_stu == 0:
            tcpClicSock.send(('drift_left').encode())
            DRS_stu = 1
    def call_drift_right(event):
        global DRS_stu
        if DRS_stu == 0:
            tcpClicSock.send(('drift_right').encode())
            DRS_stu = 1
    def call_drift_rear_left(event):
        global DRS_stu
        if DRS_stu == 0:
            tcpClicSock.send(('drift_rear_left').encode())
            DRS_stu = 1
    def call_drift_rear_right(event):
        global DRS_stu
        if DRS_stu == 0:
            tcpClicSock.send(('drift_rear_right').encode())
            DRS_stu = 1
    def call_DRS(event):
        global DRS_stu
        tcpClicSock.send(('DRS').encode())
        DRS_stu = 0


    Btn_0 = tk.Button(root, width=8, text='Left',fg=color_text,bg=color_btn,relief='ridge')
    Btn_0.place(x=x,y=y+35)
    Btn_0.bind('<ButtonPress-1>', call_left)
    Btn_0.bind('<ButtonRelease-1>', call_TS)
    root.bind('<KeyPress-a>', call_left)
    root.bind('<KeyRelease-a>', call_TS)

    Btn_1 = tk.Button(root, width=8, text='Forward',fg=color_text,bg=color_btn,relief='ridge')
    Btn_1.place(x=x+70,y=y)
    Btn_1.bind('<ButtonPress-1>', call_forward)
    Btn_1.bind('<ButtonRelease-1>', call_DS)
    root.bind('<KeyPress-w>', call_forward)
    root.bind('<KeyRelease-w>', call_DS) 

    Btn_2 = tk.Button(root, width=8, text='Backward',fg=color_text,bg=color_btn,relief='ridge')
    Btn_2.place(x=x+70,y=y+105)
    Btn_2.bind('<ButtonPress-1>', call_backward)
    Btn_2.bind('<ButtonRelease-1>', call_DS)
    root.bind('<KeyPress-s>', call_backward)
    root.bind('<KeyRelease-s>', call_DS)

    Btn_3 = tk.Button(root, width=8, text='Right',fg=color_text,bg=color_btn,relief='ridge')
    Btn_3.place(x=x+140,y=y+35)
    Btn_3.bind('<ButtonPress-1>', call_right)
    Btn_3.bind('<ButtonRelease-1>', call_TS)
    root.bind('<KeyPress-d>', call_right) 
    root.bind('<KeyRelease-d>', call_TS) 

    #drift
    Drift_FL = tk.Button(root, width=8, text='Drift_FL',fg=color_text,bg=color_btn,relief='ridge')
    Drift_FL.place(x=x,y=y)
    Drift_FL.bind('<ButtonPress-1>', call_drift_front_left)
    Drift_FL.bind('<ButtonRelease-1>', call_DRS)
    root.bind('<KeyPress-z>', call_drift_front_left) 
    root.bind('<KeyRelease-z>', call_DRS) 

    Drift_FR = tk.Button(root, width=8, text='Drift_FR',fg=color_text,bg=color_btn,relief='ridge')
    Drift_FR.place(x=x+140,y=y)
    Drift_FR.bind('<ButtonPress-1>', call_drift_front_right)
    Drift_FR.bind('<ButtonRelease-1>', call_DRS)
    root.bind('<KeyPress-x>', call_drift_front_right) 
    root.bind('<KeyRelease-x>', call_DRS) 

    Drift_L = tk.Button(root, width=8, text='Drift_L',fg=color_text,bg=color_btn,relief='ridge')
    Drift_L.place(x=x,y=y+70)
    Drift_L.bind('<ButtonPress-1>', call_drift_left)
    Drift_L.bind('<ButtonRelease-1>', call_DRS)
    root.bind('<KeyPress-c>', call_drift_left) 
    root.bind('<KeyRelease-c>', call_DRS) 

    Drift_R = tk.Button(root, width=8, text='Drift_R',fg=color_text,bg=color_btn,relief='ridge')
    Drift_R.place(x=x+140,y=y+70)
    Drift_R.bind('<ButtonPress-1>', call_drift_right)
    Drift_R.bind('<ButtonRelease-1>', call_DRS)
    root.bind('<KeyPress-v>', call_drift_right) 
    root.bind('<KeyRelease-v>', call_DRS) 

    Drift_RL = tk.Button(root, width=8, text='Drift_RL',fg=color_text,bg=color_btn,relief='ridge')
    Drift_RL.place(x=x,y=y+105)
    Drift_RL.bind('<ButtonPress-1>', call_drift_rear_left)
    Drift_RL.bind('<ButtonRelease-1>', call_DRS)
    root.bind('<KeyPress-b>', call_drift_rear_left) 
    root.bind('<KeyRelease-b>', call_DRS) 

    Drift_RR = tk.Button(root, width=8, text='Drift_RR',fg=color_text,bg=color_btn,relief='ridge')
    Drift_RR.place(x=x+140,y=y+105)
    Drift_RR.bind('<ButtonPress-1>', call_drift_rear_right)
    Drift_RR.bind('<ButtonRelease-1>', call_DRS)
    root.bind('<KeyPress-n>', call_drift_rear_right) 
    root.bind('<KeyRelease-n>', call_DRS) 


def information_screen(x,y):
    global CPU_TEP_lab, CPU_USE_lab, RAM_lab, l_ip_4, l_ip_5
    l_ip_4=tk.Label(root,width=18,text='Disconnected',fg=color_text,bg='#F44336')
    l_ip_4.place(x=x,y=y+95)                         #Define a Label and put it in position

    l_ip_5=tk.Label(root,width=18,text='Use default IP',fg=color_text,bg=color_btn)
    l_ip_5.place(x=x,y=y+130)                         #Define a Label and put it in position

def connent_input(x,y):
    global E1, Btn14
    E1 = tk.Entry(root,show=None,width=16,bg="#37474F",fg='#eceff1')
    E1.place(x=x+5,y=y+25)                              #Define a Entry and put it in position

    l_ip_3=tk.Label(root,width=10,text='IP Address:',fg=color_text,bg='#000000')
    l_ip_3.place(x=x,y=y)                                #Define a Label and put it in position

    Btn14= tk.Button(root, width=8,height=2, text='Connect',fg=color_text,bg=color_btn,relief='ridge')
    Btn14.place(x=x+120,y=y+10)                          #Define a Button and put it in position

    root.bind('<Return>', connect)
    Btn14.bind('<ButtonPress-1>', connect)

def update_radar_display():
    global radar_data, can_scan, root, color_can, color_line, color_oval
    if can_scan:
        can_scan.delete("all") 
        line = can_scan.create_line(0,62,320,62,fill='darkgray')
        line = can_scan.create_line(0,124,320,124,fill='darkgray')
        line = can_scan.create_line(0,186,320,186,fill='darkgray')
        line = can_scan.create_line(160,0,160,250,fill='darkgray')
        line = can_scan.create_line(80,0,80,250,fill='darkgray')
        line = can_scan.create_line(240,0,240,250,fill='darkgray')
        

        x_range = 2.0
        can_tex_11=can_scan.create_text(27,178,text='%sm' % round((x_range / 4), 2), fill='#aeea00')
        can_tex_12=can_scan.create_text(27,116,text='%sm' % round((x_range / 2), 2), fill='#aeea00')
        can_tex_13=can_scan.create_text(27,54,text='%sm' % round((x_range * 0.75), 2), fill='#aeea00')
        total_points = len(radar_data)
        
        for i, (angle, dist) in enumerate(radar_data):
            if dist > 0:
                len_dis_1 = int((dist / x_range)*200)
                pos = int((i / total_points) * 320)  
                pos_ra = int(((i / total_points) * 140) + 20)  
                
                x0_l, y0_l = pos, (250 - len_dis_1)
                
                if pos <= 160:
                    pos = 160 - abs(int(len_dis_1 * (math.cos(math.radians(pos_ra)))))
                    x1_l = pos - math.cos(math.radians(pos_ra)) * 130
                else:
                    pos = abs(int(len_dis_1 * (math.cos(math.radians(pos_ra))))) + 160
                    x1_l = pos + abs(math.cos(math.radians(pos_ra)) * 130)
                
                y1_l = y0_l - abs(math.sin(math.radians(pos_ra)) * 130)
                
                x0_l = max(0, min(x0_l, 320))
                y0_l = max(0, min(y0_l, 250))
                x1_l = max(0, min(x1_l, 320))
                y1_l = max(0, min(y1_l, 250))
                
                line = can_scan.create_line(pos, y0_l, x1_l, y1_l, fill=color_line)
                size = 3
                x0, y0 = (pos + size), (250 - len_dis_1 + size)
                x1, y1 = (pos - size), (250 - len_dis_1 - size)
                point_scan = can_scan.create_oval(x0, y0, x1, y1, fill=color_oval, outline=color_oval)

def ultrasonic_radar(x,y):
    global can_scan
    x_range = 2.0  
    can_scan = tk.Canvas(root,bg=color_can,height=250,width=320,highlightthickness=0) #define a canvas
    can_scan.place(x=x,y=y) #Place the canvas
    
    line = can_scan.create_line(0,62,320,62,fill='darkgray')   #Draw a line on canvas
    line = can_scan.create_line(0,124,320,124,fill='darkgray') #Draw a line on canvas
    line = can_scan.create_line(0,186,320,186,fill='darkgray') #Draw a line on canvas
    line = can_scan.create_line(160,0,160,250,fill='darkgray') #Draw a line on canvas
    line = can_scan.create_line(80,0,80,250,fill='darkgray')   #Draw a line on canvas
    line = can_scan.create_line(240,0,240,250,fill='darkgray') #Draw a line on canvas

    can_tex_11=can_scan.create_text((27,178),text='%sm'%round((x_range/4),2),fill='#aeea00')    #Create a text on canvas
    can_tex_12=can_scan.create_text((27,116),text='%sm'%round((x_range/2),2),fill='#aeea00')    #Create a text on canvas
    can_tex_13=can_scan.create_text((27,54),text='%sm'%round((x_range*0.75),2),fill='#aeea00')  #Create a text on canvas

def function_buttons(x,y):
    global function_stu, Btn_function_1, Btn_function_2, Btn_function_3, Btn_function_4, Btn_function_5, Btn_function_6, Btn_function_7, Btn_function_8
    def call_function_1(event):
        global radar_data, received_points
        radar_data = []
        received_points = 0
        tcpClicSock.send(('scan').encode())

    def call_function_2(event):
        tcpClicSock.send(('police').encode())
        print('police')

    def call_function_3(event):
        tcpClicSock.send(('automatic').encode())

    def call_function_4(event):
        tcpClicSock.send(('trackLine').encode())

    def call_function_5(event):
        tcpClicSock.send(('lightTracking').encode())

    def call_function_6(event):
        tcpClicSock.send(('matrix').encode())
        print('MatrixSmile')

    def call_function_7(event):
        tcpClicSock.send(('KD').encode())
        print('KD')
        
    # def call_function_8(event):
    #     tcpClicSock.send(('buzzerMusic').encode())
        
    def call_function_9(event):
        global function_stu
        tcpClicSock.send(('StopFunction').encode())
        function_stu = 0

    Btn_function_1 = tk.Button(root, width=14, text='Radar Scan',fg=color_text,bg=color_btn,relief='ridge')
    Btn_function_2 = tk.Button(root, width=14, text='Police',fg=color_text,bg=color_btn,relief='ridge')
    Btn_function_3 = tk.Button(root, width=14, text='Automatic',fg=color_text,bg=color_btn,relief='ridge')
    Btn_function_4 = tk.Button(root, width=14, text='Track Line',fg=color_text,bg=color_btn,relief='ridge')
    Btn_function_5 = tk.Button(root, width=14, text='Light Tracking',fg=color_text,bg=color_btn,relief='ridge')
    Btn_function_6 = tk.Button(root, width=14, text='Matrix Screen',fg=color_text,bg=color_btn,relief='ridge')
    Btn_function_7 = tk.Button(root, width=14, text='Keep Distance',fg=color_text,bg=color_btn,relief='ridge')
    # Btn_function_8 = tk.Button(root, width=14, text='Buzzer',fg=color_text,bg=color_btn,relief='ridge')
    Btn_function_1.place(x=x,y=y)
    Btn_function_2.place(x=x,y=y+35)
    Btn_function_3.place(x=x,y=y+70)
    Btn_function_4.place(x=x,y=y+105)
    Btn_function_5.place(x=x,y=y+140)
    Btn_function_6.place(x=x,y=y+175)
    Btn_function_7.place(x=x,y=y+210)
    # Btn_function_8.place(x=x,y=y+245)

    Btn_function_1.bind('<ButtonPress-1>', call_function_1)
    Btn_function_2.bind('<ButtonPress-1>', call_function_2)
    Btn_function_3.bind('<ButtonPress-1>', call_function_3)
    Btn_function_4.bind('<ButtonPress-1>', call_function_4)
    Btn_function_5.bind('<ButtonPress-1>', call_function_5)
    Btn_function_6.bind('<ButtonPress-1>', call_function_6)
    Btn_function_7.bind('<ButtonPress-1>', call_function_7)
    # Btn_function_8.bind('<ButtonPress-1>', call_function_8)
    Btn_function_9 = tk.Button(root, width=14, text='Stop Function',fg=color_text,bg=color_btn,relief='ridge')
    Btn_function_9.place(x=x+120,y=y+140)
    Btn_function_9.bind('<ButtonPress-1>', call_function_9)

def loop():
    global root, var_Speed, var_R_L, var_G_L, var_B_L, var_0, var_1, var_2, var_lip1, var_lip2, var_err, var_R, var_G, var_B, var_ec
    root = tk.Tk()            
    root.title('4WD Smart Car Kit  GUI')      
    root.geometry('620x570')  
    root.config(bg=color_bg)  

    var_Speed = tk.StringVar()
    var_Speed.set(100)

    try:
        logo =tk.PhotoImage(file = 'logo.png')
        l_logo=tk.Label(root,image = logo,bg=color_bg)
        l_logo.place(x=30,y=13)
    except:
        pass

    motor_buttons(30,135)
    information_screen(330,-80)
    connent_input(125,15)
    servo_buttons(255,135)
    function_buttons(390,275)
    ultrasonic_radar(30,290)

    root.mainloop()

if __name__ == '__main__':
    global_init()
    loop()