


`ifdef VERIFAULT_SPECIFIC
   // Use 'timescale.v' instead of directive
`else
   `timescale  1ns/1ps
`endif // VERIFAULT_SPECIFIC


module watchdog (clk, reset, en, expired);

// Maximum allowed time, in [us]. Default is 3000us=3ms
parameter DEADLINE = 3000;
// Clock period, in [ns]. Default is 20ns (from ARM CM0IK defines)
parameter CLK_PERIOD = 20;

input clk;
input reset;
input en;
output expired;

// Internal counter
reg [31:0] counter;

always @(posedge clk or negedge reset)
begin
   if(reset == 1'b0)
      counter <= 32'd0;
   else
      if(en == 1'b0)
         // For simulation modeling only: we have 32-bit counter for
         // flexibility, but a real counter will saturate automatically and have
         // the appropriate number of bits
         if(counter == (DEADLINE * 1000 / CLK_PERIOD))
            counter <= 32'd0;
         else
            counter <= counter + 1;
   end

// Max value for counter is in clock cycles. Notice that this model will
// generate an impulse rather than a sticky signal
assign expired = (counter === (DEADLINE * 1000 / CLK_PERIOD)) ? 1'b1 : 1'b0;

endmodule

