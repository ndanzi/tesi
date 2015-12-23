export HIFSUITE_DIR=/home/nicolo/hifsuite
export BITS=64
export CORE_DIR=$HIFSUITE_DIR/core/sandbox/obj_linux_$BITS/hifsuite-stable-linux-x86_$BITS
export HR_DIR=$HIFSUITE_DIR/hr/sandbox/obj_linux_$BITS/hifsuite-stable-linux-x86_$BITS
export MUFFIN_DIR=$HIFSUITE_DIR/muffin/sandbox/obj_linux_$BITS/hifsuite-stable-linux-x86_$BITS
export PATH=$PATH:$CORE_DIR/bin:$HR_DIR/bin:$MUFFIN_DIR/bin
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$CORE_DIR/lib

#cd $HIFSUITE_DIR
verilog2hif -t -o m6502.hif.xml verilog/rtl/*.v
hr -o flat_m6502.hif.xml m6502.hif.xml
muffin -o flat_m6502_injected.hif.xml -m S flat_m6502.hif.xml
hif2sc -D rtl_out -S flat_m6502_injected.hif.xml
cp rtl_out/src/m6502.cc sc_rtl_inj_stuckat/src/
cp rtl_out/inc/m6502.hh sc_rtl_inj_stuckat/inc/
#rm -rf rtl_out
