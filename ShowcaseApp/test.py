import os

directories = ['include', 'src', 'assets']  # Directories to search for files

def process_directory(directory):
    file_list = []
    for root, _, files in os.walk(directory):
        if "textures" in root:
            continue  # Skip the "textures" directory within "assets"
        for file in files:
            file_path = os.path.join(root, file)
            file_list.append(file_path)

    return file_list

def extract_code(file_path):
    with open(file_path, 'r', encoding='utf-8') as file:
        code = file.read()
    return code

def generate_code_output():
    all_code_output = ""
    for directory in directories:
        file_list = process_directory(directory)
        for file_path in file_list:
            _, extension = os.path.splitext(file_path)
            file_name = os.path.basename(file_path)
            code = extract_code(file_path)
            all_code_output += f"{file_name} --\n\n{code}\n\n"

    with open("all_code_output.txt", 'w', encoding='utf-8') as file:
        file.write(all_code_output)

    print("Code output generated in all_code_output.txt.")

generate_code_output()
