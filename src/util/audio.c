#include "audio.h"
#include "window.h"

GstElement * bin;
GstElement * src, * audioconv, * spectrum, * sink;
GstBus * bus;
GstCaps * caps;
GMainLoop * loop;

static gboolean message_handler(GstBus * bus, GstMessage * message, gpointer data) {
    if (message->type == GST_MESSAGE_ELEMENT) {
	const GstStructure * s = gst_message_get_structure(message);
	const char * name = gst_structure_get_name(s);
	GstClockTime endtime;

	if (strcmp(name, "spectrum") == 0) {
	    const GValue * magnitudes;
	    const GValue * mag, * phase;
	    gdouble freq;
	    guint i;

	    if (!gst_structure_get_clock_time(s, "endtime", &endtime)) {
		endtime = GST_CLOCK_TIME_NONE;
	    }

	    magnitudes = gst_structure_get_value(s, "magnitude");

	    GLfloat gl_pulse[BANDS];

	    for (i=0; i<BANDS; ++i) {
		freq = (gdouble)((AUDIOFREQ/2) * i + AUDIOFREQ/4) / BANDS;
		mag = gst_value_list_get_value(magnitudes, i);

		if (mag != NULL) {
		    gl_pulse[i] = (GLfloat)g_value_get_float(mag);
		} else {
		    gl_pulse[i] = -80.0f;
		}
	    }

	    glUseProgram(vis_prog);
	    glUniform1fv(glGetUniformLocation(vis_prog, "pulse"), BANDS, gl_pulse);
	    update_screen();
	}
    }

    return TRUE;
}

void init_audio() {
    gst_init(NULL, NULL);
    bin = gst_pipeline_new("bin");

    // Set the audio source, we will need to make this pulseaudio somehow.
    src = gst_element_factory_make("pulsesrc", "src");
    audioconv = gst_element_factory_make("audioconvert", NULL);
    g_assert(audioconv);

    spectrum = gst_element_factory_make("spectrum", "spectrum");
    g_object_set(G_OBJECT(spectrum), "bands", BANDS, "threshold", THRESHOLD,
	"post-messages", TRUE, "message-phase", TRUE, NULL);

    sink = gst_element_factory_make("fakesink", "sink");
    g_object_set(G_OBJECT(sink), "sync", TRUE, NULL);

    gst_bin_add_many(GST_BIN(bin), src, audioconv, spectrum, sink, NULL);

    caps = gst_caps_new_simple("audio/x-raw", "rate", G_TYPE_INT, AUDIOFREQ, NULL);

    if (!gst_element_link(src, audioconv) ||
	    !gst_element_link_filtered(audioconv, spectrum, caps) ||
	    !gst_element_link(spectrum, sink)) {
	fprintf(stderr, "Failed to link elements.\n");
	exit(1);
    }
    gst_caps_unref(caps);

    bus = gst_element_get_bus(bin);
    gst_bus_add_watch(bus, message_handler, NULL);
    gst_object_unref(bus);

    gst_element_set_state(bin, GST_STATE_PLAYING);

    // Run the main loop.
    loop = g_main_loop_new(NULL, FALSE);
    g_main_loop_run(loop);
}

void clean_audio() {
    gst_element_set_state(bin, GST_STATE_NULL);
    gst_object_unref(bin);
}
