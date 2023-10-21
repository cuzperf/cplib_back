import os

file_name = ".git/hooks/pre-commit"
code_to_write = """
rm -rf build
cmake -B build
cmake --build build --target clang-format
echo "Run clang-format done"
"""

with open(file_name, "w") as file:
  file.write(code_to_write)

os.system('chmod +x ' + file_name)
# os.system('git submodule add -b v1.14.x https://github.com/google/googletest.git 3rdparty/googletest')
os.system('git submodule update --init --recursive')
