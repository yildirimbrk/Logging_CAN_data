import sys
from tkinter import *
import serial
import threading
import gauge
import time

def serial_ports():
    #PC üzerinde oluşturulan serial portları bulur
    if sys.platform.startswith('win'):
        ports = ['COM%s' % (i + 1) for i in range(256)]

    result = []
    for port in ports:
        try:
            s = serial.Serial(port)
            s.close()
            result.append(port)

        except (OSError, serial.SerialException):
            pass
    return result

def connect_menu_init():
    global mainWindow, connect_btn, refresh_btn, graph, frame_serialInit, ReceivedDataBox,transmiteDataBox,g1,g2
    #Ana pencereyi oluştur
    mainWindow = Tk()
    mainWindow.title("OBD2 Dash Board")
    mainWindow.geometry("800x700")
    mainWindow.config(bg="white")
    
    #Serial bağlantının kurulması için gereken çerçeve oluştur
    frame_serialInit = Frame(mainWindow, bg='gray')
    frame_serialInit.place(relx=0.01, rely=0.01, relwidth=0.95, relheight=0.1)

    connect_btn = Button(frame_serialInit, text="Connect", height=2,
                         width=7, state="disabled", command=connexion)
    connect_btn.grid(row=0,column=0,pady = 20,padx=10)
    
    port_lable = Label(frame_serialInit, text="Port")
    port_lable.grid(row=0,column=1,pady = 5,padx=5)
    update_coms()

    refresh_btn = Button(frame_serialInit, text="R", height=1,
                         width=3, command=update_coms)
    refresh_btn.grid(row=0,column=3,pady = 20,padx=10)
    
    port_bd = Label(frame_serialInit, text="Baude Rate")
    port_bd.grid(row=0,column=4,pady = 20,padx=5)
    baud_select()

    #ölçü göstergenin bulunduğu çerçeve
    frame_serialDataGauge = Frame(mainWindow, bg='gray') 
    frame_serialDataGauge.place(relx=0.01, rely=0.12, relwidth=0.95, relheight=0.3) 
    
    speedValue = Checkbutton(frame_serialDataGauge)
    
    g1 = gauge.DrawGauge(frame_serialDataGauge,
                        max_value = 255.0,
                        min_value = 0.0,
                        size=200,
                        unit = "SPEED")
    g1.pack(side=LEFT)

    g2 = gauge.DrawGauge(frame_serialDataGauge,
                        max_value = 16383.75,
                        min_value = 0.0,
                        size=200,
                        unit = "RPM")
    g2.pack(side=LEFT)
    
    #gelen giden Datanın kontrol çerçevesi
    frame_SerialDataTextBox = Frame(mainWindow, bg='gray') 
    frame_SerialDataTextBox.place(relx=0.01, rely=0.43, relwidth=0.95, relheight=0.3)

    label_TransmiteDataBox = Label(frame_SerialDataTextBox, text='Transmite Data', font='Verdana 8 bold', foreground="#FF0000", background="gray")
    label_TransmiteDataBox.grid(padx=10, pady=10 ,sticky=W ,row=1,column=1)
    
    transmiteDataBox = Entry(frame_SerialDataTextBox,bd=5, width=85)
    transmiteDataBox.grid(sticky=W,padx=10,row=2,column=1)
    
    
    tranmiteSend_btn = Button(frame_SerialDataTextBox, text="Send", height=1,
                         width=5, command=SendTexBox)
    tranmiteSend_btn.grid(sticky=W,row=2,column=2)
    
    label_ReceivedDataBox = Label(frame_SerialDataTextBox, text='Received Data', font='Verdana 8 bold', foreground="#FF0000", background="gray")
    label_ReceivedDataBox.grid(padx=10, pady=10 ,sticky=W,row=3,column=1)

    clearReceiveData_btn = Button(frame_SerialDataTextBox, text="Clear received", height=1,
                         width=10, command=ClearTextBox)
    clearReceiveData_btn.grid(sticky=W,row=4,column=2)

    ReceivedDataBox = Text(frame_SerialDataTextBox, width=65, height=5)
    ReceivedDataBox.insert(INSERT, 'Received Data')
    ReceivedDataBox.grid(sticky=W,padx=10,row=4,column=1)

