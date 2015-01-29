
import Safet
import os
myhome = os.getenv("HOME")
mymedia = myhome + "/tmp"
myurl = "http://localhost"

myinflow = Safet.MainWindow(myhome)

myinflow.doLoadConfiguration("../apps/safetestudiantes.tar")



