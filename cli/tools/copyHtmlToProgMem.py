
PATH_HTML_FILE = 'arduino/libraries/webServer/WebPage/webPage.html'
PATH_OUTPUT_FILE = 'arduino/libraries/webServer/WebPage/webPage.h'

# Copy the HTML file to a C++ header file with PROGMEM format

def copy_html_to_progmem():
    with open(PATH_HTML_FILE, 'r', encoding='utf-8') as html_file:
        html_content = html_file.read()

    # Create the PROGMEM string
    progmem_string = f'''
#ifndef WEBPAGE_H
#define WEBPAGE_H

const char webPage[] PROGMEM = R"===(
{html_content}
)===";

#endif // WEBPAGE_H
'''

    # Write to the output file
    with open(PATH_OUTPUT_FILE, 'w', encoding='utf-8') as output_file:
        output_file.write(progmem_string)
    print(f"Successfully copied HTML content from {PATH_HTML_FILE} to {PATH_OUTPUT_FILE} in PROGMEM format.")    