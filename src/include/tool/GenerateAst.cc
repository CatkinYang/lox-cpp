#include <iostream>
#include <string>
#include <vector>
namespace lox {

class GenerateAst {
  public:
  private:
};

static void defineAst(std::string outputDir, std::string baseName,
                      std::vector<std::string> types) {
    std::string path = outputDir + "/" + "baseName" + ".cc";
}

int main(int argc, char *argv[]) {
    if (argc != 1) {
        std::cerr << "Usage" << std::endl;
        std::exit(64);
    }
    std::string outputDir = argv[0];
}

} // namespace lox
