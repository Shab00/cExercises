#include <gst/gst.h>
#include <gst/app/gstappsink.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WIDTH 640
#define HEIGHT 480
#define CHANNELS 3

int main(int argc, char *argv[]) {
    gst_init(&argc, &argv);
    const char *pipeline_desc =
        "avfvideosrc ! videoconvert ! video/x-raw,format=RGB,width=640,height=480 ! tee name=t "
        "t. ! queue ! appsink name=sink "
        "t. ! queue ! videoconvert ! tcpserversink host=127.0.0.1 port=5000";

    GstElement *pipeline = gst_parse_launch(pipeline_desc, NULL);
    GstElement *appsink = gst_bin_get_by_name(GST_BIN(pipeline), "sink");
    gst_element_set_state(pipeline, GST_STATE_PLAYING);

    size_t frame_size = WIDTH * HEIGHT * CHANNELS;
    unsigned char *previous = calloc(frame_size, 1);

    while (1) { 
        GstSample *sample = NULL;
        g_signal_emit_by_name(appsink, "pull-sample", &sample, NULL);

        if (sample) {
            GstBuffer *buffer = gst_sample_get_buffer(sample);
            GstMapInfo map;
            if (gst_buffer_map(buffer, &map, GST_MAP_READ)) {
                int changed_pixels = 0;
                for (size_t j = 0; j < frame_size; ++j) {
                    if (abs((int)map.data[j] - (int)previous[j]) > 30) { 
                        ++changed_pixels;
                    }
                }
                printf("Changed pixels: %d\n", changed_pixels);
                if (changed_pixels > (frame_size / 100)) {
                    printf("Motion detected!\n");
                }
                memcpy(previous, map.data, frame_size);
                gst_buffer_unmap(buffer, &map);
            }
            gst_sample_unref(sample);
        } else {
            printf("No frame grabbed :(\n");
        }
    }
    free(previous);
    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(pipeline);
    return 0;
}
