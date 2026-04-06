module uart_rx #(
    parameter CLK_FREQ = 50_000_000,
    parameter BAUD_RATE = 9600
) (
    input  logic       clk,
    input  logic       reset_n,
    input  logic       data_i,
    output logic [7:0] data_o,
    output logic       data_valid,
    output logic [7:0] data_count
);

    localparam BIT_TIMER_MAX = CLK_FREQ / BAUD_RATE;
    localparam BIT_TIMER_HALF = BIT_TIMER_MAX / 2;

    logic rx_sync_data;
    sync #(.RESET_DEFAULT_STATE(1)) rx_sync (
        .clk(clk),
        .reset_n(reset_n),
        .data_i(data_i),
        .data_o(rx_sync_data)
    );

    typedef enum logic [1:0] {
        IDLE,
        START_BIT,
        DATA_BITS,
        STOP_BIT
    } state_t;

    state_t state, next_state;

    logic [15:0] timer;
    logic [2:0]  bit_idx;
    logic [7:0]  shift_reg;

    always_ff @(posedge clk or negedge reset_n) begin
        if (!reset_n) begin
            state      <= IDLE;
            timer      <= 0;
            bit_idx    <= 0;
            shift_reg  <= 8'd0;
            data_o     <= 8'd0;
            data_valid <= 1'b0;
        end else begin
            data_valid <= 1'b0; 

            case (state)
                IDLE: begin
                    if (rx_sync_data == 1'b0) begin
                        state <= START_BIT;
                        timer <= 0;
                    end else begin

                        if (data_count != 0) begin
                            timer <= timer + 1;
                            if (timer == BIT_TIMER_MAX) begin
                                data_count <= 0;
                            end
                        end else begin
                            timer <= 0;
                        end
                    end
                end

                START_BIT: begin
                    if (timer == BIT_TIMER_HALF) begin
                        timer <= 0;
                        if (rx_sync_data == 1'b0) begin 
                            state <= DATA_BITS;
                            bit_idx <= 0;
                        end else begin
                            state <= IDLE;
                        end
                    end else begin
                        timer <= timer + 1;
                    end
                end

                DATA_BITS: begin
                    if (timer == BIT_TIMER_MAX) begin
                        timer <= 0;
                        shift_reg <= {rx_sync_data, shift_reg[7:1]};
                        
                        if (bit_idx == 7) begin
                            state <= STOP_BIT;
                        end else begin
                            bit_idx <= bit_idx + 1;
                        end
                    end else begin
                        timer <= timer + 1;
                    end
                end

                STOP_BIT: begin
                    if (timer == BIT_TIMER_MAX) begin
                        timer <= 0;
                        state <= IDLE;
                        
                        if (rx_sync_data == 1'b1) begin 
                            data_o <= shift_reg;
                            data_valid <= 1'b1;
                            data_count <= data_count + 1;
                        end
                    end else begin
                        timer <= timer + 1;
                    end
                end
                
                default: state <= IDLE;
            endcase
        end
    end
endmodule