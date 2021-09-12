module vga_controller(R, G, B, Hs, Vs, CLK);
    input CLK;
    output R, G, B;
    output Hs, Vs;
    
    reg [9:0] sx;
    reg [9:0] sy;

    wire de;

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

    assign Hs = ~(sx >= HS_STA && sx < HS_END);
    assign Vs = ~(sy >= VS_STA && sy < VS_END);
    assign de = (sx <= HA_END && sy <= VA_END);

    assign R = (de && (sx < 32 && sy < 32)) ? 1 : 0;

    assign G = (de && (sx > 600 && sy > 440)) ? 1 : 0;

    assign B = (de && (sx > 400 && sx < 500) && sy > 300 && sy < 360) ? 1 : 0;

endmodule
