module sync #(
    parameter RESET_DEFAULT_STATE = 0
) (
    input  logic clk,
    input  logic reset_n,
    input  logic data_i,
    output logic data_o
);
    logic n1;
    always_ff @(posedge clk or negedge reset_n) begin
        if (!reset_n) begin
            n1 <= RESET_DEFAULT_STATE;
            data_o <= RESET_DEFAULT_STATE;
        end else begin
            n1 <= data_i;
            data_o <= n1;
        end
    end
endmodule