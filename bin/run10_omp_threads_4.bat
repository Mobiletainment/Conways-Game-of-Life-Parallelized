@echo off

IF NOT EXIST output mkdir output
set Threads=4
set FileName=omp_messung_%Threads%threads.txt
set newline=^& echo.

echo %fileName%

IF EXIST %fileName% del %FileName%

gol.exe --mode omp --threads %Threads% --measure --load "step1000_in_250generations\random1000_in.gol" --save  "output/random1000_step250_omp%Threads%.gol" --generations 250 >> %fileName%
echo n | comp "step1000_out_250generations\random1000_out.gol" "output/random1000_step250_omp%Threads%.gol"
gol.exe --mode omp --threads %Threads% --measure --load "step1000_in_250generations\random2000_in.gol" --save  "output/random2000_step250_omp%Threads%.gol" --generations 250 >> %fileName%
echo n | comp "step1000_out_250generations\random2000_out.gol" "output/random2000_step250_omp%Threads%.gol"
gol.exe --mode omp --threads %Threads% --measure --load "step1000_in_250generations\random3000_in.gol" --save  "output/random3000_step250_omp%Threads%.gol" --generations 250 >> %fileName%
echo n | comp "step1000_out_250generations\random3000_out.gol" "output/random3000_step250_omp%Threads%.gol"
gol.exe --mode omp --threads %Threads% --measure --load "step1000_in_250generations\random4000_in.gol" --save  "output/random4000_step250_omp%Threads%.gol" --generations 250 >> %fileName%
echo n | comp "step1000_out_250generations\random4000_out.gol" "output/random4000_step250_omp%Threads%.gol"
gol.exe --mode omp --threads %Threads% --measure --load "step1000_in_250generations\random5000_in.gol" --save  "output/random5000_step250_omp%Threads%.gol" --generations 250 >> %fileName%
echo n | comp "step1000_out_250generations\random5000_out.gol" "output/random5000_step250_omp%Threads%.gol"
gol.exe --mode omp --threads %Threads% --measure --load "step1000_in_250generations\random6000_in.gol" --save  "output/random6000_step250_omp%Threads%.gol" --generations 250 >> %fileName%
echo n | comp "step1000_out_250generations\random6000_out.gol" "output/random6000_step250_omp%Threads%.gol"
gol.exe --mode omp --threads %Threads% --measure --load "step1000_in_250generations\random7000_in.gol" --save  "output/random7000_step250_omp%Threads%.gol" --generations 250 >> %fileName%
echo n | comp "step1000_out_250generations\random7000_out.gol" "output/random7000_step250_omp%Threads%.gol"
gol.exe --mode omp --threads %Threads% --measure --load "step1000_in_250generations\random8000_in.gol" --save  "output/random8000_step250_omp%Threads%.gol" --generations 250 >> %fileName%
echo n | comp "step1000_out_250generations\random8000_out.gol" "output/random8000_step250_omp%Threads%.gol"
gol.exe --mode omp --threads %Threads% --measure --load "step1000_in_250generations\random9000_in.gol" --save  "output/random9000_step250_omp%Threads%.gol" --generations 250 >> %fileName%
echo n | comp "step1000_out_250generations\random9000_out.gol" "output/random9000_step250_omp%Threads%.gol"
gol.exe --mode omp --threads %Threads% --measure --load "step1000_in_250generations\random10000_in.gol" --save  "output/random10000_step250_omp%Threads%.gol" --generations 250 >> %fileName%
echo n | comp "step1000_out_250generations\random10000_out.gol" "output/random10000_step250_omp%Threads%.gol"
pause