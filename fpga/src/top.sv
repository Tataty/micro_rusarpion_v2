module top (
    input  logic clk,
    input  logic button_s1,
    input  logic button_s2,
    input  logic uart1_rx_data,
    output logic led_ready,
    output logic led_done,
    output logic servo_pwm1,
    output logic debug_a10,
    output logic debug_e10,
    output logic debug_l11
);
    logic reset_n = 1'b1;

    /* UART1 9600 Hz */
    logic [7:0] uart1_data = 0;
    logic [7:0] uart1_data_count = 0;
    logic uart1_data_valid = 0;

    uart_rx #(.BAUD_RATE(9600)) uart1_rx (
        .clk(clk),
        .reset_n(reset_n),
        .data_i(uart1_rx_data),
        .data_o(uart1_data),
        .data_valid(uart1_data_valid),
        .data_count(uart1_data_count)
    );

    /* UART FULL DATA */
    logic [216:0] uart_full_data = 0;
    logic uart_full_data_valid = 0;
        
    always_ff @(posedge clk) begin
        if (!reset_n) begin
            uart_full_data <= 0;
        end else begin
            uart_full_data_valid <= 0;

            if (uart1_data_valid) begin

                case (uart1_data_count)
                    1: uart_full_data[7:0] <= uart1_data;
                    2: uart_full_data[15:8] <= uart1_data;
                    3: uart_full_data[23:16] <= uart1_data;
                    4: uart_full_data[31:24] <= uart1_data;
                    5: begin
                        uart_full_data[15:8] <= uart1_data;
                        uart_full_data_valid <= 1;
                    end
                    default: ;
                endcase
            end
        end
    end

    /* SERVO PWM */
    logic [19:0] pwm_duty = 28000, pwm50hz_reg = 0;

    localparam SERVO_DUTY_0 = 28000;  // ~0.5ms
    localparam SERVO_DUTY_180 = 128000; // ~2.5ms
    localparam SERVO_DUTY_90 = (SERVO_DUTY_180 + SERVO_DUTY_0) / 2;

    always_ff @(posedge clk) begin
        if (!reset_n) begin
            pwm_duty <= 0;
        end else begin
            if (uart_full_data_valid) begin
                /* 875 -> 4000 */
                /* 28000 >> 5 = 875 */
                /* 128000 >> 5 = 4000 */ 
                pwm_duty <= uart_full_data << 5;
            end
        end
    end
    
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

    assign servo_pwm1 = pwm_duty < pwm50hz_reg;
    assign debug_a10 = servo_pwm1;

    /* LED */
    assign led_done = uart_full_data[8];
    assign led_ready = button_s2;

    /* DEBUG */
    assign debug_l11 = uart1_data_count[0];
    assign debug_e10 = uart1_data_count[1];

endmodule