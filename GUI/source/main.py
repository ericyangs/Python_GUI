import sys
from PyQt5.QtWidgets import *
from PyQt5 import uic

# call ui file for making object
# "form_class" is the class which loads the ".ui" file in given directory
form_class = uic.loadUiType("../ui/main_window.ui")[0]

# basic class for Main Window
# "MyWindow:" class inherits 1) QmainWindow as well as 2) form_class(Multiple Inheritance)
class MyWindow(QMainWindow, form_class):
    def __init__(self): # constructor
        super().__init__() # call super class constructor
        self.setupUi(self) # "setupUi()" method display the UI already loaded
###############################################################################









###############################################################################
if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = MyWindow()
    window.show()
    app.exec_()
###############################################################################