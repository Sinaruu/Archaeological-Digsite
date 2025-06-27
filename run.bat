@echo off
echo Compiling the game...

gcc -ansi -pedantic -Wall -o archeology archeology.c

if errorlevel 1 (
    echo Compilation failed.
    pause
    exit /b
)

echo Running...

archeology
pause