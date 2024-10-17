from tkinter import messagebox

import serial
from tkinter import *
import tkinter as tb
import PIL
from PIL import Image, ImageTk
import threading

def rounded_rectangle(canvas, x1, y1, x2, y2, radius=25, **kwargs):
    points = [x1+radius, y1,
              x1+radius, y1,
              x2-radius, y1,
              x2-radius, y1,
              x2, y1,
              x2, y1+radius,
              x2, y1+radius,
              x2, y2-radius,
              x2, y2-radius,
              x2, y2,
              x2-radius, y2,
              x2-radius, y2,
              x1+radius, y2,
              x1+radius, y2,
              x1, y2,
              x1, y2-radius,
              x1, y2-radius,
              x1, y1+radius,
              x1, y1+radius,
              x1, y1]
    return canvas.create_polygon(points, smooth=True, **kwargs)

# Configurar la ventana principal
root = Tk()
root.title("Sistema de Temperatura")
root.geometry("1000x500")

canvas = Canvas(root,background="#8487cc",width=1000,height=500)
canvas.pack()

# Cargar la imagen usando Pillow
image = Image.open("logo.png")  # Cambia la ruta a tu imagen
image = image.resize((50, 50), Image.Resampling.LANCZOS)

image_2 = Image.open("summer.png")
image_3 = Image.open("winter.png")
image_4 = Image.open("fall.png")


#Convertir la imagen a un formato compatible con Tkinter
tk_image = ImageTk.PhotoImage(image)
tk_image_2 = ImageTk.PhotoImage(image_2)
tk_image_3 = ImageTk.PhotoImage(image_3)
tk_image_4 = ImageTk.PhotoImage(image_4)




# Mostrar la imagen en el canvas

#CUADROS BLANCOS-----------------------------------------------------
rec_1_id = rounded_rectangle(canvas,80, 50, 320, 350,radius=30, outline="white", fill="white", width=1)
rec_2_id =rounded_rectangle(canvas,380, 50, 620, 350,radius=30, outline="white", fill="white", width=1)
rec_3_id =rounded_rectangle(canvas,680, 50, 920, 350,radius=30, outline="white", fill="white", width=1)

canvas.itemconfig(rec_3_id, state='hidden')
canvas.itemconfig(rec_2_id, state='hidden')
canvas.itemconfig(rec_1_id, state='hidden')

#----------------------------------------


#CUADROS AZULES------------------------------
rec_azul_1_id=rounded_rectangle(canvas, 100, 100, 300, 250, radius=50, outline="#3b48ed", fill="#3b48ed", width=1)

rec_azul_2_id=rounded_rectangle(canvas, 400, 100, 600, 250, radius=50, outline="#3b48ed", fill="#3b48ed", width=1)

rec_azul_3_id=rounded_rectangle(canvas, 700, 100, 900, 250, radius=50, outline="#3b48ed", fill="#3b48ed", width=1)

canvas.itemconfig(rec_azul_3_id, state='hidden')
canvas.itemconfig(rec_azul_2_id, state='hidden')
canvas.itemconfig(rec_azul_1_id, state='hidden')
#-----------------------------------

#TITULO CUADRO BLANCO---------------------
lable_temperatura_titulo_1 = Label(text="",background="#8487cc", foreground="black",font=("Consolas", 20, "bold"))
lable_temperatura_titulo_1.place(x=115,y=60)

lable_temperatura_titulo_2 = Label(text="",background="#8487cc", foreground="black",font=("Consolas", 20, "bold"))
lable_temperatura_titulo_2.place(x=415,y=60)

lable_temperatura_titulo_3 = Label(text="",background="#8487cc", foreground="black",font=("Consolas", 20, "bold"))
lable_temperatura_titulo_3.place(x=715,y=60)


label_estacion = Label(text="",background="#8487cc", foreground="white",font=("Consolas", 20, "bold"))
label_estacion.place(x=115,y=420);

#--------------------------

# IMAGENES ------------------------------------
imagen_id_1 =canvas.create_image(130, 190, image=tk_image, anchor=NW)
canvas.itemconfig(imagen_id_1, state='hidden')

imagen_id_2 =canvas.create_image(430, 190, image=tk_image, anchor=NW)
canvas.itemconfig(imagen_id_2, state='hidden')

imagen_id_3 =canvas.create_image(730, 190, image=tk_image, anchor=NW)
canvas.itemconfig(imagen_id_3, state='hidden')

