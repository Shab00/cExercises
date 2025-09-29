#include <gst/gst.h>
#ifdef __APPLE__
#include <TargetConditionals.h>
#endif

int tutorial_main(int argc, char *argv[]) {
    gst_init(&argc, &argv);
    const char *pipeline_desc =
        "tcpclientsrc host=127.0.0.1 port=5000 ! video/x-raw,format=RGB,width=640,height=480 ! videoconvert ! autovideosink";
    GstElement *pipeline = gst_parse_launch(pipeline_desc, NULL);
    gst_element_set_state(pipeline, GST_STATE_PLAYING);

    GstBus *bus = gst_element_get_bus(pipeline);
    GstMessage *msg = NULL;
    while ((msg = gst_bus_timed_pop_filtered(bus, GST_CLOCK_TIME_NONE,
            GST_MESSAGE_ERROR | GST_MESSAGE_EOS)) == NULL) {
    }

    if (GST_MESSAGE_TYPE(msg) == GST_MESSAGE_ERROR)
        g_printerr("GStreamer error!\n");

    gst_message_unref(msg);
    gst_object_unref(bus);
    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(pipeline);
    return 0;
}

int main(int argc, char *argv[]) {
#if defined(__APPLE__) && TARGET_OS_MAC && !TARGET_OS_IPHONE
    return gst_macos_main((GstMainFunc)tutorial_main, argc, argv, NULL);
#else
    return tutorial_main(argc, argv);
#endif
}
