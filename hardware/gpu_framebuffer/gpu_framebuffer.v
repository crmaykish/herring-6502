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
    // TODO: double buffer by using top and bottom three bits of the same cell?
    reg [7:0] framebuffer[0:19199];

    // Indices into the framebuffer
    wire [8:0] fb_x = pixel_x[10:2];
    wire [8:0] fb_y = pixel_y[10:2];

    reg [2:0] current_pixel;

    always @(posedge CLK_PIXEL) begin
        current_pixel <= framebuffer[(fb_y * 160) + fb_x];
    end

    // Assign the VGA signals to the current pixel in the framebuffer
    assign VGA_RED = on_screen && current_pixel[0];
    assign VGA_GREEN = on_screen && current_pixel[1];
    assign VGA_BLUE = on_screen && current_pixel[2];

    // === CPU interface and state machine === //

    reg [20:0] cursor_x = 0;
    reg [20:0] cursor_y = 0;

    reg [14:0] addr_in = 0;
    reg [7:0] data_in = 0;

    reg write_flag = 0;

    always @(negedge CLK_CPU) begin
        if (~CE && ~RW) begin
            addr_in <= ADDR - 4096;
            data_in <= DATA;
            write_flag <= 1;
        end

        if (write_flag) begin
            framebuffer[addr_in] <= data_in;
            write_flag <= 0;
        end
    end



endmodule
