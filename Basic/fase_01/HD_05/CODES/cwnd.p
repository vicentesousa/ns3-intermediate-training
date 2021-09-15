# Gnuplot script file for plotting data in file "force.dat"
set   autoscale                        # scale axes automatically
unset log                              # remove any log-scaling
unset label                            # remove any previous labels
set xtic auto                          # set xtics automatically
set ytic auto                          # set ytics automatically


set terminal png size 640,480
set output "cwnd.png"

# set title "TCP Congestion Window"
set xlabel "Time"
set ylabel "Size"

plot "cwnd.dat" using 1:2 title 'Congestion Window' with linespoints lc rgb "#FF0000"
