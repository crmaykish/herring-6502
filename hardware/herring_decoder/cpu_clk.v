module cpu_clk(CLK_SRC, PHI0);

	input CLK_SRC;
	output PHI0;

	// Divde the source clock (50 MHz by DIVISOR to get the CPU speed)
	// i.e 50 MHz / 6 = 8.33 MHz
    parameter DIVISOR = 6;

    reg [32:0] counter = 0;

    always @(posedge CLK_SRC)
	begin
		counter <= counter + 32'd1;
		
		if(counter >= (DIVISOR-1))
			counter <= 32'd0;
		
	end
	
	assign PHI0 = (counter < DIVISOR / 2) ? 1'b1 : 1'b0;

endmodule
