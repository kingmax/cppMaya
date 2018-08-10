rem echo ui2py_maya2017(PySide2) ui-file, will generate ui-file.ui.py
rem "C:\Program Files\Autodesk\Maya2017\bin\mayapy.exe" "C:\Program Files\Autodesk\Maya2017\bin\pyside2-uic" -o %1.py %1

rem "C:\Program Files\Autodesk\Maya2017\bin\mayapy.exe" "C:\Program Files\Autodesk\Maya2017\bin\pyside2-uic" -o xxx_ui.py xxx.ui
cls

echo ui2py for maya2018 (pyside2)

"C:\Program Files\Autodesk\Maya2018\bin\mayapy.exe" "C:\Program Files\Autodesk\Maya2018\bin\pyside2-uic" -o %1.py %1
pause

