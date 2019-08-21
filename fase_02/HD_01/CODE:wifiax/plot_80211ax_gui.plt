set terminal png
set output 'throughput-gi.png'
set xlabel 'Distance (meters)'
set ylabel 'Throughput (Mb/s)'
set title 'Throughput (AP to STA) vs distance for different GIs'
plot 'throughput-ideal_manager_5GHz_3200ns.txt' u 1:2 w l title '3200 ns', \
     'throughput-ideal_manager_5GHz_1600ns.txt' u 1:2 w l title '1600 ns', \
     'throughput-ideal_manager_5GHz_800ns.txt' u 1:2 w l title '800 ns'
unset output
