module rising_edge_detector (
    input  logic clk,
    input  logic reset_n,
    input  logic signal_i,
    output logic pulse_o
);

    logic [1:0] signal_buffer;

    always_ff @(posedge clk or negedge reset_n) begin
        if (!reset_n) begin
            signal_buffer <= 0;
        end else begin
            signal_buffer <= {signal_buffer[0], signal_i};
        end
    end

    assign pulse_o = (signal_buffer[1] == 1'b0) && (signal_buffer[0] == 1'b1);

endmodule