
`timescale 1ns / 100ps

module m6502_tb(CLK_out,TestOn_out);

output CLK_out;
output TestOn_out;
// Default clock period
parameter period_p = 500;
   

wire            m6502_CLK;
   
reg             res_n;

 //  Free-running clock
reg             clk_tb;
reg             CLK;
reg nmi;
wire irq = 1'b0;
wire rdy = 1'b1;
wire sync;
wire rw;
tri [7:0] d;
wire [7:0] di, do;
wire [15:0] a;
wire            vpab;
wire            oeb_o;
initial begin
res_n = 0;
nmi = 0;
#100 res_n = 1;
#100 res_n = 0;
#1000 nmi = 1;
#1500 nmi = 0;
#100 res_n = 1;
end
    
    
    
    
 `ifdef RTL 
   m6502 u_m6502 (
`else
   // For the netlist

       m6502 u_m6502 (  // For the netlist
      .test_si(1'b0),
      .test_se(1'b0),
`endif
      // SYSTEM interface
      .clk                                         (m6502_CLK),
      .res_n                                       (res_n),
      .addr                                        (a),
      .datai                                       (di),
      .datao                                       (do),
      .sync                                        (sync),
      .vpab                                        (vpab),
      .we_n                                        (rw),
      .nmi_n                                       (nmi),
      .rdy                                         (rdy),
      .sob_n                                       (1'b1),
      .irq_n                                       (irq),
      .oeb                                         (oeb_o)
);

//    assign addr_bus2={addr_bus[12],addr_bus[11],addr_bus[10],addr_bus[9],addr_bus[8],addr_bus[7],addr_bus[6],addr_bus[5],addr_bus[4],addr_bus[3],addr_bus[2],addr_bus[1],addr_bus[0]};
//    assign bus_i=bus_o;
//    assign oeb_i=oeb_o;

	wire ramcs = a[15];
	wire romcs = ~a[15];
	assign d = ~rw ? do : 8'bz;
	assign di = rw ? d : 8'b0;

	rom8Kx8 rom0(.ce(romcs), .oe(~rw), .addr(a[12:0]), .d(d));
	ram32Kx8 ram0(.clk(m6502_CLK), .ce(ramcs), .oe(~rw), .we(rw), .addr(a[14:0]), .d(d));



//==============================================================================
   
   initial
      begin : clk_gen
      forever
         begin
         clk_tb <= 1'b0;
         CLK <= 1'b0;
         #(period_p>>1);
         clk_tb <= 1'b1;
         CLK <= 1'b1;
         #((period_p>>1)+(period_p & 1));
         end
      end



   assign m6502_CLK = clk_tb;
   
   //--------------------------------------------------------------------------
   // PC35 - Monitoring of the CPU program counter
   // PC35 CLOCK AND RESET:
   //   Clock: must be the CPU clock, otherwise there are timing issues.
   //      Clearly a fault may stop the CPU clock. This is NOT detected with PC35,
   //      but with the other watchdog.
   //   Reset: must be the CPU reset, for same reasons.
   //--------------------------------------------------------------------------
   wire [15:0] pc_iaex; // Program counter Cortex-M4

 `ifdef RTL 
   assign pc_iaex=m6502_tb.u_m6502.core.pc.pc;
`else

   assign pc_iaex[15]=m6502_tb.u_m6502.core.pc.\pc_reg[15] .Q;
   assign pc_iaex[14]=m6502_tb.u_m6502.core.pc.\pc_reg[14] .Q;
   assign pc_iaex[13]=m6502_tb.u_m6502.core.pc.\pc_reg[13] .Q;
   assign pc_iaex[12]=m6502_tb.u_m6502.core.pc.\pc_reg[12] .Q;
   assign pc_iaex[11]=m6502_tb.u_m6502.core.pc.\pc_reg[11] .Q;
   assign pc_iaex[10]=m6502_tb.u_m6502.core.pc.\pc_reg[10] .Q;
   assign pc_iaex[9]=m6502_tb.u_m6502.core.pc.\pc_reg[9] .Q;
   assign pc_iaex[8]=m6502_tb.u_m6502.core.pc.\pc_reg[8] .Q;
   assign pc_iaex[7]=m6502_tb.u_m6502.core.pc.\pc_reg[7] .Q;
   assign pc_iaex[6]=m6502_tb.u_m6502.core.pc.\pc_reg[6] .Q;
   assign pc_iaex[5]=m6502_tb.u_m6502.core.pc.\pc_reg[5] .Q;
   assign pc_iaex[4]=m6502_tb.u_m6502.core.pc.\pc_reg[4] .Q;
   assign pc_iaex[3]=m6502_tb.u_m6502.core.pc.\pc_reg[3] .Q;
   assign pc_iaex[2]=m6502_tb.u_m6502.core.pc.\pc_reg[2] .Q;
   assign pc_iaex[1]=m6502_tb.u_m6502.core.pc.\pc_reg[1] .Q;
   assign pc_iaex[0]=m6502_tb.u_m6502.core.pc.\pc_reg[0] .Q;
  
   
   
   `endif

   wire        pc_strobe;
   
   reg [15:0]  pc_counter;
   reg [15:0]  pc_sampled;

   // Hooks to CPU clock and reset. For the reset being a post layout netlist one of the working HFN is taken. STM32 DICE DEPENDENT.
   
   // "pc_counter" counts the number of CPU clock cycles with the PC stopped.
   always @(posedge CLK or negedge res_n)
     begin
        if (res_n == 1'b0)
          begin
             pc_counter <= 16'd0;
             pc_sampled <= 16'd0;
          end
        else
          begin
             pc_sampled <= pc_iaex; // this signal is valid only with CM0 core
             
             if ((pc_sampled == pc_iaex) && pc_counter < 25)
               pc_counter <= pc_counter + 1;
             else
               pc_counter <= 16'd0;
          end
     end 
   
   // This signal is HIGH if PC was stopped for 35 CPU clock cycles. It is a strobe point.
   assign pc_strobe = (((pc_counter === 25)) ? 1'b1 : 1'b0);
   
   assign TestOn_out=TestOnFlag;
   assign CLK_out=m6502_CLK;
endmodule
//==============================================================================


module ram32Kx8(clk, ce, oe, we, addr, d);
	input clk;
	input			ce;		// active low chip enable
	input			oe;		// active low output enable
	input			we;		// active low write enable
	input	[14:0]	addr;	// byte address
	output	[7:0]	d;		// tri-state data I/O
	tri [7:0] d;

	reg		[7:0]	mem [0:32767];
	integer nn;

	initial begin
		for (nn = 0; nn < 32768; nn = nn + 1)
			mem[nn] <= 8'b0;
	end

	assign d = (~oe & ~ce & we) ? mem[addr] : 8'bz;

	always @(posedge clk) begin
		if (clk) begin
			if (~ce & ~we) begin
				mem[addr] <= d;
				$display (" wrote: %h with %h", addr, d);
			end
		end
	end

	always @(we or oe or ce or addr) begin
		$display (" 000000: %h %h %h %h %h %h %h %h %h %h", 
			mem[0], mem[1], mem[2], mem[3], mem[4], mem[5], mem[6], mem[7], mem[8], mem[9]);
	end

endmodule

module rom8Kx8(ce, oe, addr, d);
	input			ce;	// active low chip enable
	input			oe;	// active low output enable
	input	[12:0]	addr;	// byte address
	output	[7:0]	d;		// tri-state data I/O
	tri [7:0] d;

	reg		[7:0]	mem [0:8191];

	initial begin
		$readmemh ("rom8kx8.mem", mem);
		$display ("Loaded rom8kx8.mem");
		$display (" 000000: %h %h %h %h %h %h %h %h", 
			mem[0], mem[1], mem[2], mem[3], mem[4], mem[5], mem[6], mem[7]);
	end

	assign d = (~oe & ~ce) ? mem[addr] : 8'bz;

	always @(oe or ce or addr) begin
		$display (" 000000: %h %h %h %h %h %h %h %h %h %h", 
			mem[0], mem[1], mem[2], mem[3], mem[4], mem[5], mem[6], mem[7], mem[8], mem[9]);
		$display (" read %h: %h", addr, mem[addr]);
	end

endmodule


