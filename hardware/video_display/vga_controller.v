module vga_controller(R, G, B, Hs, Vs, CLK, DATA, CE, RW);
    input CLK;
    output reg R, G, B;
    output Hs, Vs;
    
    input [7:0] DATA;
    input CE, RW;

    reg [9:0] sx;
    reg [9:0] sy;

    wire de;

    // Framebuffer, 160x120, 3-bit color
    reg [2:0] v_ram [0:159][0:119];

    initial begin
        v_ram[50][50] = 3'b001;
        v_ram[12][25] = 3'b101;
        v_ram[89][100] = 3'b010;
    end

    // 25 MHz clock generation (from 50 MHz source clock)
    reg pixel_clock = 1'b1;

    always @(posedge CLK) begin
        pixel_clock <= ~pixel_clock;
    end

    parameter HA_END = 639;
    parameter HS_STA = HA_END + 16;
    parameter HS_END = HS_STA + 96;
    parameter LINE = 799;

    parameter VA_END = 479;
    parameter VS_STA = VA_END + 10;
    parameter VS_END = VS_STA + 2;
    parameter SCREEN = 524;

    always @(posedge pixel_clock) begin
        if (sx == LINE)
            begin
                sx <= 0;
                sy <= (sy == SCREEN) ? 0 : sy + 1;
            end
        else
            begin
                sx <= sx + 1;
            end
    end

    always @(posedge pixel_clock) begin
        if (de) begin
            R <= v_ram[sx[9:2]][sy[9:2]][0];
            G <= v_ram[sx[9:2]][sy[9:2]][1];
            B <= v_ram[sx[9:2]][sy[9:2]][2];
        end
    end

    assign Hs = ~(sx >= HS_STA && sx < HS_END);
    assign Vs = ~(sy >= VS_STA && sy < VS_END);
    assign de = (sx <= HA_END && sy <= VA_END);

endmodule
