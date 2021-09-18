module herring_gpu(
    input PIXEL_CLOCK,
    output VGA_RED, VGA_GREEN, VGA_BLUE,
    output VGA_HSYNC, VGA_VSYNC,

    // CPU Interface
    input [1:0] RS,         // Register select
    inout [7:0] DATA,       // Data bus
    input CE,               // Chip-enable, active-low
    input CPU_CLOCK,        // CPU system clock
    input RWB               // Read/write flag
);

    // Screen position and status
    wire [10:0] pixel_x;
    wire [9:0] pixel_y;
    wire on_screen;

    // Command state
    reg [7:0] x_pos = 8'b0;
    reg [7:0] y_pos = 8'b0;
    reg [2:0] color = 3'b0;
    reg write_fb = 1'b0;
    reg [7:0] status = 8'b0;

    vga_timing VGATiming800x600(PIXEL_CLOCK,
                                VGA_HSYNC,
                                VGA_VSYNC,
                                pixel_x,
                                pixel_y,
                                on_screen);

    framebuffer FrameBuffer(PIXEL_CLOCK,
                            pixel_x,
                            pixel_y,
                            on_screen,
                            VGA_RED,
                            VGA_GREEN,
                            VGA_BLUE,
                            x_pos,
                            y_pos,
                            color,
                            write_fb);

    assign DATA = (~CE && RWB) ? status : 8'bZ;

    always @(negedge CPU_CLOCK) begin
        write_fb <= 1'b0;

        if (~CE) begin
            if (~RWB) begin
                // CPU is writing to GPU
                case (RS)
                    2'b00: color <= DATA[2:0];
                    2'b01: x_pos <= DATA;
                    2'b10: y_pos <= DATA;
                    2'b11: begin
                        case (DATA)
                            8'b00000000: write_fb <= 1'b1;
                        endcase
                    end
                endcase
            end
        end
    end

    always @(posedge PIXEL_CLOCK) begin
        // Update status register
        case (RS)
            // Return current color
            2'b00: status <= {5'b0, color};
            // Return current X coordinate
            2'b01: status <= x_pos;
            // Return current Y coordinate
            2'b10: status <= y_pos;
            // TODO: Return status flags
            2'b11: status <= {7'b0, ~on_screen};
        endcase
    end

endmodule
