module debouncer 
#(parameter N=19)
(
    input  logic clk,
    input  logic reset_n,
    input  logic signal_i,
    output logic debounced_o
);

    logic signal_sync = 1'b0;
    logic signal_prev = 1'b0;
    logic [N:0] counter;

    sync sync_reg(
        .clk     (clk),
        .reset_n   (reset_n),
        .data_i  (signal_i),
        .data_o  (signal_sync)
    );

    always_ff @(posedge clk or negedge reset_n) begin
        if (!reset_n) begin
            counter <= 0;
            signal_prev <= 0;
            debounced_o <= 0;
        end else begin
            signal_prev <= signal_sync;
    
            if (signal_sync ^ signal_prev) begin
                counter <= 0;
            end else begin

                if (counter[N]) begin
                    debounced_o <= signal_sync;
                end else begin
                    counter <= counter + 1'b1;
                end
            end
        end
    end
endmodule