module new_gpu(
    input RST, RW, CE, CLK_CPU,
    input [7:0] DATA,
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

    // Font ROM (127 x 8 x 8)
    reg [7:0] font [0:63][0:7];

    // Character buffer
    reg [6:0] framebuffer[0:59][0:79];

    initial begin
        // Load the font character set
        $readmemb("font.txt", font);
    end

    // which framebuffer cell is the pixel in? is there a sprite there? if yes, find the pixel in the sprite

    wire [6:0] fb_x = pixel_x[10:3];
    wire [6:0] fb_y = pixel_y[10:3];

    reg [6:0] sprite_index;

    // indices into the sprite table
    wire [2:0] sp_x = pixel_x - (fb_x << 3);
    wire [2:0] sp_y = pixel_y - (fb_y << 3);

    // Is the exact pixel in the sprite on or off?
    wire sprite_pixel = font[sprite_index][sp_y][sp_x ^ 3'b111];

    assign VGA_GREEN = on_screen && sprite_pixel;

    always @(posedge CLK_PIXEL) begin
        sprite_index <= framebuffer[fb_y][fb_x];
    end

    reg [20:0] cursor_x = 0;
    reg [20:0] cursor_y = 0;

    reg [7:0] data_in = 0;

    always @(negedge CLK_CPU) begin
        if (~CE && ~RW) begin
            data_in <= DATA;

            cursor_x <= cursor_x + 2;

            if (cursor_x == 78) begin
                cursor_x <= 2;
                cursor_y <= cursor_y + 2;
            end

        end

        framebuffer[cursor_y][cursor_x] <= data_in;
    end



endmodule
