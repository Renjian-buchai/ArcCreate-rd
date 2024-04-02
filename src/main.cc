#include <zip.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>

int main(int argc, const char** argv, const char** envp) {
  (void)argc, (void)argv, (void)envp;

  int errcode = 0;
  zip_t* archive = nullptr;

  if (std::filesystem::exists(argv[1])) {
    archive = zip_open(argv[1], 0, &errcode);
  }

  std::unique_ptr<struct zip_stat> info = std::make_unique<struct zip_stat>();
  zip_stat_init(info.get());

  char* text;
  zip_file_t* descriptor = nullptr;

  std::ofstream outf;
  std::string txt;

  for (int i = 0; zip_stat_index(archive, i, 0, info.get()) == 0; ++i) {
    text = new char[info->size];

    descriptor = zip_fopen_index(archive, i, 0);

    zip_fread(descriptor, text, info->size);

    outf.open(std::string(zip_get_name(archive, i, 0)) + "-out");
    txt = std::string(text);
    txt.erase(txt.size());
    std::cout << txt;
    outf.write(txt.data(), txt.size());
    outf.close();

    delete[] text;
  }

  zip_close(archive);

  return errcode;
}
