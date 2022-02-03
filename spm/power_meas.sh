mkdir csv_files
sudo powertop --csv=csv_files/output --sample=150 --time=1
./home/pi/Desktop/liboqs/certs/compile_signatures.sh > csv_files/compile_sig_timestamps.txt

