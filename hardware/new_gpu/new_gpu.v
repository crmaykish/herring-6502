module new_gpu(
    input RST, RW, CE, CLK_CPU,
    input [7:0] DATA,
    input CLK_PIXEL,
    output VGA_RED, VGA_GREEN, VGA_BLUE,
    output VGA_HSYNC, VGA_VSYNC
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

    // Font ROM (127 x 8 x 8)
    reg [7:0] font [0:127][0:7];

    // Character buffer
    reg [6:0] framebuffer[0:2047];

    initial begin
        // Initialize the font "ROM" and character buffer
        $readmemb("font.txt", font);
        $readmemh("framebuffer.txt", framebuffer);
    end

    // which framebuffer cell is the pixel in? is there a sprite there? if yes, find the pixel in the sprite

    wire [5:0] fb_x = pixel_x[10:4];
    wire [5:0] fb_y = pixel_y[10:4];

    reg [6:0] sprite_index;

    // indices into the sprite table
    wire [2:0] sp_x = pixel_x[7:1] - (fb_x << 4);
    wire [2:0] sp_y = pixel_y[7:1] - (fb_y << 4);

    // Is the exact pixel in the sprite on or off?
    wire sprite_pixel = font[sprite_index][sp_y][sp_x ^ 3'b111];

    // assign VGA_RED = on_screen && sprite_pixel;
    assign VGA_GREEN = on_screen && sprite_pixel;
    // assign VGA_BLUE = on_screen && sprite_pixel;

    always @(posedge CLK_PIXEL) begin
        sprite_index <= framebuffer[(fb_y * 50) + fb_x];
    end

    reg [20:0] cursor_pos = 0;

    reg [6:0] data_in = 0;

    always @(negedge CLK_CPU) begin
        if (~CE && ~RW) begin
            data_in <= DATA[6:0];
            cursor_pos <= cursor_pos + 1;
        end

        framebuffer[cursor_pos] <= data_in;
    end



endmodule
