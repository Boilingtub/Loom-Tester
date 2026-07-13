from glob import glob
import tkinter as tk 
from tkinter import BOTH, ttk
from tkinter import filedialog
import coms 
import loom

lug_lines = 2 
lug_line_height = 1 
lug_line_width = 8 
lug_dot_size = 20
lug_last_x = 0



board_pad_x = 25
board_pad_y = 25

board_lines = 2 
board_line_height = 2 
board_line_width = 14
board_dot_size = 5

lug_lines = 2 
lug_line_height = 1 
lug_line_width = 8 
lug_dot_size = 20
lug_last_x = 0

working_loom:loom.Loom
canvas:tk.Canvas 
txt_info:tk.Text
port_name:str

def reset_canvas():
    global canvas
    canvas.delete('all')
    i = 0


    global lug_last_x
    lug_last_x = board_pad_x*(1+lug_line_width)+lug_line_width*lug_dot_size

    for x in range(board_lines):
        for y in range(board_line_height):
            for z in range(board_line_width):
                dot_x = lug_last_x+board_pad_x*(1+z)
                dot_y = board_pad_y*(1+y+(x*board_line_height*2))
                canvas.create_oval(dot_x,
                                   dot_y,
                                   dot_x + board_dot_size,
                                   dot_y + board_dot_size,
                                   fill='black'
                )
                canvas.create_text(int(dot_x + board_dot_size/2),
                                   dot_y + (board_dot_size*(1+(-1)**y))*(-1)**y + (-1+(-1)**y)*3,
                                   font=("Arial, " + str(board_dot_size)),
                                   text=str(i)
                )
                i=i+1
                    
    for x in range(lug_lines):
        for y in range(lug_line_height):
            for z in range(lug_line_width):
                lug_x = board_pad_x*(1+z)+z*lug_dot_size
                lug_y = board_pad_y*(1+y+(x*board_line_height*2)) + board_dot_size
                canvas.create_oval(lug_x,
                                   lug_y,
                                   lug_x + lug_dot_size,
                                   lug_y + lug_dot_size,
                                   fill='black'
                )
                canvas.create_text(int(lug_x + lug_dot_size/2),
                                   lug_y + ((lug_dot_size+5)*((-1)**y))*(-1)**y + (-1+(-1)**y),
                                   font=("Arial, " + str(board_dot_size)),
                                   text=str(i)
                )
                i=i+1

