 idf_component_register(SRCS 
    main.c 
    app_bluetooth.c 
    app_encoder.c 
    app_display.c 
    app_ui.c 
    app_system.c
    INCLUDE_DIRS "include"
    REQUIRES 
        lvgl 
        esp_lvgl_adapter 
        driver 
        nvs_flash 
        bt 
        esp_hid 
        json
)