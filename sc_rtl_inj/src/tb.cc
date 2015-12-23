#include "../inc/tb.hh"

//#define PRINT
#define PERIOD 500
#define NMUTANTS 1910
#define MAX_CLOCK_CYCLES 100

tb::tb(sc_core::sc_module_name name) :
    sc_core::sc_module(name),
    _ram_free(),
    _rom_free(),
    _previous_rom_ce_free(sc_dt::sc_logic('X')),
    _previous_rom_oe_free(sc_dt::sc_logic('X')),
    _previous_rom_addr_free(sc_dt::sc_lv<13>("XXXXXXXXXXXXX")),
    _ram_inj(),
    _rom_inj(),
    _previous_rom_ce_inj(sc_dt::sc_logic('X')),
    _previous_rom_oe_inj(sc_dt::sc_logic('X')),
    _previous_rom_addr_inj(sc_dt::sc_lv<13>("XXXXXXXXXXXXX"))
{

    _initializeRAM();
    _initializeROM();

    SC_THREAD(clk_gen);

    SC_THREAD(check_mutation);

}

tb::~tb()
{
}

void tb::clk_gen()
{
    while (true)
    {
        clk = sc_dt::sc_logic('1');
        wait(PERIOD / 2, sc_core::SC_NS);
        clk = sc_dt::sc_logic('0');
        wait(PERIOD / 2, sc_core::SC_NS);
    }
}

void tb::run()
{
    //initialize_free();
    //initialize_inj();
    unsigned int clock_cycles = 0;

    while (clock_cycles < 100) {
       //memory_processes_free();
       clock_cycles++;
       //memory_processes_inj(); 
    } 

    

    sc_core::sc_stop();
}

void tb::check_mutation()
{
    unsigned int clock_cycles = 0;
    unsigned int killed_count = 0;
    bool killed;

    muffinMutPort_free.write(0);
    //std::cout << "go_on: " << _go_on << std::endl;
    //wait();
    for(int i = 1; i <= NMUTANTS; ++i) {
        killed = false;
        clock_cycles = 0;
        std::cout << "*****************\n" << "initialize\n";
        initialize_mix();

        muffinMutPort_inj.write(i);
        std::cout << "run\n";
        while(clock_cycles < MAX_CLOCK_CYCLES) {

            memory_processes_mix();

            //std::cout << "clk_cycles = " << clock_cycles << std::endl;
            clock_cycles++;
            if(
                vpab_free.read() != vpab_inj.read() ||
                sync_free.read() != sync_inj.read() ||
                we_n_free.read() != we_n_inj.read() ||
                oeb_free.read() != oeb_inj.read() ||
                datao_free.read() != datao_inj.read() ||
                addr_free.read() != addr_inj.read()
            ) 
            {
                if (!killed)
                    killed_count++;
                killed = true;
                break;
            }
        }

        if (killed)
            std::cout << "Mutant #" << i << " has been killed after " << clock_cycles << " clock cycles\n" << "*****************\n";
        else
            std::cout << "Mutant #" << i << " is alive\n" << "*****************\n";
    }


    std::cout << "\n--- RESULTS ---\n";
    std::cout << killed_count << " mutant(s) killed out of " << NMUTANTS << "\n";
    if (NMUTANTS > 0)
        std::cout << "MUTANT COVERAGE = " << (float)(killed_count * 100 / NMUTANTS) << "%\n\n";

    sc_core::sc_stop();
}

void tb::_initializeRAM()
{
    for (int i = 0; i < RAM_SIZE; ++i)
    {
        _ram_free[i] = sc_dt::sc_lv<8>(0);
        _ram_inj[i] = sc_dt::sc_lv<8>(0);
    }
}

void tb::_initializeROM()
{
    hif_verilog_standard::hif_verilog__system_readmemh("rom8kx8.mem", _rom_inj);
    hif_verilog_standard::hif_verilog__system_readmemh("rom8kx8.mem", _rom_free);
}


void tb::initialize_mix()
{
    res_n_free = sc_dt::sc_logic('0');
    nmi_n_free = sc_dt::sc_logic('0');
    sob_n_free = sc_dt::sc_logic('1');
    irq_n_free = sc_dt::sc_logic('0');
    rdy_free = sc_dt::sc_logic('1');

    res_n_inj = sc_dt::sc_logic('0');
    nmi_n_inj = sc_dt::sc_logic('0');
    sob_n_inj = sc_dt::sc_logic('1');
    irq_n_inj = sc_dt::sc_logic('0');
    rdy_inj = sc_dt::sc_logic('1');

    memory_processes_mix();

    nmi_n_free = sc_dt::sc_logic('1');

    nmi_n_inj = sc_dt::sc_logic('1');

    for (int i = 0; i < 3; ++i) memory_processes_mix();

    res_n_free = sc_dt::sc_logic('1');
    nmi_n_free = sc_dt::sc_logic('0');

    res_n_inj = sc_dt::sc_logic('1');
    nmi_n_inj = sc_dt::sc_logic('0');
    std::cout << "initialized both designs" << std::endl;
}


