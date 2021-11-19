module gpu_framebuffer(
    input RW, CE, CLK_CPU,
    input [6:0] DATA,
    input [14:0] ADDR,
    input CLK_SYS,
    output VGA_RED, VGA_GREEN, VGA_BLUE,
    output VGA_HSYNC, VGA_VSYNC
);
    reg CLK_PIXEL;
    
    always @(posedge CLK_SYS) begin
        CLK_PIXEL <= ~CLK_PIXEL;
    end

    wire [10:0] pixel_x;
    wire [10:0] pixel_y;
    wire on_screen;

    vga_timing VGATiming(CLK_PIXEL,
                         VGA_HSYNC,
                         VGA_VSYNC,
                         pixel_x,
                         pixel_y,
                         on_screen);

    // Character buffer
    reg [2:0] front_buffer[0:4799];
    reg [2:0] back_buffer[0:4799];

    // Indices into the framebuffer
    wire [7:0] fb_x = pixel_x[10:3];
    wire [7:0] fb_y = pixel_y[10:3];

    reg [2:0] current_pixel;
    reg [2:0] current_pixel2;

    always @(posedge CLK_PIXEL) begin
        current_pixel <= back_buffer[(fb_y * 80) + fb_x];
        current_pixel2 <= front_buffer[(fb_y * 80) + fb_x];
    end

    wire [2:0] actual_pixel = (buffer_flag ? current_pixel : current_pixel2);

    // Assign the VGA signals to the current pixel in the framebuffer
    assign VGA_RED = on_screen && actual_pixel[0];
    assign VGA_GREEN = on_screen && actual_pixel[1];
    assign VGA_BLUE = on_screen && actual_pixel[2];

    // === CPU interface and state machine === //

    reg [20:0] cursor_x = 0;
    reg [20:0] cursor_y = 0;

    reg [14:0] addr_in = 0;
    reg [7:0] data_in = 0;

    reg buffer_flag = 0;
    reg buffer_swap_flag = 0;

    reg write_flag = 0;

    always @(negedge CLK_CPU) begin
        // Don't accept anything new while there's a buffer swap request waiting

        if (~CE && ~RW) begin
            addr_in <= ADDR - 4096;
            data_in <= DATA;
            write_flag <= 1;
        end

        if (write_flag) begin
            write_flag <= 0;

            if (buffer_flag) begin
                front_buffer[addr_in] <= data_in[2:0];
            end else begin
                back_buffer[addr_in] <= data_in[2:0];
            end

            if (data_in[6] == 1'b1) begin
                buffer_swap_flag <= 1;
            end
        end

        // NOTE: With Vsync, there's no tearing, but the other buffer seems to flash through
        // Why?

        if (buffer_swap_flag) begin
            buffer_flag <= ~buffer_flag;
            buffer_swap_flag <= 0;
        end

    end

endmodule
