import sys
from PyQt5.QtWidgets import *
from PyQt5 import uic
import serial

# <object name --- class>
# -----------------------------------------
#  MainWindow --- QMainWindow -- serial code
#  |_____<inheritance>
#  |     | dial --- QDial
#  |     | label --- QLabel
#  |     | lcdNumber --- QLCDNumber
#  |     | pushButton_Hit --- QPushButton -- 'k'
#  |     | toolButton_EE --- QToolButton -- 'd'
#  |     | toolButton_NE --- QToolButton -- 'i'
#  |     | toolButton_NN --- QToolButton -- 'a'
#  |     | toolButton_NW --- QToolButton -- 'g'
#  |     | toolButton_SE --- QToolButton -- 'j'
#  |     | toolButton_SS --- QToolButton -- 'b'
#  |     | toolButton_SW --- QToolButton -- 'h'
#  |     | toolButton_WW --- QToolButton -- 'c'
#  |statusbar --- QStatusBar
# __________________________________________

# Setting for serial
AVR_Serial = serial.Serial(port = 'COM3', baudrate = 9600, parity = serial.PARITY_NONE, stopbits = serial.STOPBITS_ONE, bytesize = serial.EIGHTBITS, timeout = 0)
###############################################################################

# call ui file for making object
# "form_class" is the class which loads the ".ui" file in given directory
form_class = uic.loadUiType("main_window.ui")[0]

# basic class for Main Window
# "MyWindow:" class inherits 1) QmainWindow as well as 2) form_class(Multiple Inheritance)
class MyWindow(QMainWindow, form_class):
    def __init__(self): # constructor
        super().__init__() # super class init
        self.setupUi(self) # generate UI
        self.pushButton_Hit.clicked.connect(self.btn_clicked)

    # event handler function
    def btn_clicked(self):
        QMessageBox.about(self, "message", "clicked")
        print ("hit button pressed") # for debug

###############################################################################







###############################################################################
if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = MyWindow()
    window.show()
    app.exec_()
###############################################################################