void tb::ram_process_free()
{

    /******RAM process for free fault design*******/
    sc_dt::sc_lv<15> ram_addr_free = addr_free.read().range(14, 0);
    if (((~(addr_free.read()[15])) == sc_dt::sc_logic('1')) &&
        ((~we_n_free.read()) == sc_dt::sc_logic('1')) &&
        ram_addr_free.is_01())
    {
        _ram_free[ram_addr_free.to_uint()] = datao_free.read();
        /*std::cout << "[RAM free] " << std::setw(6) << std::setfill(' ') << std::dec << (sc_core::sc_time_stamp().value() / 1000) << " ns: "
                  << "wrote: " << std::hex << std::setw(4) << std::setfill('0') << ram_addr_free.to_uint()
                  << " with " << std::setw(2) << std::setfill('0') << datao_free.read().to_uint() << std::endl;
    */
    }
    if (((we_n_free.read() & (~(addr_free.read()[15]))) == sc_dt::sc_logic('1')) && ram_addr_free.is_01())
    {
        datai_free = _ram_free[ram_addr_free.to_uint()];
    }

}

void tb::rom_process_free()
{


    /******RAM process for free fault design*******/
    sc_dt::sc_lv<13> rom_addr_free = addr_free.read().range(12, 0);
    if ((((~(addr_free.read()[15])) != _previous_rom_ce_free) ||
         ((~(we_n_free.read())) != _previous_rom_oe_free) ||
         (rom_addr_free != _previous_rom_addr_free)) && rom_addr_free.is_01())
    {
        /*std::cout << "[ROM free] " << std::setw(6) << std::setfill(' ') << std::dec << (sc_core::sc_time_stamp().value() / 1000) << " ns: "
                  << "read " << std::hex << std::setw(4) << std::setfill('0') << rom_addr_free.to_uint() 
                  << ": " << std::setw(2) << std::setfill('0') << _rom_free[rom_addr_free.to_uint()].to_uint() << std::endl;
    */
    }
    if ((we_n_free.read() & (addr_free.read()[15])) == sc_dt::sc_logic('1'))
    {
        datai_free = _rom_free[rom_addr_free.to_uint()];
    }
    _previous_rom_ce_free = ~(addr_free.read()[15]);
    _previous_rom_oe_free = ~(we_n_free.read());
    _previous_rom_addr_free = addr_free.read().range(12, 0);

    
}


void tb::ram_process_inj()
{

    /******RAM process for injected design*******/
    sc_dt::sc_lv<15> ram_addr_inj = addr_inj.read().range(14, 0);
    if (((~(addr_inj.read()[15])) == sc_dt::sc_logic('1')) &&
        ((~we_n_inj.read()) == sc_dt::sc_logic('1')) &&
        ram_addr_inj.is_01())
    {
        _ram_inj[ram_addr_inj.to_uint()] = datao_inj.read();
        /*std::cout << "[RAM inj] " << std::setw(6) << std::setfill(' ') << std::dec << (sc_core::sc_time_stamp().value() / 1000) << " ns: "
                  << "wrote: " << std::hex << std::setw(4) << std::setfill('0') << ram_addr_inj.to_uint()
                  << " with " << std::setw(2) << std::setfill('0') << datao_inj.read().to_uint() << std::endl;
    */
    }
    if (((we_n_inj.read() & (~(addr_inj.read()[15]))) == sc_dt::sc_logic('1')) && ram_addr_inj.is_01())
    {
        datai_inj = _ram_inj[ram_addr_inj.to_uint()];
    }


}

void tb::rom_process_inj()
{

    /******ROM process for injected design*******/
    sc_dt::sc_lv<13> rom_addr_inj = addr_inj.read().range(12, 0);
    if ((((~(addr_inj.read()[15])) != _previous_rom_ce_inj) ||
         ((~(we_n_inj.read())) != _previous_rom_oe_inj) ||
         (rom_addr_inj != _previous_rom_addr_inj)) && rom_addr_inj.is_01())
    {
    /*    std::cout << "[ROM inj] " << std::setw(6) << std::setfill(' ') << std::dec << (sc_core::sc_time_stamp().value() / 1000) << " ns: "
                  << "read " << std::hex << std::setw(4) << std::setfill('0') << rom_addr_inj.to_uint() 
                  << ": " << std::setw(2) << std::setfill('0') << _rom_inj[rom_addr_inj.to_uint()].to_uint() << std::endl;
    */
    }
    if ((we_n_inj.read() & (addr_inj.read()[15])) == sc_dt::sc_logic('1'))
    {
        datai_inj = _rom_inj[rom_addr_inj.to_uint()];
    }
    _previous_rom_ce_inj = ~(addr_inj.read()[15]);
    _previous_rom_oe_inj = ~(we_n_inj.read());
    _previous_rom_addr_inj = addr_inj.read().range(12, 0);


    
}

void tb::memory_processes_mix()
{
    ram_process_inj();
    ram_process_free();
    for (int i = 0; i < 5; ++i)
    {
        wait(sc_core::SC_ZERO_TIME);
    }
    rom_process_inj();
    rom_process_free();

    if (we_n_inj.read() == sc_dt::sc_logic('0'))
    {
        datai_inj = sc_dt::sc_lv<8>(0);
    }

    if (we_n_free.read() == sc_dt::sc_logic('0'))
    {
        datai_free = sc_dt::sc_lv<8>(0);
    }

    //std::cout << "memory_processes (mix) " << std::endl;
    wait(PERIOD, sc_core::SC_NS);
    
}