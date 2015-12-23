#ifndef TB_HH
#define TB_HH

#include <iostream>
#include <iomanip>
#include <systemc> 
#include "hif_verilog_standard.hh"

#define RAM_SIZE 32768
#define ROM_SIZE 8192


class tb : public sc_core::sc_module
{
public:
    sc_core::sc_out< sc_dt::sc_logic > clk;
    sc_core::sc_in< sc_dt::sc_lv< 16L > > addr;
    sc_core::sc_out< sc_dt::sc_lv< 8L > > datai;
    sc_core::sc_in< sc_dt::sc_lv< 8L > > datao;
    sc_core::sc_out< sc_dt::sc_logic > irq_n;
    sc_core::sc_out< sc_dt::sc_logic > nmi_n;
    sc_core::sc_out< sc_dt::sc_logic > sob_n;
    sc_core::sc_out< sc_dt::sc_logic > res_n;
    sc_core::sc_out< sc_dt::sc_logic > rdy;
    sc_core::sc_in< sc_dt::sc_logic > vpab;
    sc_core::sc_in< sc_dt::sc_logic > sync;
    sc_core::sc_in< sc_dt::sc_logic > we_n;
    sc_core::sc_in< sc_dt::sc_logic > oeb;

    sc_dt::sc_lv<8> _ram[RAM_SIZE];
    sc_dt::sc_lv<8> _rom[ROM_SIZE];

    sc_dt::sc_logic _previous_rom_ce;
    sc_dt::sc_logic _previous_rom_oe;
    sc_dt::sc_lv<13> _previous_rom_addr;

    SC_HAS_PROCESS(tb);

    void _initializeRAM();
    void _initializeROM();
    void clk_gen();
    void run();
    void ram_process();
    void rom_process();
    void memory_processes();

    tb(sc_core::sc_module_name name);
    ~tb();
};

#endif

