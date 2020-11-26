set(COMPONENT_ADD_INCLUDEDIRS include)
set(COMPONENT_SRCS  "src/user_http.c"
                    "src/user_nvc.c"
					"src/user_uart.c"
					"src/user_wifi.c"
					"src/hytera_protol.c")


register_component()