def main():
    root = tk.Tk()
    root.title("Loom Tester")
    root.configure(background="white")
    root.geometry("960x540+50+50")



    frm_top = tk.Frame(root, background='white')
    btn_json = tk.Button(root,
                         text="Open JSON",
                         command=choose_loom,
                         activebackground="blue",
                         activeforeground="lightgray",
                         anchor="center",
                         bd=3,
                         bg="white",
                         cursor="hand2",
                         disabledforeground="gray",
                         fg="black",
                         font=("Arial, 11"),
                         height=1,
                         highlightbackground="black",
                         highlightcolor="green",
                         highlightthickness=2,
                         justify="center",
                         overrelief="raised",
                         padx=2,
                         pady=1,
                         width=10,
                         wraplength=100)

    serial_devices = coms.list_serial_devices();
    cb_serial = ttk.Combobox(root,
                             values=serial_devices,
                             
                             height=3,
                             width=12,
                             font=("Arial, 12")
                             )
    cb_serial.bind('<Return>', lambda event, entry=cb_serial, text=cb_serial.get():
                                    choose_serial(entry.get(), entry=text))
    cb_serial.bind('<<ComboboxSelected>>', lambda event, entry=cb_serial, text=cb_serial.get():
                                                choose_serial(entry.get(), entry=text))
    cb_serial.set("Serial Port")

    # btn_coms_test = tk.Button(root,
    #                      text="Test Serial",
    #                      command=test_serial_coms,
    #                      activebackground="blue",
    #                      activeforeground="lightgray",
    #                      anchor="center",
    #                      bd=3,
    #                      bg="white",
    #                      cursor="hand2",
    #                      disabledforeground="gray",
    #                      fg="black",
    #                      font=("Arial, 11"),
    #                      height=1,
    #                      highlightbackground="black",
    #                      highlightcolor="green",
    #                      highlightthickness=2,
    #                      justify="center",
    #                      overrelief="raised",
    #                      padx=2,
    #                      pady=1,
    #                      width=10,
    #                      wraplength=100)

    btn_go = tk.Button(root,
                         text="Go",
                         command=test_loom,
                         activebackground="blue",
                         activeforeground="lightgray",
                         anchor="center",
                         bd=3,
                         bg="white",
                         cursor="hand2",
                         disabledforeground="gray",
                         fg="black",
                         font=("Arial, 11"),
                         height=1,
                         highlightbackground="black",
                         highlightcolor="green",
                         highlightthickness=2,
                         justify="center",
                         overrelief="raised",
                         padx=2,
                         pady=1,
                         width=5,
                         wraplength=100)


    width = ((board_pad_x*((board_line_width+1)+(lug_line_width))+(lug_dot_size*lug_line_width+board_pad_x)+board_dot_size))
    global canvas
    canvas = tk.Canvas(root,
                       width=width,
                       height=(board_pad_y*(board_line_height*board_lines*2-1)+board_dot_size),
                       background='white')
    reset_canvas()

    global txt_info
    txt_info = tk.Text(root,width=width)
    txt_info.insert(tk.INSERT,"Loom tester started succesfully\n")

    frm_top.pack(anchor='nw',side="top")
    btn_json.pack(in_=frm_top, padx=5,pady=5,anchor="nw",side="left")
    cb_serial.pack(in_=frm_top, padx=5, pady=5, anchor='nw', side="left")
    #btn_coms_test.pack(in_=frm_top, padx=5,pady=5,anchor="nw",side="left")
    btn_go.pack(in_=frm_top, padx=5,pady=5,anchor="nw",side="left")
    canvas.pack(padx=5,pady=5, anchor='w')
    txt_info.pack(padx=5,pady=5, anchor='w', expand=1, fill=BOTH)



    root.mainloop()

def choose_serial(updated_entry, entry):
    print("CHOOSING SERIAL AS: " + updated_entry)
    global port_name
    port_name = updated_entry
    txt_info.insert(tk.END,"Succesfully assigned " + updated_entry + "\n")
    return

def test_serial_coms():
    global port_name
    print("Testing Serial Coms: " + port_name)
    coms.diagnostic_test(port_name)
    return


def choose_loom():
    reset_canvas()
    path = filedialog.askopenfilename()
    global working_loom
    working_loom = loom.json_to_loom(path)
    #print(working_loom)
    global txt_info 
    txt_info.insert(tk.END,"Succesfully opened " + path + "\n")

    assign_loom(working_loom)


def assign_loom(loom:loom.Loom):
    global canvas
    last_x:list[int] = [0]*board_lines
    last_y:list[int] = [0]*board_line_height
    last_lug_x:list[int] = [0]*lug_lines
    cur_line:list[int] = [0]
    cur_lug_line:list[int] = [0]
    drawn_ids:list[str] = []
    for c1 in loom.connectors.values():
        assign_parent_connector(True, loom, c1, drawn_ids, last_x, last_y, last_lug_x, cur_lug_line, cur_line)



