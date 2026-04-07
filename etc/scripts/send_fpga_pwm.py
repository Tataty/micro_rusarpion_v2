import serial
import sys
import argparse
import time

def pack_12bit_data(values):
    """Пакує 18 значень по 12 біт у 27 байтів (Little-endian layout)."""
    payload = bytearray()
    for i in range(0, len(values), 2):
        v1 = values[i] & 0xFFF
        v2 = values[i+1] & 0xFFF
        
        # [v1_low8] [v1_high4 | v2_low4] [v2_high8]
        b1 = v1 & 0xFF
        b2 = ((v1 >> 8) & 0x0F) | ((v2 << 4) & 0xF0)
        b3 = (v2 >> 4) & 0xFF
        
        payload.extend([b1, b2, b3])
    return payload

def main():
    # Налаштування парсера аргументів
    parser = argparse.ArgumentParser(
        description="Управління 18-канальним PWM контролером через UART (FPGA)."
    )
    
    # Обов'язковий позиційний аргумент
    parser.add_argument("port", help="Шлях до пристрою (напр. /dev/ttyUSB2)")

    # Додаємо можливість задавати кожен канал окремо (від -0 до -17)
    for i in range(18):
        parser.add_argument(f"-{i}", type=int, help=f"Значення для каналу {i} (875-4000)")

    args, unknown = parser.parse_known_args()

    # Початкові значення (статичний масив за замовчуванням - наприклад, центр 2437)
    # Або замініть на значення, які ви вважаєте "безпечними"
    servo_values = [2437] * 18

    # Оновлюємо значення з аргументів командного рядка
    # vars(args) повертає словник усіх аргументів
    args_dict = vars(args)
    for i in range(18):
        val = args_dict.get(str(i))
        if val is not None:
            servo_values[i] = val

    # Пакуємо та відправляємо
    data_to_send = pack_12bit_data(servo_values)

    try:
        with serial.Serial(args.port, 9600, timeout=1) as ser:
            time.sleep(0.05)  # Коротка пауза для стабілізації лінії
            ser.write(data_to_send)
            ser.flush()
            
            print(f"--- Відправка на {args.port} ---")
            print(f"Канали: {servo_values}")
            print(f"Байтів надіслано: {len(data_to_send)} (HEX: {data_to_send.hex(' ')})")

    except serial.SerialException as e:
        print(f"Помилка порту: {e}")
    except Exception as e:
        print(f"Виникла помилка: {e}")

if __name__ == "__main__":
    main()