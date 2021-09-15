set terminal png
set output 'throughput_80211_a-ax.png'
set xlabel 'Distance (meters)'
set ylabel 'Throughput (Mbps)'
set title 'Throughput (AP to STA) vs distance'
plot 'throughput_a_5GHz_Minstrel.txt' u 1:2 w l title '802.11a 5 GHz', \
     'throughput_b_2-4GHz_Minstrel.txt' u 1:2 w l title '802.11b 2.4 GHz', \
     'throughput_g_2-4GHz_Minstrel.txt' u 1:2 w l title '802.11g 2.4 GHz', \
     'throughput_n_2-4GHz_Minstrel.txt' u 1:2 w l title '802.11n 2.4 GHz', \
     'throughput_n_5GHz_Minstrel.txt' u 1:2 w l title '802.11n 5 GHz', \
     'throughput_ax_5GHz_Ideal.txt' u 1:2 w l title '802.11ax 5 GHz'
unset output
