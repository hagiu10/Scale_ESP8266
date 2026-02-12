
# Copy the HTML file to a C++ header file with PROGMEM format

def copy_html_to_progmem(path_html_file, path_output_file):
    with open(path_html_file, 'r', encoding='utf-8') as html_file:
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
    with open(path_output_file, 'w', encoding='utf-8') as output_file:
        output_file.write(progmem_string)
    print(f"Successfully copied HTML content from {path_html_file} to {path_output_file} in PROGMEM format.")    