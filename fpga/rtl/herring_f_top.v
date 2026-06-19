module herring_f_top (
    wire clk_27,
    wire [5:0] led,

    wire uart_rx,
    wire uart_tx
);

wire clk_6502;
wire [15:0] address_bus;
// reg [7:0] data_bus_in;
wire [7:0] data_bus_out;
wire we;

// Clock Divider
reg [15:0] clk_cnt = 16'b0;
always @(posedge clk_27) begin
    clk_cnt <= clk_cnt + 1'b1;
end
assign clk_6502 = clk_cnt[10];  // Super slow CPU clock


// Reset counter
reg rst = 1'b1;
reg [7:0] rst_cnt = 8'b0;
always @(posedge clk_6502) begin
    if (rst_cnt == 8'd100) rst <= 1'b0;
    else rst_cnt <= rst_cnt + 1'b1;
end

// CPU Core
cpu c1(
    .clk(clk_6502),
    .reset(rst),
    .AB(address_bus),
    .DI(8'hEA), // NOP
    .DO(data_bus_out),
    .WE(we),
    .IRQ(1'b0),
    .NMI(1'b0),
    .RDY(1'b1)
);

assign led[5:0] = ~address_bus[15:10];

endmodule