def assign_parent_connector(primary:bool, l:loom.Loom, c1:loom.Connector, drawn_ids:list[str], last_x:list[int],
                            last_y:list[int], last_lug_x:list[int], cur_lug_line:list[int], cur_line:list[int]):
    match c1.shape:
        case "Molex":
            if c1.id not in drawn_ids:
                draw_molex(c1.width, c1.height, c1.id, last_x[cur_line[0]], last_y[cur_line[0]], cur_line[0])
                assign_molex(c1.width, c1.height, c1, last_x[cur_line[0]], last_y[cur_line[0]], cur_line[0])
            last_x[cur_line[0]] = last_x[cur_line[0]]+c1.width+1
            last_y[cur_line[0]] = (last_y[cur_line[0]]+c1.height) % board_line_height
            if primary:
                cur_line[0] = (cur_line[0]+1) % board_lines

        case "junction":
            if c1.id not in drawn_ids:
                draw_junction(c1.width, c1.height, c1.id, last_x[cur_line[0]], last_y[cur_line[0]], cur_line[0])
                assign_junction(c1.width, c1.height, c1, last_x[cur_line[0]], last_y[cur_line[0]], cur_line[0])
            last_y[cur_line[0]] = last_y[cur_line[0]]+c1.height
            if last_y[cur_line[0]] > board_line_height:
                last_x[cur_line[0]] = last_x[cur_line[0]] + c1.width 
                last_y[cur_line[0]] = 0
            # cur_line[0] = (cur_line[0]+1) % board_lines

        case "lug":
            if c1.id not in drawn_ids:
                draw_lug(c1.width, c1.height, c1.id, last_lug_x[cur_lug_line[0]], cur_lug_line[0])
                assign_lug(c1.width, c1.height, c1, last_lug_x[cur_lug_line[0]], cur_lug_line[0])
            last_lug_x[cur_lug_line[0]] = last_lug_x[cur_lug_line[0]] + 1
            if last_lug_x[cur_lug_line[0]] >= lug_line_width:
                cur_lug_line[0] = (cur_lug_line[0]+1) % lug_lines 
            

    to_ids: list[str] = []
    for c2 in c1.contacts.values():
        for c3 in c2.connections:
            if c3.connector.id not in to_ids:
                to_ids.append(c3.connector.id)
    for id in to_ids:
        #print("Assigning: " + id + " as connection to " + c1.id);
        assign_parent_connector(False, l, l.connectors[id], drawn_ids, last_x, last_y, last_lug_x, cur_lug_line, cur_line)
    drawn_ids.extend(to_ids)
            

def draw_junction(width:int, height:int, id:str, x_offset:int, y_offset:int, cur_line:int):
    x0 = lug_last_x+(board_pad_x*x_offset)+(board_pad_x - board_dot_size)     
    x1 = lug_last_x+(board_pad_x*x_offset)+(board_pad_x*width + board_dot_size*2)
    y_base = (cur_line*board_pad_y*board_line_height*2)+board_pad_y*y_offset
    y0 = y_base + (board_pad_y - board_dot_size)
    y1 = y_base + (board_pad_y*height + board_dot_size*2)
    canvas.create_rectangle(x0,y0,x1,y1)              
    canvas.create_text((x0+x1)/2, (y0+y1)/2+(board_dot_size*3.5*(-1)**(y_offset+1)), text=id)
    return

def assign_junction(width:int, height:int, con:loom.Connector, x_offset:int, y_offset:int, cur_line:int):
    pn = cur_line*board_line_width*board_line_height + y_offset*board_line_width + x_offset
    con.contacts[""].pin_num = pn
    #print("Assigning: " + con.id + " to " + str(pn));
    #print("Assigning: " + con.id);  
    return

def draw_lug(width:int, height:int, id:str, x_offset:int, cur_line:int):
    x_base = ((board_pad_x+lug_dot_size)*x_offset)+lug_dot_size/2
    x0 = x_base+(board_pad_x - lug_dot_size/1.2)     
    x1 = x_base+((board_pad_x+lug_dot_size/1.2)*width)
    y_base = cur_line*board_pad_y*board_line_height*2+lug_dot_size
    y0 = y_base + (board_pad_y - lug_dot_size/1.2)
    y1 = y_base + ((board_pad_y+lug_dot_size/1.2)*height)
    canvas.create_oval(x0,y0,x1,y1)              
    canvas.create_oval(x0-2,y0-2,x1+2,y1+2) 
    canvas.create_text((x0+x1)/2, y0+(board_dot_size*2*(-1)), text=id)


def assign_lug(width:int, height:int, con:loom.Connector, x_offset:int, cur_line:int):
    pn = board_lines*board_line_height*board_line_width + x_offset + cur_line*lug_line_width
    con.contacts[con.id].pin_num = pn
    print("Assigning: " + con.id + " to " + str(pn));
    return


