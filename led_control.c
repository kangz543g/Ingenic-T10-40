#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define RED_LED_GPIO "78"

int is_gpio_exported(const char *gpio_pin) {
    char gpio_path[50];
    snprintf(gpio_path, sizeof(gpio_path), "/sys/class/gpio/gpio%s", gpio_pin);

    // Check if the GPIO directory exists
    return access(gpio_path, F_OK) == 0;
}

void export_gpio(const char *gpio_pin) {
    if (is_gpio_exported(gpio_pin)) {
        printf("GPIO pin %s is already exported.\n", gpio_pin);
        return;
    }

    int export_file = open("/sys/class/gpio/export", O_WRONLY);
    if (export_file == -1) {
        perror("Error opening export file");
        exit(EXIT_FAILURE);
    }

    if (write(export_file, gpio_pin, strlen(gpio_pin)) == -1) {
        perror("Error exporting GPIO");
        close(export_file);
        exit(EXIT_FAILURE);
    }

    close(export_file);
}

void set_direction(const char *gpio_pin, const char *direction) {
    char gpio_direction_path[50];
    snprintf(gpio_direction_path, sizeof(gpio_direction_path), "/sys/class/gpio/gpio%s/direction", gpio_pin);

    int direction_file = open(gpio_direction_path, O_WRONLY);
    if (direction_file == -1) {
        perror("Error opening direction file");
        exit(EXIT_FAILURE);
    }

    if (write(direction_file, direction, strlen(direction)) == -1) {
        perror("Error setting direction");
        close(direction_file);
        exit(EXIT_FAILURE);
    }

    close(direction_file);
}

void control_led(const char *gpio_pin, const char *value) {
    char gpio_value_path[50];
    snprintf(gpio_value_path, sizeof(gpio_value_path), "/sys/class/gpio/gpio%s/value", gpio_pin);

    int value_file = open(gpio_value_path, O_WRONLY);
    if (value_file == -1) {
        perror("Error opening value file");
        exit(EXIT_FAILURE);
    }

    if (write(value_file, value, strlen(value)) == -1) {
        perror("Error setting LED value");
        close(value_file);
        exit(EXIT_FAILURE);
    }

    close(value_file);
}

int main() {
    if (!is_gpio_exported(RED_LED_GPIO)) {
        export_gpio(RED_LED_GPIO);
    }

    set_direction(RED_LED_GPIO, "out");

    while (1) {
        // LED를 켜기 위해
        control_led(RED_LED_GPIO, "1");
        sleep(30);  // 30초 동안 유지

        // LED를 끄기 위해
        control_led(RED_LED_GPIO, "0");
        sleep(30);  // 30초 동안 유지
    }

    return 0;
}
