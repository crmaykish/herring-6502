module vga_timing(PIXEL_CLOCK, Hs, Vs, SCREEN_X, SCREEN_Y, ON_SCREEN);
    input PIXEL_CLOCK;
    output reg Hs, Vs;

    output reg [9:0] SCREEN_X;
    output reg [9:0] SCREEN_Y;

    output reg ON_SCREEN;

    parameter HA_END = 639;
    parameter HS_STA = HA_END + 16;
    parameter HS_END = HS_STA + 96;
    parameter LINE = 799;

    parameter VA_END = 479;
    parameter VS_STA = VA_END + 10;
    parameter VS_END = VS_STA + 2;
    parameter SCREEN = 524;

    always @(posedge PIXEL_CLOCK) begin
        if (SCREEN_X == LINE)
            begin
                SCREEN_X <= 0;
                SCREEN_Y <= (SCREEN_Y == SCREEN) ? 0 : SCREEN_Y + 1;
            end
        else
            begin
                SCREEN_X <= SCREEN_X + 1;
            end
    end

    always @(posedge PIXEL_CLOCK) begin
        // Set SCREEN_Ync pulses
        Hs <= ~(SCREEN_X >= HS_STA && SCREEN_X < HS_END);
        Vs <= ~(SCREEN_Y >= VS_STA && SCREEN_Y < VS_END);

        // Is the pixel within the visible portion of the display?
        ON_SCREEN <= (SCREEN_X <= HA_END) && (SCREEN_Y <= VA_END);
    end

endmodule