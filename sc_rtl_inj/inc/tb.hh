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

    /*free fault design ports and signals*/
    sc_core::sc_in< sc_dt::sc_lv< 16L > > addr_free;
    sc_core::sc_out< sc_dt::sc_lv< 8L > > datai_free;
    sc_core::sc_in< sc_dt::sc_lv< 8L > > datao_free;
    sc_core::sc_out< sc_dt::sc_logic > irq_n_free;
    sc_core::sc_out< sc_dt::sc_logic > nmi_n_free;
    sc_core::sc_out< sc_dt::sc_logic > sob_n_free;
    sc_core::sc_out< sc_dt::sc_logic > res_n_free;
    sc_core::sc_out< sc_dt::sc_logic > rdy_free;
    sc_core::sc_in< sc_dt::sc_logic > vpab_free;
    sc_core::sc_in< sc_dt::sc_logic > sync_free;
    sc_core::sc_in< sc_dt::sc_logic > we_n_free;
    sc_core::sc_in< sc_dt::sc_logic > oeb_free;
    sc_core::sc_out< int32_t > muffinMutPort_free;

    sc_dt::sc_lv<8> _ram_free[RAM_SIZE];
    sc_dt::sc_lv<8> _rom_free[ROM_SIZE];

    sc_dt::sc_logic _previous_rom_ce_free;
    sc_dt::sc_logic _previous_rom_oe_free;
    sc_dt::sc_lv<13> _previous_rom_addr_free;

    /*injected design ports and signals*/
    sc_core::sc_in< sc_dt::sc_lv< 16L > > addr_inj;
    sc_core::sc_out< sc_dt::sc_lv< 8L > > datai_inj;
    sc_core::sc_in< sc_dt::sc_lv< 8L > > datao_inj;
    sc_core::sc_out< sc_dt::sc_logic > irq_n_inj;
    sc_core::sc_out< sc_dt::sc_logic > nmi_n_inj;
    sc_core::sc_out< sc_dt::sc_logic > sob_n_inj;
    sc_core::sc_out< sc_dt::sc_logic > res_n_inj;
    sc_core::sc_out< sc_dt::sc_logic > rdy_inj;
    sc_core::sc_in< sc_dt::sc_logic > vpab_inj;
    sc_core::sc_in< sc_dt::sc_logic > sync_inj;
    sc_core::sc_in< sc_dt::sc_logic > we_n_inj;
    sc_core::sc_in< sc_dt::sc_logic > oeb_inj;
    sc_core::sc_out< int32_t > muffinMutPort_inj;

    sc_dt::sc_lv<8> _ram_inj[RAM_SIZE];
    sc_dt::sc_lv<8> _rom_inj[ROM_SIZE];

    sc_dt::sc_logic _previous_rom_ce_inj;
    sc_dt::sc_logic _previous_rom_oe_inj;
    sc_dt::sc_lv<13> _previous_rom_addr_inj;


    SC_HAS_PROCESS(tb);

    void _initializeRAM();
    void _initializeROM();
    void check_mutation();
    void initialize_mix();
    void clk_gen();
    void run();
    void ram_process_free();
    void rom_process_free();
    void ram_process_inj();
    void rom_process_inj();
    void memory_processes_mix();

    tb(sc_core::sc_module_name name);
    ~tb();
};

#endif

