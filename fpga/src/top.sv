module top (
    input  logic clk,
    input  logic button_s1,
    input  logic button_s2,
    output logic led_ready,
    output logic led_done
);

    logic [1:0] led_array = 2'b00;
    
    // Оптимізація 1: Двокаскадний синхронізатор + детектор фронту в одному регістрі
    logic [2:0] s2_sync;

    always_ff @(posedge clk) begin
        s2_sync <= {s2_sync[1:0], button_s2};

        // Оптимізація 2: Пріоритетність та уникнення конфліктів
        if (s2_sync[2:1] == 2'b01) begin
            led_array <= led_array << 1;
        end 
        else if (button_s1) begin
            led_array[0] <= 1'b1;
        end
    end

    assign {led_done, led_ready} = led_array;

endmodule