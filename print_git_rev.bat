@echo off
FOR /F "tokens=1 delims=" %%A in ('git rev-parse --short HEAD') do echo -DBUILD_REV=%%A
