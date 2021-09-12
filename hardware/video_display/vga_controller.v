module vga_controller(R, G, B, Hs, Vs, CLK, LED);
    input CLK;
    output R, G, B, Hs, Vs, LED;

    reg [9:0] counter_x;
    reg [8:0] counter_y;
    wire x_max = (counter_x == 767);

    reg vga_hs, vga_vs;

    reg [32:0] clk_count = 0;

    wire clk_slow = clk_count[25];
    wire clk_25mhz = clk_count[0];

    always @(posedge CLK) begin
        clk_count <= clk_count + 1;
    end

    always @(posedge clk_25mhz) begin
        if (x_max)
            counter_x <= 0;
        else
            counter_x <= counter_x + 1;
    end

    always @(posedge clk_25mhz) begin
        if (x_max)
            counter_y <= counter_y + 1;
    end

    always @(posedge clk_25mhz) begin
        vga_hs <= (counter_x[9:4] == 0);
        vga_vs <= (counter_y == 0);
    end


    assign Hs = ~vga_hs;
    assign Vs = ~vga_vs;


    assign R = counter_y[3] | (counter_x == 256);
    assign G = (counter_x[5] ^ counter_x[6]) | (counter_x == 256);
    assign B = counter_x[4] | (counter_x == 256);

    assign LED = clk_slow;

endmodule