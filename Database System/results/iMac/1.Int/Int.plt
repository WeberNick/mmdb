reset
set grid
set autoscale
set ylabel "Average time in ms" font ",12"
set xlabel "Attributes updated" font ",12" offset 0,4
set title "Average Update Time for a different Number of Attributes (iMac)" font ",15"
set key top left font ", 10"
plot "r_test_updateInt_4096.txt" using 1:2 title 'NSM(4096)' w lp, \
"c_test_updateInt_4096.txt" using 1:2 title 'PAX(4096)' w lp, \
"r_test_updateInt_8192.txt" using 1:2 title 'NSM(8192)' w lp, \
"c_test_updateInt_8192.txt" using 1:2 title 'PAX(8192)' w lp, \
"r_test_updateInt_16384.txt" using 1:2 title 'NSM(16384)' w lp, \
"c_test_updateInt_16384.txt" using 1:2 title 'PAX(16384)' w lp