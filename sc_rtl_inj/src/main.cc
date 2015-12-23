#include "../inc/tb.hh"
#include "../inc/m6502.hh"

#define TRACE

int sc_main(int argc, char* argv[])
{
    sc_core::sc_report_handler::set_actions (sc_core::SC_WARNING, sc_core::SC_DO_NOTHING);
    sc_core::sc_report_handler::set_actions("/IEEE_Std_1666/deprecated", sc_core::SC_DO_NOTHING);

    tb itb("testbench");
    m6502 dut_free("u_m6502_free");
    m6502 dut_inj("u_m6502_inj");

    sc_core::sc_signal< sc_dt::sc_logic > clk;

    sc_core::sc_signal< sc_dt::sc_lv< 16L > > addr_free;
    sc_core::sc_signal< sc_dt::sc_lv< 8L > > datai_free;
    sc_core::sc_signal< sc_dt::sc_lv< 8L > > datao_free;
    sc_core::sc_signal< sc_dt::sc_logic > irq_n_free;
    sc_core::sc_signal< sc_dt::sc_logic > nmi_n_free;
    sc_core::sc_signal< sc_dt::sc_logic > sob_n_free;
    sc_core::sc_signal< sc_dt::sc_logic > res_n_free;
    sc_core::sc_signal< sc_dt::sc_logic > rdy_free;
    sc_core::sc_signal< sc_dt::sc_logic > vpab_free;
    sc_core::sc_signal< sc_dt::sc_logic > sync_free;
    sc_core::sc_signal< sc_dt::sc_logic > we_n_free;
    sc_core::sc_signal< sc_dt::sc_logic > oeb_free;
    sc_core::sc_signal< int32_t > muffinMutPort_free;

    sc_core::sc_signal< sc_dt::sc_lv< 16L > > addr_inj;
    sc_core::sc_signal< sc_dt::sc_lv< 8L > > datai_inj;
    sc_core::sc_signal< sc_dt::sc_lv< 8L > > datao_inj;
    sc_core::sc_signal< sc_dt::sc_logic > irq_n_inj;
    sc_core::sc_signal< sc_dt::sc_logic > nmi_n_inj;
    sc_core::sc_signal< sc_dt::sc_logic > sob_n_inj;
    sc_core::sc_signal< sc_dt::sc_logic > res_n_inj;
    sc_core::sc_signal< sc_dt::sc_logic > rdy_inj;
    sc_core::sc_signal< sc_dt::sc_logic > vpab_inj;
    sc_core::sc_signal< sc_dt::sc_logic > sync_inj;
    sc_core::sc_signal< sc_dt::sc_logic > we_n_inj;
    sc_core::sc_signal< sc_dt::sc_logic > oeb_inj;
    sc_core::sc_signal< int32_t > muffinMutPort_inj;

    itb.clk(clk);

    itb.addr_free(addr_free);
    itb.datai_free(datai_free);
    itb.datao_free(datao_free);
    itb.irq_n_free(irq_n_free);
    itb.nmi_n_free(nmi_n_free);
    itb.sob_n_free(sob_n_free);
    itb.res_n_free(res_n_free);
    itb.rdy_free(rdy_free);
    itb.vpab_free(vpab_free);
    itb.sync_free(sync_free);
    itb.we_n_free(we_n_free);
    itb.oeb_free(oeb_free);
    itb.muffinMutPort_free(muffinMutPort_free);

    itb.addr_inj(addr_inj);
    itb.datai_inj(datai_inj);
    itb.datao_inj(datao_inj);
    itb.irq_n_inj(irq_n_inj);
    itb.nmi_n_inj(nmi_n_inj);
    itb.sob_n_inj(sob_n_inj);
    itb.res_n_inj(res_n_inj);
    itb.rdy_inj(rdy_inj);
    itb.vpab_inj(vpab_inj);
    itb.sync_inj(sync_inj);
    itb.we_n_inj(we_n_inj);
    itb.oeb_inj(oeb_inj);
    itb.muffinMutPort_inj(muffinMutPort_inj);

    dut_free.clk(clk);
    dut_free.addr(addr_free);
    dut_free.datai(datai_free);
    dut_free.datao(datao_free);
    dut_free.irq_n(irq_n_free);
    dut_free.nmi_n(nmi_n_free);
    dut_free.sob_n(sob_n_free);
    dut_free.res_n(res_n_free);
    dut_free.rdy(rdy_free);
    dut_free.vpab(vpab_free);
    dut_free.sync(sync_free);
    dut_free.we_n(we_n_free);
    dut_free.oeb(oeb_free);
    dut_free.muffinMutPort(muffinMutPort_free);

    dut_inj.clk(clk);
    dut_inj.addr(addr_inj);
    dut_inj.datai(datai_inj);
    dut_inj.datao(datao_inj);
    dut_inj.irq_n(irq_n_inj);
    dut_inj.nmi_n(nmi_n_inj);
    dut_inj.sob_n(sob_n_inj);
    dut_inj.res_n(res_n_inj);
    dut_inj.rdy(rdy_inj);
    dut_inj.vpab(vpab_inj);
    dut_inj.sync(sync_inj);
    dut_inj.we_n(we_n_inj);
    dut_inj.oeb(oeb_inj);
    dut_inj.muffinMutPort(muffinMutPort_inj);
   
#ifdef TRACE
    sc_core::sc_trace_file* tf = sc_core::sc_create_vcd_trace_file("wave");
    sc_core::sc_trace(tf, dut_inj.clk, "clk");
    sc_core::sc_trace(tf, dut_inj.res_n, "res_n");
    sc_core::sc_trace(tf, dut_inj.addr, "addr");
    sc_core::sc_trace(tf, dut_inj.datai, "datai");
    sc_core::sc_trace(tf, dut_inj.datao, "datao");
    sc_core::sc_trace(tf, dut_inj.irq_n, "irq_n");
    sc_core::sc_trace(tf, dut_inj.nmi_n, "nmi_n");
    sc_core::sc_trace(tf, dut_inj.vpab, "vpab");
    sc_core::sc_trace(tf, dut_inj.sync, "sync");
    sc_core::sc_trace(tf, dut_inj.we_n, "we_n");
    sc_core::sc_trace(tf, dut_inj.oeb, "oeb");
    sc_core::sc_trace(tf, dut_inj.core_pc_pc, "pc");
    sc_core::sc_trace(tf, dut_inj.core_domux_sel, "domux_sel");
    sc_core::sc_trace(tf, dut_inj.core_alu_outl, "alu_outl");
    sc_core::sc_trace(tf, dut_inj.core_alu_ctl, "alu_ctl");
    sc_core::sc_trace(tf, dut_inj.core_sm_alu_ctl_carry, "core_sm_alu_ctl_carry");
    sc_core::sc_trace(tf, dut_inj.core_sm_inc_op, "core_sm_inc_op");
    sc_core::sc_trace(tf, dut_inj.core_sm_opcode, "core_sm_opcode");
#endif

	//sc_core::sc_start(20, sc_core::SC_US);
    sc_core::sc_start();

#ifdef TRACE
    sc_core::sc_close_vcd_trace_file(tf);
#endif

	return 0;
}
