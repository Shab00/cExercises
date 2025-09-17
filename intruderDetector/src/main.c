#include <stdlib.h>

int main() {
    system("gst-launch-1.0 avfvideosrc ! videoconvert ! osxvideosink");
    return 0;
}
