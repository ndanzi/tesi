#include "../inc/tb.hh"

//#define PRINT
#define PERIOD 500

tb::tb(sc_core::sc_module_name name) :
    sc_core::sc_module(name),
    _ram(),
    _rom(),
    _previous_rom_ce(sc_dt::sc_logic('X')),
    _previous_rom_oe(sc_dt::sc_logic('X')),
    _previous_rom_addr(sc_dt::sc_lv<13>("XXXXXXXXXXXXX"))
{
    SC_THREAD(clk_gen);

    SC_THREAD(run);

    _initializeRAM();
    _initializeROM();
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

void tb::_initializeRAM()
{
    for (int i = 0; i < RAM_SIZE; ++i)
    {
        _ram[i] = sc_dt::sc_lv<8>(0);
    }
}

void tb::_initializeROM()
{
    //hif_verilog_standard::hif_verilog__system_readmemh("rom8kx8.mem", _rom);
    std::cout << "load ROM" << std::endl;
    hif_verilog_standard::hif_verilog__system_readmemh("rom2.mem", _rom);
    std::cout << "ROM loaded" << std::endl;
}

void tb::run()
{
    res_n = sc_dt::sc_logic('0');
    nmi_n = sc_dt::sc_logic('0');
    sob_n = sc_dt::sc_logic('1');
    irq_n = sc_dt::sc_logic('0');
    rdy = sc_dt::sc_logic('1');
    memory_processes();

    nmi_n = sc_dt::sc_logic('1');
    for (int i = 0; i < 3; ++i) memory_processes();

    res_n = sc_dt::sc_logic('1');
    nmi_n = sc_dt::sc_logic('0');

    while (true) memory_processes();

    sc_core::sc_stop();
}

void tb::ram_process()
{
    sc_dt::sc_lv<15> ram_addr = addr.read().range(14, 0);
    if (((~(addr.read()[15])) == sc_dt::sc_logic('1')) &&
        ((~we_n.read()) == sc_dt::sc_logic('1')) &&
        ram_addr.is_01())
    {
        _ram[ram_addr.to_uint()] = datao.read();
        std::cout << "[RAM] " << std::setw(6) << std::setfill(' ') << std::dec << (sc_core::sc_time_stamp().value() / 1000) << " ns: "
                  << "wrote: " << std::hex << std::setw(4) << std::setfill('0') << ram_addr.to_uint()
                  << " with " << std::setw(2) << std::setfill('0') << datao.read().to_uint() << std::endl;

        std::cout << "\t[datao] =" << std::hex << std::setw(2) << std::setfill('0') << datao.read().to_uint() << std::endl;
    }
    if (((we_n.read() & (~(addr.read()[15]))) == sc_dt::sc_logic('1')) && ram_addr.is_01())
    {
        datai = _ram[ram_addr.to_uint()];
        std::cout << "\t[datai] =" << std::hex << std::setw(2) << std::setfill('0') << _ram[ram_addr.to_uint()] << std::endl;
    }
}

void tb::rom_process()
{
    sc_dt::sc_lv<13> rom_addr = addr.read().range(12, 0);

    if ((((~(addr.read()[15])) != _previous_rom_ce) ||
         ((~(we_n.read())) != _previous_rom_oe) ||
         (rom_addr != _previous_rom_addr)) && rom_addr.is_01())
    {
        std::cout << "[ROM] " << std::setw(6) << std::setfill(' ') << std::dec << (sc_core::sc_time_stamp().value() / 1000) << " ns: "
                  << "read " << std::setw(4) << std::setfill('0') << rom_addr.to_uint() << std::hex 
                  << ": " << std::setw(2) << std::setfill('0') << _rom[rom_addr.to_uint()].to_uint() << std::endl;
    }
    if ((we_n.read() & (addr.read()[15])) == sc_dt::sc_logic('1'))
    {
        datai = _rom[rom_addr.to_uint()];
        std::cout << "\t[datai] =" << std::hex << std::setw(2) << std::setfill('0') << _rom[rom_addr.to_uint()].to_uint() << std::endl;
    }
    _previous_rom_ce = ~(addr.read()[15]);
    _previous_rom_oe = ~(we_n.read());
    _previous_rom_addr = addr.read().range(12, 0);
}

void tb::memory_processes()
{
    ram_process();
    for (int i = 0; i < 5; ++i)
    {
        wait(sc_core::SC_ZERO_TIME);
    }
    rom_process();
    if (we_n.read() == sc_dt::sc_logic('0'))
    {
        datai = sc_dt::sc_lv<8>(0);
    }
    wait(PERIOD, sc_core::SC_NS);
}
