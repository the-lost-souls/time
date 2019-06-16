@echo off
tasm %1 /m3
dlink %1 ..\math\trig1-8.obj ..\general\debug.obj ,,, ..\nms\nms -A=32 -S -C
pause
%1
