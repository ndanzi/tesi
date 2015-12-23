export HIFSUITE_DIR=/home/nicolo/hifsuite
export BITS=64
export CORE_DIR=$HIFSUITE_DIR/core/sandbox/obj_linux_$BITS/hifsuite-stable-linux-x86_$BITS
export HR_DIR=$HIFSUITE_DIR/hr/sandbox/obj_linux_$BITS/hifsuite-stable-linux-x86_$BITS
export MUFFIN_DIR=$HIFSUITE_DIR/muffin/sandbox/obj_linux_$BITS/hifsuite-stable-linux-x86_$BITS
export PATH=$PATH:$CORE_DIR/bin:$HR_DIR/bin:$MUFFIN_DIR/bin
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$CORE_DIR/lib

#cd tb_and_CPU_verilog
verilog2hif -t -o testbench.hif.xml testbench/m6502_tb.v
hr -o testbench.hif.xml testbench.hif.xml
#muffin -o flat_m6502_injected.hif.xml flat_m6502.hif.xml
hif2sc -D testbench_rtl_out -S testbench.hif.xml
#cp testbench_rtl_out/src/watchdog.cc sc_rtl/src/
#cp testbench_rtl_out/inc/watchdog.hh sc_rtl/inc/
#rm -rf rtl_out
