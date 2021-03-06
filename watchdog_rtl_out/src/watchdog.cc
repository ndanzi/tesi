// /////////////////////////////////////////////////////////////////////////
// SystemC code automatically generated by hif2sc
// Part of HIFSuite - Version stable
// Site: www.hifsuite.com - Contact: hifsuite@edalab.it
//
// HIFSuite copyright: EDALab s.r.l. - Networked Embedded Systems
// Site: www.edalab.it - Contact: info@edalab.it
// /////////////////////////////////////////////////////////////////////////


#include "../inc/watchdog.hh"




watchdog::watchdog( sc_core::sc_module_name name_, const sc_dt::sc_lv< 32L > DEADLINE_,
     const sc_dt::sc_lv< 32L > CLK_PERIOD_ ) :
    sc_core::sc_module( name_ ),
    clk("clk"),
    reset("reset"),
    en("en"),
    expired("expired"),
    DEADLINE(DEADLINE_),
    CLK_PERIOD(CLK_PERIOD_),
    counter("counter"),
    hif_verilog_timescale_unit(sc_core::sc_time( 1, sc_core::SC_NS )),
    hif_verilog_timescale_precision(sc_core::sc_time( 1, sc_core::SC_PS ))

{
    expired.initialize( sc_dt::sc_logic('Z') );
    counter = sc_dt::sc_lv< 32L >("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX");

    SC_METHOD( process );
    dont_initialize();
    sensitive << clk.pos();
    sensitive << reset.neg();

    SC_METHOD( st );
    sensitive << counter;

}

watchdog::~watchdog()
{}

void watchdog::start_of_simulation()
{
    expired = sc_dt::sc_logic('Z');
}


void watchdog::process()
{
    if (hif_systemc_extensions::hif_logicEquals( reset.read(), sc_dt::sc_logic('0'
            ) ) == sc_dt::sc_logic('1'))
    {
        counter = sc_dt::sc_lv< 32L >("00000000000000000000000000000000");
    }
    else if (hif_systemc_extensions::hif_logicEquals( en.read(), sc_dt::sc_logic
            ('0') ) == sc_dt::sc_logic('1'))
    {
        if (hif_systemc_extensions::hif_logicEquals( counter.read(), static_cast<
                 sc_dt::sc_lv< 32L > >( hif_vhdl_ieee_std_logic_signed::hif_vhdl__op_mult
                ( static_cast< sc_dt::sc_lv< 32L > >( DEADLINE.to_uint() / CLK_PERIOD.to_uint
                    () ), sc_dt::sc_lv< 32L >("00000000000000000000001111101000"
                    ) ) ), false ) == sc_dt::sc_logic('1'))
        {
            counter = sc_dt::sc_lv< 32L >("00000000000000000000000000000000");
        }
        else
        {
            counter = hif_vhdl_ieee_std_logic_unsigned::hif_vhdl__op_plus( counter.read
                    (), sc_dt::sc_lv< 32L >("00000000000000000000000000000001")
                 );
        }
    }
}

void watchdog::st()
{
    expired = counter.read() == static_cast< sc_dt::sc_lv< 32L > >( hif_vhdl_ieee_std_logic_signed::hif_vhdl__op_mult
        ( static_cast< sc_dt::sc_lv< 32L > >( DEADLINE.to_uint() / CLK_PERIOD.to_uint
            () ), sc_dt::sc_lv< 32L >("00000000000000000000001111101000") ) ) ?
         sc_dt::sc_logic('1') : sc_dt::sc_logic('0');
}


