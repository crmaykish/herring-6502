module vga_controller(R, G, B, Hs, Vs, CLK, ADDR, DATA, RESB, CPU_CLK, RWB, CE);
    // VGA connections
    input CLK;
    output reg R, G, B;
    output reg Hs, Vs;

    // CPU interface
    input RESB, CPU_CLK, RWB, CE;
    
    input [1:0] ADDR;
    input [7:0] DATA;

    reg [1:0] TARGET;
    reg [7:0] IO;

    reg [7:0] x_coord;
    reg [7:0] y_coord;
    reg [2:0] color;

    reg [10:0] sx;
    reg [9:0] sy;

    reg de;

    // Framebuffer, 200x150, 3-bit color
    reg [2:0] v_ram [0:199][0:149];

    initial begin
        v_ram[10][10] <= 3'b001;
        v_ram[20][20] <= 3'b010;
        v_ram[30][30] <= 3'b011;
        v_ram[40][40] <= 3'b100;
        v_ram[50][50] <= 3'b101;
        v_ram[60][60] <= 3'b110;
        v_ram[70][70] <= 3'b111;
    end

    parameter HA_END = 799;
    parameter HS_STA = HA_END + 40;
    parameter HS_END = HS_STA + 128;
    parameter LINE = 1055;

    parameter VA_END = 599;
    parameter VS_STA = VA_END + 1;
    parameter VS_END = VS_STA + 4;
    parameter SCREEN = 627;

    always @(posedge CLK) begin
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

    always @(posedge CLK) begin
        Hs <= (sx >= HS_STA && sx < HS_END);
        Vs <= (sy >= VS_STA && sy < VS_END);
        de <= (sx <= HA_END && sy <= VA_END);

        if (de)
            begin
                R <= v_ram[sx[9:2]][sy[9:2]][0];
                G <= v_ram[sx[9:2]][sy[9:2]][1];
                B <= v_ram[sx[9:2]][sy[9:2]][2];
            end

        begin
            case (TARGET)
                // Set X-coord
                2'b00: x_coord <= IO;
                // Set Y-coord
                2'b01: y_coord <= IO;
                // Set Color
                2'b10: color <= IO[2:0];
                // Draw (write to framebuffer)
                2'b11: v_ram[x_coord][y_coord] <= color;
                
                default:
                    begin
                        x_coord <= 0;
                        y_coord <= 0;
                        color <= 0;
                    end
            endcase
        end
    end

    always @(posedge CPU_CLK)
        begin
            if (~CE && ~RWB)
                // Latch the address bus into storage register
                TARGET <= ADDR;
        end

    always @(negedge CPU_CLK)   // Note: the data bus is not ready on the rising edge of the CPU clock, this is a hack to ignore bus timing concerns
        begin
            if (~CE && ~RWB)
                // Latch the data bus into the IO register
                IO <= DATA;
        end

endmodule