reset
set grid
set autoscale
set ylabel "Average time in sec" font ",12"
set title "Average Time of Bulk Loading each Relation (iMac)" font ",15"
set boxwidth 0.7 relative
set style data histograms
set style histogram cluster
set style fill solid 1.0 border lt -1
set auto x
set yrange [0:14]
set key top left font ", 10"
plot "r_bulk_load_4096.txt" using 2:xticlabels(1) title 'NSM(4096)', \
"c_bulk_load_4096.txt" using 2:xticlabels(1) title 'PAX(4096)', \
"r_bulk_load_8192.txt" using 2:xticlabels(1) title 'NSM(8192)', \
"c_bulk_load_8192.txt" using 2:xticlabels(1) title 'PAX(8192)', \
"r_bulk_load_16384.txt" using 2:xticlabels(1) title 'NSM(16384)', \
"c_bulk_load_16384.txt" using 2:xticlabels(1) title 'PAX(16384)'
