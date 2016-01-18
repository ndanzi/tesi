#include "../inc/tb.hh"
#include "../inc/m6502.hh"

#define TRACE

int sc_main(int argc, char* argv[])
{
    sc_core::sc_report_handler::set_actions (sc_core::SC_WARNING, sc_core::SC_DO_NOTHING);
    sc_core::sc_report_handler::set_actions("/IEEE_Std_1666/deprecated", sc_core::SC_DO_NOTHING);

    tb itb("testbench");
    m6502 dut("u_m6502");

    sc_core::sc_signal< sc_dt::sc_logic > clk;
    sc_core::sc_signal< sc_dt::sc_lv< 16L > > addr;
    sc_core::sc_signal< sc_dt::sc_lv< 8L > > datai;
    sc_core::sc_signal< sc_dt::sc_lv< 8L > > datao;
    sc_core::sc_signal< sc_dt::sc_logic > irq_n;
    sc_core::sc_signal< sc_dt::sc_logic > nmi_n;
    sc_core::sc_signal< sc_dt::sc_logic > sob_n;
    sc_core::sc_signal< sc_dt::sc_logic > res_n;
    sc_core::sc_signal< sc_dt::sc_logic > rdy;
    sc_core::sc_signal< sc_dt::sc_logic > vpab;
    sc_core::sc_signal< sc_dt::sc_logic > sync;
    sc_core::sc_signal< sc_dt::sc_logic > we_n;
    sc_core::sc_signal< sc_dt::sc_logic > oeb;

    itb.clk(clk);
    itb.addr(addr);
    itb.datai(datai);
    itb.datao(datao);
    itb.irq_n(irq_n);
    itb.nmi_n(nmi_n);
    itb.sob_n(sob_n);
    itb.res_n(res_n);
    itb.rdy(rdy);
    itb.vpab(vpab);
    itb.sync(sync);
    itb.we_n(we_n);
    itb.oeb(oeb);

    dut.clk(clk);
    dut.addr(addr);
    dut.datai(datai);
    dut.datao(datao);
    dut.irq_n(irq_n);
    dut.nmi_n(nmi_n);
    dut.sob_n(sob_n);
    dut.res_n(res_n);
    dut.rdy(rdy);
    dut.vpab(vpab);
    dut.sync(sync);
    dut.we_n(we_n);
    dut.oeb(oeb);
   
#ifdef TRACE
    sc_core::sc_trace_file* tf = sc_core::sc_create_vcd_trace_file("wave");
    sc_core::sc_trace(tf, dut.clk, "clk");
    sc_core::sc_trace(tf, dut.res_n, "res_n");
    sc_core::sc_trace(tf, dut.addr, "addr");
    sc_core::sc_trace(tf, dut.datai, "datai");
    sc_core::sc_trace(tf, dut.datao, "datao");
    sc_core::sc_trace(tf, dut.irq_n, "irq_n");
    sc_core::sc_trace(tf, dut.nmi_n, "nmi_n");
    sc_core::sc_trace(tf, dut.vpab, "vpab");
    sc_core::sc_trace(tf, dut.sync, "sync");
    sc_core::sc_trace(tf, dut.we_n, "we_n");
    sc_core::sc_trace(tf, dut.oeb, "oeb");
    sc_core::sc_trace(tf, dut.core_pc_pc, "pc");
    sc_core::sc_trace(tf, dut.core_domux_sel, "domux_sel");
    sc_core::sc_trace(tf, dut.core_alu_outl, "alu_outl");
    sc_core::sc_trace(tf, dut.core_alu_ctl, "alu_ctl");
    sc_core::sc_trace(tf, dut.core_sm_alu_ctl_carry, "core_sm_alu_ctl_carry");
    sc_core::sc_trace(tf, dut.core_sm_inc_op, "core_sm_inc_op");
    sc_core::sc_trace(tf, dut.core_sm_opcode, "core_sm_opcode");
#endif

	sc_core::sc_start(103, sc_core::SC_US);

#ifdef TRACE
    sc_core::sc_close_vcd_trace_file(tf);
#endif

	return 0;
}
