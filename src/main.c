#include <pebble.h>
#include <stdlib.h>

#define KEY_USERNAME 0
#define KEY_BENIS 1
#define KEY_DICKBUTT 2
#define KEY_DICKBUTT_INTENSITY 3

static Window *s_main_window;
static Window *s_dickbutt_window;

static TextLayer *s_time_layer;
static TextLayer *s_username_layer;
static TextLayer *s_benis_layer;

static GFont s_time_font;
static GFont s_username_font;
static GFont s_benis_font;

static BitmapLayer *s_background_layer;
static GBitmap *s_background_bitmap;

static bool dickbutt_visible                = false;
static bool config_dickbutt                 = false;
static signed int config_dickbutt_intensity = 5;

static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);

  // Create a long-lived buffer
  static char buffer[] = "00:00";

  // Write the current hours and minutes into the buffer
  if(clock_is_24h_style() == true) {
    //Use 2h hour format
    strftime(buffer, sizeof("00:00"), "%H:%M", tick_time);
  } else {
    //Use 12 hour format
    strftime(buffer, sizeof("00:00"), "%I:%M", tick_time);
  }

  // Display this time on the TextLayer
  text_layer_set_text(s_time_layer, buffer);
}

static void dickbutt_window_load(Window *window) {

  //Create GBitmap, then set to created BitmapLayer
  s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_DICKBUTT);
  s_background_layer  = bitmap_layer_create(GRect(0, 0, 144, 168));
  bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_background_layer));
}

static void main_window_load(Window *window) {
  //Create GBitmap, then set to created BitmapLayer
  s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BACKGROUND);
  s_background_layer  = bitmap_layer_create(GRect(0, 0, 144, 168));
  bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_background_layer));

  // Create time TextLayer
  s_time_layer = text_layer_create(GRect(0, 0, 139, 50));
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorWhite);
  text_layer_set_text(s_time_layer, "00:00");

  //Create GFont
  s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_SANS_SERIF_36));

  //Apply to TextLayer
  text_layer_set_font(s_time_layer, s_time_font);
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentRight);

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));

  // Create username Layer
  s_username_layer = text_layer_create(GRect(0, 60, 144, 80));
  text_layer_set_background_color(s_username_layer, GColorClear);
  text_layer_set_text_color(s_username_layer, GColorWhite);
  text_layer_set_text_alignment(s_username_layer, GTextAlignmentCenter);
  text_layer_set_text(s_username_layer, "Lade...");

  // Create second custom font, apply it and add to Window
  s_username_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_SANS_SERIF_36));
  text_layer_set_font(s_username_layer, s_username_font);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_username_layer));

  // Create benis Layer
  s_benis_layer = text_layer_create(GRect(0, 120, 144, 25));
  text_layer_set_background_color(s_benis_layer, GColorClear);
  text_layer_set_text_color(s_benis_layer, GColorWhite);
  text_layer_set_text_alignment(s_benis_layer, GTextAlignmentCenter);
  text_layer_set_text(s_benis_layer, "Lade Benis...");

  // Create second custom font, apply it and add to Window
  s_benis_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_SANS_SERIF_20));
  text_layer_set_font(s_benis_layer, s_benis_font);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_benis_layer));

  // Make sure the time is displayed from the start
  update_time();
}

static void main_window_unload(Window *window) {
  //Unload GFont
  fonts_unload_custom_font(s_time_font);

  //Destroy GBitmap
  gbitmap_destroy(s_background_bitmap);

  //Destroy BitmapLayer
  bitmap_layer_destroy(s_background_layer);

  // Destroy TextLayer
  text_layer_destroy(s_time_layer);

  // Destroy pr0gramm elements
  text_layer_destroy(s_username_layer);
  fonts_unload_custom_font(s_username_font);
  text_layer_destroy(s_benis_layer);
  fonts_unload_custom_font(s_benis_font);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();

  // Get pr0gramm update every 30 minutes
  if(tick_time->tm_min % 30 == 0) {
    // Begin dictionary
    DictionaryIterator *iter;
    app_message_outbox_begin(&iter);

    // Add a key-value pair
    dict_write_uint8(iter, 0, 0);

    // Send the message!
    app_message_outbox_send();
  }
}

static void dickbutt_handler(struct tm *tick_time, TimeUnits units_changed) {

  APP_LOG(APP_LOG_LEVEL_INFO, "dickbutt_handler called.");

  if ( !config_dickbutt ) {
    return;
  }

  APP_LOG(APP_LOG_LEVEL_INFO, "dickbutt_handler active.");

  // remove dickbutt if visible and skip
  if ( dickbutt_visible ) {

    APP_LOG(APP_LOG_LEVEL_INFO, "dickbutt visible, removing.");

    window_stack_remove(s_dickbutt_window, true);
    dickbutt_visible = false;
    return;
  }

  // overwrite intensity 0 with 1 since
  // 0 generates huge numbers the user
  // won't expect
  if ( config_dickbutt_intensity == 0 ) {
    config_dickbutt_intensity = 1;
  }

  int randomness = rand() % config_dickbutt_intensity;

  APP_LOG(APP_LOG_LEVEL_INFO, "dickbutt randomness = %d,.", randomness);

  if (randomness == 0) {

    APP_LOG(APP_LOG_LEVEL_INFO, "dickbutt show your beautiful face!");

    window_stack_push(s_dickbutt_window, true);
    dickbutt_visible = true;
  }
}

static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  // Store incoming information
  static char benis_buffer[32];
  static char username_buffer[32];

  // Read first item
  Tuple *t = dict_read_first(iterator);

  // For all items
  while(t != NULL) {
    // Which key was received?
    switch(t->key) {
    case KEY_USERNAME:
      snprintf(username_buffer, sizeof(username_buffer), "%s", t->value->cstring);
      break;
    case KEY_BENIS:
      snprintf(benis_buffer, sizeof(benis_buffer), "%d", (int)t->value->int32);
      break;
    case KEY_DICKBUTT:
      config_dickbutt = (bool)t->value->int32;
      break;
    case KEY_DICKBUTT_INTENSITY:
      config_dickbutt_intensity = (int)t->value->int32;
      break;
    default:
      APP_LOG(APP_LOG_LEVEL_ERROR, "Key %d not recognized!", (int)t->key);
      break;
    }

    // Look for next item
    t = dict_read_next(iterator);
  }

  // Assemble full string and display
  text_layer_set_text(s_username_layer, username_buffer);
  text_layer_set_text(s_benis_layer, benis_buffer);
}

static void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}

static void init() {
  // Create main Window element and assign to pointer
  s_main_window = window_create();

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load   = main_window_load,
    .unload = main_window_unload
  });

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);

  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);

  // Create main Window element and assign to pointer
  s_dickbutt_window = window_create();

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_dickbutt_window, (WindowHandlers) {
    .load = dickbutt_window_load,
  });

  // Register dickbutt handler with TickTimerService
  tick_timer_service_subscribe(SECOND_UNIT, dickbutt_handler);

  // Register callbacks
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);

  // Open AppMessage
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
}

static void deinit() {
  // Destroy Window
  window_destroy(s_main_window);
  window_destroy(s_dickbutt_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
