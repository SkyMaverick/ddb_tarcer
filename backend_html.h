#pragma once

#define BACKEND_HTML_STYLE                                                                         \
    "<style type=\"text/css\">"                                                                    \
    "\tp {padding: 10; margin: 0; border-bottom: solid white 1px;white-space:pre-line}"            \
    "\tp.spy_logger {background-color:#90EE90;}"                                                   \
    "\tp.spy_message {background-color:#ADD8E6;}"                                                  \
    "\tp.spy_intrnl {background-color:#FA8072;}"                                                   \
    "</style>"

#define BACKEND_HTML_HEADER "<html>" BACKEND_HTML_STYLE "<body>"
#define BACKEND_HTML_FOOTER                                                                        \
    "</body>"                                                                                      \
    "</html>"
