from tkinter import *
import tkinter as tk
import math
import cmath

class ini(tk.Frame):
    def __init__(self, parent, size=100):
        tk.Frame.__init__(self, parent, padx=0, pady=0, borderwidth=0,highlightthickness=0,)
        self.size = size
    
    def to_absolute(self, x, y):
        return x + self.size/2, y + self.size/2

    

def draw_needle(canv,x0,y0,degree, t,r):
    this_color = "#00A2E8"
    xr=x0
    yr=y0
    angle = math.radians(degree)
    cos_val = math.cos(angle)
    sin_val = math.sin(angle)
    dy=r*sin_val
    dx=r*cos_val
    dx2=t*sin_val
    dy2=t*cos_val
    mlx=xr+dx
    mly=yr-dy
    mrx=xr-dx
    mry=yr+dy
    px=xr+dx2
    py=yr+dy2
    xy = x0-r,y0-r,x0+1*r,y0+1*r
    xyz = mlx,mly,px,py,mrx,mry
    canv.delete('dial')
    canv.create_arc(xy,start=degree,extent=180,fill=this_color,tags=('dial', 'one', 'two', 'three', 'four'))
    canv.create_polygon(xyz, fill=this_color,tags=('dial', 'two'))
    canv.create_oval(xr-5,yr-5,xr+5,yr+5,fil=this_color,tags=('dial', 'three'))
    

class DrawGauge(ini):
    def __init__(self, parent,
                 max_value: (int)=100.0,
                 min_value: (int)= 0.0,
                 size: (int)=100,
                 img_data: str=None,
                 bg_col:str='gray',
                 unit: str=None,
                 **options):
        super().__init__(parent, size=size, **options)

        self.max_value = int(max_value)
        self.min_value = int(min_value)
        self.size = size
        self.bg_col = bg_col
        
        self.unit = '' if not unit else unit
        self.canvas = tk.Canvas(self, 
                                width=self.size,
                                height=self.size-self.size/12,
                                bg=bg_col,
                                highlightthickness=0)
        self.canvas.grid(row=0)
        
        self.draw_background()
        self.draw_tick()

    def draw_background(self, divisions=100):
        
        self.canvas.create_arc(self.size/5, self.size/6, self.size-self.size/6, self.size-self.size/6,
                            style="arc",width=self.size/10,start=-61, extent=61,
                            outline = "red")
        
        self.canvas.create_arc(self.size/6, self.size/6, self.size-self.size/6, self.size-self.size/6,
                               width=self.size/10,style="arc",start=240, extent=-240,
                               outline = "green")
                
        self.readout = self.canvas.create_text(self.size/2,4*self.size/5, font=("Arial",int(self.size/18),'bold'),fill="white", text='')
    
    def draw_tick(self,divisions=100):
        
        label = self.unit
        self.canvas.create_text(self.size/2,2*self.size/5, font=("Arial",int(self.size/20)),fill="white", text=label,angle=0)
        
        self.readout = self.canvas.create_text(self.size/2,4*self.size/5, font=("Arial",int(self.size/18),'bold'),fill="white", text='')
        
        inner_tick_radius2 = int((self.size-self.size/9) * 0.48)
        outer_tick_radius2 = int((self.size-self.size/9) * 0.50)
        inner_tick_radius3 = int((self.size-self.size/9) * 0.35)
        outer_tick_radius3 = int((self.size-self.size/9) * 0.40)

        for tick in range(divisions+1):
            inner_tick_radius = int((self.size-self.size/9) * 0.35)# çentiğin alt ucu (baslangıç ucu)
            outer_tick_radius = int((self.size-self.size/9) * 0.45)# çentiğin ust ucu (bitiş ucu)

            angle_in_radians = (2.0 * cmath.pi / 3.0)+tick/divisions * (5.0 * cmath.pi / 3.0)
            inner_point = cmath.rect(inner_tick_radius, angle_in_radians)
            outer_point = cmath.rect(outer_tick_radius, angle_in_radians)

            if (tick%10) == 0: # ana Tırtıklar 
                self.canvas.create_line(
                    *self.to_absolute(inner_point.real, inner_point.imag),
                    *self.to_absolute(outer_point.real, outer_point.imag),
                    width=2,fill='blue')
            
            if (tick%10) == 0: # ana tırtıkların değerleri
                inner_point2 = cmath.rect(inner_tick_radius2, angle_in_radians)
                outer_point2 = cmath.rect(outer_tick_radius2, angle_in_radians)
                x= outer_point2.real + self.size/2
                y= outer_point2.imag + self.size/2
                label = str(int(self.min_value + tick * (self.max_value-self.min_value)/100))
                self.canvas.create_text(x,y, font=("Arial",int(self.size/25)),fill="white", text=label)

            else: # ara tırtıklar 
                inner_point3 = cmath.rect(inner_tick_radius3, angle_in_radians)
                outer_point3 = cmath.rect(outer_tick_radius3, angle_in_radians)
                self.canvas.create_line(
                    *self.to_absolute(inner_point3.real, inner_point3.imag),
                    *self.to_absolute(outer_point3.real, outer_point3.imag),
                    width=1,fill='black')
    
    def set_value(self, number: (int)):
        number = number if number <= self.max_value else self.max_value
        number = number if number > self.min_value else self.min_value

        degree = 30.0 + (number- self.min_value) / (self.max_value - self.min_value) * 300.0 # değer

        draw_needle(self.canvas,
                    self.size/2,
                    self.size/2,
                    -1*degree,
                    self.size/3,
                    8)

        label = str('%.2f' % number)
        self.canvas.delete(self.readout)
        self.readout = self.canvas.create_text(self.size/2,4*self.size/5, font=("Arial",int(self.size/14),'bold'),fill="white", text=label,angle=0)
            