imagen_id_summer =canvas.create_image(400,390, image=tk_image_2, anchor=NW)
canvas.itemconfig(imagen_id_summer, state='hidden')

imagen_id_winter =canvas.create_image(400,390, image=tk_image_3, anchor=NW)
canvas.itemconfig(imagen_id_winter, state='hidden')

imagen_id_fall =canvas.create_image(400,390, image=tk_image_4, anchor=NW)
canvas.itemconfig(imagen_id_fall, state='hidden')




#--------------------------

# Crear las etiquetas en la interfaz


label_s1=tb.Label(text=" ",background="#8487cc", foreground="white",font=("consolas", 18, "bold"))
label_s1.place(x=130, y=120)

label_s2=tb.Label(text="SISTEMA OFF",background="#8487cc", foreground="white",font=("consolas", 18, "bold"))
label_s2.place(x=420, y=120)

label_s3=tb.Label(text="",background="#8487cc", foreground="white",font=("consolas", 18, "bold"))
label_s3.place(x=720, y=120)

#-----------------------------------------
#VALOR DE LA TEMPERATURA
label_s1_temp = tb.Label(text="", background="#8487cc", foreground="white", font=("Consolas", 28, "bold")) # Etiqueta para mostrar la temperatura
label_s1_temp.place(x=170, y=190)

label_s2_temp = tb.Label(text="", background="#8487cc", foreground="white", font=("Consolas", 28, "bold")) # Etiqueta para mostrar la temperatura
label_s2_temp.place(x=470, y=190)

label_s3_temp = tb.Label(text="", background="#8487cc", foreground="white", font=("Consolas", 28, "bold")) # Etiqueta para mostrar la temperatura
label_s3_temp.place(x=770, y=190)

#-----------------------------------------


# Intentar abrir el puerto serial
try:
    # Configura el puerto serial (cambia COM6 por el puerto que estés usando)
    ser = serial.Serial('COM6', 9600, timeout=1)
except serial.SerialException:
    # Mostrar un cuadro de diálogo de error si no se puede abrir el puerto
    messagebox.showerror("Error de conexión", "No se pudo conectar al puerto COM6. Asegúrate de que el dispositivo esté conectado.")
    root.destroy()  # Cerrar la ventana principal si no hay conexión
    exit()  # Finalizar el programa


# Función que leerá el puerto serial en un hilo separado
def read_from_port():
    while True:
        if ser.in_waiting > 0:
            # Lee los datos disponibles en el buffer
            data = ser.readline().decode('utf-8').rstrip()
            if data:
                # Iniciar un nuevo hilo para procesar la cadena recibida
                threading.Thread(target=process_data, args=(data,)).start()


# Función para procesar la cadena recibida (en otro hilo)
def process_data(data):
    # Aquí se puede hacer cualquier procesamiento con la cadena
    print(f"Procesando datos: {data}, longitud {len(data)}")

    if(data.count(',')==0):
        processed_data = data
    if(data.count(',')==2):
        datos = data.split(',')
        datos[1]=int(datos[1])
        datos[2] = int(datos[2])
        processed_data = f"{datos[0]},{datos[1]}°C,{datos[2]}"
    if(data.count(',')==3):
        datos = data.split(',')
        datos[1] = int(datos[1])
        datos[2] = int(datos[2])
        datos[3] = int(datos[3])
        processed_data = f"{datos[0]},{datos[1]}°C,{datos[2]}°C ,{datos[3]}"

    if(data.count(',')==4):
        datos = data.split(',')
        datos[1] = int(datos[1])
        datos[2] = int(datos[2])
        datos[3] = int(datos[3])
        datos[4] = int(datos[4])

        processed_data = f"{datos[0]},{datos[1]}°C,{datos[2]}°C,{datos[3]}°C,{datos[4]} "







    # Simulación de procesamiento de datos
    #processed_data = f"Datos procesados: {data}"

    # Llamar a la función de actualización del label si es necesario
    root.after(0, update_label, processed_data)


def ocultar_iconos_ml():
    canvas.itemconfig(imagen_id_winter, state='hidden')
    canvas.itemconfig(imagen_id_summer, state='hidden')
    canvas.itemconfig(imagen_id_fall, state='hidden')

