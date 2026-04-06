module top (
    input  logic clk,
    input  logic button_s1,
    input  logic button_s2,
    input  logic uart1_rx_data,
    output logic led_ready,
    output logic led_done,
    output logic [17:0] servo_pwm,
    output logic debug_a10,
    output logic debug_e10,
    output logic debug_l11
);
    logic reset_n = 1'b1;

    /* UART1 9600 Hz */
    logic [215:0] uart1_full_data = 0;
    logic uart1_data_valid = 0;

    block_uart_rx #(
        .BAUD_RATE(9600),
        .BLOCK_LENGTH(216)
    ) block_uart1_rx (
        .clk(clk),
        .reset_n(reset_n),
        .data_i(uart1_rx_data),
        .data_o(uart1_full_data),
        .data_valid(uart1_data_valid)
    );

    /* SERVO PWM */
    localparam SERVO_DUTY_0 = 28000;  // ~0.5ms
    localparam SERVO_DUTY_180 = 128000; // ~2.5ms
    localparam SERVO_DUTY_90 = (SERVO_DUTY_180 + SERVO_DUTY_0) / 2;

    logic [19:0] pwm_duty [0:17]; 

    always_ff @(posedge clk) begin
        if (!reset_n) begin
            pwm_duty <= '{default: SERVO_DUTY_90};
        end else begin
            if (uart1_data_valid) begin
                /* 875 -> 4000 | 28000 >> 5 = 875 | 128000 >> 5 = 4000 */
                for (int i = 0; i < 18; i++) begin
                    pwm_duty[i] <= uart1_full_data[i*12 +: 12] << 5;
                end
            end
        end
    end

    /* PWM REGISTER */
    logic [19:0] pwm50hz_reg = 0;
    
    always_ff @(posedge clk) begin
        if (!reset_n) begin
            pwm50hz_reg <= 1;
        end else begin

            if (pwm50hz_reg == 1000000) begin
                pwm50hz_reg <= 1;
            end else begin
                pwm50hz_reg <= pwm50hz_reg + 1'b1;
            end
        end
    end

    always_comb begin
        for (int i = 0; i < 18; i++) begin
            servo_pwm[i] = (pwm50hz_reg < pwm_duty[i]);
        end
    end
    
    /* LED */
    assign led_done = button_s1;
    assign led_ready = button_s2;

    /* DEBUG */
    assign debug_a10 = reset_n;
    assign debug_l11 = reset_n;
    assign debug_e10 = reset_n;

endmodule