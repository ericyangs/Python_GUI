import sys
from PyQt5.QtWidgets import *
from PyQt5 import uic

# call ui file for making object
form_class = uic.loadUiType("../ui/main_window.ui")[0]

# basic class for Main Window
class MyWindow(QMainWindow, form_class):
    def __init__(self): # constructor
        super().__init__() # call super class constructor
        self.setupUi(self) # set Ui
###############################################################################









###############################################################################
if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = MyWindow()
    window.show()
    app.exec_()
###############################################################################