def ocultar_iconos_ml_solo_1(data):
    if(data==1):
        canvas.itemconfig(imagen_id_winter, state='normal')
        canvas.itemconfig(imagen_id_summer, state='hidden')
        canvas.itemconfig(imagen_id_fall, state='hidden')
    elif(data==2):
        canvas.itemconfig(imagen_id_winter, state='hidden')
        canvas.itemconfig(imagen_id_summer, state='hidden')
        canvas.itemconfig(imagen_id_fall, state='normal')
    elif(data==3):
        canvas.itemconfig(imagen_id_winter, state='hidden')
        canvas.itemconfig(imagen_id_summer, state='normal')
        canvas.itemconfig(imagen_id_fall, state='hidden')




# Función para actualizar la etiqueta con los datos procesados
def update_label(data):
    datos = str(data).split(',');
    if (int(datos[0]) != 7):
        label_s1_temp.config( background="#3b48ed")
        label_s2_temp.config(background="#3b48ed")
        label_s3_temp.config(background="#3b48ed")
        label_s1.config(background="#3b48ed")
        label_s2.config( background="#3b48ed")
        label_s3.config(background="#3b48ed")

        canvas.itemconfig(rec_3_id, state='normal')
        canvas.itemconfig(rec_2_id, state='normal')
        canvas.itemconfig(rec_1_id, state='normal')

        canvas.itemconfig(rec_azul_3_id, state='normal')
        canvas.itemconfig(rec_azul_2_id, state='normal')
        canvas.itemconfig(rec_azul_1_id, state='normal')

        lable_temperatura_titulo_1.config(text="Temperatura",background="white")
        lable_temperatura_titulo_2.config(text="Temperatura", background="white")
        lable_temperatura_titulo_3.config(text="Temperatura", background="white")

        canvas.itemconfig(imagen_id_1, state='normal')
        canvas.itemconfig(imagen_id_2, state='normal')
        canvas.itemconfig(imagen_id_3, state='normal')

        ocultar_iconos_ml()



    if(int(datos[0])==0):
        label_s1_temp.config(text="N.C")
        label_s2_temp.config(text="N.C")
        label_s3_temp.config(text="N.C")

        label_s1.config(text="Sensor 1")
        label_s2.config(text="Sensor 2")
        label_s3.config(text="Sensor 3")
        label_estacion.config(text="")
        ocultar_iconos_ml()

    elif (int(datos[0]) == 7):
        label_s1_temp.config(text=" ",background="#8487cc")
        label_s2_temp.config(text=" ",background="#8487cc")
        label_s3_temp.config(text=" ",background="#8487cc")
        label_s1.config(text="",background="#8487cc")
        label_s2.config(text="SISTEMA OFF",background="#8487cc")
        label_s3.config(text="",background="#8487cc")
        canvas.itemconfig(rec_3_id, state='hidden')
        canvas.itemconfig(rec_2_id, state='hidden')
        canvas.itemconfig(rec_1_id, state='hidden')

        canvas.itemconfig(rec_azul_3_id, state='hidden')
        canvas.itemconfig(rec_azul_2_id, state='hidden')
        canvas.itemconfig(rec_azul_1_id, state='hidden')

        lable_temperatura_titulo_1.config(text="", background="#8487cc")
        lable_temperatura_titulo_2.config(text="", background="#8487cc")
        lable_temperatura_titulo_3.config(text="", background="#8487cc")

        canvas.itemconfig(imagen_id_1, state='hidde')
        canvas.itemconfig(imagen_id_2, state='hidde')
        canvas.itemconfig(imagen_id_3, state='hidde')
        label_estacion.config(text="")
        ocultar_iconos_ml()






    elif(int(datos[0])==11):
        label_s1_temp.config(text=datos[1])
        label_s2_temp.config(text="N.C")
        label_s3_temp.config(text="N.C")

        label_s1.config(text="Sensor 1")
        label_s2.config(text="Sensor 2")
        label_s3.config(text="Sensor 3")



        if (int(datos[2]) == 0):
            label_estacion.config(text="INVIERNO")
            ocultar_iconos_ml_solo_1(1)

        if (int(datos[2]) == 1):
            label_estacion.config(text="PRIMAVERA-OTOÑO")
            ocultar_iconos_ml_solo_1(2)
        if (int(datos[2]) == 2):
            label_estacion.config(text="VERANO")
            ocultar_iconos_ml_solo_1(3)

    elif(int(datos[0])==12):
        label_s1_temp.config(text="N.C")
        label_s2_temp.config(text=datos[1])
        label_s3_temp.config(text="N.C")
        label_s1.config(text="Sensor 1")
        label_s2.config(text="Sensor 2")
        label_s3.config(text="Sensor 3")

        if (int(datos[2]) == 0):
            label_estacion.config(text="INVIERNO")
            ocultar_iconos_ml_solo_1(1)

        if (int(datos[2]) == 1):
            label_estacion.config(text="PRIMAVERA-OTOÑO")
            ocultar_iconos_ml_solo_1(2)
        if (int(datos[2]) == 2):
            label_estacion.config(text="VERANO")
            ocultar_iconos_ml_solo_1(3)
    elif (int(datos[0]) == 13):
        label_s1_temp.config(text="N.C")
        label_s2_temp.config(text="N.C")
        label_s3_temp.config(text=datos[1])

        label_s1.config(text="Sensor 1")
        label_s2.config(text="Sensor 2")
        label_s3.config(text="Sensor 3")

        if (int(datos[2]) == 0):
            label_estacion.config(text="INVIERNO")
            ocultar_iconos_ml_solo_1(1)

        if (int(datos[2]) == 1):
            label_estacion.config(text="PRIMAVERA-OTOÑO")
            ocultar_iconos_ml_solo_1(2)
        if (int(datos[2]) == 2):
            label_estacion.config(text="VERANO")
            ocultar_iconos_ml_solo_1(3)

    elif (int(datos[0]) == 212):
        label_s1_temp.config(text=datos[1])
        label_s2_temp.config(text=datos[2])
        label_s3_temp.config(text="N.C")

        label_s1.config(text="Sensor 1")
        label_s2.config(text="Sensor 2")
        label_s3.config(text="Sensor 3")

        if (int(datos[3]) == 0):
            label_estacion.config(text="INVIERNO")
            ocultar_iconos_ml_solo_1(1)

        if (int(datos[3]) == 1):
            label_estacion.config(text="PRIMAVERA-OTOÑO")
            ocultar_iconos_ml_solo_1(2)
        if (int(datos[3]) == 2):
            label_estacion.config(text="VERANO")
            ocultar_iconos_ml_solo_1(3)


    elif (int(datos[0]) == 213):
        label_s1_temp.config(text=datos[1])
        label_s2_temp.config(text="N.C")
        label_s3_temp.config(text=datos[2])

        label_s1.config(text="Sensor 1")
        label_s2.config(text="Sensor 2")
        label_s3.config(text="Sensor 3")
        if (int(datos[3]) == 0):
            label_estacion.config(text="INVIERNO")
            ocultar_iconos_ml_solo_1(1)

        if (int(datos[3]) == 1):
            label_estacion.config(text="PRIMAVERA-OTOÑO")
            ocultar_iconos_ml_solo_1(2)
        if (int(datos[3]) == 2):
            label_estacion.config(text="VERANO")
            ocultar_iconos_ml_solo_1(3)

    elif (int(datos[0]) == 223):
        label_s1_temp.config(text="N.C")
        label_s2_temp.config(text=datos[1])
        label_s3_temp.config(text=datos[2])

        label_s1.config(text="Sensor 1")
        label_s2.config(text="Sensor 2")
        label_s3.config(text="Sensor 3")

        if (int(datos[3]) == 0):
            label_estacion.config(text="INVIERNO")
            ocultar_iconos_ml_solo_1(1)

        if (int(datos[3]) == 1):
            label_estacion.config(text="PRIMAVERA-OTOÑO")
            ocultar_iconos_ml_solo_1(2)
        if (int(datos[3]) == 2):
            label_estacion.config(text="VERANO")
            ocultar_iconos_ml_solo_1(3)

    elif (int(datos[0]) == 3):
        label_s1_temp.config(text=datos[1])
        label_s2_temp.config(text=datos[2])
        label_s3_temp.config(text=datos[3])

        label_s1.config(text="Sensor 1")
        label_s2.config(text="Sensor 2")
        label_s3.config(text="Sensor 3")

        if(int(datos[4])==0):
            label_estacion.config(text="INVIERNO")
            ocultar_iconos_ml_solo_1(1)

        if (int(datos[4]) == 1):
            label_estacion.config(text="PRIMAVERA-OTOÑO")
            ocultar_iconos_ml_solo_1(2)
        if (int(datos[4]) == 2):
            label_estacion.config(text="VERANO")
            ocultar_iconos_ml_solo_1(3)









# Iniciar el hilo para leer desde el puerto serial
thread = threading.Thread(target=read_from_port)
thread.daemon = True  # El hilo se cierra cuando la aplicación termina
thread.start()

# Ejecutar la ventana principal
root.mainloop()

# Cerrar el puerto serial al finalizar
ser.close()
