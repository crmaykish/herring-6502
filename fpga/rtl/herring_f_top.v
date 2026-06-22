module herring_f_top #(
    parameter BAUD   = 115200   // ACIA console baud rate (firmware must set CONTROL SBR=0).
                                // baud_nco hits this accurately at 27 MHz (~0.001% error).
) (
    input  wire       clk_27,
    output wire [5:0] led,

    input  wire       uart_rx,
    output wire       uart_tx
);

wire clk_6502;
wire [15:0] address_bus;
reg [7:0] data_bus_in;
wire [7:0] data_bus_out;
wire we;

// CPU Clock
localparam integer CPU_HZ = 27_000_000;
assign clk_6502 = clk_27;
reg rst = 1'b1;
reg [7:0] rst_cnt = 8'b0;
always @(posedge clk_6502) begin
    if (rst_cnt == 8'd100) rst <= 1'b0;
    else rst_cnt <= rst_cnt + 1'b1;
end

// ROM (8KB)
reg [7:0] rom[0:8191];
reg [7:0] rom_out;
initial $readmemh("rom.hex", rom);
always @(posedge clk_6502) rom_out <= rom[address_bus[12:0]];

// GPIO
reg [7:0] gpio = 8'h00;
reg [7:0] gpio_out;
always @(posedge clk_6502) gpio_out <= gpio;

// CPU Core
cpu_65c02 c1(
    .clk(clk_6502),
    .reset(rst),
    .AB(address_bus),
    .DI(data_bus_in),
    .DO(data_bus_out),
    .WE(we),
    .IRQ(1'b0),
    .NMI(1'b0),
    .RDY(1'b1)
);

// Memory Map
wire cs_ram  = ~address_bus[15];                  // 0x0000-0x7FFF  RAM       32 KB
wire cs_bank =  address_bus[15:14] == 2'b10;      // 0x8000-0xBFFF  RAM bank  16 KB
wire cs_io   =  address_bus[15:13] == 3'b110;     // 0xC000-0xDFFF  I/O        8 KB
wire cs_rom  =  address_bus[15:13] == 3'b111;     // 0xE000-0xFFFF  ROM        8 KB

// I/O Space
wire [2:0] io_slot = address_bus[9:7];
wire cs_acia = cs_io && io_slot == 3'd0;    // 0xC000: UART
wire cs_gpio = cs_io && io_slot == 3'd1;    // 0xC080: GPIO (LEDs)
wire cs_sys  = cs_io && io_slot == 3'd2;    // 0xC100: System Register (bank control)

// Registered Chip-select Lines (delayed one cycle to match the registered reads)
reg cs_ram_d, cs_bank_d, cs_rom_d;
reg cs_sys_d, cs_gpio_d, cs_acia_d;
always @(posedge clk_6502) begin
    cs_ram_d  <= cs_ram;
    cs_bank_d <= cs_bank;
    cs_rom_d  <= cs_rom;
    cs_sys_d  <= cs_sys;
    cs_gpio_d <= cs_gpio;
    cs_acia_d <= cs_acia;
end

// RAM (32KB)
reg [7:0] ram[0:32767];
reg [7:0] ram_out;
always @(posedge clk_6502) begin
    if (we && cs_ram) ram[address_bus[14:0]] <= data_bus_out;
    ram_out <= ram[address_bus[14:0]];
end

// 6551 ACIA
wire [7:0] acia_dout;
wire       acia_irq_n;
wire       acia_baud_clk;

baud_nco #(.CLK_HZ(CPU_HZ), .BAUD(BAUD)) acia_baud (
    .clk(clk_6502), .rst_n(~rst), .en_16x(acia_baud_clk)
);

acia6551 a1 (
    .clk(clk_6502),
    .rst_n(~rst),
    .baud_clk(acia_baud_clk),

    .cs_n(~cs_acia),
    .rw_n(~we),
    .rs(address_bus[1:0]),
    .din(data_bus_out),
    .dout(acia_dout),
    .irq_n(acia_irq_n),

    .rx(uart_rx),
    .tx(uart_tx),

    .rxc_in(1'b1),
    .rxc_out(),
    .rxc_oe(),

    .rts_n(),
    .cts_n(1'b0),
    .dtr_n(),
    .dcd_n(1'b1),
    .dsr_n(1'b1)
);

// Register the ACIA read data one cycle to match the delayed read mux
reg [7:0] acia_q;
always @(posedge clk_6502) acia_q <= acia_dout;

// Data Bus Read Mux
always @(*) begin
    if (cs_ram_d) data_bus_in = ram_out;
    else if (cs_bank_d) data_bus_in = 8'h00;    // TODO: RAM bank
    else if (cs_sys_d) data_bus_in = 8'h00;     // TODO: system register
    else if (cs_gpio_d) data_bus_in = gpio_out;
    else if (cs_acia_d) data_bus_in = acia_q;
    else if (cs_rom_d) data_bus_in = rom_out;
    else data_bus_in = 8'b00;
end

// Data I/O Bus Writes
always @(posedge clk_6502) begin
    if (we) begin
        if (cs_gpio) gpio <= data_bus_out;
    end
end

assign led[5:0] = ~gpio[5:0];

endmodule
