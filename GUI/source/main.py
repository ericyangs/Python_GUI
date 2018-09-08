import sys
from PyQt5.QtWidgets import *
from PyQt5 import uic
import serial

# <object name --- class>
# -----------------------------------------
#  MainWindow --- QMainWindow -- serial code
#  ^
#  |_____<inheritance>
#  |     | dial           --- QDial
#  |     | label          --- QLabel
#  |     | lcdNumber      --- QLCDNumber
#  |     | toolButton_hit --- QPushButton -- 'k'
#  |     | toolButton_EE  --- QToolButton -- 'd'
#  |     | toolButton_NE  --- QToolButton -- 'i'
#  |     | toolButton_NN  --- QToolButton -- 'a'
#  |     | toolButton_NW  --- QToolButton -- 'g'
#  |     | toolButton_SE  --- QToolButton -- 'j'
#  |     | toolButton_SS  --- QToolButton -- 'b'
#  |     | toolButton_SW  --- QToolButton -- 'h'
#  |     | toolButton_WW  --- QToolButton -- 'c'
#  | statusbar --- QStatusBar
# __________________________________________

# Setting for serial
TX_Code = serial.Serial(port = 'COM3', baudrate = 9600, parity = serial.PARITY_NONE, stopbits = serial.STOPBITS_ONE, bytesize = serial.EIGHTBITS, timeout = 0)
###############################################################################

# call ui file for making object
# "form_class" is the class which loads the ".ui" file in given directory
form_class = uic.loadUiType("main_window.ui")[0] #summon xml(.ui) file and make it instantiated

# basic class for Main Window
# "MyWindow:" class inherits 1) QmainWindow as well as 2) form_class(Multiple Inheritance)
class MyWindow(QMainWindow, form_class):
    def __init__(self): # constructor
        super().__init__() # super class init
        self.setupUi(self) # generate UI
        # each button INIT
        self.toolButton_hit.clicked.connect(self.hit_btn_clicked)
        self.toolButton_EE.clicked.connect(self.EE_btn_clicked)
        self.toolButton_NE.clicked.connect(self.NE_btn_clicked)
        self.toolButton_NN.clicked.connect(self.NN_btn_clicked)
        self.toolButton_NW.clicked.connect(self.NW_btn_clicked)
        self.toolButton_SE.clicked.connect(self.SE_btn_clicked)
        self.toolButton_SS.clicked.connect(self.SS_btn_clicked)
        self.toolButton_SW.clicked.connect(self.SW_btn_clicked)
        self.toolButton_WW.clicked.connect(self.WW_btn_clicked)

    @classmethod # event handler
    def hit_btn_clicked(self):
        #QMessageBox.about(self, "message", "clicked")
        print ("hit button pressed") # for debug
        TX_Code.write(bytes("k\n", encoding = 'ascii'))

    def EE_btn_clicked(self):
        print ("EE") # for debug
        TX_Code.write(bytes("d\n", encoding='ascii'))

    def NE_btn_clicked(self):
        print ("NE") # for debug
        TX_Code.write(bytes("i\n", encoding='ascii'))

    def NN_btn_clicked(self):
        print ("NN") # for debug
        TX_Code.write(bytes("a\n", encoding='ascii'))

    def NW_btn_clicked(self):
        print ("NW") # for debug
        TX_Code.write(bytes("g\n", encoding='ascii'))

    def SE_btn_clicked(self):
        print ("SE") # for debug
        TX_Code.write(bytes("j\n", encoding='ascii'))

    def SS_btn_clicked(self):
        print ("SS") # for debug
        TX_Code.write(bytes("b\n", encoding='ascii'))

    def SW_btn_clicked(self):
        print ("SW") # for debug
        TX_Code.write(bytes("h\n", encoding='ascii'))

    def WW_btn_clicked(self):
        print ("WW") # for debug
        TX_Code.write(bytes("c\n", encoding='ascii'))
###############################################################################

###############################################################################
if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = MyWindow()
    window.show()
    app.exec_()
###############################################################################
