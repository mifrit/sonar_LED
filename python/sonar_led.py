import tkinter as tk
from tkinter import ttk


class App(tk.Tk):
    def __init__(self):
        super().__init__()

        # configure the root window
        self.title('sonar LED sequence construction set')
        self.geometry('400x800')

        # led-grid
        self.led_grid = tk.Canvas(self, width=400, height=400, bg='white')
        self.led_grid.pack(side=tk.TOP)
        for i in range(64):
            x0 = 50*(i%8)
            y0 = 50*(int(i/8))
            self.led_grid.create_rectangle((x0, y0), (x0+50, y0+50), fill='#000000', outline='#f0f0f0', width=4)
        self.led_grid.bind('<Button>', self.grid_click)
        #self.sp1 = ttk.Separator(self, orient=tk.HORIZONTAL)
        #self.sp1.pack(side=tk.TOP, pady=10)
        self.l1 = tk.Label(self, text='L1', bg='red', fg='white')
        self.l2 = tk.Label(self, text='L2', bg='red', fg='white')
        self.l3 = tk.Label(self, text='L3', bg='red', fg='white')
        self.l4 = tk.Label(self, text='L4', bg='red', fg='white')
        self.l1.pack(side=tk.TOP, expand=False)
        self.l2.pack(side=tk.LEFT, expand=False)
        self.l3.pack(side=tk.TOP, expand=False)
        self.l4.pack(side=tk.LEFT, expand=False)



        # color grid

    def grid_click(self, event):
        led_nr = int(event.y / 50) * 8 + int(event.x/50)
        if event.num == 1:
            self.led_grid.itemconfig(led_nr+1, fill='#ffffff')
        elif event.num == 2:
            for i in range(64):
                self.led_grid.itemconfig(i+1, fill='#000000')
        elif event.num == 3:
            self.led_grid.itemconfig(led_nr+1, fill='#000000')
            

        
    

if __name__ == "__main__":
  app = App()
  app.mainloop()