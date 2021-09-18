module vga_timing(PIXEL_CLOCK, R, G, B, Hs, Vs);
    input PIXEL_CLOCK;
    output reg R, G, B;
    output reg Hs, Vs;

    reg [10:0] sx;
    reg [9:0] sy;

    reg on_screen;

    parameter HA_END = 799;
    parameter HS_STA = HA_END + 40;
    parameter HS_END = HS_STA + 128;
    parameter LINE = 1055;

    parameter VA_END = 599;
    parameter VS_STA = VA_END + 1;
    parameter VS_END = VS_STA + 4;
    parameter SCREEN = 627;

    always @(posedge PIXEL_CLOCK) begin
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

    always @(posedge PIXEL_CLOCK) begin
        // Set sync pulses
        Hs <= (sx >= HS_STA && sx < HS_END);
        Vs <= (sy >= VS_STA && sy < VS_END);

        // Is the pixel within the visible portion of the display?
        on_screen <= (sx <= HA_END && sy <= VA_END);

        if (on_screen)
            begin
                R <= (sx > 100 && sx < 200 && sy > 100 && sy < 200);
                G <= (sx > 150 && sx < 250 && sy > 150 && sy < 250);
                B <= (sx > 200 && sx < 300 && sy > 200 && sy < 300);
            end
    end

endmodule
