// 

module m6502 (clk, addr, datai, datao, irq_n, nmi_n, sob_n,
                          res_n, rdy, vpab, sync, we_n, oeb);

input   clk; 
output  [15:0] addr; //  address bus
input   [7:0] datai; //  data bus
output [7:0] datao; // data bus
output oeb; // data bus negtive enable
input   irq_n; //  interrupt request
input   nmi_n; //  Non-makable interrupt request
input   res_n; //  reset
input   rdy ; //  ready 
input   sob_n; //  Set Overflow bit
output  sync; //  indicates an opcode fetch
output  vpab; //  Vector Pull Address
output  we_n; //  1=read,0=write

wire dataenb; //  TRI enable from the M6502
wire vpa, oen;

assign vpab=~vpa;

assign oen = ~(!clk | we_n);

assign oeb=~oen;

m6502_org core (.clk(clk), .addr(addr), .datai(datai), .datao(datao), 
	    .irq_n(irq_n), .nmi_n(nmi_n), .sob_n(sob_n), .res_n(res_n),
            .rdy(rdy), .vpa(vpa), .sync(sync), .we_n(we_n), .dataenb(dataenb));

endmodule // module m6502


