set terminal eps enhanced font 'Times,18'
#set timefmt "%H:%M%S"
#set timefmt "%d/%m/%Y_%H:%M%S"
#set xrange ['12/03/19_00:00:00':'14/03/19_00:00:00']

set xdata time
#set timefmt "%d/%m/%Y"
#set xrange ["12/12/2010":"12/12/2019"]
set format x "%H:%M"
set timefmt "%Y/%m/%d_%H:%M:%S"
#set format x "%d/%m/%y_%H:%M%S"
#set termoption dash
set output "teste.eps"
set ylabel 'Temperatura (°Celsius)'
set xlabel 'Tempo (horas:minutos:segundos)'
set key left top box 
#set key at 1.5,65
#set yrange [0:2500]
#set xrange [0:400]
#set xtics 1

plot 'async2.txt' using 1:2 with lp lt 1 pt 66 lw 1 lc rgb 'blue'  title 'Nó Assíncrono' , 'sleep2.txt' using 1:2 with p lt 2 pt 2 lw 1 lc rgb 'red' title 'Nó Síncrono'