def draw_molex(width:int, height:int, id:str, x_offset:int, y_offset:int, cur_line:int):
    x0 = lug_last_x+(board_pad_x*x_offset)+(board_pad_x - board_dot_size)     
    x1 = lug_last_x+(board_pad_x*x_offset)+(board_pad_x*width + board_dot_size*2)
    y_base = cur_line*board_pad_y*board_line_height*2
    y0 = y_base + (board_pad_y - board_dot_size)
    y1 = y_base + (board_pad_y*height + board_dot_size*2)
    latch_x0 = (x0+x1)/2 - board_pad_x/3
    latch_x1 = (x0+x1)/2 + board_pad_x/3
    latch_y0 = y_base + (board_pad_y - board_dot_size*2)
    latch_y1 = y_base + (board_pad_y - board_dot_size)
    canvas.create_rectangle(x0,y0,x1,y1)              
    canvas.create_rectangle(latch_x0,latch_y0,latch_x1,latch_y1)
    canvas.create_text((x0+x1)/2, y1+board_dot_size*2, text=id)
    return

def assign_molex(width:int, height:int, con:loom.Connector, x_offset:int, y_offset:int, cur_line:int):
    pn:int = (cur_line*board_line_width*board_line_height)+(x_offset+width-1)+(y_offset+height-1)*board_line_width 
    i = 0
    for c in con.contacts.values():
        if (i > width-1):
            i = 0
            pn = pn - board_line_width
        c.pin_num = pn-i
        i = i+1
        #print("Assigning: " + c.id + " to " + str(c.pin_num));
    return


def test_loom():
    global working_loom
    serial_outputs: list[int] = []
    output_contacts: list[loom.Contact] = []
    input_contacts: list[loom.Contact] = []
    serial_inputs:list[int] = []
    for c1 in working_loom.connectors.values():
        for c2 in c1.contacts.values():
            if len(c2.connections) != 0:
                serial_outputs.append(c2.pin_num)
                output_contacts.append(c2)
                for c3 in c2.connections:
                    serial_inputs.append(c3.contact.pin_num)
                    input_contacts.append(c3.contact)
    conf_serial: list[int] = [0xFF,0xFF]
    conf_serial.append(len(serial_outputs))
    conf_serial.extend(serial_outputs)
    conf_serial.extend(serial_inputs)
    #print(conf_serial)
    global port_name
    com = coms.do_conf(port_name, conf_serial)
    results = coms.do_go(com)
    check_test_results(results, output_contacts, input_contacts)
    return

def check_test_results(results:str, output_contacts:list[loom.Contact], input_contacts:list[loom.Contact]):
    print(results)
    decode_results: list[list[int]] = []
    global txt_info
    res1 = results.split("\n")
    for r in res1:
        if len(r) > 1 and r != "go ACK":
            res2 = r.split(":")
            decode_results.append([int(res2[0])])
            if ',' in r:
                ins = [element for element in res2[1].split(",") if "" != element]
                decode_results[len(decode_results)-1].extend(list(map(int,ins)))

    break_str = "-"*50
    txt_info.insert(tk.END,break_str + "\n")

    for i,c1 in enumerate(output_contacts):
        if c1.pin_num != decode_results[i][0]:
            print("Warning Transmission order different, possible transmission data corruption")
        conf: list[int] = []
        conf_data: list[str] = []

        for c2 in c1.connections:
            conf.append(c2.contact.pin_num)
            conf_data.append(c2.connector.id + c2.contact.id)

        meas: list[int] = decode_results[i][1:] 
        color:str = ""
        output_str:str = ""

        
        if meas != conf:
            meas_names = []
            for m in meas:
                found:bool = False
                for i,c3 in enumerate(input_contacts):
                    print(c3)
                    if c3.pin_num == m:
                        meas_names.append(c3.connector.id + c3.id)
                        found = True
                        break
                if found == False:
                    meas_names.append(m)

                
            output_str = output_str + ("Expected " + str(c1.connector.id) + str(c1.id) +
                                       " -> " + str(conf_data) + " but Measured " +
                                       str(c1.connector.id) + str(c1.id) + " -> " +
                                       str(meas_names) + "\n")
            color = "red"
        else:
            output_str = output_str + (str(c1.connector.id) + c1.id + " Correct !" + "\n")
            color = "green"

        txt_info.insert(tk.END,output_str);
        text_lines_count = int(txt_info.index('end-1c').split('.')[0])-1
        #print(str(text_lines_count))
        txt_info.tag_add("err-"+str(text_lines_count),str(text_lines_count)+".0",'end')
        txt_info.tag_config("err-"+str(text_lines_count), foreground=color)
        txt_info.see("end")
        #print(output_str)

    return

main()