def connect_check(args):
    #bağlantı kontrolü baud ve port seçilirse bağlantı butonu açılır
    if "-" in clicked_com.get() or "-" in clicked_bd.get():
        connect_btn["state"] = "disable"
    else:
        connect_btn["state"] = "active"

def update_coms():
    #port menusunu olusturur, menu varsa silinir tekrar oluşturulur
    global clicked_com, drop_COM
    coms=serial_ports()
    coms.insert(0, "-")
    try:
        drop_COM.destroy()
    except:
        pass
    clicked_com = StringVar()
    clicked_com.set(coms[0])
    drop_COM = OptionMenu(frame_serialInit, clicked_com, *coms, command=connect_check)
    drop_COM.config(width=20)
    drop_COM.grid(row=0,column=2)
    connect_check(0)

def baud_select():
    #baud menusu oluşturulur
    global clicked_bd, drop_bd
    clicked_bd = StringVar()
    bds = ["-",
           "300",
           "600",
           "1200",
           "2400",
           "4800",
           "9600",
           "14400",
           "19200",
           "28800",
           "38400",
           "56000",
           "57600",
           "115200",
           "128000",
           "256000"]
    clicked_bd.set(bds[0])
    drop_bd = OptionMenu(frame_serialInit, clicked_bd, *bds, command=connect_check)
    drop_bd.config(width=20)
    drop_bd.grid(row=0,column=5,pady = 10,padx=10)

def connexion():
    #bağlantı kurulur
    global ser, serialData
    if connect_btn["text"] in "Disconnect":
        serialData = False
        connect_btn["text"] = "Connect"
        refresh_btn["state"] = "active"
        drop_bd["state"] = "active"
        drop_COM["state"] = "active"

    else:
        serialData = True
        connect_btn["text"] = "Disconnect"
        refresh_btn["state"] = "disable"
        drop_bd["state"] = "disable"
        drop_COM["state"] = "disable"
        port = clicked_com.get()
        baud = clicked_bd.get()
        try:
            ser = serial.Serial(port, baud, timeout=1)
        except:
            pass
        t1 = threading.Thread(target=readSerial)
        t1.deamon = True
        t1.start()

        #t2 = threading.Thread(target=emulatorSend)
        #t2.deamon = True
        #t2.start()

def SendTexBox():
    #gönderi kutusu
    global transmiteDataBox, ser
    sendMsg = transmiteDataBox.get()
    sendMsg = sendMsg + '\r\n'
    ser.write(str(sendMsg).encode('utf-8'))

def ReadmsgBox(receivedData):
    #gelen kutusu
    global ReceivedDataBox
    ReceivedDataBox.insert(INSERT, receivedData)
    ReceivedDataBox.grid(sticky=W,padx=10,row=4,column=1)

def ClearTextBox():
    #gelen kutusunu temizle
    global ReceivedDataBox
    ReceivedDataBox.delete('1.0', END)


def readSerial():
    global serialData, graph
   
    while serialData:
        data = ser.readline()
        if len(data) > 0:
            try:
                sensor = data.decode('utf8').split('\r\n')
                print(sensor)
                head = sensor[0].split(' ')
                if head[0] == '41' :
                    if head[1] == 'd':
                        A = int(head[2],16)
                        speed = A
                        print(f'speed : {speed}')
                    if head[1] == 'c':
                        A = int(head[2],16)
                        B = int(head[3],16)
                        rpm = (256*A + B) / 4
                        print(f'rpm :{rpm}')

                g1.set_value(speed)
                g2.set_value(rpm)
                    
                

            except:
                pass

                
#def emulatorSend():
# simülasyon programı kullanırken veri talebi oluşturma
#    global serialData
#    while serialData:
#        try:
#            rpmPıd = "01 0C"
#            speedPıd = "01 0D"
#            speedPıd = speedPıd + '\r\n'
#            rpmPıd = rpmPıd + '\r\n'
#            ser.write(str(speedPıd).encode('utf-8'))
#            time.sleep(1)
#            ser.write(str(rpmPıd).encode('utf-8'))
#            time.sleep(1)
#        except:
#            pass
                

connect_menu_init()
mainloop()

