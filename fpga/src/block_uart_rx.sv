module block_uart_rx #(
    parameter CLK_FREQ = 50_000_000,
    parameter BAUD_RATE = 9600,
    parameter BLOCK_LENGTH = 216
) (
    input  logic       clk,
    input  logic       reset_n,
    input  logic       data_i,
    output logic [(BLOCK_LENGTH-1):0] data_o,
    output logic       data_valid
);

    /* UART1 9600 Hz */
    logic [7:0] uart_data = 0;
    logic [7:0] uart_data_count = 0;
    logic uart_data_valid = 0;

    uart_rx #(
        .CLK_FREQ(CLK_FREQ),
        .BAUD_RATE(BAUD_RATE)
    ) module_uart_rx (
        .clk(clk),
        .reset_n(reset_n),
        .data_i(data_i),
        .data_o(uart_data),
        .data_valid(uart_data_valid),
        .data_count(uart_data_count)
    );

    /* UART FULL DATA */
    always_ff @(posedge clk) begin
        if (!reset_n) begin
            data_o <= 0;
            data_valid <= 0;
        end else begin
            data_valid <= 0;

            if (uart_data_valid) begin
                data_o <= {uart_data, data_o[(BLOCK_LENGTH-1):8]};

                if (uart_data_count == (BLOCK_LENGTH/8)) begin 
                    data_valid <= 1'b1;
                end
            end
        end
    end
endmodule