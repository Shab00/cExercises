#include <stdio.h>

enum alert_type { SUNNY, RAIN, SNOW, STORM };

typedef struct {
    char *recipient;
    enum alert_type type;
} alert;

void sunny(alert a) {
    printf("Hello %s, it's a sunny day! Enjoy!\n", a.recipient);
}

void rain(alert a) {
    printf("Hello %s, bring an umbrella. Rain is expected.\n", a.recipient);
}

void snow(alert a) {
    printf("Hello %s, bundle up! Snow is coming.\n", a.recipient);
}

void storm(alert a) {
    printf("Hello %s, stay safe. A storm is forecast.\n", a.recipient);
}

void (*notifications[])(alert) = {sunny, rain, snow, storm};

int main() {
    alert alerts[] = {
        {"Alice", SUNNY},
        {"Bob", RAIN},
        {"Charlie", SNOW},
        {"Dina", STORM}
    };
    int n = sizeof(alerts) / sizeof(alerts[0]);
    for (int i = 0; i < n; i++) {
        (notifications[alerts[i].type])(alerts[i]);
    }
    return 0;
}
