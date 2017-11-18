@ECHO OFF
title=binhack32 test automatic input batch file
ECHO binhack32 test automatic input batch file

copy TEST\*.bin .

echo.
echo Testing Katana binary ...

echo 1ST_READ.BIN > katana.txt
echo ip.katana >> katana.txt
echo 0 >> katana.txt
binhack32.exe < katana.txt
del katana.txt >> nul

echo.
echo Testing WinCE binary ...

echo 0WINCEOS.BIN > wince.txt
echo ip.wince >> wince.txt
binhack32.exe < wince.txt
del wince.txt >> nul

echo.
echo Testing Bincon'd binary ...

echo BINCONED.BIN > bincon.txt
echo ip.binconed >> bincon.txt
binhack32.exe < bincon.txt
del bincon.txt >> nul

echo.
ECHO DONE!
pause
