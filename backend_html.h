#pragma once

// clang-format off

#define BACKEND_HTML_STYLE \
    "<style type=\"text/css\">\n" \
    "  p {padding: 5; margin: 0; border-bottom: solid white 1px;white-space:pre-line}\n" \
    "  p.spy_logger {background-color:#90EE90}\n" \
    "  p.spy_message {background-color:#ADD8E6}\n" \
    "  p.spy_intrnl {background-color:#FA8072}\n" \
    "  span.spy_ptr {color: red}\n" \
    "  span.spy_ev {font-weight: bold}\n" \
    "  span.spy_head {color: darkblue}\n" \
    "  span.spy_arg {color: maroon}\n" \
    "</style>\n"

#define BACKEND_HTML_SCRIPT \
    "<script>\n" \
    "function text_process (block) {\n" \
    "  var text = block.textContent;\n" \
    "  text = text.replace(/(DB_EV_.*?\\s)/ig, '<span class=\"spy_ev\">$1</span>');\n" \
    "  text = text.replace(/(\\t(\\=>|<\\=))/g, '<span class=\"spy_ev\">$1</span>');\n" \
    "  text = text.replace(/(:\\[(.*?)\\])/g, '<span class=\"spy_arg\">$1</span>');\n" \
    "  text = text.replace(/(\\{(.*?)\\})/g, '<span class=\"spy_arg\">$1</span>');\n" \
    "  text = text.replace(/(\\[(MSG|LOG|TRACK).*?\\])/ig, '<span class=\"spy_head\">$1</span>');\n" \
    "  block.innerHTML = text;\n" \
    "  }\n" \
    "function colorize_document() {\n" \
    "    document.querySelectorAll('p').forEach((block) => {text_process(block);});\n" \
    "}\n" \
    "</script>\n"

#define BACKEND_HTML_HEADER "<html><head>" BACKEND_HTML_STYLE BACKEND_HTML_SCRIPT "</head><body>"
#define BACKEND_HTML_FOOTER "<script>colorize_document();</script></body></html>"

//clang-format on
