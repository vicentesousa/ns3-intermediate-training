set terminal png
set output 'throughput-2.4-5-ghz.png'
set xlabel 'Distance (meters)'
set ylabel 'Throughput (Mb/s)'
set title 'Throughput (AP to STA) vs distance'
plot 'throughput-ideal_manager_2_4GHz_80Mhz.txt' u 1:2 w l title '2.4 GHz', \
     'throughput-ideal_manager_5GHz_80Mhz.txt' u 1:2 w l title '5 GHz'
unset output
