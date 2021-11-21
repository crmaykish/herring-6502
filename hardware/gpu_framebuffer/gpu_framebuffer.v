module gpu_framebuffer(
    input CLK,
    input RW, CE, CLK_CPU,
    inout [6:0] DATA,
    input [14:0] ADDR,
    output VGA_RED, VGA_GREEN, VGA_BLUE,
    output VGA_HSYNC, VGA_VSYNC
);

    // Generate pixel clock with PLL
    wire CLK_PIXEL;

    // 12 MHz, 65 MHz out
    SB_PLL40_CORE #(
        .FEEDBACK_PATH("SIMPLE"),
        .DIVR(4'b0000),
        .DIVF(7'b1010110),
        .DIVQ(3'b100),
        .FILTER_RANGE(3'b001)
    ) pll (
        .LOCK(locked),
        .RESETB(1'b1),
        .BYPASS(1'b0),
        .REFERENCECLK(CLK),
        .PLLOUTGLOBAL(CLK_PIXEL)
    );

    wire [10:0] pixel_x;
    wire [10:0] pixel_y;
    wire on_screen;

    vga_timing VGATiming(CLK_PIXEL,
                         VGA_HSYNC,
                         VGA_VSYNC,
                         pixel_x,
                         pixel_y,
                         on_screen);

    parameter FB_WIDTH = 128;
    parameter FB_HEIGHT = 96;
    // NOTE: when the screen size changes, the framebuffer indices also need to change in order to scale to full screen

    // Framebuffers
    reg [2:0] front_buffer[0:((FB_WIDTH * FB_HEIGHT) - 1)];
    reg [2:0] back_buffer[0:((FB_WIDTH * FB_HEIGHT) - 1)];

    // Indices into the framebuffer
    wire [7:0] fb_x = pixel_x[10:3];
    wire [7:0] fb_y = pixel_y[10:3];

    reg [2:0] current_pixel;
    reg [2:0] current_pixel2;

    always @(posedge CLK_PIXEL) begin
        current_pixel <= back_buffer[(fb_y * FB_WIDTH) + fb_x];
        current_pixel2 <= front_buffer[(fb_y * FB_WIDTH) + fb_x];
    end

    wire [2:0] actual_pixel = (buffer_flag ? current_pixel : current_pixel2);

    // Assign the VGA signals to the current pixel in the framebuffer
    assign VGA_RED = on_screen && actual_pixel[0];
    assign VGA_GREEN = on_screen && actual_pixel[1];
    assign VGA_BLUE = on_screen && actual_pixel[2];

    // === CPU interface and state machine === //

    assign DATA = (~CE && RW) ? data_out : 7'bZ;

    reg [20:0] cursor_x = 0;
    reg [20:0] cursor_y = 0;

    reg [14:0] addr_in = 0;
    reg [6:0] data_in = 0;
    reg [6:0] data_out = 0;

    reg buffer_flag = 0;
    reg write_flag = 0;

    always @(negedge CLK_CPU) begin
        if (~CE && ~RW) begin
            addr_in <= ADDR - 4096;
            data_in <= DATA;
            write_flag <= 1;
        end

        if (write_flag) begin
            write_flag <= 0;

            if (data_in[6] == 1'b1) begin
                // Buffer swap command
                buffer_flag <= ~buffer_flag;
            end else begin
                // Write to inactive framebuffer
                if (buffer_flag) begin
                    front_buffer[addr_in] <= data_in[2:0];
                end else begin
                    back_buffer[addr_in] <= data_in[2:0];
                end
            end
            
        end

        // Expose VSYNC signal to CPU
        data_out <= VGA_VSYNC;
    end

endmodule
