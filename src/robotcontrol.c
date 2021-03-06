#include <pebble.h>

static Window *window;
static TextLayer *text_layer;

/*
static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(text_layer, "Select");
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(text_layer, "Up");
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(text_layer, "Down");
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}
*/



static void window_load(Window *window) {
/*
 Layer *window_layer = window_get_root_layer(window);
 GRect bounds = layer_get_bounds(window_layer);

 text_layer = text_layer_create((GRect) { .origin = { 0, 72 }, .size = { bounds.size.w, 20 } });
 text_layer_set_text(text_layer, "Press a button");
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
layer_add_child(window_layer, text_layer_get_layer(text_layer));
  */

  
}

static void window_unload(Window *window) {
  text_layer_destroy(text_layer);
}

static void send_data() {
  AccelData data;

  accel_service_peek(&data);

  DictionaryIterator *iter;
  app_message_outbox_begin(&iter);
  dict_write_int(iter, 0, &data.x, sizeof(data.x), true);
  dict_write_int(iter, 1, &data.y, sizeof(data.y), true);
  dict_write_int(iter, 2, &data.z, sizeof(data.z), true);
  app_message_outbox_send();
  
}

static void outbox_sent(DictionaryIterator *iter, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Out sent");
  send_data();
}

static void outbox_failed(DictionaryIterator *itor, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Out failed - err=%i", reason);
  send_data();
}

static void init(void) {
  window = window_create();
  //window_set_click_config_provider(window, click_config_provider);
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  const bool animated = true;
  window_stack_push(window, animated);
  
  accel_data_service_subscribe(0, NULL);
  app_comm_set_sniff_interval(SNIFF_INTERVAL_REDUCED);

  app_message_register_outbox_sent(outbox_sent);
  app_message_register_outbox_failed(outbox_failed);
  app_message_open(100, 100);

  

  send_data();
}

static void deinit(void) {
  accel_data_service_unsubscribe();
  window_destroy(window);
 
  
}

int main(void) {
  init();

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window);

  app_event_loop();
  deinit();
}
