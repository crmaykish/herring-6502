module gpu_textmode(
    input RW, CE, CLK_CPU,
    input [6:0] DATA,
    input CLK,
    output VGA_RED, VGA_GREEN, VGA_BLUE,
    output VGA_HSYNC, VGA_VSYNC
);
    // Generate pixel clock with PLL
    wire CLK_PIXEL;

    // 12 MHz, 25.125 MHz out
    SB_PLL40_CORE #(
        .FEEDBACK_PATH("SIMPLE"),
        .DIVR(4'b0000),
        .DIVF(7'b1000010),
        .DIVQ(3'b101),
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

    // TODO: explore storing the font in SPRAM instead of BRAM

    // Font ROM (64 x 8 x 8)
    reg [7:0] font [0:63][0:7];

    // Character buffer
    reg [6:0] framebuffer[0:59][0:79];

    initial begin
        // Load the font character set
        $readmemb("font.txt", font);
    end

    // Indices into the framebuffer
    wire [6:0] fb_x = pixel_x[10:3];
    wire [6:0] fb_y = pixel_y[10:3];

    // Indices into the font table
    wire [2:0] sp_x = pixel_x - (fb_x << 3);
    wire [2:0] sp_y = pixel_y - (fb_y << 3);

    reg [6:0] sprite_index;
    reg sprite_pixel;

    always @(posedge CLK_PIXEL) begin
        sprite_index <= framebuffer[fb_y][fb_x];
        sprite_pixel <= font[sprite_index][sp_y][sp_x ^ 3'b111];
    end

    // Assign the VGA signals to the current pixel in the framebuffer
    assign VGA_GREEN = on_screen && sprite_pixel;


    // === CPU interface and state machine === //

    reg [20:0] cursor_x = 0;
    reg [20:0] cursor_y = 0;

    reg [7:0] data_in = 0;

    parameter IDLE = 3'b000;
    parameter WRITING = 3'b001;
    parameter CLEARING = 3'b010;
    parameter CLEARED = 3'b011;

    reg [2:0] state = IDLE;

    always @(negedge CLK_CPU) begin

        case (state)
            IDLE:
                begin
                    if (~CE && ~RW) begin
                        data_in <= DATA;
                        state <= WRITING;
                    end
                end
            WRITING:
                begin
                    state <= IDLE;

                    case (data_in)
                        // New Line
                        8'h0A: cursor_y <= cursor_y + 2;
                        // Return
                        8'h0D: cursor_x <= 0;
                        8'h7F:
                            begin
                                cursor_x <= 0;
                                cursor_y <= 0;
                                state <= CLEARING;
                            end
                        // Printable character
                        default:
                            begin
                                framebuffer[cursor_y][cursor_x] <= data_in;
                                cursor_x <= cursor_x + 1;

                                if (cursor_x == 78) begin
                                    cursor_x <= 0;
                                    cursor_y <= cursor_y + 2;
                                end
                            end
                    endcase
                end
            CLEARING:
                begin
                    framebuffer[cursor_y][cursor_x] <= 0;
                    cursor_x <= cursor_x + 1;

                    if (cursor_x == 79) begin
                        cursor_x <= 0;
                        cursor_y <= cursor_y + 1;

                        if (cursor_y == 59) begin
                            state <= CLEARED;
                        end
                    end
                end
            CLEARED:
                begin
                    cursor_x <= 0;
                    cursor_y <= 0;
                    state <= IDLE;
                end
            default: state <= IDLE;
        endcase

    end

endmodule
